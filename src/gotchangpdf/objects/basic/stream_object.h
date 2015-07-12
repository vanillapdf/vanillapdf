#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "buffer.h"
#include "dictionary_object.h"

#include <iosfwd>

namespace gotchangpdf
{
	class StreamObject : public Object
	{
	public:
		StreamObject() = default;

		virtual inline Object::Type GetType(void) const _NOEXCEPT override{ return Object::Type::Stream; }

		inline DictionaryObjectPtr GetHeader() const { return _header; }
		inline void SetHeader(DictionaryObjectPtr header) { _header = header; }

		BufferPtr GetBody() const;
		BufferPtr GetBodyDecoded() const;

	public:
		DictionaryObjectPtr _header;
		types::stream_offset _raw_data_offset = std::_BADOFF;

	private:
		mutable BufferPtr _body;
		mutable BufferPtr _body_decoded;
	};
}

#endif /* _STREAM_OBJECT_H */
