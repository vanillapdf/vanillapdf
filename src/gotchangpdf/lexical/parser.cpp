#include "parser.h"
#include "exception.h"
#include "file.h"

#include "stream_object.h"
#include "boolean_object.h"
#include "null_object.h"
#include "function_object.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "array_object.h"
#include "integer_object.h"
#include "stream_object.h"
#include "real_object.h"
#include "string_object.h"
#include "indirect_object_reference.h"

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace lexical;
		using namespace exceptions;

		Parser::Parser(files::File * file, CharacterSource & stream)
			: lexical::Stream(stream), _file(file) {}

		Parser::Parser(const gotchangpdf::lexical::Parser &other)
			: lexical::Stream(other) { _file = other.file(); }

		DirectObject Parser::readObject()
		{
			auto offset = tellg();

			switch (PeekTokenType())
			{
			case Token::Type::DICTIONARY_BEGIN:
				{
					DictionaryObject result;
					*this >> result;

					if (PeekTokenType() == Token::Type::EOL)
						ReadToken();

					if (PeekTokenType() == Token::Type::STREAM_BEGIN)
					{
						StreamObject resultStream;
						*this >> resultStream;

						return Deferred<StreamObject>(resultStream);
					}

					return Deferred<DictionaryObject>(result);
				}
			case Token::Type::INTEGER_OBJECT:
				{
					auto token = ReadTokenWithType(Token::Type::INTEGER_OBJECT);

					if (PeekTokenType() == Token::Type::INTEGER_OBJECT)
					{
						auto pos = tellg();
						auto gen_number = ReadToken();

						switch (PeekTokenType())
						{
						case Token::Type::INDIRECT_REFERENCE_MARKER:
						{
							auto reference_marker = ReadTokenWithType(Token::Type::INDIRECT_REFERENCE_MARKER);
							return Deferred<IndirectObjectReference>(IndirectObjectReference(_file, IntegerObject(token), IntegerObject(gen_number)));
						}

						case Token::Type::INDIRECT_OBJECT_BEGIN:
						{
							auto reference_marker = ReadTokenWithType(Token::Type::INDIRECT_OBJECT_BEGIN);
							auto indirect = _file->GetIndirectObject(IntegerObject(token), IntegerObject(gen_number));

							ReadTokenWithType(Token::Type::EOL);
							auto data = readObject();

							//indirect->SetObject(data);
							// TODO check cast if needed
							indirect->SetOffset(offset);

							return NullObject::GetInstance();
						}
						default:
							seekg(pos);
						}
					}

					return Deferred<IntegerObject>(IntegerObject(token));
				}
			case Token::Type::ARRAY_BEGIN:
				{
					MixedArrayObject result;
					*this >> result;

					//auto token = readToken();
					//if (token->type() == Token::Type::INDIRECT_OBJECT_END)
					return Deferred<MixedArrayObject>(result);

					// TODO is this possible?
					//assert(false);
					//break;
				}
			case Token::Type::NAME_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::NAME_OBJECT);
				return Deferred<NameObject>(NameObject(token));
			}
			case Token::Type::HEXADECIMAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::HEXADECIMAL_STRING);
				return Deferred<HexadecimalString>(HexadecimalString(token));
			}
			case Token::Type::LITERAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::LITERAL_STRING);
				return Deferred<LiteralString>(LiteralString(token));
			}
			case Token::Type::REAL_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::REAL_OBJECT);
				return Deferred<RealObject>(RealObject(token));
			}
			default:
				throw Exception("No valid object could be found at offset " + static_cast<int>(offset));
			}
		}

		DirectObject Parser::peekObject()
		{
			auto position = tellg();
			auto obj = readObject();
			seekg(position);

			return obj;
		}
		/*
		DirectObject Parser::readObjectWithType(gotchangpdf::Object::Type type)
		{
			auto obj = readObject();

			switch (type)
			{
			case Object::Type::Unknown:
				throw Exception("FIXME: Are your really trying to return unknown type??");
			case Object::Type::ArrayObject:
				return dynamic_wrapper_cast<MixedArrayObject>(obj);
			case Object::Type::Boolean:
				return dynamic_wrapper_cast<BooleanObject>(obj);
			case Object::Type::DictionaryObject:
				return dynamic_wrapper_cast<DictionaryObject>(obj);
			case Object::Type::Function:
				return dynamic_wrapper_cast<FunctionObject>(obj);
			case Object::Type::IntegerObject:
				return dynamic_wrapper_cast<IntegerObject>(obj);
			case Object::Type::NameObject:
				return dynamic_wrapper_cast<NameObject>(obj);
			case Object::Type::NullObject:
				return dynamic_wrapper_cast<NullObject>(obj);
			case Object::Type::RealObject:
				return dynamic_wrapper_cast<RealObject>(obj);
			case Object::Type::StreamObject:
				return dynamic_wrapper_cast<StreamObject>(obj);
			case Object::Type::HexadecimalString:
				return dynamic_wrapper_cast<HexadecimalString>(obj);
			case Object::Type::LiteralString:
				return dynamic_wrapper_cast<LiteralString>(obj);
			case Object::Type::IndirectObjectReference:
				return dynamic_wrapper_cast<IndirectObjectReference>(obj);
			case Object::Type::IndirectObject:
				return dynamic_wrapper_cast<IndirectObject>(obj);
			default:
				assert(false);
				throw Exception("FIXME: Unknown object type");
			}
		}
		*/
		//void Stream::SetDeep( bool deep ) { _deep = deep; }

		//bool Stream::GetDeep( void ) const { return _deep; }
	}
}
