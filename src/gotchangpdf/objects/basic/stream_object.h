#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "buffer.h"

#include <memory>

namespace gotchangpdf
{
	class StreamObject : public Object
	{
	public:

		explicit StreamObject(DictionaryObjectPtr dictionary);

		BufferPtr GetData() const;

		virtual inline Object::Type GetType(void) const override { return Object::Type::Stream; }

	public:
		DictionaryObjectPtr _dictionary;
		types::stream_offset _raw_data_offset = std::_BADOFF;

	private:
		mutable BufferPtr _data;

		explicit StreamObject() = default;
		friend StreamObjectPtr;

		template <typename T>
		friend T* Allocate();
	};
}

#endif /* _STREAM_OBJECT_H */
