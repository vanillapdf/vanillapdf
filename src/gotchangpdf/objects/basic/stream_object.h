#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "buffer.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	class StreamObject : public Object
	{
	public:
		StreamObject() = default;

		virtual inline Object::Type GetType(void) const override { return Object::Type::Stream; }

		BufferPtr GetBody() const { return _body; }
		void SetBody(BufferPtr body) { _body = body; }

		DictionaryObjectPtr GetHeader() const { return _header; }
		void SetHeader(DictionaryObjectPtr header) {_header = header; }

		BufferPtr GetBodyDecoded() const;

	public:
		DictionaryObjectPtr _header;
		types::stream_offset _raw_data_offset = std::_BADOFF;

	private:
		BufferPtr _body;
	};
}

#endif /* _STREAM_OBJECT_H */
