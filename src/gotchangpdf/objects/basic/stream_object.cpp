#include "stream_object.h"

#include "integer_object.h"
#include "indirect_object_reference.h"
#include "invalid_object_type_exception.h"
#include "parser.h"
#include "file.h"
#include "buffer.h"
#include "character.h"
#include "object_visitors.h"

// TODO
#include "flate_decode_filter.h"

#include <sstream>

namespace gotchangpdf
{
	using namespace lexical;
	using namespace std;
	using namespace exceptions;
	using namespace character;

	StreamObject::StreamObject(files::File * file) : _file(file) {}
	StreamObject::StreamObject(const DictionaryObject& dictionary) : _dictionary(dictionary) {}

	Buffer StreamObject::GetData() const
	{
		if (_data.empty())
		{
			auto stream = _file->GetInputStream();
			if (auto locked = stream.lock())
			{
				auto size_raw = _dictionary->Find(constant::Name::Length);
				KillIndirectionVisitor<IntegerObjectPtr> visitor;
				IntegerObjectPtr size = size_raw.apply_visitor(visitor);

				size_t len = static_cast<size_t>(*size);

				auto stream = raw::Stream(*locked);
				auto pos = stream.tellg();
				stream.seekg(_raw_data_offset);
				_data = stream.read(len);
				stream.seekg(pos);

				stream.seekg(pos);
			}
			else
			{
				throw exceptions::Exception("Could not obtain fstream lock");
			}
		}

		auto filter = _dictionary->Find(constant::Name::Filter);
		if (filter.empty())
			return _data;

		ObjectVisitor<NameObject> visitor;
		auto filter_name = filter.apply_visitor(visitor);

		// TODO
		filters::FlateDecodeFilter a;
		return a.Decode(_data);
		//return ((Filters::FlateDecodeFilter*)(&*filter_name))->Apply(*_data);
	}

	Parser& operator>>(Parser& s, StreamObject& o)
	{
		/*
		if (!o._dictionary.HasContents())
		{
			s >> *o._dictionary;
		}
		*/
		if (s.PeekTokenType() == Token::Type::STREAM_BEGIN)
			s.ReadToken();

		if (s.PeekTokenType() == Token::Type::EOL)
		{
			auto token = s.ReadToken();
			if (token.Value().size() == 1 && Equals(token.Value()[0], WhiteSpace::CARRIAGE_RETURN))
			{
				stringstream buffer;
				buffer << "After stream keyword is single CR character at offset " << s.tellg();

				throw Exception(buffer.str());
			}
		}

		IntegerObjectPtr size;

		ObjectVisitor<boost::variant<IntegerObject, IndirectObjectReference>> visitor;
		auto size_raw = o._dictionary->Find(constant::Name::Length);
		int type = size_raw.which();

		/*
		auto type = size_raw->GetType();
		if (type == Object::Type::IntegerObject)
		{
			size = dynamic_wrapper_cast<IntegerObject>(size_raw);
		}
		else if (type == Object::Type::IndirectObjectReference)
		{
			auto ref = dynamic_wrapper_cast<IndirectObjectReference>(size_raw);
			auto indirect = ref->GetReferencedObject();
			auto obj = indirect->GetObject();

			if (obj->GetType() != Object::Type::IntegerObject)
			{
				stringstream buffer;
				buffer << "Length value is an Indirect Reference to an object of type "
					<< Object::TypeName(obj->GetType()) << " at offset " << indirect->GetOffset();

				throw Exception(buffer.str());
			}
			size = dynamic_wrapper_cast<IntegerObject>(obj);
		}
		else
		{
			throw InvalidObjectTypeException(*size_raw);
		}
		*/
		size_t len = static_cast<size_t>(*size);
		streamoff offset = s.tellg();

		auto data = s.read(len);

		if (s.PeekTokenType() == Token::Type::EOL)
			s.ReadToken();

		s.ReadTokenWithType(Token::Type::STREAM_END);
		s.ReadTokenWithType(Token::Type::EOL);
		s.ReadTokenWithType(Token::Type::INDIRECT_OBJECT_END);

		o._raw_data_offset = offset;
		o._data = data;

		return s;
	}
}