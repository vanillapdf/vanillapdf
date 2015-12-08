#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "buffer.h"
#include "dictionary_object.h"

#include <iosfwd>

namespace gotchangpdf
{
	namespace syntax
	{
		class StreamObject : public Object
		{
		public:
			StreamObject() = default;
			StreamObject(DictionaryObjectPtr header, types::stream_offset offset);
			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Stream; }
			virtual std::string ToString(void) const override;

			inline DictionaryObjectPtr GetHeader() const { return _header; }
			inline void SetHeader(DictionaryObjectPtr header) { _header = header; }

			inline types::stream_offset GetDataOffset() const { return _raw_data_offset; }
			inline void SetDataOffset(types::stream_offset offset) { _raw_data_offset = offset; }

			BufferPtr GetBody() const;
			BufferPtr GetBodyDecoded() const;

		private:
			DictionaryObjectPtr _header;
			types::stream_offset _raw_data_offset = std::_BADOFF;

			mutable BufferPtr _body;
			mutable BufferPtr _body_decoded;
		};
	}
}

#endif /* _STREAM_OBJECT_H */
