#include "precompiled.h"

#include "syntax/files/file.h"

#include "syntax/parsers/parser.h"
#include "syntax/parsers/token.h"
#include "syntax/parsers/parser_utils.h"
#include "syntax/parsers/reverse_parser.h"
#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/utils/name_constants.h"

#include "utils/character.h"
#include "utils/math_utils.h"
#include "utils/misc_utils.h"

#include "utils/streams/input_reverse_stream.h"

#include <regex>

namespace vanillapdf {
namespace syntax {

#pragma region Objects

ParserBase::ParserBase(WeakReference<File> file, IInputStreamPtr stream)
    : Tokenizer(stream), _file(file) {
}

Parser::Parser(WeakReference<File> file, IInputStreamPtr stream)
    : ParserBase(file, stream) {
    _dictionary = make_unique<ParserTokenDictionary>();
    _dictionary->Initialize();
}

WeakReference<File> ParserBase::GetFile(void) const { return _file; }

IntegerObjectPtr ParserBase::ReadInteger() {
    auto token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
    auto result = ParserUtils::CreateInteger(token);
    result->SetFile(_file);
    return result;
}

ObjectPtr ParserBase::ReadIndirectReference() {
    auto integer = ReadInteger();

    auto pos = m_stream->GetInputPosition();
    if (PeekTokenTypeSkip() == Token::Type::INTEGER_OBJECT) {
        auto ahead = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
        auto gen_number = ParserUtils::GetIntegerValue(ahead);

        if (PeekTokenTypeSkip() == Token::Type::INDIRECT_REFERENCE_MARKER) {
            auto reference_marker = ReadTokenWithTypeSkip(Token::Type::INDIRECT_REFERENCE_MARKER);
            IndirectReferenceObjectPtr result = make_deferred<IndirectReferenceObject>(integer->GetUnsignedIntegerValue(), ValueConvertUtils::SafeConvert<types::ushort>(gen_number));
            result->SetFile(_file);
            return result;
        }

        // TODO we can peek only one next token, therefore we need to seek back
        m_stream->SetInputPosition(pos);
    }

    return integer;
}

RealObjectPtr ParserBase::ReadReal() {
    auto token = ReadTokenWithTypeSkip(Token::Type::REAL_OBJECT);
    auto result = ParserUtils::CreateReal(token);
    result->SetFile(_file);
    return result;
}

NullObjectPtr ParserBase::ReadNull() {
    ReadTokenWithTypeSkip(Token::Type::NULL_OBJECT);
    auto result = NullObject::GetInstance();
    result->SetFile(_file);
    return result;
}

DictionaryObjectPtr ParserBase::ReadDictionary() {

    DictionaryObjectPtr dictionary;
    dictionary->SetFile(_file);

    ReadTokenWithTypeSkip(Token::Type::DICTIONARY_BEGIN);
    while (PeekTokenTypeSkip() != Token::Type::DICTIONARY_END) {
        auto key = ReadDirectObject();
        auto direct = ReadDirectObject();

        if (direct->GetObjectType() == Object::Type::Null) {
            continue;
        }

        // There was an incorrect flow in the document signing, which did not include the signature ByteRange
        // <</ByteRange                                                                                      
        // What this did was to skip the expected array and then beyond the dictionary key was an incorrect type
        // Adobe is able to open such document without any issue and we should not refuse to do so as well
        // Let's just include the warning in the log, that the entries may be missing
        if (key->GetObjectType() != Object::Type::Name) {
            auto locked_file = _file.GetReference();
            auto filename = locked_file->GetFilenameString();

            auto key_type_str = Object::TypeName(key->GetObjectType());
            spdlog::warn("Found dictionary key with type {}, skipping", key_type_str);

            continue;
        }

        auto name = ConvertUtils<ObjectPtr>::ConvertTo<NameObjectPtr>(key);

        if (dictionary->Contains(name)) {
            auto locked_file = _file.GetReference();
            auto filename = locked_file->GetFilenameString();
            spdlog::warn("Found duplicate entry for {}, skipping", name->ToString());
            continue;
        }

        // Watch out! I am intentionally using ConvertUtils because
        // object utils was trying to dereference object references.
        // We just need cast the item without any other fancy mechanisms.
        //auto containable = ObjectUtils::ConvertTo<ContainableObjectPtr>(direct);

        auto containable = ConvertUtils<ObjectPtr>::ConvertTo<ContainableObjectPtr>(direct);
        dictionary->Insert(name, containable);
    }

    ReadTokenWithTypeSkip(Token::Type::DICTIONARY_END);
    return dictionary;
}

ObjectPtr ParserBase::ReadDictionaryStream() {
    auto dictionary = ReadDictionary();
    if (PeekTokenTypeSkip() == Token::Type::STREAM_BEGIN) {
        ReadTokenWithTypeSkip(Token::Type::STREAM_BEGIN);
        ReadTokenWithTypeSkip(Token::Type::EOL);
        auto stream_offset = m_stream->GetInputPosition();

        do {
            if (!dictionary->Contains(constant::Name::Length)) {
                auto locked_file = _file.GetReference();
                auto filename = locked_file->GetFilenameString();

                // Log warning as this is not standard, however not fatal
                spdlog::warn("The stream at offset {} does not contain length, using the fallback method", stream_offset);
                break;
            }

            auto length_obj = dictionary->Find(constant::Name::Length);
            if (length_obj->GetObjectType() != Object::Type::Integer) {
                auto locked_file = _file.GetReference();
                if (!locked_file->IsInitialized()) {
                    auto filename = locked_file->GetFilenameString();

                    // Log warning as this is not standard, however not fatal
                    spdlog::warn("The stream at offset {} does have length specified as indirect object, however the document is broken, using the fallback method", stream_offset);
                    break;
                }
            }

            // Seek to the end of the stream to verify terminating token
            auto length = ObjectUtils::ConvertTo<IntegerObjectPtr>(length_obj);
            m_stream->SetInputPosition(length->GetIntegerValue(), SeekDirection::Current);

            // Read the token beyond the stream data
            auto expect_stream_end = ReadTokenSkip();

            // Verify the terminating token type and try fallback method
            if (expect_stream_end->GetType() != Token::Type::STREAM_END) {
                auto locked_file = _file.GetReference();
                auto filename = locked_file->GetFilenameString();

                // Log warning as this is not standard, however not fatal
                spdlog::warn("Failed to verify stream length {} for stream object at offset {}, using the fallback method", length->GetIntegerValue(), stream_offset);
                break;
            }

            auto result = make_deferred<StreamObject>(dictionary, stream_offset);
            result->SetFile(_file);
            return result;
        } while (false);

        // Recalculate stream length
        m_stream->SetInputPosition(stream_offset, SeekDirection::Beginning);

        for (;;) {
            auto offset = m_stream->GetInputPosition();
            auto data = m_stream->Readline();
            auto line = data->ToString();
            auto pos = line.find("endstream");

            if (pos == std::string::npos) {
                continue;
            }

            auto end_obj_token = PeekTokenTypeSkip();
            assert(end_obj_token == Token::Type::INDIRECT_OBJECT_END); UNUSED(end_obj_token);

            // If and only if the endstream is at the start of the line
            // try to search for the EOL on the previous line
            if (pos == 0) {
                m_stream->SetInputPosition(offset - 2);
                auto new_line1 = m_stream->Get();
                auto new_line2 = m_stream->Get();
                if (new_line1 == '\r') {
                    offset -= 1;

                    assert(new_line2 == '\n');
                    if (new_line2 == '\n') {
                        offset -= 1;
                    }
                }
                else if (new_line2 == '\r' || new_line2 == '\n') {
                    offset -= 1;
                }

                ReadTokenWithTypeSkip(Token::Type::STREAM_END);
            }

            // Ensure stream_size is signed
            static_assert(std::is_signed<types::stream_size>::value, "Stream size is unsigned, zero comparison won't work");
            types::stream_size stream_end_offset = offset + pos;
            types::stream_size computed_length = stream_end_offset - stream_offset;

            if (computed_length < 0) {
                // There is a document called Scan2.pdf, in which the real length of the stream is zero
                // For this case the computed length was -1, which is invalid
                // For any other cases this algorithm works
                computed_length = 0;
            }

            if (!dictionary->Contains(constant::Name::Length)) {
                dictionary->Insert(constant::Name::Length, make_deferred<IntegerObject>(computed_length));
                break;
            }

            auto length_obj = dictionary->Find(constant::Name::Length);
            if (length_obj->GetObjectType() != Object::Type::Integer) {
                auto locked_file = _file.GetReference();
                if (!locked_file->IsInitialized()) {
                    dictionary->Insert(constant::Name::Length, make_deferred<IntegerObject>(computed_length), true);
                    break;
                }
            }

            auto length = ObjectUtils::ConvertTo<IntegerObjectPtr>(length_obj);
            *length = computed_length;
            break;
        }

        auto result = make_deferred<StreamObject>(dictionary, stream_offset);
        result->SetFile(_file);
        return result;
    }

    return dictionary;
}

MixedArrayObjectPtr ParserBase::ReadArray() {
    MixedArrayObjectPtr result;
    ReadTokenWithTypeSkip(Token::Type::ARRAY_BEGIN);
    while (PeekTokenTypeSkip() != Token::Type::ARRAY_END) {
        auto direct = ReadDirectObject();
        auto containable = ConvertUtils<ObjectPtr>::ConvertTo<ContainableObjectPtr>(direct);

        // Watch out! I am intentionally using ConvertUtils because
        // object utils was trying to dereference object references.
        // We just need cast the item without any other fancy mechanisms.
        //auto containable = ObjectUtils::ConvertTo<ContainableObjectPtr>(direct);

        result->Append(containable);
    }

    ReadTokenWithTypeSkip(Token::Type::ARRAY_END);
    result->SetFile(_file);
    return result;
}

NameObjectPtr ParserBase::ReadName() {
    auto token = ReadTokenWithTypeSkip(Token::Type::NAME_OBJECT);
    auto result = ParserUtils::CreateName(token);
    result->SetFile(_file);
    return result;
}

LiteralStringObjectPtr ParserBase::ReadLiteralString() {
    auto token = ReadTokenWithTypeSkip(Token::Type::LITERAL_STRING);
    auto result = ParserUtils::CreateLitString(token);
    result->SetFile(_file);
    return result;
}

HexadecimalStringObjectPtr ParserBase::ReadHexadecimalString() {
    auto token = ReadTokenWithTypeSkip(Token::Type::HEXADECIMAL_STRING);
    auto result = ParserUtils::CreateHexString(token);
    result->SetFile(_file);
    return result;
}

BooleanObjectPtr ParserBase::ReadTrue() {
    auto token = ReadTokenWithTypeSkip(Token::Type::TRUE_VALUE);
    auto result = ParserUtils::CreateBoolean(token);
    result->SetFile(_file);
    return result;
}

BooleanObjectPtr ParserBase::ReadFalse() {
    auto token = ReadTokenWithTypeSkip(Token::Type::FALSE_VALUE);
    auto result = ParserUtils::CreateBoolean(token);
    result->SetFile(_file);
    return result;
}

ObjectPtr Parser::ReadIndirectObject(types::big_uint& obj_number, types::ushort& gen_number) {
    auto offset = m_stream->GetInputPosition();
    auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
    auto gen_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
    auto begin_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_BEGIN);
    auto direct = ReadDirectObject();
    auto end_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_END);

    auto obj_number_value = ParserUtils::GetIntegerValue(obj_number_token);
    auto gen_number_value = ParserUtils::GetIntegerValue(gen_number_token);
    obj_number = ValueConvertUtils::SafeConvert<types::big_uint>(obj_number_value);
    gen_number = ValueConvertUtils::SafeConvert<types::ushort>(gen_number_value);

    direct->SetOffset(offset);
    direct->SetFile(_file);
    direct->SetInitialized();
    return direct;
}

ObjectPtr Parser::ReadIndirectObject(types::stream_offset offset, types::big_uint& obj_number, types::ushort& gen_number) {
    m_stream->SetInputPosition(offset, SeekDirection::Beginning);
    return ReadIndirectObject(obj_number, gen_number);
}

ObjectPtr ParserBase::ReadDirectObject() {
    auto offset = m_stream->GetInputPosition();
    auto type = PeekTokenType();

    while (type == Token::Type::EOL) {
        ReadTokenWithType(Token::Type::EOL);
        type = PeekTokenType();
    }

    switch (type) {
        case Token::Type::DICTIONARY_BEGIN:
            return ReadDictionaryStream();
        case Token::Type::INTEGER_OBJECT:
            return ReadIndirectReference();
        case Token::Type::ARRAY_BEGIN:
            return ReadArray();
        case Token::Type::NAME_OBJECT:
            return ReadName();
        case Token::Type::HEXADECIMAL_STRING:
            return ReadHexadecimalString();
        case Token::Type::LITERAL_STRING:
            return ReadLiteralString();
        case Token::Type::REAL_OBJECT:
            return ReadReal();
        case Token::Type::TRUE_VALUE:
            return ReadTrue();
        case Token::Type::FALSE_VALUE:
            return ReadFalse();
        case Token::Type::NULL_OBJECT:
            return ReadNull();
        default:
            throw ParseException(offset);
    }
}

ObjectStreamEntry Parser::ReadObjectStreamHeader() {
    ObjectStreamEntry result;

    auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
    auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

    auto obj_number = ParserUtils::GetIntegerValue(obj_number_token);
    auto offset = ParserUtils::GetIntegerValue(offset_token);

    result.object_number = ValueConvertUtils::SafeConvert<types::big_uint>(obj_number);
    result.offset = ValueConvertUtils::SafeConvert<types::stream_offset>(offset);
    return result;
}

ObjectStreamEntries Parser::ReadObjectStreamHeaders(types::size_type size) {
    ObjectStreamEntries result;
    for (decltype(size) i = 0; i < size; ++i) {
        auto item = ReadObjectStreamHeader();
        result.push_back(item);
    }

    return result;
}

ObjectStreamEntries Parser::ReadObjectStreamEntries(types::big_uint first, types::size_type size) {
    auto entries = ReadObjectStreamHeaders(size);
    for (auto& entry : entries) {
        m_stream->SetInputPosition(first + entry.offset);
        auto obj = ReadDirectObject();

        // Objects within streams shall not be encrypted
        // because streams themselves are encrypted
        obj->SetEncryptionExempted();
        obj->SetInitialized();

        entry.object = obj;
    }

    return entries;
}

ObjectPtr ParserBase::ReadDirectObject(types::stream_offset offset) {
    m_stream->SetInputPosition(offset, SeekDirection::Beginning);
    return ReadDirectObject();
}

ObjectPtr ParserBase::PeekDirectObject() {
    auto position = m_stream->GetInputPosition();
    auto obj = ReadDirectObject();
    m_stream->SetInputPosition(position);

    return obj;
}

#pragma endregion

#pragma region Tokens

TokenPtr ParserBase::ReadTokenSkip() {
    for (;;) {
        auto token = ReadToken();
        if (token->GetType() == Token::Type::EOL)
            continue;

        return token;
    }
}

TokenPtr ParserBase::PeekTokenSkip() {
    auto position = m_stream->GetInputPosition();
    bool rewind = false;
    for (;;) {
        auto token = PeekToken();
        if (token->GetType() == Token::Type::EOL) {
            ReadToken();
            rewind = true;
            continue;
        }

        if (rewind) {
            m_stream->SetInputPosition(position);
        }

        return token;
    }
}

Token::Type ParserBase::PeekTokenTypeSkip() {
    auto token = PeekTokenSkip();
    return token->GetType();
}

TokenPtr ParserBase::ReadTokenWithTypeSkip(Token::Type type) {
    auto offset = m_stream->GetInputPosition();
    for (;;) {
        auto token = ReadToken();

        if (token->GetType() == type) {
            return token;
        }

        if (token->GetType() == Token::Type::EOL) {
            continue;
        }

        throw ParseException(offset);
    }
}

#pragma endregion

#pragma region Xref

XrefEntryBasePtr Parser::ReadTableEntry(types::big_uint objNumber) {
    auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
    auto generation_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

    auto offset = ParserUtils::GetIntegerValue(offset_token);
    auto gen_number = ParserUtils::GetIntegerValue(generation_token);

    // 7.5.4 Cross-Reference Table
    // The maximum generation number is 65,535;
    // when a cross - reference entry reaches this value, it shall never be reused.
    // File: cv_juraj_matys.pdf, Producer(Microsoft Word 2016)
    // 0000000000 65536 f

    if (gen_number > constant::MAX_GENERATION_NUMBER) {
        spdlog::warn("Invalid object generation number {}, converting", gen_number);
        gen_number = constant::MAX_GENERATION_NUMBER;
    }

    auto peeked_token = PeekTokenSkip();
    if (peeked_token->ValueView() == "n") {
        ReadTokenSkip();
        XrefUsedEntryPtr result = make_deferred<XrefUsedEntry>(objNumber, ValueConvertUtils::SafeConvert<types::ushort>(gen_number), ValueConvertUtils::SafeConvert<types::stream_offset>(offset));
        result->SetFile(_file);
        return result;
    }

    if (peeked_token->ValueView() == "f") {
        ReadTokenSkip();
        XrefFreeEntryPtr result = make_deferred<XrefFreeEntry>(objNumber, ValueConvertUtils::SafeConvert<types::ushort>(gen_number));
        result->SetFile(_file);
        return result;
    }

    throw ParseException(m_stream->GetInputPosition());
}

XrefTablePtr Parser::ReadXrefTable() {
    XrefTablePtr table;

    ReadTokenWithTypeSkip(Token::Type::XREF_MARKER);
    while (PeekTokenTypeSkip() != Token::Type::TRAILER) {
        auto revision_base_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
        auto size_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

        auto revision_base_token_view = revision_base_token->ValueView();
        auto size_token_view = size_token->ValueView();

        auto revision_base = MiscUtils::FromChars<types::big_uint>(revision_base_token_view);
        auto size = MiscUtils::FromChars<types::big_uint>(size_token_view);

        for (decltype(size) i = 0; i < size; ++i) {
            auto entry = ReadTableEntry(SafeAddition<types::big_uint>(revision_base, i));
            table->Add(entry);
        }
    }

    ReadTokenWithTypeSkip(Token::Type::TRAILER);
    auto trailer_dictionary = ReadDirectObjectWithType<DictionaryObjectPtr>();
    trailer_dictionary->SetInitialized();

    table->SetTrailerDictionary(trailer_dictionary);
    table->SetFile(_file);
    table->SetInitialized();
    return table;
}

XrefStreamPtr Parser::ParseXrefStream(
    StreamObjectPtr stream,
    types::big_uint stream_obj_number,
    types::ushort stream_gen_number) {
    XrefStreamPtr result;

    // Get stream object data
    auto header = stream->GetHeader();

    auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);

    assert(fields->GetSize() == 3);
    if (fields->GetSize() != 3) {
        LOG_ERROR_AND_THROW(ParseException, "Xref stream /W shall contain three integers, but contains {}", fields->GetSize());
    }

    auto size = header->FindAs<IntegerObjectPtr>(constant::Name::Size);
    ArrayObjectPtr<IntegerObjectPtr> index = { make_deferred<IntegerObject>(0), size };
    if (header->Contains(constant::Name::Index)) {
        index = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::Index);
    }

    auto index_size = index->GetSize();
    assert(index_size % 2 == 0);

    auto body = stream->GetBody();

    auto field1_size = fields->GetValue(0);
    auto field2_size = fields->GetValue(1);
    auto field3_size = fields->GetValue(2);

    types::big_int field1_width = field1_size->GetIntegerValue();
    types::big_int field2_width = field2_size->GetIntegerValue();
    types::big_int field3_width = field3_size->GetIntegerValue();

    // A field can never be wider than the value it is read into. The widths are
    // stream-level constants declared in /W, so they are validated once here
    // rather than on every entry, and each field is named so the error points at it.
    const types::big_int MAXIMUM_FIELD_WIDTH = sizeof(types::big_uint);

    if (field1_width < 0 || field1_width > MAXIMUM_FIELD_WIDTH) {
        LOG_ERROR_AND_THROW(ParseException, "Unsupported cross-reference stream type field width {}", field1_width);
    }

    if (field2_width < 0 || field2_width > MAXIMUM_FIELD_WIDTH) {
        LOG_ERROR_AND_THROW(ParseException, "Unsupported cross-reference stream offset field width {}", field2_width);
    }

    if (field3_width < 0 || field3_width > MAXIMUM_FIELD_WIDTH) {
        LOG_ERROR_AND_THROW(ParseException, "Unsupported cross-reference stream generation field width {}", field3_width);
    }

    // 7.5.8.2 Cross-reference stream dictionary, Table 17
    // The three widths are the byte lengths of the type, offset and generation
    // fields, so their sum is the stride of a single entry. A zero stride would
    // decouple the entry count from the body length: a stream of a few hundred
    // bytes could then declare millions of entries and force that many
    // allocations. Every entry occupies at least one byte.
    if (field1_width + field2_width + field3_width == 0) {
        LOG_ERROR_AND_THROW(ParseException, "Cross-reference stream declares zero-width entries in /W");
    }

    bool contains_self = false;

    // Iterate over entries
    auto it = body.begin();

    // Reads a single big-endian cross-reference field of the given width. A width
    // of zero means the field is not present and takes its default value.
    //
    // The declared entry count and the decoded body length are independent values,
    // both taken from the document, therefore every byte is bounds checked instead
    // of letting the iterator run past the end of the buffer.
    auto read_field = [&it, &body](types::big_int width, types::big_uint default_value) {
        if (width == 0) {
            return default_value;
        }

        types::big_uint result = 0;
        for (types::big_int i = 0; i < width; ++i) {
            if (it == body.end()) {
                LOG_ERROR_AND_THROW(ParseException, "Cross-reference stream body is shorter than the declared entry count");
            }

            result = (result << 8) | static_cast<unsigned char>(*it);
            it += 1;
        }

        return result;
    };

    for (decltype(index_size) i = 0; i < index_size; i += 2) {

        auto subsection_index = index->GetValue(i);
        auto subsection_size = index->GetValue(i + 1);

        for (auto idx = 0; idx < *subsection_size; idx++) {

            // 7.5.8.2 Cross-reference stream dictionary, Table 17
            // "A value of zero for an element in the W array indicates that the corresponding
            // field shall not be present in the stream, and the default value shall be used,
            // if there is one. [...] If the first element is zero, the type field shall not be
            // present, and shall default to Type 1."
            auto field1 = read_field(field1_width, 1);
            auto field2 = read_field(field2_width, 0);
            auto field3 = read_field(field3_width, 0);

            types::big_uint obj_number = SafeAddition<types::big_uint, types::big_uint, int>(*subsection_index, idx);

            // 7.5.4 Cross-reference table
            // "The first entry in the table (object number 0) shall always be free and shall have
            // a generation number of 65,535" and "The maximum generation number is 65,535; when a
            // cross-reference entry reaches this value, it shall never be reused."
            //
            // Producers declaring a generation field wider than two bytes in /W store that value
            // with all bits set across the whole declared width, which no longer fits the 16-bit
            // generation number and used to abort the parse of the whole document.
            // Seen in the wild with /W [1 4 4], where the free list head carries 0xFFFFFFFF.
            //
            // Field 3 only holds a generation number for types 0 and 1,
            // for type 2 it is an index within the object stream.
            auto gen_number = field3;
            if ((field1 == 0 || field1 == 1) && gen_number > constant::MAX_GENERATION_NUMBER) {
                spdlog::warn("Invalid object generation number {}, converting", gen_number);
                gen_number = constant::MAX_GENERATION_NUMBER;
            }

            if (field1 == 0) {
                XrefFreeEntryPtr entry = make_deferred<XrefFreeEntry>(obj_number, ValueConvertUtils::SafeConvert<types::ushort>(gen_number), field2);
                entry->SetFile(_file);
                result->Add(entry);
                continue;
            }

            if (field1 == 1) {

                // 7.5.8.3 Cross-reference stream data, Table 18
                // Field 2 of a type 1 entry is "The byte offset of the object,
                // starting from the beginning of the PDF file".
                //
                // Annex C.1 states that the standard "does not restrict the size or quantity of
                // things described in the PDF file format", while "a particular PDF processor
                // running on a particular device and in a particular operating environment will
                // always have practical limits". The declared width can therefore carry a value
                // larger than the signed offset that addresses the whole document, and no seek
                // could ever reach it. Such an entry is left out, so that reading the object
                // reports it as missing.
                //
                // Reaching this limit takes a document larger than 8 exbibytes, roughly 9.2
                // exabytes, which no storage in reach produces today. Should documents of that
                // size ever become real, widening this branch is not enough on its own, the
                // offset has to grow in the xref entries and in the whole seeking interface.
                if (!ValueConvertUtils::IsInRange<types::stream_offset>(field2)) {
                    spdlog::warn("Skipping cross-reference stream entry {}, the offset {} is beyond the addressable range", obj_number, field2);
                    continue;
                }

                auto entry_gen_number = ValueConvertUtils::SafeConvert<types::ushort>(gen_number);
                XrefUsedEntryPtr entry = make_deferred<XrefUsedEntry>(obj_number, entry_gen_number, ValueConvertUtils::SafeConvert<types::stream_offset>(field2));

                // This case is when XrefStream contains reference to itself
                if (obj_number == stream_obj_number && entry_gen_number == stream_gen_number) {
                    entry->SetReference(stream);
                    entry->SetInitialized();
                    contains_self = true;
                }

                entry->SetFile(_file);
                result->Add(entry);
                continue;
            }

            if (field1 == 2) {
                XrefCompressedEntryPtr entry = make_deferred<XrefCompressedEntry>(obj_number, static_cast<types::ushort>(0), field2, ValueConvertUtils::SafeConvert<types::size_type>(field3));
                entry->SetFile(_file);
                result->Add(entry);
                continue;
            }

            // 7.5.8.3 Cross-reference stream data
            // "In PDF 1.5 through PDF 2.0, only types 0, 1, and 2 are allowed. Any other value
            // shall be interpreted as a reference to the null object, thus permitting new entry
            // types to be defined in the future."
            spdlog::warn("Skipping cross-reference stream entry {} of unknown type {}", obj_number, field1);
        }
    }

    // If stream does not contain entry for itself
    if (!contains_self) {
        if (!_file.IsActive()) {
            throw FileDisposedException();
        }

        auto locked_file = _file.GetReference();
        auto chain = locked_file->GetXrefChain(false);

        // 7.5.8.3 Cross-Reference Stream Data
        // Like any stream, a cross-reference stream shall be an indirect object.
        // Therefore, an entry for it shall exist in either a cross-reference stream (usually itself)
        // or in a cross-reference table (in hybrid-reference files; see 7.5.8.4,
        // "Compatibility with Applications That Do Not Support Compressed Reference Streams").

        // In the document request_for_taxpayer.pdf there is no entry for the Xref stream object
        // Since we do want to have the maximum freedom how the final document is structured
        // we do not modify the existing xref table, however we create a virtual table.
        // The virtual table is used only as metadata to the current document and is
        // NOT stored in the final document.

        if (!chain->Contains(stream_obj_number, stream_gen_number)) {

            // This is let's say a standard procedure, however not well defined in the standard.
            // We are creating a virtual table to keep additional information and it is often
            // not handled properly throughout the source code as it was added later in the development.

            spdlog::info("Could not find object {} {} in the xref chain, creating a virtual table", stream_obj_number, stream_gen_number);

            XrefUsedEntryPtr entry = make_deferred<XrefUsedEntry>(stream_obj_number, stream_gen_number);
            entry->SetFile(_file);

            XrefVirtualTablePtr virtual_xref;
            virtual_xref->Add(entry);
            virtual_xref->SetFile(_file);
            virtual_xref->SetInitialized();

            chain->Append(virtual_xref);
        }

        auto entry = chain->GetXrefEntry(stream_obj_number, stream_gen_number);

        if (!ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryPtr>(entry)) {
            assert(false && "How could this be entry of different type");
            LOG_ERROR_AND_THROW(ParseException, "Xref entry has incorrect type");
        }

        auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(entry);
        used_entry->SetReference(stream);
        used_entry->SetInitialized();
    }

    stream->SetEncryptionExempted();

    result->SetStreamObject(stream);
    result->SetFile(_file);
    result->SetInitialized();
    return result;
}

XrefStreamPtr Parser::ReadXrefStream() {
    types::big_uint stream_obj_number = 0;
    types::ushort stream_gen_number = 0;
    auto indirect = ReadIndirectObject(stream_obj_number, stream_gen_number);
    auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(indirect);
    return ParseXrefStream(stream, stream_obj_number, stream_gen_number);
}

XrefBasePtr Parser::ReadXref(void) {
    if (PeekTokenTypeSkip() == Token::Type::XREF_MARKER) {
        return ReadXrefTable();
    }

    return ReadXrefStream();
}

XrefBasePtr Parser::ReadXref(types::stream_offset offset) {
    m_stream->SetInputPosition(offset, SeekDirection::Beginning);
    XrefBasePtr result = ReadXref();
    result->SetOffset(offset);
    return result;
}

#pragma endregion

HeaderPtr Parser::ReadHeader(types::stream_offset offset) {
    m_stream->SetInputPosition(offset, SeekDirection::Beginning);
    return ReadHeader();
}

HeaderPtr Parser::ReadHeader(void) {
    while (!m_stream->Eof()) {
        auto data = m_stream->Readline();
        std::string line = data->ToString();

        std::smatch sm;
        std::regex header_regex("%PDF-([0-9])\\.([0-9]).*");
        if (!std::regex_match(line, sm, header_regex)) {
            continue;
        }

        HeaderPtr result;

        if (stoi(sm[1]) == 1) {
            switch (stoi(sm[2])) {
                case 0:
                    result->SetVersion(Version::PDF10); break;
                case 1:
                    result->SetVersion(Version::PDF11); break;
                case 2:
                    result->SetVersion(Version::PDF12); break;
                case 3:
                    result->SetVersion(Version::PDF13); break;
                case 4:
                    result->SetVersion(Version::PDF14); break;
                case 5:
                    result->SetVersion(Version::PDF15); break;
                case 6:
                    result->SetVersion(Version::PDF16); break;
                case 7:
                    result->SetVersion(Version::PDF17); break;
                default:
                    LOG_ERROR_AND_THROW(NotSupportedException, "Invalid PDF version: {}", line);
            }

            return result;
        }

        if (stoi(sm[1]) == 2) {
            switch (stoi(sm[2])) {
                case 0:
                    result->SetVersion(Version::PDF20); break;
                default:
                    LOG_ERROR_AND_THROW(NotSupportedException, "Invalid PDF version: {}", line);
            }

            return result;
        }
    }

    LOG_ERROR_AND_THROW(syntax::ParseException, "Could not find PDF header");
}

XrefChainPtr Parser::FindAllObjects(void) {
    XrefChainPtr result;

    XrefTablePtr xref;

    m_stream->SetInputPosition(0);
    while (!m_stream->Eof()) {
        auto offset_before = m_stream->GetInputPosition();

        auto first_token = ReadToken();
        if (first_token->GetType() == Token::Type::END_OF_INPUT) {
            break;
        }

        if (first_token->GetType() == Token::Type::EOL) {
            continue;
        }

        if (first_token->GetType() == Token::Type::TRAILER) {
            auto trailer_dictionary = ReadDictionary();
            trailer_dictionary->SetInitialized();
            xref->SetTrailerDictionary(trailer_dictionary);
            continue;
        }

        if (first_token->GetType() == Token::Type::START_XREF) {
            auto next_token = PeekTokenSkip();
            if (next_token->GetType() == Token::Type::INTEGER_OBJECT) {
                ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
                auto xref_offset = ParserUtils::GetIntegerValue(next_token);
                xref->SetLastXrefOffset(ValueConvertUtils::SafeConvert<types::stream_offset>(xref_offset));
            }

            continue;
        }

        if (first_token->GetType() != Token::Type::INTEGER_OBJECT) {
            if (first_token->GetType() != Token::Type::EOL) {
                m_stream->Readline();
            }

            continue;
        }

        auto gen_number_token = ReadToken();
        if (gen_number_token->GetType() != Token::Type::INTEGER_OBJECT) {
            if (gen_number_token->GetType() != Token::Type::EOL) {
                m_stream->Readline();
            }

            continue;
        }

        auto begin_token = ReadToken();
        if (begin_token->GetType() != Token::Type::INDIRECT_OBJECT_BEGIN) {
            if (begin_token->GetType() != Token::Type::EOL) {
                m_stream->Readline();
            }

            continue;
        }

        auto first_token_view = first_token->ValueView();
        auto gen_number_token_view = gen_number_token->ValueView();

        auto obj_number = MiscUtils::FromChars<types::big_uint>(first_token_view);
        auto gen_number = MiscUtils::FromChars<types::ushort>(gen_number_token_view);

        auto obj = ReadDirectObject();
        obj->SetOffset(offset_before);
        obj->SetFile(_file);
        obj->SetInitialized();

        XrefUsedEntryPtr entry = make_deferred<XrefUsedEntry>(obj_number, gen_number, obj->GetOffset());
        entry->SetReference(obj);
        entry->SetFile(_file);
        entry->SetInitialized();
        xref->Add(entry);

        // Check if read object is Xref stream
        if (ObjectUtils::IsType<StreamObjectPtr>(obj)) {
            auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(obj);
            auto stream_header = stream->GetHeader();
            if (stream_header->Contains(constant::Name::Type)) {
                auto stream_type = stream_header->FindAs<NameObjectPtr>(constant::Name::Type);

                if (constant::Name::XRef == stream_type) {

                    // This is already the recovery path, every object has been found by scanning
                    // the whole document. Parsing the cross-reference stream only recovers the
                    // additional entries for compressed objects, so a damaged one degrades the
                    // result instead of failing the document that we were asked to repair.
                    try {
                        auto xref_stream = ParseXrefStream(stream, obj_number, gen_number);
                        result->Append(xref_stream);
                    } catch (ExceptionBase& e) {
                        spdlog::warn("Could not parse cross-reference stream in object {} {}: {}", obj_number, gen_number, e.what());
                    }
                }
            }
        }

        auto end_token = ReadTokenSkip();
        assert(end_token->GetType() == Token::Type::INDIRECT_OBJECT_END);
    }

    xref->SetFile(_file);
    result->Append(xref);
    return result;
}

} // syntax
} // vanillapdf
