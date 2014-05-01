#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "Fwd.h"
#include "Constants.h"
#include "BaseObjects/DictionaryObject.h"

#include "boost/intrusive_ptr.hpp"

#include <memory>

namespace Pdf
{
	class StreamObject : public Object
	{
	public:
		StreamObject();
		explicit StreamObject(boost::intrusive_ptr<DictionaryObject> dictionary);

		//friend Streams::Lexical::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, StreamObject& o);
		friend Lexical::Parser& operator>> (Lexical::Parser& s, StreamObject& o);

	private:
		boost::intrusive_ptr<DictionaryObject> _dictionary;
		std::shared_ptr<Buffer> _data;
		streamOffsetValueType _rawDataOffset;
		streamSizeValueType _rawDataLength;

		friend void ::boost::intrusive_ptr_add_ref(StreamObject*);
		friend void ::boost::intrusive_ptr_release(StreamObject*);
	};
}

#endif /* _STREAM_OBJECT_H */
