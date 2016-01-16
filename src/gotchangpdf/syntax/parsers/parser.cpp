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
			static IntegerObjectPtr CreateInteger(TokenPtr token)
			{
				if (token->GetType() != Token::Type::INTEGER_OBJECT)
					throw GeneralException("Expected integer token type");

				auto buffer = token->Value();
				auto value = stoi(buffer->ToString());
				return IntegerObjectPtr(value);
			}

			static RealObjectPtr CreateReal(TokenPtr token)
			{
				if (token->GetType() != Token::Type::REAL_OBJECT)
					throw GeneralException("Expected real token type");

				auto buffer = token->Value();
				auto value = stof(buffer->ToString());
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

					auto containable_ptr = dynamic_cast<ContainableObject*>(val.Content.get());
					if (nullptr == containable_ptr)
						throw GeneralException("Could not convert parsed object to containable: " + val->ToString());

					//ContainableObjectPtr containable = ObjectUtils::ConvertTo<ContainableObjectPtr>(val);
					dictionary._list[name] = ContainableObjectPtr(containable_ptr);
				}

				ReadTokenWithType(Token::Type::DICTIONARY_END);

				//s.LexicalSettingsPop();
				//return s;
				////

				if (PeekTokenType() == Token::Type::STREAM_BEGIN)
				{
					ReadTokenWithType(Token::Type::STREAM_BEGIN);
					auto stream_offset = tellg();
					auto result = StreamObjectPtr(dictionary, stream_offset);
					result->SetFile(_file);
					return result;
				}

				dictionary.SetFile(_file);
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
						direct->SetFile(_file);

						return direct;
					}
					default:
						seekg(pos);
					}
				}

				auto result = ObjectFactory::CreateInteger(token);
				result->SetFile(_file);
				return result;
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

				result->SetFile(_file);
				return result;
			}
			case Token::Type::NAME_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::NAME_OBJECT);
				auto result = ObjectFactory::CreateName(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::HEXADECIMAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::HEXADECIMAL_STRING);
				auto result = ObjectFactory::CreateHexString(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::LITERAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::LITERAL_STRING);
				auto result = ObjectFactory::CreateLitString(token);
				result->SetFile(_file);
				return result;
			}
			case Token::Type::REAL_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::REAL_OBJECT);
				auto result = ObjectFactory::CreateReal(token);
				result->SetFile(_file);
				return result;
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
