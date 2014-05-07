#include "StreamObject.h"
#include "IntegerObject.h"
#include "IndirectObjectReference.h"
#include "InvalidObjectTypeException.h"
#include "Lexical/Parser.h"
#include "File.h"
#include "Buffer.h"

// TODO
#include "FlateDecodeFilter.h"

#include <sstream>

namespace Pdf
{
	using Lexical::Parser;
	using namespace Lexical;
	using namespace std;

	StreamObject::StreamObject()
		: Object(Object::Type::StreamObject), _data(nullptr), _rawDataOffset(_BADOFF), _dictionary(nullptr), _rawDataLength(-1), _type(Type::UNKNOWN) {}

	StreamObject::StreamObject(DictionaryObject& dictionary)
		: Object(Object::Type::StreamObject), _data(nullptr), _rawDataOffset(_BADOFF), _dictionary(boost::intrusive_ptr<DictionaryObject>(&dictionary)), _rawDataLength(-1), _type(Type::UNKNOWN) {}

	Buffer StreamObject::GetData() const
	{
		auto filter_name = _dictionary->Find(Pdf::Constant::Name::Filter);
		if (nullptr == filter_name)
			return *_data;

		// TODO
		Filters::FlateDecodeFilter a;
		return a.Apply(*_data);
		//return ((Filters::FlateDecodeFilter*)(&*filter_name))->Apply(*_data);
	}

	Parser& operator>>(Parser& s, StreamObject& o)
	{
		if (nullptr == o._dictionary)
		{
			o._dictionary = boost::intrusive_ptr<DictionaryObject>(new DictionaryObject());
			s >> *o._dictionary;
		}

		if (s.PeekTokenType() == Token::Type::STREAM_BEGIN)
			s.ReadToken();

		if (s.PeekTokenType() == Token::Type::EOL)
		{
			auto token = s.ReadToken();
			if (token->value().Size() == 1 && token->value()[0] == Character::WhiteSpace::CARRIAGE_RETURN)
			{
				stringstream buffer;
				buffer << "After stream keyword is single CR character at offset " << s.tellg();

				throw Exception(buffer.str());
			}
		}

		auto size_raw = o._dictionary->Find(Constant::Name::Length);
		IntegerObject size;
		auto type = size_raw->GetType();
		if (type == Object::Type::IntegerObject)
		{
			size = *boost::dynamic_pointer_cast<IntegerObject>(size_raw);
		}
		else if (type == Object::Type::IndirectReference)
		{
			auto ref = boost::dynamic_pointer_cast<IndirectObjectReference>(size_raw);
			auto indirect = ref->GetReference();
			auto obj = indirect->GetObject();

			if (obj->GetType() != Object::Type::IntegerObject)
			{
				stringstream buffer;
				buffer << "Length value is an Indirect Reference to an object of type "
					<< Object::TypeName(obj->GetType()) << " at offset " << indirect->GetOffset();

				throw Exception(buffer.str());
			}

			size = *boost::dynamic_pointer_cast<IntegerObject>(obj);
		}
		else
		{
			throw InvalidObjectTypeException(*size_raw);
		}

		size_t len = static_cast<size_t>(size);
		streamoff offset = s.tellg();

		auto data = s.Read(len);
		auto buffer = std::shared_ptr<Buffer>(new Buffer(data.get(), len));

		if (s.PeekTokenType() == Token::Type::EOL)
			s.ReadToken();

		s.ReadTokenWithType(Token::Type::STREAM_END);
		s.ReadTokenWithType(Token::Type::EOL);
		s.ReadTokenWithType(Token::Type::INDIRECT_OBJECT_END);

		o._rawDataOffset = offset;
		o._rawDataLength = len;
		o._data = buffer;

		return s;
	}
}