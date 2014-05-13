#include "stream_object.h"
#include "integer_object.h"
#include "indirect_object_reference.h"
#include "invalid_object_type_exception.h"
#include "parser.h"
#include "File.h"
#include "Buffer.h"

// TODO
#include "flate_decode_filter.h"

#include <sstream>

namespace gotchangpdf
{
	using namespace lexical;
	using namespace std;
	using namespace exceptions;

	StreamObject::StreamObject()
		: Object(Object::Type::StreamObject), _data(nullptr), _rawDataOffset(_BADOFF), _rawDataLength(-1), _type(Type::UNKNOWN) {}

	StreamObject::StreamObject(DictionaryObject& dictionary)
		: Object(Object::Type::StreamObject), _data(nullptr), _rawDataOffset(_BADOFF), _dictionary(&dictionary), _rawDataLength(-1), _type(Type::UNKNOWN) {}

	Buffer StreamObject::GetData() const
	{
		auto filter_name = _dictionary->Find(gotchangpdf::constant::Name::Filter);
		if (nullptr == filter_name)
			return *_data;

		// TODO
		filters::FlateDecodeFilter a;
		return a.Decode(*_data);
		//return ((Filters::FlateDecodeFilter*)(&*filter_name))->Apply(*_data);
	}

	Parser& operator>>(Parser& s, StreamObject& o)
	{
		if (nullptr == o._dictionary)
		{
			o._dictionary = ObjectReferenceWrapper<DictionaryObject>(new DictionaryObject());
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

		auto size_raw = o._dictionary->Find(constant::Name::Length);
		IntegerObject size;
		auto type = size_raw->GetType();
		if (type == Object::Type::IntegerObject)
		{
			size = *size_raw.GetAs<IntegerObject>();
		}
		else if (type == Object::Type::IndirectReference)
		{
			auto ref = size_raw.GetAs<IndirectObjectReference>();
			auto indirect = ref->GetReference();
			auto obj = indirect->GetObject();

			if (obj->GetType() != Object::Type::IntegerObject)
			{
				stringstream buffer;
				buffer << "Length value is an Indirect Reference to an object of type "
					<< Object::TypeName(obj->GetType()) << " at offset " << indirect->GetOffset();

				throw Exception(buffer.str());
			}
			size = *obj.GetAs<IntegerObject>();
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