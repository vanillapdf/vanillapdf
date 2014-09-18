#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "dictionary_object.h"
#include "smart_ptr.h"

#include <memory>

namespace gotchangpdf
{
	class StreamObject : public Object
	{
	public:
		enum class Type : unsigned char
		{
			UNKNOWN = 0,
			CLASSIC,
			OBJECT_STREAM
		};

		StreamObject();
		explicit StreamObject(DictionaryObject& dictionary);

		Buffer GetData() const;

		virtual inline Object::Type GetType(void) const override { return Object::Type::StreamObject; }

		//friend Streams::Lexical::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, StreamObject& o);
		friend lexical::Parser& operator>> (lexical::Parser& s, StreamObject& o);

	private:
		SmartPtr<DictionaryObject> _dictionary = nullptr;
		Buffer _data;
		streamOffsetValueType _rawDataOffset = std::_BADOFF;
		streamSizeValueType _rawDataLength = -1;
		Type _type = Type::UNKNOWN;
	};
}

#endif /* _STREAM_OBJECT_H */
