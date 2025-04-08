#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/files/xref_chain.h"
#include "syntax/files/header.h"

#include "syntax/parsers/parser.h"
#include "syntax/parsers/reverse_parser.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include "syntax/utils/encryption.h"
#include "syntax/utils/name_constants.h"
#include "syntax/utils/object_stream_metadata_attribute.h"

#include "utils/streams/input_reverse_stream.h"
#include "utils/streams/input_stream.h"
#include "utils/streams/output_stream.h"
#include "utils/streams/input_output_stream.h"
#include "utils/streams/stream_utils.h"

#include "utils/misc_utils.h"
#include "utils/windows_utils.h"

#include <fstream>
#include <filesystem>

namespace vanillapdf {
namespace syntax {

FilePtr File::Open(const std::string& path) {

    std::ios_base::openmode flags = static_cast<std::ios_base::openmode>(0);
    flags |= std::ios_base::in;
    flags |= std::ios_base::binary;
    flags |= std::ios_base::ate;

    auto input_stream = GetFilestream(path, flags);
    return FilePtr(pdf_new File(input_stream, path));
}

FilePtr File::OpenStream(IInputOutputStreamPtr stream, const std::string& name) {
    return FilePtr(pdf_new File(stream, name));
}

FilePtr File::Create(const std::string& path) {

    std::ios_base::openmode flags = static_cast<std::ios_base::openmode>(0);
    flags |= std::ios_base::in;
    flags |= std::ios_base::out;
    flags |= std::ios_base::binary;
    flags |= std::ios_base::trunc;

    auto input_stream = GetFilestream(path, flags);

    return CreateStream(input_stream, path);
}

FilePtr File::CreateStream(IInputOutputStreamPtr stream, const std::string& name) {

    FilePtr result(pdf_new File(stream, name));

    result->_initialized = true;
    return result;
}

IInputOutputStreamPtr File::GetFilestream(const std::string& path, std::ios_base::openmode mode) {

    auto fs_path = std::filesystem::path(path);

    // On windows there is an issue with unicode filenames.
    // By default the std::string does not work even if the path is in UTF-8 encoding.
    // This can be switched in the OS regional settings, however requires a user interaction.
    // The std::wstring seems to be capable of opening such file, however it's not portable.

#if _WIN32
    fs_path = WindowsUtils::MultiByteToWideChar(path);
#endif /* _WIN32 */

    auto input_file = std::make_shared<std::fstream>();
    input_file->open(fs_path, mode);

    if (!input_file || !input_file->good()) {
        throw GeneralException("Could not open file: " + path + ", errno: " + std::to_string(errno));
    }

    // TODO: Add file open flag support
    //// Support the option, where the file is not held for the entire duration
    //// This is handy, when another tool is generating output files and we need
    //// to quickly reopen the file, without closing the entire application.
    //bool flag = false;
    //if (flag) {
    //	// Seek to the end of file
    //	input_file->seekg(0, std::ios::beg);
    //	
    //	auto input_memory_file = std::make_shared<std::stringstream>();
    //	*input_memory_file << input_file->rdbuf();
    //
    //	return make_deferred<InputOutputStream>(input_memory_file);
    //}

    return make_deferred<InputOutputStream>(input_file);
}

File::File(IInputOutputStreamPtr stream, const std::string& path) : _full_path(path), _input(stream) {
    auto filename = MiscUtils::ExtractFilename(path);

    spdlog::info("File constructor {}", filename);

    _filename = make_deferred_container<Buffer>(filename);
}

File::~File(void) {
    try {
        auto filename = GetFilenameString();
        spdlog::info("File destructor {}", filename);
    }
    catch (std::exception&) {
        // Exceptions in destructor could not be really handled
    }
}

void File::Initialize() {

    if (_initialized) {
        return;
    }

    auto filename = GetFilenameString();

    spdlog::info("File initialization {}", filename);

    _input->ExclusiveInputLock();

    SCOPE_GUARD_CAPTURE_REFERENCES( _input->ExclusiveInputUnlock() );

    // Seek to the end of file
    _input->SetInputPosition(0, SeekDirection::End);

    // Check file size by getting the offset of the end of file
    auto file_size = _input->GetInputPosition();

    Parser stream(GetWeakReference<File>(), _input);
    _header = stream.ReadHeader(0);

    try {
        auto offset = GetLastXrefOffset(file_size);
        ReadXref(offset);
    } catch (NotSupportedException&) {
        throw;
    } catch (ExceptionBase& e) {
        spdlog::error("Could not parse xref chain, using fallback mechanism");
        spdlog::error("Reason: {}", e.what());

        _xref = stream.FindAllObjects();
    }

    // Mark file as initialized
    _initialized = true;

    // Encryption dictionary should be parsed and exepted as first in the order
    // The file "bps_park_rpk.pdf" is encrypted,
    // and the AcroForm dictionary is stored within object stream.
    // Accessing such object fails on decompression, as the stream was not decrypted.
    ExemptEncryptionDictionary();

    // Cross-reference streams could contain information about file signatures
    ExemptCrossReferenceStreams();

    // Signature search needs to be done after the file has been initialized,
    // since we need to process indirect references as well
    ExemptDocumentId();
    ExemptFileSignatures();
}

void File::InitializeObjectStream(types::big_uint object_stream_number) {

    ACCESS_LOCK_GUARD(m_object_stream_lock);

    auto stm = GetIndirectObject(object_stream_number, 0);

    // Mark the object stream with the attribute to prevent duplicit initialization
    if (stm->ContainsAttribute(BaseAttribute::Type::ObjectStreamMetadata)) {
        auto object_stream_attribute = stm->GetAttributeAs<ObjectStreamMetadataAttributePtr>(BaseAttribute::Type::ObjectStreamMetadata);

        // The object stream was already initialized
        if (object_stream_attribute->GetInitialized()) {
            return;
        }
    }

    auto converted = ObjectUtils::ConvertTo<StreamObjectPtr>(stm);
    auto header = converted->GetHeader();
    auto size = header->FindAs<IntegerObjectPtr>(constant::Name::N);
    auto first = header->FindAs<IntegerObjectPtr>(constant::Name::First);
    auto body = converted->GetBody();
    auto input_stream = body->ToInputStream();

    if (body->empty()) {
        throw GeneralException("Could not find data for the ObjStm " + std::to_string(object_stream_number));
    }

    spdlog::debug("Initializing object stream {}", object_stream_number);

    Parser parser(GetWeakReference<File>(), input_stream);
    auto stream_entries = parser.ReadObjectStreamEntries(first->GetUnsignedIntegerValue(), size->SafeConvert<types::size_type>());

    spdlog::trace("Object stream {} has been parsed and contains {} entries", object_stream_number, stream_entries.size());

    for (auto stream_entry : stream_entries) {
        auto entry_object_number = stream_entry.object_number;
        auto entry_object = stream_entry.object;

        auto stream_entry_xref = _xref->GetXrefEntry(entry_object_number, 0);
        auto is_used = ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(stream_entry_xref);

        if (!is_used) {
            spdlog::warn("Object stream {} contains unused xref entry {}", object_stream_number, entry_object_number);
            continue;
        }

        spdlog::trace("Initializing object {} from object stream {}", entry_object_number, object_stream_number);

        auto stream_used_entry_xref = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(stream_entry_xref);
        stream_used_entry_xref->SetReference(entry_object);
        stream_used_entry_xref->SetInitialized();
    }

    ObjectStreamMetadataAttributePtr stream_initialized_attribute;
    stream_initialized_attribute->SetInitialized(true);

    // Mark the object stream as already initialized
    stm->AddAttribute(stream_initialized_attribute);

    spdlog::debug("Initializing object stream {} has finished", object_stream_number);
}

bool File::SetEncryptionKey(IEncryptionKey& key) {
    auto filename = GetFilenameString();

    if (!IsEncrypted()) {
        return false;
    }

    auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
    auto filter = dict->FindAs<NameObjectPtr>(constant::Name::Filter);
    auto sub_filter = dict->FindAs<NameObjectPtr>(constant::Name::SubFilter);
    EncryptionAlgorithm algorithm = EncryptionAlgorithm::None;

    IntegerObjectPtr length_bits = make_deferred<IntegerObject>(40);
    if (dict->Contains(constant::Name::Length)) {
        length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
        assert(length_bits->GetIntegerValue() % 8 == 0 && "Key length is not multiplier of 8");
    }

    ArrayObjectPtr<StringObjectPtr> recipients;
    if (sub_filter == constant::Name::AdbePkcs7s3) {
        recipients = dict->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
    }

    if (sub_filter == constant::Name::AdbePkcs7s4) {
        recipients = dict->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
    }

    if (sub_filter == constant::Name::AdbePkcs7s5) {

        do {
            if (!dict->Contains(constant::Name::CF)) {
                break;
            }

            auto crypt_filter_dictionary = dict->FindAs<DictionaryObjectPtr>(constant::Name::CF);
            if (!crypt_filter_dictionary->Contains(constant::Name::DefaultCryptFilter)) {
                break;
            }

            auto crypt_filter = crypt_filter_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::DefaultCryptFilter);
            if (!crypt_filter->Contains(constant::Name::CFM)) {
                break;
            }

            auto method = crypt_filter->FindAs<NameObjectPtr>(constant::Name::CFM);
            if (method == constant::Name::AESV2) {
                algorithm = EncryptionAlgorithm::AES;
            }

            if (method == constant::Name::AESV3) {
                algorithm = EncryptionAlgorithm::AES;

                if (crypt_filter->Contains(constant::Name::Length)) {
                    length_bits = crypt_filter->FindAs<IntegerObjectPtr>(constant::Name::Length);
                    assert(length_bits->GetIntegerValue() == 256 && "AESV3 length is not 256 bits");
                }
            }

            if (method == constant::Name::None) {
                algorithm = EncryptionAlgorithm::None;
            }

            if (method == constant::Name::V2) {
                algorithm = EncryptionAlgorithm::RC4;
            }

            recipients = crypt_filter->FindAs<ArrayObjectPtr<StringObjectPtr>>(constant::Name::Recipients);
        } while (false);
    }

    try {
        _decryption_key = EncryptionUtils::GetRecipientKey(recipients, length_bits, algorithm, key);
        return true;
    } catch (GeneralException& ex) {
        spdlog::error("Error setting recepient encryption key: {}", ex.what());
        return false;
    } catch (...) {
        spdlog::error("Error setting recepient encryption key");
        return false;
    }
}

bool File::SetEncryptionPassword(const std::string& password) {
    Buffer buffer(password.begin(), password.end());
    return SetEncryptionPassword(buffer);
}

bool File::SetEncryptionPassword(const Buffer& password) {
    if (!IsEncrypted()) {
        return false;
    }

    // Find the proper xref section
    auto xref_iterator = _xref->Begin();
    auto xref = xref_iterator->Value();

    // Find the trailer dictionary
    auto trailer_dictionary = xref->GetTrailerDictionary();

    if (!trailer_dictionary->Contains(constant::Name::ID)) {
        LOG_ERROR_AND_THROW_GENERAL("Trailer dictionary does not contain document ID");
    }

    auto document_ids = trailer_dictionary->FindAs<MixedArrayObjectPtr>(constant::Name::ID);
    if (document_ids->GetSize() == 0) {
        LOG_ERROR_AND_THROW_GENERAL("Document ID list is empty");
    }

    // Identify the document ID object - it could be other types than string
    auto document_id_obj = document_ids[0];

    BufferPtr document_id_buffer;
    if (ObjectUtils::IsType<StringObjectPtr>(document_id_obj)) {
        auto document_id_string = ObjectUtils::ConvertTo<StringObjectPtr>(document_id_obj);
        document_id_buffer = document_id_string->GetValue();
    }

    if (document_id_buffer->empty()) {
        LOG_ERROR_AND_THROW_GENERAL("Could not decrypt document with empty document ID");
    }

    auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
    auto filter = dict->FindAs<NameObjectPtr>(constant::Name::Filter);

    if (filter != constant::Name::Standard) {
        return false;
    }

    auto user_value = dict->FindAs<StringObjectPtr>(constant::Name::U);
    auto owner_value = dict->FindAs<StringObjectPtr>(constant::Name::O);
    auto permissions = dict->FindAs<IntegerObjectPtr>(constant::Name::P);
    auto revision = dict->FindAs<IntegerObjectPtr>(constant::Name::R);
    auto version = dict->FindAs<IntegerObjectPtr>(constant::Name::V);

    IntegerObjectPtr length_bits = make_deferred<IntegerObject>(40);
    if (dict->Contains(constant::Name::Length)) {
        length_bits = dict->FindAs<IntegerObjectPtr>(constant::Name::Length);
        assert(length_bits->GetIntegerValue() % 8 == 0 && "Key length is not multiplier of 8");
    }

    // The value of the P entry shall be interpreted as an unsigned 32-bit quantity
    // containing a set of flags specifying which access permissions shall be granted
    // when the document is opened with user access.

    int64_t permissions_int64_value = permissions->GetIntegerValue();

    // By default it should be sufficient to check only for positive uint32_t limit,
    // however anything lower than FFFFFFFF00000000 = -4294967296 on 64-bit integer
    // would not fall under this condition.

    constexpr uint32_t numeric_limit_max = std::numeric_limits<uint32_t>::max();
    int64_t numeric_limit_min = -static_cast<int64_t>(numeric_limit_max);

    if (permissions_int64_value < numeric_limit_min || permissions_int64_value > numeric_limit_max) {
        spdlog::warn("Permission field is outside specified threshold ({}), truncating", permissions_int64_value);
    }

    // This seems very dirty, however it works, so just clean hands afterwards.
    uint32_t permissions_uint32_value = static_cast<uint32_t>(permissions_int64_value);

    auto permissions_value = ValueConvertUtils::SafeConvert<uint32_t>(permissions_uint32_value);
    auto revision_value = ValueConvertUtils::SafeConvert<int32_t>(revision->GetIntegerValue());
    auto key_length_value = ValueConvertUtils::SafeConvert<int32_t>(length_bits->GetIntegerValue());

    // Pad password with predefined scheme
    BufferPtr padPassword = EncryptionUtils::PadTruncatePassword(password);
    BufferPtr encrypted_owner_data = EncryptionUtils::ComputeAuthenticationOwnerData(padPassword, dict);

    // Check if entered password was owner password
    BufferPtr temp_decryption_key;
    if (EncryptionUtils::CheckKey(
        encrypted_owner_data, document_id_buffer, owner_value->GetValue(), user_value->GetValue(),
        permissions_value, revision_value, key_length_value, temp_decryption_key)) {
        _decryption_key = temp_decryption_key;
        return true;
    }

    // Check if entered password was user password
    if (EncryptionUtils::CheckKey(
        padPassword, document_id_buffer, owner_value->GetValue(), user_value->GetValue(),
        permissions_value, revision_value, key_length_value, temp_decryption_key)) {
        _decryption_key = temp_decryption_key;
        return true;
    }

    return false;
}

bool File::IsEncrypted(void) const {
    auto filename = GetFilenameString();

    if (!_initialized) {
        throw FileNotInitializedException(filename);
    }

    return (!_encryption_dictionary.empty() && _encryption_dictionary != NullObject::GetInstance());
}

BufferPtr File::DecryptStream(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber) {
    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
    auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

    if (version == 4 || version == 5) {
        auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StmF);
        return DecryptData(data, objNumber, genNumber, filter_name);
    }

    return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::DecryptString(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber) {
    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
    auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

    if (version == 4 || version == 5) {
        auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StrF);
        return DecryptData(data, objNumber, genNumber, filter_name);
    }

    return DecryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::DecryptData(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber,
    const NameObject& filter_name) {
    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    EncryptionAlgorithm algorithm = GetEncryptionAlgorithmForFilter(filter_name);
    return DecryptData(data, objNumber, genNumber, algorithm);
}

BufferPtr File::DecryptData(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber,
    EncryptionAlgorithm alg) {
    // Object number zero is most probably invalid
    // I'd like to see the document where object number zero
    // is encrypted, because it most probably violates many
    // rules and assertions I already have
    assert(objNumber != 0);

    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    // Decrypting empty content should result in empty content as well
    // For AES this is also missing the IV, so the application crashes
    if (data.size() == 0) {
        return make_deferred_container<Buffer>();
    }

    auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);

    // Check if we are trying to decrypt data inside encryption dictionary
    if (encryption_dictionary->IsIndirect()) {
        auto dictionary_object_number = encryption_dictionary->GetObjectNumber();
        auto dictionary_generation_number = encryption_dictionary->GetGenerationNumber();

        // The idea behind this was originally check whether we are dealing with
        // encryption dictionary based on object and generation number.
        // However, encryption dictionary does not have to be indirect object
        // therefore I have created SetEncryptionExempted property, which shall
        // prohibit any decryption on such objects. If this triggers
        // error in encryption exemption most probably occurred
        assert(!(dictionary_object_number == objNumber
            && dictionary_generation_number == genNumber)
            && "Encryption dictionary shall be exempted from encryption");

        // data inside encryption dictionary are not encrypted
        if ((dictionary_object_number == objNumber && dictionary_generation_number == genNumber)) {
            return make_deferred_container<Buffer>(data);
        }
    }

    // Same idea as above. SetEncryptionExempted shall be used for every
    // compressed object and there should not by any call trying to decrypt
    // compressed objects. They are contained in streams which themselves are compressed
    auto object_entry = _xref->GetXrefEntry(objNumber, genNumber);
    assert(object_entry->GetUsage() != XrefEntryBase::Usage::Compressed
        && "Compressed objects shall be exempted from encryption");

    // Any strings that are inside streams such as content streams and compressed object streams,
    // which themselves are encrypted
    if (object_entry->GetUsage() == XrefEntryBase::Usage::Compressed) {
        return make_deferred_container<Buffer>(data);
    }

    if (_decryption_key.empty()) {

        // Encrypted documents shall be opened with default empty password
        bool passed = SetEncryptionPassword("");
        if (!passed) {
            throw InvalidPasswordException("Could not decrypt file using default password");
        }
    }

    // AES 256 bits behaves differently
    // It was not part of the core specification, but it was added in extension
    if (_decryption_key->size() == 32 && alg == EncryptionAlgorithm::AES) {
        return EncryptionUtils::AESDecrypt(_decryption_key, 32, data);
    }

    BufferPtr object_key = EncryptionUtils::ComputeObjectKey(_decryption_key, objNumber, genNumber, alg);

    switch (alg) {
        default:
        case EncryptionAlgorithm::None:
            // The application shall not decrypt data but shall direct the input stream to the security handler for decryption.
            // No clue
        case EncryptionAlgorithm::RC4:
            return EncryptionUtils::ComputeRC4(object_key, data);
        case EncryptionAlgorithm::AES:
            return EncryptionUtils::AESDecrypt(object_key, data);
    }
}

BufferPtr File::EncryptStream(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber) {
    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
    auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

    if (version == 4 || version == 5) {
        auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StmF);
        return EncryptData(data, objNumber, genNumber, filter_name);
    }

    return EncryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::EncryptString(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber) {
    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
    auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

    if (version == 4 || version == 5) {
        auto filter_name = encryption_dictionary->FindAs<NameObjectPtr>(constant::Name::StrF);
        return EncryptData(data, objNumber, genNumber, filter_name);
    }

    return EncryptData(data, objNumber, genNumber, EncryptionAlgorithm::RC4);
}

BufferPtr File::EncryptData(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber,
    const NameObject& filter_name) {
    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    EncryptionAlgorithm algorithm = GetEncryptionAlgorithmForFilter(filter_name);
    return EncryptData(data, objNumber, genNumber, algorithm);
}

BufferPtr File::EncryptData(const Buffer& data,
    types::big_uint objNumber,
    types::ushort genNumber,
    EncryptionAlgorithm alg) {
    if (!IsEncrypted()) {
        return make_deferred_container<Buffer>(data);
    }

    if (_decryption_key.empty()) {

        // Encrypted documents shall be opened with default empty password
        bool passed = SetEncryptionPassword("");
        if (!passed) {
            throw InvalidPasswordException("Could not decrypt file using default password");
        }
    }

    auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);

    // Check if we are trying to encrypt data inside encryption dictionary
    if (encryption_dictionary->IsIndirect()) {
        auto dictionary_object_number = encryption_dictionary->GetObjectNumber();
        auto dictionary_generation_number = encryption_dictionary->GetGenerationNumber();

        // data inside encryption dictionary are not encrypted
        if (objNumber == 0 || (dictionary_object_number == objNumber && dictionary_generation_number == genNumber)) {
            return make_deferred_container<Buffer>(data);
        }
    }

    // AES 256 bits behaves differently
    // It was not part of the core specification, but it was added in extension
    if (_decryption_key->size() == 32 && alg == EncryptionAlgorithm::AES) {
        return EncryptionUtils::AESEncrypt(_decryption_key, 32, data);
    }

    BufferPtr object_key = EncryptionUtils::ComputeObjectKey(_decryption_key, objNumber, genNumber, alg);

    switch (alg) {
        default:
        case EncryptionAlgorithm::None:
            // The application shall not decrypt data but shall direct the input stream to the security handler for decryption.
            // No clue
        case EncryptionAlgorithm::RC4:
            return EncryptionUtils::ComputeRC4(object_key, data);
        case EncryptionAlgorithm::AES:
            return EncryptionUtils::AESEncrypt(object_key, data);
    }
}

EncryptionAlgorithm File::GetEncryptionAlgorithmForFilter(const NameObject& filter_name) {
    auto encryption_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(_encryption_dictionary);
    auto version = encryption_dictionary->FindAs<IntegerObjectPtr>(constant::Name::V);

    do {
        if (version != 4 && version != 5) break;
        if (!encryption_dictionary->Contains(constant::Name::CF)) break;

        auto crypt_filter_dictionary = encryption_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::CF);
        if (!crypt_filter_dictionary->Contains(filter_name)) break;

        auto crypt_filter = crypt_filter_dictionary->FindAs<DictionaryObjectPtr>(filter_name);
        if (!crypt_filter->Contains(constant::Name::CFM)) break;

        auto method = crypt_filter->FindAs<NameObjectPtr>(constant::Name::CFM);
        if (method == constant::Name::AESV2 || method == constant::Name::AESV3) {
            return EncryptionAlgorithm::AES;
        }

        if (method == constant::Name::None) {
            return EncryptionAlgorithm::None;
        }

        if (method == constant::Name::V2) {
            return EncryptionAlgorithm::RC4;
        }
    } while (false);

    return EncryptionAlgorithm::None;
}

void File::ReadXref(types::stream_offset offset) {
    Parser stream(GetWeakReference<File>(), _input);

    // Keep track of current offset to xref
    types::stream_offset current_offset = offset;

    // Clear all previously allocated xrefs
    _xref->Clear();

    for (;;) {
        auto xref = stream.ReadXref(current_offset);

        _xref->Append(xref);

        auto trailer_dictionary = xref->GetTrailerDictionary();
        if (!trailer_dictionary->Contains(constant::Name::Prev)) {
            break;
        }

        auto prev = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::Prev);
        current_offset = prev->GetIntegerValue();
    }

    // After all regular files have been parsed look for a hybrid entry
    // In file "annotation-border-styles.pdf" there is object defined in the
    // section that would not have been parsed

    for (auto& xref : _xref) {

        if (!ConvertUtils<XrefBasePtr>::IsType<XrefTablePtr>(xref)) {
            continue;
        }

        auto trailer_dictionary = xref->GetTrailerDictionary();
        if (trailer_dictionary->Contains(constant::Name::XRefStm)) {
            auto stm_offset_obj = trailer_dictionary->FindAs<IntegerObjectPtr>(constant::Name::XRefStm);
            auto stm_offset = stm_offset_obj->GetIntegerValue();
            auto hybrid_xref = stream.ReadXref(stm_offset);

            if (!ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(hybrid_xref)) {
                throw GeneralException("Hybrid xref is not a stream");
            }

            auto hybrid_xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(hybrid_xref);

            auto xref_table = ConvertUtils<XrefBasePtr>::ConvertTo<XrefTablePtr>(xref);
            xref_table->SetHybridStream(hybrid_xref_stream);
        }
    }

    // Put the virtual table to the end to avoid disruptions of chain->Begin()
    if (_xref->GetSize() > 0) {

        // TODO: There may be multiple virtual tables
        auto first_xref = _xref->Begin()->Value();
        if (first_xref->GetType() == XrefBase::Type::Virtual) {
            _xref->PopFront();
            _xref->Append(first_xref);
        }
    }
}

types::stream_offset File::GetLastXrefOffset(types::stream_size file_size) {
    //InputReverseStreamPtr raw_reversed = make_deferred<InputReverseStream>(_input, file_size);
    //auto reverse_stream = ReverseParser(raw_reversed);

    // TODO:
    // 16 kilobytes seems to be reasonable size from the end of the document.
    // There are some documents explicitly testing 4 kilobytes.
    // For the time being it is sufficient, however a proper solution is desired.
    // The InputReverseStream had some issues on MAC platform, so it was disabled.
    types::stream_size to_read = 16 * 1024;

    if (file_size < to_read) {
        to_read = file_size;
    }

    _input->SetInputPosition(-to_read, SeekDirection::End);
    if (_input->IsFail()) {
        throw GeneralException("Failed to seek for reading the file trailer");
    }

    BufferPtr file_trailer(make_deferred_container<Buffer>(to_read));
    auto bytes_read = _input->Read(file_trailer, to_read);
    if (bytes_read != to_read) {
        throw GeneralException("Failed to read file trailer");
    }

    std::reverse(file_trailer.begin(), file_trailer.end());
    auto memory_stream = StreamUtils::InputStreamFromBuffer(file_trailer);
    auto reverse_stream = ReverseParser(memory_stream);

    return reverse_stream.ReadLastXrefOffset();
}

ObjectPtr File::GetIndirectObject(
    types::big_uint obj_number,
    types::ushort gen_number) const {

    if (!_initialized) {
        auto filename = GetFilenameString();
        throw FileNotInitializedException(filename);
    }

    return GetIndirectObjectInternal(obj_number, gen_number);
}

ObjectPtr File::GetIndirectObjectInternal(
    types::big_uint obj_number,
    types::ushort gen_number) const {

    spdlog::debug("GetIndirectObject {} and {}", obj_number, gen_number);

    if (!_xref->Contains(obj_number, gen_number)) {
        return NullObject::GetInstance();
    }

    auto item = _xref->GetXrefEntry(obj_number, gen_number);

    switch (item->GetUsage()) {
        case XrefEntryBase::Usage::Used:
        {
            auto used = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(item);
            auto result = used->GetReference();

            // Park-University-USA.pdf
            // In this file the XREF entry does not match the indirect object header.
            // The object is present, but we do a workaround by initializing all of the other objects,
            // in order to find the correct one.

            // This logic was previously in the parser, which loaded all of the objects immediately.
            // The downside of the approach was slower loading time for all the files,
            // even though they did not have this issue.

            // With this approach the performance impact should be negligible and still fix the issue.

            if (item->GetObjectNumber() != obj_number ||
                item->GetGenerationNumber() != gen_number) {

                spdlog::warn("Xref entry {} {} is actually pointing to object {} {}, trying to recover",
                    obj_number,
                    gen_number,
                    item->GetObjectNumber(),
                    item->GetGenerationNumber());

                // Force the entry initialization
                // In some cases the XREF numbering does not match the actual object
                // The object numbering does take precedence before the XREF
                // Forcing the initialization will immediately resolve the differences
                for (auto xref_table : _xref) {
                    for (auto xref_entry : xref_table) {
                        auto is_used_entry = ConvertUtils<XrefEntryBasePtr>::IsType<XrefUsedEntryBasePtr>(xref_entry);
                        if (!is_used_entry) {
                            continue;
                        }

                        auto used_entry = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryBasePtr>(xref_entry);
                        used_entry->GetReference();
                    }

                    // After all of the objects have been initialized, we need to rebuild xref map indexes.
                    // The issue was observed when using unordered_set, that the object could not be found.
                    // Chaning the object key is not a way to go, so we remove and reinsert all entries.

                    // Store entries before removal
                    auto entries = xref_table->Entries();

                    // Remove all xref entries
                    xref_table->Clear();

                    // Reinsert all of them once again to rebuild the hash buckets
                    for (auto& entry : entries) {
                        xref_table->Add(entry);
                    }
                }

                // Find the item once again after the object reinitialization has finished
                item = _xref->GetXrefEntry(obj_number, gen_number);

                // In case the type of the entry has changed we will get terminated
                used = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefUsedEntryPtr>(item);
                result = used->GetReference();

                // The object numbers could have changed according to parsing
                assert(item->GetObjectNumber() == obj_number && "Could not find correct xref entry");
                assert(item->GetGenerationNumber() == gen_number && "Could not find correct xref entry");
            }

            return result;
        }
        case XrefEntryBase::Usage::Compressed:
        {
            auto compressed = ConvertUtils<XrefEntryBasePtr>::ConvertTo<XrefCompressedEntryPtr>(item);
            return compressed->GetReference();
        }
        case XrefEntryBase::Usage::Null:
            spdlog::error("Xref entry type is null for object {} {}", obj_number, gen_number);
            return NullObject::GetInstance();
        case XrefEntryBase::Usage::Free:
            return NullObject::GetInstance();
        default:
            throw GeneralException("Unknown xref entry type: " + std::to_string(static_cast<int>(item->GetUsage())));
    }
}

HeaderPtr File::GetHeader(void) const {

    if (!_initialized) {
        auto filename = GetFilenameString();
        throw FileNotInitializedException(filename);
    }

    return _header;
}

void File::SetHeader(HeaderPtr header) {
    _header = header;
}

XrefChainPtr File::GetXrefChain() const {
    return GetXrefChain(true);
}

XrefChainPtr File::GetXrefChain(bool check_initialization) const {
    if (check_initialization && !_initialized) {
        auto filename = GetFilenameString();
        throw FileNotInitializedException(filename);
    }

    return _xref;
}


XrefUsedEntryBasePtr File::AllocateNewEntry() {

    if (_xref.empty()) {
        XrefTablePtr new_table;
        _xref->Append(new_table);
    }

    for (types::big_uint i = m_next_allocation; i < std::numeric_limits<types::big_uint>::max(); ++i) {

        types::size_type eligible = 0;
        for (auto it = _xref.begin(); it != _xref.end(); it++) {
            auto xref = (*it);

            if (!xref->Contains(i)) {
                eligible++;
                continue;
            }

            auto item = xref->Find(i);
            if (item->GetUsage() != XrefEntryBase::Usage::Free) {
                continue;
            }

            // Disable free entries with max generation number
            if (item->GetGenerationNumber() == constant::MAX_GENERATION_NUMBER) {
                continue;
            }

            // TODO: I've tried reusing free entries in xref streams
            // but somehow it didn't worked. This would require
            // deleteing unused free entries and replacing them
            // is creating new pdf file or creating additional xref
            // for these new entries
            //eligible++;
        }

        if (eligible != _xref->GetSize()) {
            continue;
        }

        auto xref_it = _xref.begin();
        auto xref = *xref_it;
        types::ushort gen_number = 0;

        //if (xref->Contains(i)) {
        //	auto item = xref->Find(i);
        //	if (item->GetUsage() == XrefEntryBase::Usage::Free) {
        //		gen_number = item->GetGenerationNumber() + 1;
        //	}
        //}

        XrefUsedEntryPtr new_entry = make_deferred<XrefUsedEntry>(i, gen_number, -1);

        // Newly allocated entries are marked as dirty, so when calculating incremental update
        // differences we can check for new entries as well for changed entries
        new_entry->SetDirty();

        // Immediately associate current file, as it is tightly coupled
        new_entry->SetFile(GetWeakReference<File>());

        // Initialized flag prevents to seek within the input file,
        // which is not wanted for newly allocated entries
        new_entry->SetInitialized();

        xref->Add(new_entry);

        m_next_allocation = i + 1;
        return new_entry;
    }

    throw GeneralException("Unable to allocate new entry");
}

void File::FixObjectReferences(const std::map<ObjectPtr, ObjectPtr>& map, std::map<ObjectPtr, bool>& visited, ObjectPtr copied) {
    auto has_visited = visited.find(copied);
    if (visited.end() != has_visited && has_visited->second) {
        // already visited
        return;
    }

    // Visit
    visited[copied] = true;

    if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(copied)) {
        auto copied_ref = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(copied);

        bool found = false;
        for (auto item : map) {
            auto original_obj = item.first;
            if (!original_obj->IsIndirect()) {
                continue;
            }

            if (original_obj->GetObjectNumber() != copied_ref->GetReferencedObjectNumber()
                || original_obj->GetGenerationNumber() != copied_ref->GetReferencedGenerationNumber()) {
                continue;
            }

            auto copied_referenced_obj = item.second;
            copied_ref->SetReferencedObjectNumber(copied_referenced_obj->GetObjectNumber());
            copied_ref->SetReferencedGenerationNumber(copied_referenced_obj->GetGenerationNumber());

            FixObjectReferences(map, visited, copied_referenced_obj);

            found = true;
            break;
        }

        assert(found && "Found reference which was not copied"); UNUSED(found);
    }

    if (ObjectUtils::IsType<MixedArrayObjectPtr>(copied)) {
        auto arr = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(copied);
        for (auto item : arr) {
            FixObjectReferences(map, visited, item);
        }
    }

    if (ObjectUtils::IsType<DictionaryObjectPtr>(copied)) {
        auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(copied);
        for (auto item : dict) {
            FixObjectReferences(map, visited, item.second);
        }
    }

    if (ObjectUtils::IsType<StreamObjectPtr>(copied)) {
        auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(copied);
        auto dict = stream->GetHeader();
        for (auto item : dict) {
            FixObjectReferences(map, visited, item.second);
        }
    }
}

void File::ShallowCopyObject(std::map<ObjectPtr, ObjectPtr>& map, ObjectPtr original) {
    auto found = map.find(original);
    if (map.end() != found) {
        // already copied
        return;
    }

    // Copy only indirect objects
    if (original->IsIndirect()) {
        auto new_obj = ShallowCopyObject(original);

        auto pair = std::pair<ObjectPtr, ObjectPtr>(original, new_obj);
        map.insert(pair);
    }
}

ObjectPtr File::ShallowCopyObject(ObjectPtr original) {
    ObjectPtr new_obj(original->Clone());

    if (original->IsIndirect()) {
        auto new_entry = AllocateNewEntry();
        new_entry->SetReference(new_obj);
        new_entry->SetInitialized();
    }

    new_obj->SetFile(GetWeakReference<File>());
    new_obj->SetInitialized();

    // Mark all new objects as dirty, so they are included in the xref incremental section.
    // The dirty flag is also important for streams, since the original file might not be encrypted
    // and the the stream content recalculation does have specific optimizations for unchanged objects.
    // 
    // If this causes issues just add a flag to all deep/shallow copy
    // functions whether it should mark objects as dirty.
    // For the time being this is only called from append page/document.
    new_obj->SetDirty();

    return new_obj;
}

void File::DeepCopyObject(std::map<ObjectPtr, ObjectPtr>& map, std::map<ObjectPtr, bool>& visited, ObjectPtr original) {
    auto has_visited = visited.find(original);
    if (visited.end() != has_visited && has_visited->second) {
        // already visited
        return;
    }

    // Visit
    visited[original] = true;

    ShallowCopyObject(map, original);

    if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(original)) {
        auto ref = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(original);
        auto referenced_obj = ref->GetReferencedObject();
        DeepCopyObject(map, visited, referenced_obj);
    }

    if (ObjectUtils::IsType<MixedArrayObjectPtr>(original)) {
        auto arr = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(original);
        for (auto item : arr) {
            DeepCopyObject(map, visited, item);
        }
    }

    if (ObjectUtils::IsType<DictionaryObjectPtr>(original)) {
        auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(original);
        for (auto item : dict) {
            DeepCopyObject(map, visited, item.second);
        }
    }

    if (ObjectUtils::IsType<StreamObjectPtr>(original)) {
        auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(original);
        auto dict = stream->GetHeader();
        for (auto item : dict) {
            DeepCopyObject(map, visited, item.second);
        }
    }
}

std::vector<ObjectPtr> File::DeepCopyObject(ObjectPtr object) {
    std::vector<ObjectPtr> objects { object };
    return DeepCopyObjects(objects);
}

std::vector<ObjectPtr> File::DeepCopyObjects(const std::vector<ObjectPtr>& objects) {
    // Original to copied mapping
    std::map<ObjectPtr, ObjectPtr> map;

    std::map<ObjectPtr, bool> copy_visited;
    for (auto obj : objects) {
        DeepCopyObject(map, copy_visited, obj);
    }

    // Fix references
    std::map<ObjectPtr, bool> ref_visited;
    for (auto check : map) {
        FixObjectReferences(map, ref_visited, check.second);
    }

    std::vector<ObjectPtr> result;
    for (auto item : objects) {
        auto found = map.find(item);
        assert(found != map.end());
        result.push_back(found->second);
    }

    return result;
}

IInputStreamPtr File::GetInputStream(void) {
    return _input;
}

IOutputStreamPtr File::GetOutputStream(void) {
    return _input;
}

IInputOutputStreamPtr File::GetInputOutputStream(void) {
    return _input;
}

BufferPtr File::GetByteRange(types::stream_size begin, types::size_type length) {
    if (!_initialized) {
        auto filename = GetFilenameString();
        throw FileNotInitializedException(filename);
    }

    _input->SetInputPosition(begin);
    return _input->Read(length);
}

IInputStreamPtr File::GetByteRangeStream(types::stream_size begin, types::size_type length) {
    if (!_initialized) {
        auto filename = GetFilenameString();
        throw FileNotInitializedException(filename);
    }

    // Avoid huge allocation and use filtering stream buffer
    _input->SetInputPosition(begin);
    auto buffer = _input->Read(length);
    return buffer->ToInputStream();
}

void File::ExemptEncryptionDictionary() {

    // Find the proper xref section
    auto xref_iterator = _xref->Begin();
    auto xref = xref_iterator->Value();

    // Find the trailer dictionary
    auto trailer_dictionary = xref->GetTrailerDictionary();

    // After xref informations are parsed, check for encryption
    if (trailer_dictionary->Contains(constant::Name::Encrypt)) {
        _encryption_dictionary = trailer_dictionary->Find(constant::Name::Encrypt);

        if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(_encryption_dictionary)) {
            auto reference = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(_encryption_dictionary);
            auto obj_number = reference->GetReferencedObjectNumber();
            auto gen_number = reference->GetReferencedGenerationNumber();

            _encryption_dictionary = GetIndirectObjectInternal(obj_number, gen_number);
        }

        // Encryption dictionary itself is not encrypted
        _encryption_dictionary->SetEncryptionExempted();
    }
}

void File::ExemptDocumentId() {

    // ID entries in trailer are exempted from encryption
    for (auto xref : _xref) {
        auto trailer_dictionary = xref->GetTrailerDictionary();

        if (trailer_dictionary->Contains(constant::Name::ID)) {
            auto ids = trailer_dictionary->Find(constant::Name::ID);
            assert(!ObjectUtils::IsType<IndirectReferenceObjectPtr>(ids) && "Document ID is indirect reference");

            // I have found document, that have document ID as a single integer "77777777777777777" issue5599.pdf.
            // Adobe acrobat can open this document without any notifications,
            // therefore following requirement from specification 7.5.5 is considered invalid:
            // "An array of two byte-strings constituting a file identifier"
            //assert(ObjectUtils::IsType<ArrayObjectPtr<StringObjectPtr>>(ids) && "Document ID is not array of strings");

            ids->SetEncryptionExempted();
        }
    }
}

void File::ExemptFileSignatures() {

    // Find the proper xref section
    auto xref_iterator = _xref->Begin();
    auto xref = xref_iterator->Value();

    // Find the trailer dictionary
    auto trailer_dictionary = xref->GetTrailerDictionary();

    // File signatures are also exempted from the encryption
    if (!trailer_dictionary->Contains(constant::Name::Root)) {
        return;
    }

    auto document_root_obj = trailer_dictionary->Find(constant::Name::Root);

    if (!ObjectUtils::IsType<DictionaryObjectPtr>(document_root_obj)) {
        return;
    }

    auto document_root = ObjectUtils::ConvertTo<DictionaryObjectPtr>(document_root_obj);
    if (!document_root->Contains(constant::Name::AcroForm)) {
        return;
    }

    auto acro_form_dictionary = document_root->FindAs<DictionaryObjectPtr>(constant::Name::AcroForm);
    if (!acro_form_dictionary->Contains(constant::Name::Fields)) {
        return;
    }

    auto fields_reference_list = acro_form_dictionary->FindAs<MixedArrayObjectPtr>(constant::Name::Fields);
    for (auto field_item_reference : fields_reference_list) {
        if (!ObjectUtils::IsType<DictionaryObjectPtr>(field_item_reference)) {
            continue;
        }

        auto field_item = ObjectUtils::ConvertTo<DictionaryObjectPtr>(field_item_reference);
        if (!field_item->Contains(constant::Name::FT)) {
            continue;
        }

        auto field_item_type = field_item->FindAs<NameObjectPtr>(constant::Name::FT);
        if (field_item_type != constant::Name::Sig) {
            continue;
        }

        if (!field_item->Contains(constant::Name::V)) {
            continue;
        }

        auto signature_dictionary = field_item->FindAs<DictionaryObjectPtr>(constant::Name::V);
        if (!signature_dictionary->Contains(constant::Name::Contents)) {
            continue;
        }

        auto signature_contents = signature_dictionary->FindAs<HexadecimalStringObjectPtr>(constant::Name::Contents);
        signature_contents->SetEncryptionExempted();
    }
}

void File::ExemptCrossReferenceStreams() {

    for (auto xref : _xref) {

        if (!ConvertUtils<XrefBasePtr>::IsType<XrefStreamPtr>(xref)) {
            continue;
        }

        auto xref_stream = ConvertUtils<XrefBasePtr>::ConvertTo<XrefStreamPtr>(xref);
        auto xref_stream_dictionary = xref_stream->GetTrailerDictionary();

        // The cross-reference stream shall not be encrypted and strings appearing in the cross-reference stream dictionary
        // shall not be encrypted. It shall not have a Filter entry that specifies a Crypt filter (see 7.4.10, "Crypt Filter").
        xref_stream_dictionary->SetEncryptionExempted();
    }
}

} // syntax
} // vanillapdf
