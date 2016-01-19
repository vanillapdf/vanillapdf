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
				auto value = stoi(buffer->ToString());
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

		Parser::Parser(std::weak_ptr<File> file, CharacterSource & stream)
			: Tokenizer(stream), _file(file) {}

		Parser::Parser(const Parser & other)
			: Tokenizer(other), _file(other._file) {}

		std::weak_ptr<File> Parser::GetFile(void) const { return _file; }

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

				if (PeekTokenTypeSkip() == Token::Type::INTEGER_OBJECT)
				{
					auto pos = tellg();
					auto ahead = ReadTokenWithTypeSkip(Token::Type::INTEGER_OBJECT);
					auto obj_number = ObjectFactory::CreateInteger(token);
					auto gen_number = ObjectFactory::CreateInteger(ahead);

					switch (PeekTokenTypeSkip())
					{
					case Token::Type::INDIRECT_REFERENCE_MARKER:
					{
						auto reference_marker = ReadTokenWithTypeSkip(Token::Type::INDIRECT_REFERENCE_MARKER);
						IndirectObjectReferencePtr result(obj_number->Value(), gen_number->SafeConvert<types::ushort>());
						result->SetFile(_file);
						return result;
					}

					case Token::Type::INDIRECT_OBJECT_BEGIN:
					{
						ReadTokenWithTypeSkip(Token::Type::INDIRECT_OBJECT_BEGIN);

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
	}
}
