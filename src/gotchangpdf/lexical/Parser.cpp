#include "Lexical/Parser.h"
#include "Exception.h"
#include "File.h"

#include "StreamObject.h"
#include "Boolean.h"
#include "NullObject.h"
#include "Function.h"
#include "IndirectObject.h"
#include "DictionaryObject.h"
#include "ArrayObject.h"
#include "IntegerObject.h"
#include "StreamObject.h"
#include "RealObject.h"
#include "StringObject.h"
#include "IndirectObjectReference.h"

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace lexical;
		using namespace exceptions;

		boost::intrusive_ptr<Object> Parser::readObject()
		{
			auto offset = tellg();

			switch (PeekTokenType())
			{
			case Token::Type::DICTIONARY_BEGIN:
				{
					auto result = boost::intrusive_ptr<DictionaryObject>(new DictionaryObject());
					*this >> *result;

					if (PeekTokenType() == Token::Type::EOL)
						ReadToken();

					if (PeekTokenType() == Token::Type::STREAM_BEGIN)
					{
						auto resultStream = boost::intrusive_ptr<StreamObject>(new StreamObject(*result));
						*this >> *resultStream;

						return resultStream;
					}

					return result;
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
							return boost::intrusive_ptr<IndirectObjectReference>(new IndirectObjectReference(_file, IntegerObject(*token), IntegerObject(*gen_number)));
						}

						case Token::Type::INDIRECT_OBJECT_BEGIN:
						{
							auto reference_marker = ReadTokenWithType(Token::Type::INDIRECT_OBJECT_BEGIN);
							auto indirect = _file->GetIndirectObject(IntegerObject(*token), IntegerObject(*gen_number));

							ReadTokenWithType(Token::Type::EOL);
							auto data = readObject();

							indirect->SetObject(data);
							// TODO check cast if needed
							indirect->SetOffset(offset);

							return indirect;
						}
						default:
							seekg(pos);
						}
					}

					return boost::intrusive_ptr<IntegerObject>(new IntegerObject(*token));
				}
			case Token::Type::ARRAY_BEGIN:
				{
					auto result = boost::intrusive_ptr<ArrayObject>(new ArrayObject());
					*this >> *result;

					//auto token = readToken();
					//if (token->type() == Token::Type::INDIRECT_OBJECT_END)
					return result;

					// TODO is this possible?
					//assert(false);
					//break;
				}
			case Token::Type::NAME_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::NAME_OBJECT);
				return boost::intrusive_ptr<NameObject>(new NameObject(*token));
			}
			case Token::Type::HEXADECIMAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::HEXADECIMAL_STRING);
				return boost::intrusive_ptr<HexadecimalString>(new HexadecimalString(*token));
			}
			case Token::Type::LITERAL_STRING:
			{
				auto token = ReadTokenWithType(Token::Type::LITERAL_STRING);
				return boost::intrusive_ptr<LiteralString>(new LiteralString(*token));
			}
			case Token::Type::REAL_OBJECT:
			{
				auto token = ReadTokenWithType(Token::Type::REAL_OBJECT);
				return boost::intrusive_ptr<RealObject>(new RealObject(*token));
			}
			default:
				throw Exception("No valid object could be found at offset " + static_cast<int>(offset));
			}
		}

		boost::intrusive_ptr<Object> Parser::peekObject()
		{
			auto position = tellg();
			auto obj = readObject();
			seekg(position);

			return obj;
		}

		Parser::Parser(std::shared_ptr<files::File> file, std::shared_ptr<std::istream> stream) : lexical::Stream(*stream), _file(file) {}
		Parser::Parser(const gotchangpdf::lexical::Parser &other) : lexical::Stream(other) { _file = other.file(); }
		std::shared_ptr<files::File> Parser::file(void) const { return _file; }

		boost::intrusive_ptr<gotchangpdf::Object> Parser::readObjectWithType(gotchangpdf::IObject::Type type)
		{
			auto obj = readObject();

			switch (type)
			{
			case gotchangpdf::IObject::Type::Unknown:
				throw Exception("FIXME: Are your really trying to return unknown type??");
			case gotchangpdf::IObject::Type::ArrayObject:
				return boost::dynamic_pointer_cast<ArrayObject>(obj);
			case gotchangpdf::IObject::Type::Boolean:
				return boost::dynamic_pointer_cast<Boolean>(obj);
			case gotchangpdf::IObject::Type::DictionaryObject:
				return boost::dynamic_pointer_cast<DictionaryObject>(obj);
			case gotchangpdf::IObject::Type::Function:
				return boost::dynamic_pointer_cast<Function>(obj);
			case gotchangpdf::IObject::Type::IntegerObject:
				return boost::dynamic_pointer_cast<IntegerObject>(obj);
			case gotchangpdf::IObject::Type::NameObject:
				return boost::dynamic_pointer_cast<NameObject>(obj);
			case gotchangpdf::IObject::Type::NullObject:
				return boost::dynamic_pointer_cast<NullObject>(obj);
			case gotchangpdf::IObject::Type::RealObject:
				return boost::dynamic_pointer_cast<RealObject>(obj);
			case gotchangpdf::IObject::Type::StreamObject:
				return boost::dynamic_pointer_cast<StreamObject>(obj);
			case gotchangpdf::IObject::Type::HexadecimalString:
				return boost::dynamic_pointer_cast<HexadecimalString>(obj);
			case gotchangpdf::IObject::Type::LiteralString:
				return boost::dynamic_pointer_cast<LiteralString>(obj);
			case gotchangpdf::IObject::Type::IndirectReference:
				return boost::dynamic_pointer_cast<IndirectObjectReference>(obj);
			case gotchangpdf::IObject::Type::IndirectObject:
				return boost::dynamic_pointer_cast<IndirectObject>(obj);
			default:
				assert(false);
				throw Exception("FIXME: Unknown object type");
			}
		}

		//void Stream::SetDeep( bool deep ) { _deep = deep; }

		//bool Stream::GetDeep( void ) const { return _deep; }
	}
}
