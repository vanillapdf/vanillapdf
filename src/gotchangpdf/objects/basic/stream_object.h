#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "dictionary_object.h"

#include "boost/intrusive_ptr.hpp"

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

		//friend Streams::Lexical::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, StreamObject& o);
		friend lexical::Parser& operator>> (lexical::Parser& s, StreamObject& o);

	private:
		boost::intrusive_ptr<DictionaryObject> _dictionary;
		std::shared_ptr<Buffer> _data;
		streamOffsetValueType _rawDataOffset;
		streamSizeValueType _rawDataLength;
		Type _type;

		friend void ::boost::intrusive_ptr_add_ref(StreamObject*);
		friend void ::boost::intrusive_ptr_release(StreamObject*);
	};
}

#endif /* _STREAM_OBJECT_H */
