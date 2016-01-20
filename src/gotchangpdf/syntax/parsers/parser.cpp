#include "precompiled.h"
#include "parser.h"
#include "exception.h"
#include "token.h"
#include "file.h"
#include "character.h"

namespace gotchangpdf
{
	namespace syntax
	{
#pragma region Objects

		Parser::Parser(std::weak_ptr<File> file, CharacterSource & stream)
			: Tokenizer(stream), _file(file) {}

		Parser::Parser(const Parser & other)
			: Tokenizer(other), _file(other._file) {}

		std::weak_ptr<File> Parser::GetFile(void) const { return _file; }

		class ObjectFactory
		{
		public:
			static BooleanObjectPtr CreateBoolean(TokenPtr token)
			{
				if (token->GetType() == Token::Type::TRUE_VALUE)
					return BooleanObjectPtr(true);

				if (token->GetType() == Token::Type::FALSE_VALUE)
					return BooleanObjectPtr(false);

				throw GeneralException("Expected boolean token type");

			}
			static IntegerObjectPtr CreateInteger(TokenPtr token)
			{
				if (token->GetType() != Token::Type::INTEGER_OBJECT)
					throw GeneralException("Expected integer token type");

				auto buffer = token->Value();
				auto value = std::stoi(buffer->ToString());
				return IntegerObjectPtr(value);
			}

			static RealObjectPtr CreateReal(TokenPtr token)
			{
				if (token->GetType() != Token::Type::REAL_OBJECT)
					throw GeneralException("Expected real token type");

				auto buffer = token->Value();
				auto str = buffer->ToString();
				auto value = std::stof(str);
				auto pos = str.rfind('.');
				if (-1 != pos) {
					auto precision = str.size() - pos - 1;
					auto converted = SafeConvert<unsigned char>(precision);
					return RealObjectPtr(value, converted);
				}

				return RealObjectPtr(value);
			}

			static NameObjectPtr CreateName(TokenPtr token)
			{
				if (token->GetType() != Token::Type::NAME_OBJECT)
					throw GeneralException("Expected name token type");

				auto buffer = token->Value();
				return NameObjectPtr(buffer);
			}

			static HexadecimalStringObjectPtr CreateHexString(TokenPtr token)
			{
				if (token->GetType() != Token::Type::HEXADECIMAL_STRING)
					throw GeneralException("Expected hexadecimal string token type");

				auto buffer = token->Value();
				return HexadecimalStringObjectPtr(buffer);
			}

			static LiteralStringObjectPtr CreateLitString(TokenPtr token)
			{
				if (token->GetType() != Token::Type::LITERAL_STRING)
					throw GeneralException("Expected literal string token type");

				auto buffer = token->Value();
				return LiteralStringObjectPtr(buffer);
			}
		};

		ObjectPtr Parser::ReadIndirectObject(void)
		{
			auto offset = tellg();
			auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto gen_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto begin_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_BEGIN);
			auto direct = ReadDirectObject();
			auto end_token = ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_END);

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			auto obj_number = ObjectFactory::CreateInteger(obj_number_token);
			auto gen_number = ObjectFactory::CreateInteger(gen_number_token);
			direct->SetObjectNumber(obj_number->Value());
			direct->SetGenerationNumber(gen_number->SafeConvert<types::ushort>());
			direct->SetOffset(offset);
			direct->SetFile(_file);

			assert(direct->IsIndirect());
			return direct;
		}

		ObjectPtr Parser::ReadIndirectObject(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadIndirectObject();
		}

		ObjectPtr Parser::ReadDirectObject()
		{
			auto offset = tellg();

			switch (PeekTokenTypeSkip())
			{
			case Token::Type::DICTIONARY_BEGIN:
			{
				DictionaryObject dictionary;
				ReadTokenWithTypeSkip(Token::Type::DICTIONARY_BEGIN);
				while (PeekTokenTypeSkip() != Token::Type::DICTIONARY_END)
				{
					auto name = ReadDirectObjectWithType<NameObjectPtr>();
					auto val = ReadDirectObject();

					if (val->GetType() == Object::Type::Null)
						continue;

					auto containable_ptr = dynamic_cast<ContainableObject*>(val.Content.get());
					if (nullptr == containable_ptr)
						throw GeneralException("Could not convert parsed object to containable: " + val->ToString());

					dictionary._list[name] = ContainableObjectPtr(containable_ptr);
				}

				ReadTokenWithTypeSkip(Token::Type::DICTIONARY_END);

				if (PeekTokenTypeSkip() == Token::Type::STREAM_BEGIN)
				{
					ReadTokenWithTypeSkip(Token::Type::STREAM_BEGIN);
					ReadTokenWithTypeSkip(Token::Type::EOL);
					auto stream_offset = tellg();
					auto length = dictionary.FindAs<IntegerObjectPtr>(constant::Name::Length);
					seekg(length->Value(), ios_base::cur);
					ReadTokenWithTypeSkip(Token::Type::STREAM_END);

					auto result = StreamObjectPtr(dictionary, stream_offset);
					result->SetFile(_file);
					result->GetHeader()->SetFile(_file);
					return result;
				}

				dictionary.SetFile(_file);
				return DictionaryObjectPtr(dictionary);
			}
			case Token::Type::INTEGER_OBJECT:
			{
				auto token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

				auto pos = tellg();
				if (PeekTokenTypeSkip() == Token::Type::INTEGER_OBJECT)
				{
					auto ahead = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
					auto obj_number = ObjectFactory::CreateInteger(token);
					auto gen_number = ObjectFactory::CreateInteger(ahead);

					if (PeekTokenTypeSkip() == Token::Type::INDIRECT_REFERENCE_MARKER) {
						auto reference_marker = ReadTokenWithTypeSkip(Token::Type::INDIRECT_REFERENCE_MARKER);
						IndirectObjectReferencePtr result(obj_number->Value(), gen_number->SafeConvert<types::ushort>());
						result->SetFile(_file);
						return result;
					}
				}

				// TODO we can peek only one next token, therefore we need to seek back
				seekg(pos);

				auto result = ObjectFactory::CreateInteger(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::ARRAY_BEGIN:
			{
				MixedArrayObjectPtr result;
				ReadTokenWithTypeSkip(Token::Type::ARRAY_BEGIN);
				while (PeekTokenTypeSkip() != Token::Type::ARRAY_END)
				{
					auto val = ReadDirectObject();
					auto containable_ptr = dynamic_cast<ContainableObject*>(val.Content.get());
					if (nullptr == containable_ptr)
						throw GeneralException("Could not convert parsed object to containable: " + val->ToString());

					result->push_back(containable_ptr);
				}

				ReadTokenWithTypeSkip(Token::Type::ARRAY_END);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::NAME_OBJECT:
			{
				auto token = ReadTokenWithTypeSkip(Token::Type::NAME_OBJECT);
				auto result = ObjectFactory::CreateName(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::HEXADECIMAL_STRING:
			{
				auto token = ReadTokenWithTypeSkip(Token::Type::HEXADECIMAL_STRING);
				auto result = ObjectFactory::CreateHexString(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::LITERAL_STRING:
			{
				auto token = ReadTokenWithTypeSkip(Token::Type::LITERAL_STRING);
				auto result = ObjectFactory::CreateLitString(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::REAL_OBJECT:
			{
				auto token = ReadTokenWithTypeSkip(Token::Type::REAL_OBJECT);
				auto result = ObjectFactory::CreateReal(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::TRUE_VALUE:
			{
				auto token = ReadTokenWithTypeSkip(Token::Type::TRUE_VALUE);
				auto result = ObjectFactory::CreateBoolean(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::FALSE_VALUE:
			{
				auto token = ReadTokenWithTypeSkip(Token::Type::FALSE_VALUE);
				auto result = ObjectFactory::CreateBoolean(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::NULL_OBJECT:
			{
				ReadTokenWithTypeSkip(Token::Type::NULL_OBJECT);
				auto result = NullObject::GetInstance();
				result->SetFile(_file);
				return result;
			}
			default:
				throw GeneralException("No valid object could be found at offset " + std::to_string(offset));
			}
		}

		ObjectStreamHeader Parser::ReadObjectStreamHeader()
		{
			ObjectStreamHeader result;

			auto obj_number_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

			auto obj_number = ObjectFactory::CreateInteger(obj_number_token);
			auto offset = ObjectFactory::CreateInteger(offset_token);

			result.object_number = obj_number->Value();
			result.offset = offset->Value();
			return result;
		}

		ObjectStreamHeaders Parser::ReadObjectStreamHeaders(types::integer size)
		{
			ObjectStreamHeaders result;
			result.reserve(size);
			for (types::integer i = 0; i < size; ++i) {
				auto item = ReadObjectStreamHeader();
				result.push_back(item);
			}

			return result;
		}

		std::vector<ObjectPtr> Parser::ReadObjectStreamEntries(types::integer first, types::integer size)
		{
			std::vector<ObjectPtr> result;
			auto headers = ReadObjectStreamHeaders(size);
			for (auto header : headers) {
				seekg(first + header.offset);
				auto obj = ReadDirectObject();
				obj->SetObjectNumber(header.object_number);

				result.push_back(obj);
			}

			return result;
		}

		ObjectPtr Parser::ReadDirectObject(types::stream_offset offset)
		{
			seekg(offset, ios_base::beg);
			return ReadDirectObject();
		}

		ObjectPtr Parser::PeekDirectObject()
		{
			auto position = tellg();
			auto obj = ReadDirectObject();
			seekg(position);

			return obj;
		}

#pragma endregion

#pragma region Tokens

		TokenPtr Parser::ReadTokenSkip()
		{
			for (;;) {
				auto token = ReadToken();
				if (token->GetType() == Token::Type::EOL)
					continue;

				return token;
			}
		}

		TokenPtr Parser::PeekTokenSkip()
		{
			auto position = tellg();
			auto obj = ReadTokenSkip();
			seekg(position);

			return obj;
		}

		Token::Type Parser::PeekTokenTypeSkip()
		{
			auto token = PeekTokenSkip();
			return token->GetType();
		}

		TokenPtr Parser::ReadTokenWithTypeSkip(Token::Type type)
		{
			auto offset = tellg();
			for (;;) {
				auto token = ReadToken();

				if (token->GetType() == type)
					return token;

				if (token->GetType() == Token::Type::EOL)
					continue;

				std::stringstream ss;
				ss << "Could not find token type " << Token::GetTypeValueName(type) << " at offset " << offset;
				throw GeneralException(ss.str());
			}
		}

#pragma endregion

#pragma region Xref

		XrefEntryBasePtr Parser::ReadTableEntry(types::integer objNumber)
		{
			auto offset_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
			auto generation_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

			auto offset = ObjectFactory::CreateInteger(offset_token);
			auto gen_number = ObjectFactory::CreateInteger(generation_token);

			if (PeekTokenTypeSkip() == Token::Type::XREF_USED_ENTRY) {
				ReadTokenWithTypeSkip(Token::Type::XREF_USED_ENTRY);
				XrefUsedEntryPtr result(objNumber, gen_number->SafeConvert<types::ushort>(), offset->Value());
				result->SetFile(_file);
				return result;
			}

			if (PeekTokenTypeSkip() == Token::Type::XREF_FREE_ENTRY) {
				ReadTokenWithTypeSkip(Token::Type::XREF_FREE_ENTRY);
				XrefFreeEntryPtr result(objNumber, gen_number->SafeConvert<types::ushort>(), offset->Value());
				result->SetFile(_file);
				return result;
			}

			std::stringstream buffer;
			buffer << "Key in XRef table is neither of " << 'n' << " or " << 'f';
			throw GeneralException(buffer.str());
		}

		XrefTablePtr Parser::ReadXrefTable()
		{
			XrefTablePtr table;

			ReadTokenWithTypeSkip(Token::Type::XREF_MARKER);
			while (PeekTokenTypeSkip() != Token::Type::TRAILER) {
				auto revision_base_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
				auto size_token = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);

				auto revision_base = ObjectFactory::CreateInteger(revision_base_token);
				auto size = ObjectFactory::CreateInteger(size_token);

				XrefSubsectionPtr subsection(revision_base->Value(), size->Value());
				for (int i = 0; i < size->Value(); ++i) {
					auto entry = ReadTableEntry(SafeAddition(revision_base->Value(), i));
					subsection->Add(entry);
				}

				subsection->SetFile(_file);
				table->Add(subsection);
			}

			ReadTokenWithTypeSkip(Token::Type::TRAILER);
			auto trailer_dictionary = ReadDirectObjectWithType<DictionaryObjectPtr>();

			table->SetTrailerDictionary(trailer_dictionary);
			table->SetFile(_file);
			return table;
		}

		XrefStreamPtr Parser::ReadXrefStream()
		{
			XrefStreamPtr result;

			auto indirect = ReadIndirectObject();
			auto stream = ObjectUtils::ConvertTo<StreamObjectPtr>(indirect);

			// Get stream object data
			auto header = stream->GetHeader();

			auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);
			auto size = header->FindAs<IntegerObjectPtr>(constant::Name::Size);

			assert(fields->Size() == 3);
			ArrayObjectPtr<IntegerObjectPtr> index = { IntegerObjectPtr(0), size };
			if (header->Contains(constant::Name::Index)) {
				index = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::Index);
			}

			auto index_size = index->Size();
			assert(index_size % 2 == 0);

			auto body = stream->GetBodyDecoded();

			auto field1_size = fields->At(0);
			auto field2_size = fields->At(1);
			auto field3_size = fields->At(2);

			// Iterate over entries
			auto it = body.begin();
			for (unsigned int i = 0; i < index_size; i += 2) {

				auto subsection_index = index->At(i);
				auto subsection_size = index->At(i + 1);

				XrefSubsectionPtr subsection(subsection_index->Value(), subsection_size->Value());
				for (auto idx = 0; idx < *subsection_size; idx++) {

					IntegerObject field1;
					for (int j = 0; j < *field1_size; ++j) {
						field1 = (field1 << 8) + (*it & 0xff);
						it++;
					}

					assert(field1 == 0 || field1 == 1 || field1 == 2);

					IntegerObject field2;
					for (int j = 0; j < *field2_size; ++j) {
						field2 = (field2 << 8) + (*it & 0xff);
						it++;
					}

					IntegerObject field3;
					for (int j = 0; j < *field3_size; ++j) {
						field3 = (field3 << 8) + (*it & 0xff);
						it++;
					}

					switch (field1) {
					case 0:
					{
						XrefFreeEntryPtr entry(*subsection_index + idx, field3.SafeConvert<types::ushort>(), field2);
						entry->SetFile(_file);
						subsection->Add(entry);
						break;
					}
					case 1:
					{
						XrefUsedEntryPtr entry(*subsection_index + idx, field3.SafeConvert<types::ushort>(), field2);
						entry->SetFile(_file);
						subsection->Add(entry);
						break;
					}
					case 2:
					{
						XrefCompressedEntryPtr entry(*subsection_index + idx, static_cast<types::ushort>(0), field2, field3);
						entry->SetFile(_file);
						subsection->Add(entry);
						break;
					}
					default:
						LOG_ERROR << "Unrecognized data found in xref stream data";
						break;
					}
				}

				subsection->SetFile(_file);
				result->Add(subsection);
			}

			result->SetFile(_file);
			result->SetTrailerDictionary(header);
			return result;
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
			return ReadXref();
		}

#pragma endregion
	}
}
