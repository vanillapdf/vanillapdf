#include "precompiled.h"
#include "parser.h"
#include "exception.h"
#include "token.h"
#include "file.h"
#include "character.h"

#include "reverse_parser.h"
#include "raw_reverse_stream.h"

#include "content_stream_operations.h"
#include "content_stream_operation_generic.h"
#include "content_stream_operators.h"
#include "content_stream_objects.h"
#include "content_utils.h"

#include <regex>

namespace gotchangpdf
{
	namespace syntax
	{
#pragma region Objects

		ParserBase::ParserBase(std::weak_ptr<File> file, CharacterSource & stream)
			: Tokenizer(stream), _file(file) {}

		Parser::Parser(std::weak_ptr<File> file, CharacterSource & stream)
			: ParserBase(file, stream)
		{
			_dictionary = std::make_unique<ParserTokenDictionary>();
			_dictionary->Initialize();
		}

		ContentStreamParser::ContentStreamParser(std::weak_ptr<File> file, CharacterSource & stream)
			: ParserBase(file, stream)
		{
			_dictionary = std::make_unique<ContentStreamTokenDictionary>();
			_dictionary->Initialize();
		}

		std::weak_ptr<File> ParserBase::GetFile(void) const { return _file; }

		class ObjectFactory
		{
		public:
			static BooleanObjectPtr CreateBoolean(TokenPtr token)
			{
				if (token->GetType() == Token::Type::TRUE_VALUE)
					return BooleanObjectPtr(true);

				if (token->GetType() == Token::Type::FALSE_VALUE)
					return BooleanObjectPtr(false);

				assert(!"Expected boolean token type");
				throw GeneralException("Expected boolean token type");
			}

			static IntegerObjectPtr CreateInteger(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::INTEGER_OBJECT && "Expected integer token type");

				auto buffer = token->Value();
				auto value = std::stoll(buffer->ToString());
				return IntegerObjectPtr(value);
			}

			static RealObjectPtr CreateReal(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::REAL_OBJECT && "Expected real token type");

				auto buffer = token->Value();
				auto str = buffer->ToString();
				auto value = std::stod(str);
				auto pos = str.rfind('.');
				if (-1 != pos) {
					auto precision = str.size() - pos - 1;
					auto converted = ValueConvertUtils::SafeConvert<uint32_t>(precision);
					return RealObjectPtr(value, converted);
				}

				return RealObjectPtr(value);
			}

			static NameObjectPtr CreateName(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::NAME_OBJECT && "Expected name token type");

				auto buffer = token->Value();
				return NameObjectPtr(buffer);
			}

			static HexadecimalStringObjectPtr CreateHexString(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::HEXADECIMAL_STRING && "Expected hexadecimal string token type");

				auto buffer = token->Value();
				return HexadecimalStringObjectPtr(buffer);
			}

			static LiteralStringObjectPtr CreateLitString(TokenPtr token)
			{
				assert(token->GetType() == Token::Type::LITERAL_STRING && "Expected literal string token type");

				auto buffer = token->Value();
				return LiteralStringObjectPtr(buffer);
			}
		};

		IntegerObjectPtr ParserBase::ReadInteger()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto result = ObjectFactory::CreateInteger(token);
			result->SetFile(_file);
			return result;
		}

		ObjectPtr ParserBase::ReadIndirectReference()
		{
			auto integer = ReadInteger();

			auto pos = GetPosition();
			if (PeekTokenTypeSkip() == Token::Type::INTEGER_OBJECT) {
				auto ahead = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
				auto gen_number = ObjectFactory::CreateInteger(ahead);

				if (PeekTokenTypeSkip() == Token::Type::INDIRECT_REFERENCE_MARKER) {
					auto reference_marker = ReadTokenWithTypeSkip(Token::Type::INDIRECT_REFERENCE_MARKER);
					IndirectObjectReferencePtr result(integer->GetUnsignedIntegerValue(), gen_number->SafeConvert<types::ushort>());
					result->SetFile(_file);
					return result;
				}

				// TODO we can peek only one next token, therefore we need to seek back
				SetPosition(pos);
			}

			return integer;
		}

		RealObjectPtr ParserBase::ReadReal()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::REAL_OBJECT);
			auto result = ObjectFactory::CreateReal(token);
			result->SetFile(_file);
			return result;
		}

		NullObjectPtr ParserBase::ReadNull()
		{
			ReadTokenWithTypeSkip(Token::Type::NULL_OBJECT);
			auto result = NullObject::GetInstance();
			result->SetFile(_file);
			return result;
		}

		DictionaryObjectPtr ParserBase::ReadDictionary()
		{
			DictionaryObjectPtr dictionary;
			ReadTokenWithTypeSkip(Token::Type::DICTIONARY_BEGIN);
			while (PeekTokenTypeSkip() != Token::Type::DICTIONARY_END)
			{
				auto name = ReadDirectObjectWithType<NameObjectPtr>();
				auto val = ReadDirectObject();

				if (val->GetType() == Object::Type::Null)
					continue;

				auto containable_ptr = dynamic_cast<ContainableObject*>(val.get());
				if (nullptr == containable_ptr)
					throw ConversionExceptionFactory<ContainableObject>::Construct(val);

				dictionary->Insert(name, ContainableObjectPtr(containable_ptr));
			}

			ReadTokenWithTypeSkip(Token::Type::DICTIONARY_END);
			dictionary->SetFile(_file);
			return dictionary;
		}

		ObjectPtr ParserBase::ReadDictionaryStream()
		{
			auto dictionary = ReadDictionary();
			if (PeekTokenTypeSkip() == Token::Type::STREAM_BEGIN) {
				ReadTokenWithTypeSkip(Token::Type::STREAM_BEGIN);
				ReadTokenWithTypeSkip(Token::Type::EOL);
				auto stream_offset = GetPosition();

				do
				{
					if (!dictionary->Contains(constant::Name::Length)) {
						break;
					}

					auto length_obj = dictionary->Find(constant::Name::Length);
					if (length_obj->GetType() != Object::Type::Integer) {
						auto locked_file = _file.lock();
						if (!locked_file->IsInitialized()) {
							break;
						}
					}

					auto length = ObjectUtils::ConvertTo<IntegerObjectPtr>(length_obj);
					seekg(length->GetIntegerValue(), ios_base::cur);
					auto expect_stream_end = ReadTokenSkip();
					if (expect_stream_end->GetType() != Token::Type::STREAM_END) {
						break;
					}

					auto result = StreamObjectPtr(dictionary, stream_offset);
					result->SetFile(_file);
					return result;
				} while (false);

				// Recalculate stream length
				seekg(stream_offset, ios_base::beg);

				for (;;) {
					auto offset = GetPosition();
					auto data = readline();
					auto line = data->ToString();
					auto pos = line.find("endstream");

					if (pos == std::string::npos) {
						continue;
					}

					auto end_obj_token = PeekTokenTypeSkip();
					assert(end_obj_token == Token::Type::INDIRECT_OBJECT_END); (void)end_obj_token;

					SetPosition(offset - 2);
					auto new_line1 = get();
					auto new_line2 = get();
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
					auto stream_end_offset = offset + pos;
					auto computed_length = stream_end_offset - stream_offset;
					if (!dictionary->Contains(constant::Name::Length)) {
						dictionary->Insert(constant::Name::Length, IntegerObjectPtr(computed_length));
						break;
					}

					auto length_obj = dictionary->Find(constant::Name::Length);
					if (length_obj->GetType() != Object::Type::Integer) {
						auto locked_file = _file.lock();
						if (!locked_file->IsInitialized()) {
							dictionary->Remove(constant::Name::Length);
							dictionary->Insert(constant::Name::Length, IntegerObjectPtr(computed_length));
							break;
						}
					}

					auto length = ObjectUtils::ConvertTo<IntegerObjectPtr>(length_obj);
					*length = computed_length;
					break;
				}

				auto result = StreamObjectPtr(dictionary, stream_offset);
				result->SetFile(_file);
				return result;
			}

			return dictionary;
		}

		MixedArrayObjectPtr ParserBase::ReadArray()
		{
			MixedArrayObjectPtr result;
			ReadTokenWithTypeSkip(Token::Type::ARRAY_BEGIN);
			while (PeekTokenTypeSkip() != Token::Type::ARRAY_END) {
				auto val = ReadDirectObject();
				auto containable_ptr = dynamic_cast<ContainableObject*>(val.get());
				if (nullptr == containable_ptr)
					throw ConversionExceptionFactory<ContainableObject>::Construct(val);

				result->Append(containable_ptr);
			}

			ReadTokenWithTypeSkip(Token::Type::ARRAY_END);
			result->SetFile(_file);
			return result;
		}

		NameObjectPtr ParserBase::ReadName()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::NAME_OBJECT);
			auto result = ObjectFactory::CreateName(token);
			result->SetFile(_file);
			return result;
		}

		LiteralStringObjectPtr ParserBase::ReadLiteralString()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::LITERAL_STRING);
			auto result = ObjectFactory::CreateLitString(token);
			result->SetFile(_file);
			return result;
		}

		HexadecimalStringObjectPtr ParserBase::ReadHexadecimalString()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::HEXADECIMAL_STRING);
			auto result = ObjectFactory::CreateHexString(token);
			result->SetFile(_file);
			return result;
		}

		BooleanObjectPtr ParserBase::ReadTrue()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::TRUE_VALUE);
			auto result = ObjectFactory::CreateBoolean(token);
			result->SetFile(_file);
			return result;
		}

		BooleanObjectPtr ParserBase::ReadFalse()
		{
			auto token = ReadTokenWithTypeSkip(Token::Type::FALSE_VALUE);
			auto result = ObjectFactory::CreateBoolean(token);
			result->SetFile(_file);
			return result;
		}

		ObjectPtr Parser::ReadIndirectObject(types::big_uint& obj_number, types::ushort& gen_number)
		{
			auto offset = GetPosition();
			auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto gen_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto begin_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_BEGIN);
			auto direct = ReadDirectObject();
			auto end_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_END);

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			auto obj_number_value = ObjectFactory::CreateInteger(obj_number_token);
			auto gen_number_value = ObjectFactory::CreateInteger(gen_number_token);
			obj_number = obj_number_value->GetUnsignedIntegerValue();
			gen_number = gen_number_value->SafeConvert<types::ushort>();

			direct->SetOffset(offset);
			direct->SetFile(_file);
			direct->SetInitialized();
			return direct;
		}

		ObjectPtr Parser::ReadIndirectObject(types::stream_offset offset, types::big_uint& obj_number, types::ushort& gen_number)
		{
			seekg(offset, ios_base::beg);
			return ReadIndirectObject(obj_number, gen_number);
		}

		ObjectPtr ParserBase::ReadDirectObject()
		{
			auto offset = GetPosition();
			auto type = PeekTokenTypeSkip();
			switch (type)
			{
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

		ObjectStreamEntry Parser::ReadObjectStreamHeader()
		{
			ObjectStreamEntry result;

			auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

			auto obj_number = ObjectFactory::CreateInteger(obj_number_token);
			auto offset = ObjectFactory::CreateInteger(offset_token);

			result.object_number = obj_number->GetUnsignedIntegerValue();
			result.offset = offset->GetIntegerValue();
			return result;
		}

		ObjectStreamEntries Parser::ReadObjectStreamHeaders(size_t size)
		{
			ObjectStreamEntries result;
			result.reserve(size);
			for (decltype(size) i = 0; i < size; ++i) {
				auto item = ReadObjectStreamHeader();
				result.push_back(item);
			}

			return result;
		}

		ObjectStreamEntries Parser::ReadObjectStreamEntries(types::big_uint first, size_t size)
		{
			auto entries = ReadObjectStreamHeaders(size);
			for (auto& entry : entries) {
				SetPosition(first + entry.offset);
				auto obj = ReadDirectObject();

				// Objects within streams shall not be encrypted
				// because streams themselves are encrypted
				obj->SetEncryptionExempted();
				obj->SetInitialized();

				entry.object = obj;
			}

			return entries;
		}

		ObjectPtr ParserBase::ReadDirectObject(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadDirectObject();
		}

		ObjectPtr ParserBase::PeekDirectObject()
		{
			auto position = GetPosition();
			auto obj = ReadDirectObject();
			SetPosition(position);

			return obj;
		}

#pragma endregion

#pragma region Tokens

		TokenPtr ParserBase::ReadTokenSkip()
		{
			for (;;) {
				auto token = ReadToken();
				if (token->GetType() == Token::Type::EOL)
					continue;

				return token;
			}
		}

		TokenPtr ParserBase::PeekTokenSkip()
		{
			auto position = GetPosition();
			bool rewind = false;
			for (;;) {
				auto token = PeekToken();
				if (token->GetType() == Token::Type::EOL) {
					ReadToken();
					rewind = true;
					continue;
				}

				if (rewind) {
					SetPosition(position);
				}

				return token;
			}
		}

		Token::Type ParserBase::PeekTokenTypeSkip()
		{
			auto token = PeekTokenSkip();
			return token->GetType();
		}

		TokenPtr ParserBase::ReadTokenWithTypeSkip(Token::Type type)
		{
			auto offset = GetPosition();
			for (;;) {
				auto token = ReadToken();

				if (token->GetType() == type)
					return token;

				if (token->GetType() == Token::Type::EOL)
					continue;

				throw ParseException(offset);
			}
		}

#pragma endregion

#pragma region Xref

		XrefEntryBasePtr Parser::ReadTableEntry(types::big_uint objNumber)
		{
			auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto generation_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

			auto offset = ObjectFactory::CreateInteger(offset_token);
			auto gen_number = ObjectFactory::CreateInteger(generation_token);

			auto peeked_token = PeekTokenSkip();
			if (*peeked_token->Value() == "n") {
				ReadTokenSkip();
				XrefUsedEntryPtr result(objNumber, gen_number->SafeConvert<types::ushort>(), offset->GetIntegerValue());
				result->SetFile(_file);
				return result;
			}

			if (*peeked_token->Value() == "f") {
				ReadTokenSkip();
				XrefFreeEntryPtr result(objNumber, gen_number->SafeConvert<types::ushort>());
				result->SetFile(_file);
				return result;
			}

			throw ParseException(GetPosition());
		}

		XrefTablePtr Parser::ReadXrefTable()
		{
			XrefTablePtr table;

			ReadTokenWithTypeSkip(Token::Type::XREF_MARKER);
			while (PeekTokenTypeSkip() != Token::Type::TRAILER) {
				auto revision_base_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
				auto size_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

				auto revision_base = std::stoull(revision_base_token->Value()->ToString());
				auto size = std::stoull(size_token->Value()->ToString());

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
			types::ushort stream_gen_number)
		{
			XrefStreamPtr result;

			// Get stream object data
			auto header = stream->GetHeader();

			auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);

			assert(fields->Size() == 3);
			if (fields->Size() != 3) {
				throw GeneralException("Xref stream width does not contain three integers");
			}

			auto size = header->FindAs<IntegerObjectPtr>(constant::Name::Size);
			ArrayObjectPtr<IntegerObjectPtr> index = { IntegerObjectPtr(0), size };
			if (header->Contains(constant::Name::Index)) {
				index = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::Index);
			}

			auto index_size = index->Size();
			assert(index_size % 2 == 0);

			auto body = stream->GetBody();

			auto field1_size = fields->At(0);
			auto field2_size = fields->At(1);
			auto field3_size = fields->At(2);

			bool contains_self = false;

			// Iterate over entries
			auto it = body.begin();
			for (unsigned int i = 0; i < index_size; i += 2) {

				auto subsection_index = index->At(i);
				auto subsection_size = index->At(i + 1);

				for (auto idx = 0; idx < *subsection_size; idx++) {

					IntegerObject field1;
					for (int j = 0; j < *field1_size; ++j) {
						unsigned char next_value = reinterpret_cast<unsigned char&>(*it);
						field1 = (field1 << 8) + next_value;
						it++;
					}

					assert(field1 == 0 || field1 == 1 || field1 == 2);

					IntegerObject field2;
					for (int j = 0; j < *field2_size; ++j) {
						unsigned char next_value = reinterpret_cast<unsigned char&>(*it);
						field2 = (field2 << 8) + next_value;
						it++;
					}

					IntegerObject field3;
					for (int j = 0; j < *field3_size; ++j) {
						unsigned char next_value = reinterpret_cast<unsigned char&>(*it);
						field3 = (field3 << 8) + next_value;
						it++;
					}

					types::big_uint obj_number = SafeAddition<types::big_uint, types::big_uint, int>(*subsection_index, idx);

					if (0 == field1) {
						XrefFreeEntryPtr entry(obj_number, field3.SafeConvert<types::ushort>());
						entry->SetFile(_file);
						result->Add(entry);
						continue;
					}

					if (1 == field1) {
						XrefUsedEntryPtr entry(obj_number, field3.SafeConvert<types::ushort>(), field2);

						// This case is when XrefStream contains reference to itself
						if (obj_number == stream_obj_number && field3.SafeConvert<types::ushort>() == stream_gen_number) {
							entry->SetReference(stream);
							entry->SetInitialized();
							contains_self = true;
						}

						entry->SetFile(_file);
						result->Add(entry);
						continue;
					}

					if (2 == field1) {
						XrefCompressedEntryPtr entry(obj_number, static_cast<types::ushort>(0), field2, field3.SafeConvert<size_t>());
						entry->SetFile(_file);
						result->Add(entry);
						continue;
					}

					throw GeneralException("Unknown entry type");
				}
			}

			// If stream does not contain entry for itself
			if (!contains_self) {
				auto locked_file = _file.lock();
				if (!locked_file)
					throw FileDisposedException();

				auto chain = locked_file->GetXrefChain();

				if (!chain->Contains(stream_obj_number, stream_gen_number)) {
					assert(false && "Where is stream entry stored?");
					throw GeneralException("Could not find xref stream object entry");
				}

				auto entry = chain->GetXrefEntry(stream_obj_number, stream_gen_number);

				if (!XrefUtils::IsType<XrefUsedEntryPtr>(entry)) {
					assert(false && "How could this be entry of different type");
					throw GeneralException("Xref entry has incorrect type");
				}

				auto used_entry = XrefUtils::ConvertTo<XrefUsedEntryPtr>(entry);
				used_entry->SetReference(stream);
				used_entry->SetInitialized();
			}

			stream->SetEncryptionExempted();

			result->SetFile(_file);
			result->SetTrailerDictionary(header);
			result->SetStreamObject(stream);
			result->SetInitialized();
			return result;
		}

		XrefStreamPtr Parser::ReadXrefStream()
		{
			types::big_uint stream_obj_number = 0;
			types::ushort stream_gen_number = 0;
			auto indirect = ReadIndirectObject(stream_obj_number, stream_gen_number);
			auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(indirect);
			return ParseXrefStream(stream, stream_obj_number, stream_gen_number);
		}

		XrefBasePtr Parser::ReadXref(void)
		{
			if (PeekTokenTypeSkip() == Token::Type::XREF_MARKER) {
				return ReadXrefTable();
			}

			return ReadXrefStream();
		}

		XrefBasePtr Parser::ReadXref(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			XrefBasePtr result = ReadXref();
			result->SetOffset(offset);
			return result;
		}

#pragma endregion


#pragma region Content streams

		contents::BaseInstructionCollectionPtr ContentStreamParser::ReadContentStreamInstructions(void)
		{
			contents::BaseInstructionCollectionPtr result;
			while (PeekTokenTypeSkip() != Token::Type::END_OF_INPUT) {
				auto operation = ReadContentStreamInstruction();
				operation->SetInitialized();
				result->push_back(operation);
			}

			return result;
		}

		contents::InlineImageObjectPtr ContentStreamParser::ReadInlineImageObject(void)
		{
			// read dictionary
			DictionaryObjectPtr image_dictionary;

			for (;;) {
				auto token_type = PeekTokenTypeSkip();
				if (token_type == Token::Type::NAME_OBJECT) {
					auto name = ReadName();
					auto value = ReadDirectObject();

					if (value->GetType() == Object::Type::Null)
						continue;

					auto containable_ptr = dynamic_cast<ContainableObject*>(value.get());
					if (nullptr == containable_ptr)
						throw ConversionExceptionFactory<ContainableObject>::Construct(value);

					image_dictionary->Insert(name, ContainableObjectPtr(containable_ptr));
					continue;
				}

				if (token_type == Token::Type::BEGIN_INLINE_IMAGE_DATA) {
					break;
				}

				assert(!"Unknown data in inline image dictionary");
			}

			// read operation begin image data
			auto inline_image_data_op = ReadContentStreamOperation();
			if (inline_image_data_op->GetOperationType() != contents::OperationBase::Type::BeginInlineImageData) {
				assert(!"Invalid operation after inline image dictionary");
			}

			// read data
			BufferPtr image_data;

			int stage = 0;
			unsigned char stored_whitespace = 0;
			for (;;) {
				auto current_meta = get();
				auto current = ValueConvertUtils::SafeConvert<unsigned char>(current_meta);

				if (stage == 0 && IsWhiteSpace(current)) {
					stage = 1;
					stored_whitespace = current;
					continue;
				}

				if (stage == 1) {
					if (current == 'E') {
						stage = 2;
						continue;
					}

					if (IsWhiteSpace(current)) {
						image_data->push_back(stored_whitespace);
						stored_whitespace = current;
						continue;
					}

					// reset stage
					stage = 0;

					image_data->push_back(stored_whitespace);
					image_data->push_back(current);
					continue;
				}

				if (stage == 2) {
					if (current == 'I') {
						stage = 3;
						continue;
					}

					// reset stage
					stage = 0;

					// pop stored data
					image_data->push_back(stored_whitespace);
					image_data->push_back('E');
					image_data->push_back(current);
					continue;
				}

				if (stage == 3) {
					if (IsWhiteSpace(current)) {
						// verify data
						break;
					}

					// reset stage
					stage = 0;

					// pop stored data
					image_data->push_back(stored_whitespace);
					image_data->push_back('E');
					image_data->push_back('I');
					image_data->push_back(current);
					continue;
				}

				image_data->push_back(current);
			}

			return contents::InlineImageObjectPtr(image_dictionary, image_data);
		}

		contents::InstructionBasePtr ContentStreamParser::ReadContentStreamInstruction(void)
		{
			auto operation = ReadContentStreamOperation();

			if (operation->GetOperationType() == contents::OperationBase::Type::EndText) {
				// This seems, that someone is trying to parse content stream,
				// that is part of multiple streams
				// Please be sure, that entry source data are all concatenated into
				// stream and not parsed partially
				assert(!"Found EndText operation without begin");
			}

			if (operation->GetOperationType() == contents::OperationBase::Type::BeginInlineImageObject) {
				return ReadInlineImageObject();
			}

			if (operation->GetOperationType() == contents::OperationBase::Type::BeginText) {
				contents::BaseOperationCollection text_operations;

				for (;;) {
					if (PeekTokenTypeSkip() == Token::Type::END_OF_INPUT) {
						// This seems, that someone is trying to parse content stream,
						// that is part of multiple streams
						// Please be sure, that entry source data are all concatenated into
						// stream and not parsed partially
						assert(!"Found BeginText operation without end"); break;
					}

					auto text_operation = ReadContentStreamOperation();
					if (text_operation->GetOperationType() == contents::OperationBase::Type::EndText) {
						break;
					}

					text_operations.push_back(text_operation);
				}

				return contents::TextObjectPtr(text_operations);
			}

			return operation;
		}

		contents::OperationBasePtr ContentStreamParser::ReadContentStreamOperation(void)
		{
			std::vector<ObjectPtr> operands;
			while (IsOperand(PeekTokenTypeSkip())) {
				auto operand = ReadOperand();
				operand->SetEncryptionExempted();
				operand->SetInitialized();
				operands.push_back(operand);
			}

			return ReadOperatorReturnOperation(operands);
		}

		contents::OperationBasePtr ContentStreamParser::ReadOperatorReturnOperation(const std::vector<ObjectPtr>& operands)
		{
			auto token = ReadTokenSkip();
			switch (token->GetType())
			{
			case Token::Type::LINE_WIDTH:
				return contents::OperationGenericPtr(operands, contents::LineWidthOperatorPtr());
			case Token::Type::LINE_CAP:
				return contents::OperationGenericPtr(operands, contents::LineCapOperatorPtr());
			case Token::Type::LINE_JOIN:
				return contents::OperationGenericPtr(operands, contents::LineJoinOperatorPtr());
			case Token::Type::MITER_LIMIT:
				return contents::OperationGenericPtr(operands, contents::MiterLimitOperatorPtr());
			case Token::Type::DASH_PATTERN:
				return contents::OperationGenericPtr(operands, contents::DashPatternOperatorPtr());
			case Token::Type::COLOR_RENDERING_INTENT:
				return contents::OperationGenericPtr(operands, contents::ColorRenderingIntentOperatorPtr());
			case Token::Type::FLATNESS:
				return contents::OperationGenericPtr(operands, contents::FlatnessOperatorPtr());
			case Token::Type::GRAPHICS_STATE:
				return contents::OperationGenericPtr(operands, contents::GraphicsStateOperatorPtr());
			case Token::Type::SAVE_GRAPHICS_STATE:
				return contents::OperationGenericPtr(operands, contents::SaveGraphicsStateOperatorPtr());
			case Token::Type::RESTORE_GRAPHIC_STATE:
				return contents::OperationGenericPtr(operands, contents::RestoreGraphicsStateOperatorPtr());
			case Token::Type::TRANSFORMATION_MATRIX:
				return contents::OperationGenericPtr(operands, contents::TransformationMatrixOperatorPtr());
			case Token::Type::BEGIN_SUBPATH:
				return contents::OperationGenericPtr(operands, contents::BeginSubpathOperatorPtr());
			case Token::Type::LINE:
				return contents::OperationGenericPtr(operands, contents::LineOperatorPtr());
			case Token::Type::FULL_CURVE:
				return contents::OperationGenericPtr(operands, contents::FullCurveOperatorPtr());
			case Token::Type::FINAL_CURVE:
				return contents::OperationGenericPtr(operands, contents::FinalCurveOperatorPtr());
			case Token::Type::INITIAL_CURVE:
				return contents::OperationGenericPtr(operands, contents::InitialCurveOperatorPtr());
			case Token::Type::CLOSE_SUBPATH:
				return contents::OperationGenericPtr(operands, contents::CloseSubpathOperatorPtr());
			case Token::Type::RECTANGLE:
				return contents::OperationGenericPtr(operands, contents::RectangleOperatorPtr());
			case Token::Type::STROKE:
				return contents::OperationGenericPtr(operands, contents::StrokeOperatorPtr());
			case Token::Type::CLOSE_AND_STROKE:
				return contents::OperationGenericPtr(operands, contents::CloseAndStrokeOperatorPtr());
			case Token::Type::FILL_PATH_NONZERO:
				return contents::OperationGenericPtr(operands, contents::FillPathNonzeroOperatorPtr());
			case Token::Type::FILL_PATH_COMPATIBILITY:
				return contents::OperationGenericPtr(operands, contents::FillPathCompatibilityOperatorPtr());
			case Token::Type::FILL_PATH_EVEN_ODD:
				return contents::OperationGenericPtr(operands, contents::FillPathEvenOddOperatorPtr());
			case Token::Type::FILL_STROKE_NONZERO:
				return contents::OperationGenericPtr(operands, contents::FillStrokeNonzeroOperatorPtr());
			case Token::Type::FILL_STROKE_EVEN_ODD:
				return contents::OperationGenericPtr(operands, contents::FillStrokeEvenOddOperatorPtr());
			case Token::Type::CLOSE_FILL_STROKE_NONZERO:
				return contents::OperationGenericPtr(operands, contents::CloseFillStrokeNonzeroOperatorPtr());
			case Token::Type::CLOSE_FILL_STROKE_EVEN_ODD:
				return contents::OperationGenericPtr(operands, contents::CloseFillStrokeEvenOddOperatorPtr());
			case Token::Type::END_PATH:
				return contents::OperationGenericPtr(operands, contents::EndPathOperatorPtr());
			case Token::Type::CLIP_PATH_NONZERO:
				return contents::OperationGenericPtr(operands, contents::ClipPathNonzeroOperatorPtr());
			case Token::Type::CLIP_PATH_EVEN_ODD:
				return contents::OperationGenericPtr(operands, contents::ClipPathEvenOddOperatorPtr());
			case Token::Type::BEGIN_TEXT:
				return contents::OperationBeginTextPtr(operands);
			case Token::Type::END_TEXT:
				return contents::OperationEndTextPtr(operands);
			case Token::Type::CHARACTER_SPACING:
				return contents::OperationGenericPtr(operands, contents::CharacterSpacingOperatorPtr());
			case Token::Type::WORD_SPACING:
				return contents::OperationGenericPtr(operands, contents::WordSpacingOperatorPtr());
			case Token::Type::HORIZONTAL_SCALING:
				return contents::OperationGenericPtr(operands, contents::HorizontalScalingOperatorPtr());
			case Token::Type::LEADING:
				return contents::OperationGenericPtr(operands, contents::LeadingOperatorPtr());
			case Token::Type::TEXT_FONT:
				return contents::OperationGenericPtr(operands, contents::TextFontOperatorPtr());
			case Token::Type::TEXT_RENDERING_MODE:
				return contents::OperationGenericPtr(operands, contents::TextRenderingModeOperatorPtr());
			case Token::Type::TEXT_RISE:
				return contents::OperationGenericPtr(operands, contents::TextRiseOperatorPtr());
			case Token::Type::TEXT_TRANSLATE:
				return contents::OperationGenericPtr(operands, contents::TextTranslateOperatorPtr());
			case Token::Type::TEXT_TRANSLATE_LEADING:
				return contents::OperationGenericPtr(operands, contents::TextTranslateLeadingOperatorPtr());
			case Token::Type::TEXT_MATRIX:
				return contents::OperationGenericPtr(operands, contents::TextMatrixOperatorPtr());
			case Token::Type::TEXT_NEXT_LINE:
				return contents::OperationGenericPtr(operands, contents::TextNextLineOperatorPtr());
			case Token::Type::TEXT_SHOW:
				return contents::OperationTextShowPtr(operands);
			case Token::Type::TEXT_SHOW_ARRAY:
				return contents::OperationTextShowArrayPtr(operands);
			case Token::Type::TEXT_NEXT_LINE_SHOW:
				return contents::OperationGenericPtr(operands, contents::TextNextLineShowOperatorPtr());
			case Token::Type::TEXT_NEXT_LINE_SHOW_SPACING:
				return contents::OperationGenericPtr(operands, contents::TextNextLineShowSpacingOperatorPtr());
			case Token::Type::SET_CHAR_WIDTH:
				return contents::OperationGenericPtr(operands, contents::SetCharWidthOperatorPtr());
			case Token::Type::SET_CACHE_DEVICE:
				return contents::OperationGenericPtr(operands, contents::SetCacheDeviceOperatorPtr());
			case Token::Type::COLOR_SPACE_STROKE:
				return contents::OperationGenericPtr(operands, contents::ColorSpaceStrokeOperatorPtr());
			case Token::Type::COLOR_SPACE_NONSTROKE:
				return contents::OperationGenericPtr(operands, contents::ColorSpaceNonstrokeOperatorPtr());
			case Token::Type::SET_COLOR_STROKE:
				return contents::OperationGenericPtr(operands, contents::SetColorStrokeOperatorPtr());
			case Token::Type::SET_COLOR_STROKE_EXTENDED:
				return contents::OperationGenericPtr(operands, contents::SetColorStrokeExtendedOperatorPtr());
			case Token::Type::SET_COLOR_NONSTROKE:
				return contents::OperationGenericPtr(operands, contents::SetColorNonstrokeOperatorPtr());
			case Token::Type::SET_COLOR_NONSTROKE_EXTENDED:
				return contents::OperationGenericPtr(operands, contents::SetColorNonstrokeExtendedOperatorPtr());
			case Token::Type::SET_STROKING_COLOR_SPACE_GRAY:
				return contents::OperationGenericPtr(operands, contents::SetStrokingColorSpaceGrayOperatorPtr());
			case Token::Type::SET_NONSTROKING_COLOR_SPACE_GRAY:
				return contents::OperationGenericPtr(operands, contents::SetNonstrokingColorSpaceGrayOperatorPtr());
			case Token::Type::SET_STROKING_COLOR_SPACE_RGB:
				return contents::OperationGenericPtr(operands, contents::SetStrokingColorSpaceRGBOperatorPtr());
			case Token::Type::SET_NONSTROKING_COLOR_SPACE_RGB:
				return contents::OperationGenericPtr(operands, contents::SetNonstrokingColorSpaceRGBOperatorPtr());
			case Token::Type::SET_STROKING_COLOR_SPACE_CMYK:
				return contents::OperationGenericPtr(operands, contents::SetStrokingColorSpaceCMYKOperatorPtr());
			case Token::Type::SET_NONSTROKING_COLOR_SPACE_CMYK:
				return contents::OperationGenericPtr(operands, contents::SetNonstrokingColorSpaceCMYKOperatorPtr());
			case Token::Type::SHADING_PAINT:
				return contents::OperationGenericPtr(operands, contents::ShadingPaintOperatorPtr());
			case Token::Type::BEGIN_INLINE_IMAGE_OBJECT:
				return contents::OperationBeginInlineImageObjectPtr(operands);
			case Token::Type::BEGIN_INLINE_IMAGE_DATA:
				return contents::OperationBeginInlineImageDataPtr(operands);
			case Token::Type::END_INLINE_IMAGE_OBJECT:
				return contents::OperationEndInlineImageObjectPtr(operands);
			case Token::Type::INVOKE_X_OBJECT:
				return contents::OperationGenericPtr(operands, contents::InvokeXObjectOperatorPtr());
			case Token::Type::DEFINE_MARKED_CONTENT_POINT:
				return contents::OperationGenericPtr(operands, contents::DefineMarkedContentPointOperatorPtr());
			case Token::Type::DEFINE_MARKED_CONTENT_POINT_WITH_PROPERTY_LIST:
				return contents::OperationGenericPtr(operands, contents::DefineMarkedContentPointWithPropertyListOperatorPtr());
			case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE:
				return contents::OperationGenericPtr(operands, contents::BeginMarkedContentSequenceOperatorPtr());
			case Token::Type::BEGIN_MARKED_CONTENT_SEQUENCE_WITH_PROPERTY_LIST:
				return contents::OperationGenericPtr(operands, contents::BeginMarkedContentSequenceWithPropertyListOperatorPtr());
			case Token::Type::END_MARKED_CONTENT_SEQUENCE:
				return contents::OperationGenericPtr(operands, contents::EndMarkedContentSequenceOperatorPtr());
			case Token::Type::BEGIN_COMPATIBILITY_SECTION:
				return contents::OperationGenericPtr(operands, contents::BeginCompatibilitySectionOperatorPtr());
			case Token::Type::END_COMPATIBILITY_SECTION:
				return contents::OperationGenericPtr(operands, contents::EndCompatibilitySectionOperatorPtr());
			default:
				return contents::OperationGenericPtr(operands, contents::UnknownOperatorPtr(token->Value()));
			}
		}

		bool ContentStreamParser::IsOperand(Token::Type type)
		{
			switch (type)
			{
			case Token::Type::DICTIONARY_BEGIN:
			case Token::Type::INTEGER_OBJECT:
			case Token::Type::ARRAY_BEGIN:
			case Token::Type::NAME_OBJECT:
			case Token::Type::HEXADECIMAL_STRING:
			case Token::Type::LITERAL_STRING:
			case Token::Type::REAL_OBJECT:
			case Token::Type::TRUE_VALUE:
			case Token::Type::FALSE_VALUE:
			case Token::Type::NULL_OBJECT:
				return true;
			}

			return false;
		}

		ObjectPtr ContentStreamParser::ReadOperand()
		{
			auto offset = GetPosition();
			switch (PeekTokenTypeSkip())
			{
			case Token::Type::DICTIONARY_BEGIN:
				return ReadDictionary();
			case Token::Type::INTEGER_OBJECT:
				return ReadInteger();
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

#pragma endregion

		HeaderPtr Parser::ReadHeader(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadHeader();
		}

		HeaderPtr Parser::ReadHeader(void)
		{
			HeaderPtr result;

			std::smatch sm;
			std::regex header_regex("%PDF-1\\.([0-7]).*");
			std::string line;

			for (;;) {
				auto data = readline();
				line = data->ToString();

				if (std::regex_match(line, sm, header_regex))
					break;
			}

			switch (stoi(sm[1]))
			{
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
				assert(!"If happens, error in regular expression");
			}

			return result;
		}

		XrefChainPtr Parser::FindAllObjects(void)
		{
			XrefChainPtr result;

			XrefTablePtr xref;

			seekg(0);
			while (!eof()) {
				auto offset_before = GetPosition();

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
						auto xref_offset = ObjectFactory::CreateInteger(next_token);
						xref->SetLastXrefOffset(xref_offset->GetIntegerValue());
					}

					continue;
				}

				if (first_token->GetType() != Token::Type::INTEGER_OBJECT) {
					if (first_token->GetType() != Token::Type::EOL) {
						readline();
					}

					continue;
				}

				auto gen_number_token = ReadToken();
				if (gen_number_token->GetType() != Token::Type::INTEGER_OBJECT) {
					if (gen_number_token->GetType() != Token::Type::EOL) {
						readline();
					}

					continue;
				}

				auto begin_token = ReadToken();
				if (begin_token->GetType() != Token::Type::INDIRECT_OBJECT_BEGIN) {
					if (begin_token->GetType() != Token::Type::EOL) {
						readline();
					}

					continue;
				}

				auto obj_number = std::stoull(first_token->Value()->ToString());
				auto gen_number_ul = std::stoul(gen_number_token->Value()->ToString());
				auto gen_number = ValueConvertUtils::SafeConvert<types::ushort>(gen_number_ul);

				auto obj = ReadDirectObject();
				obj->SetOffset(offset_before);
				obj->SetFile(_file);

				XrefUsedEntryPtr entry(obj_number, gen_number, obj->GetOffset());
				entry->SetReference(obj);
				entry->SetFile(_file);
				xref->Add(entry);

				// Check if read object is Xref stream
				if (ObjectUtils::IsType<StreamObjectPtr>(obj)) {
					auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(obj);
					auto stream_header = stream->GetHeader();
					if (stream_header->Contains(constant::Name::Type)) {
						auto stream_type = stream_header->FindAs<NameObjectPtr>(constant::Name::Type);

						if (constant::Name::XRef == stream_type) {
							auto xref_stream = ParseXrefStream(stream, obj_number, gen_number);
							result->Append(xref_stream);
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
	}
}
