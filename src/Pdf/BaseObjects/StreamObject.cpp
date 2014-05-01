#include "StreamObject.h"
#include "IntegerObject.h"
#include "IndirectObjectReference.h"
#include "InvalidObjectTypeException.h"
#include "Lexical/Parser.h"
#include "File.h"
#include "Buffer.h"

#include <sstream>

namespace Pdf
{
	using Lexical::Parser;
	using namespace Lexical;
	using namespace std;

	StreamObject::StreamObject()
		: Object(Object::Type::StreamObject), _data(nullptr), _rawDataOffset(_BADOFF), _dictionary(nullptr), _rawDataLength(-1) {}

	StreamObject::StreamObject(boost::intrusive_ptr<DictionaryObject> dictionary)
		: Object(Object::Type::StreamObject), _data(nullptr), _rawDataOffset(_BADOFF), _dictionary(dictionary), _rawDataLength(-1) {}

	Parser& operator>>(Parser& s, StreamObject& o)
	{
		if (nullptr == o._dictionary)
		{
			o._dictionary = boost::intrusive_ptr<DictionaryObject>(new DictionaryObject());
			s >> *o._dictionary;
		}

		auto token = s.PeekToken();

		if (token->type() == Token::Type::STREAM_BEGIN)
			s.ReadToken();

		if (token->type() == Token::Type::EOL)
			s.ReadToken();

		// TODO "Length" 7 hardcoded
		auto size_raw = o._dictionary->Find(NameObject(Buffer("Length", 7)));
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

		if (s.PeekTokenType() == Token::Type::EOL)
			s.ReadToken();

		auto data = s.Read(len);
		Buffer se(data.get(), len);

		// TODO apply filter

		if (s.PeekTokenType() == Token::Type::EOL)
			s.ReadToken();

		s.ReadTokenWithType(Token::Type::STREAM_END);
		s.ReadTokenWithType(Token::Type::EOL);
		s.ReadTokenWithType(Token::Type::INDIRECT_OBJECT_END);

		o._rawDataOffset = offset;
		o._rawDataLength = len;
		o._data = std::shared_ptr<Buffer>(new Buffer(se));

		return s;
	}
}