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
		class ObjectFactory
		{
		public:
			static IntegerObjectPtr CreateInteger(Token token)
			{
				if (token.GetType() != Token::Type::INTEGER_OBJECT)
					throw GeneralException("Expected integer token type");

				auto buffer = token.Value();
				auto value = stoi(buffer->ToString());
				return IntegerObjectPtr(value);
			}

			static RealObjectPtr CreateReal(Token token)
			{
				if (token.GetType() != Token::Type::REAL_OBJECT)
					throw GeneralException("Expected real token type");

				auto buffer = token.Value();
				auto value = stof(buffer->ToString());
				return RealObjectPtr(value);
			}

			static NameObjectPtr CreateName(Token token)
			{
				if (token.GetType() != Token::Type::NAME_OBJECT)
					throw GeneralException("Expected name token type");

				auto buffer = token.Value();
				if (buffer[0] != Delimiter::SOLIDUS) {
					assert(!"Name object does not start with solidus");
					throw GeneralException("Name object does not start with solidus");
				}

				buffer->erase(buffer.begin());
				return NameObjectPtr(buffer);
			}

			static HexadecimalStringObjectPtr CreateHexString(Token token)
			{
				if (token.GetType() != Token::Type::HEXADECIMAL_STRING)
					throw GeneralException("Expected hexadecimal string token type");

				auto buffer = token.Value();

				if (buffer->front() != Delimiter::LESS_THAN_SIGN ) {
					assert(!"Missing <");
					throw GeneralException("Hexadecimal string does not start with " + Delimiter::LESS_THAN_SIGN);
				}

				if (buffer->back() != Delimiter::GREATER_THAN_SIGN) {
					assert(!"Missing >");
					throw GeneralException("Hexadecimal string does not end with " + Delimiter::GREATER_THAN_SIGN);
				}

				buffer->erase(buffer.begin());
				buffer->erase(buffer.end());
				return HexadecimalStringObjectPtr(buffer);
			}

			static LiteralStringObjectPtr CreateLitString(Token token)
			{
				if (token.GetType() != Token::Type::LITERAL_STRING)
					throw GeneralException("Expected literal string token type");

				auto buffer = token.Value();

				if (buffer->front() != Delimiter::LEFT_PARENTHESIS) {
					assert(!"Missing (");
					throw GeneralException("Literal string does not start with " + Delimiter::LEFT_PARENTHESIS);
				}

				if (buffer->back() != Delimiter::RIGHT_PARENTHESIS) {
					assert(!"Missing )");
					throw GeneralException("Literal string does not end with " + Delimiter::RIGHT_PARENTHESIS);
				}

				buffer->erase(buffer.begin());
				buffer->erase(buffer.end());
				return LiteralStringObjectPtr(buffer);
			}
		};

		Parser::Parser(std::weak_ptr<File> file, CharacterSource & stream)
			: Tokenizer(stream), _file(file) {}

		Parser::Parser(const Parser & other)
			: Tokenizer(other), _file(other._file) {}

		std::weak_ptr<File> Parser::GetFile(void) const { return _file; }

		ObjectPtr Parser::ReadDirectObject()
		{
			auto offset = tellg();

			switch (PeekTokenType())
			{
			case Token::Type::DICTIONARY_BEGIN:
			{
				DictionaryObject dictionary;

				////
				//s.LexicalSettingsPush();
				//auto settings = s.LexicalSettingsGet();
				//settings->skip.push_back(Token::Type::EOL);

				ReadTokenWithType(Token::Type::DICTIONARY_BEGIN);
				while (PeekTokenType() != Token::Type::DICTIONARY_END)
				{
					auto name = ReadDirectObjectWithType<NameObjectPtr>();
					auto val = ReadDirectObject();

					if (val->GetType() == Object::Type::Null)
						continue;

					ContainableObjectPtr containable = ObjectUtils::ConvertTo<ContainableObjectPtr>(val);
					dictionary._list[name] = containable;
				}

				ReadTokenWithType(Token::Type::DICTIONARY_END);

				//s.LexicalSettingsPop();
				//return s;
				////

				if (PeekTokenType() == Token::Type::STREAM_BEGIN)
				{
					ReadTokenWithType(Token::Type::STREAM_BEGIN);
					auto stream_offset = tellg();
					return StreamObjectPtr(dictionary, stream_offset);
				}

				return DictionaryObjectPtr(dictionary);
			}
			case Token::Type::INTEGER_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::INTEGER_OBJECT);

				if (PeekTokenType() == Token::Type::INTEGER_OBJECT)
				{
					auto pos = tellg();
					auto ahead = ReadToken();
					auto obj_number = ObjectFactory::CreateInteger(token);
					auto gen_number = ObjectFactory::CreateInteger(ahead);

					switch (PeekTokenType())
					{
					case Token::Type::INDIRECT_REFERENCE_MARKER:
					{
						auto reference_marker = ReadTokenWithType(Token::Type::INDIRECT_REFERENCE_MARKER);
						IndirectObjectReferencePtr result(obj_number->Value(), gen_number->SafeConvert<types::ushort>());
						result->SetFile(_file);
						return result;
					}

					case Token::Type::INDIRECT_OBJECT_BEGIN:
					{
						ReadTokenWithType(Token::Type::INDIRECT_OBJECT_BEGIN);

						auto locked_file = _file.lock();
						if (!locked_file)
							throw FileDisposedException();

						auto direct = ReadDirectObject();
						direct->SetObjectNumber(obj_number->Value());
						direct->SetGenerationNumber(gen_number->SafeConvert<types::ushort>());
						direct->SetOffset(offset);

						return direct;
					}
					default:
						seekg(pos);
					}
				}

				return ObjectFactory::CreateInteger(token);
			}
			case Token::Type::ARRAY_BEGIN:
			{
				MixedArrayObjectPtr result;

				////
				//s.LexicalSettingsPush();
				//auto settings = s.LexicalSettingsGet();
				//settings->skip.push_back(Token::Type::EOL);

				ReadTokenWithType(Token::Type::ARRAY_BEGIN);
				while (PeekTokenType() != Token::Type::ARRAY_END)
				{
					auto val = ReadDirectObject();
					ContainableObjectPtr containable = ObjectUtils::ConvertTo<ContainableObjectPtr>(val);
					result->push_back(containable);
				}

				ReadTokenWithType(Token::Type::ARRAY_END);

				//s.LexicalSettingsPop();
				///

				return result;
			}
			case Token::Type::NAME_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::NAME_OBJECT);
				return ObjectFactory::CreateName(token);
			}
			case Token::Type::HEXADECIMAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::HEXADECIMAL_STRING);
				return ObjectFactory::CreateHexString(token);
			}
			case Token::Type::LITERAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::LITERAL_STRING);
				return ObjectFactory::CreateLitString(token);
			}
			case Token::Type::REAL_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::REAL_OBJECT);
				return ObjectFactory::CreateReal(token);
			}
			default:
				throw GeneralException("No valid object could be found at offset " + static_cast<int>(offset));
			}
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
	}
}
