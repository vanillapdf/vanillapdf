#ifndef _JPX_DECODE_FILTER_H
#define _JPX_DECODE_FILTER_H

#include "syntax/filters/filter.h"

namespace vanillapdf {
namespace syntax {

class JPXDecodeFilter : public FilterBase {
public:
	virtual Type GetType(void) const noexcept override { return FilterBase::Type::JPXDecode; }

	virtual BufferPtr Encode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;
	virtual BufferPtr Decode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;

	virtual BufferPtr Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;
	virtual BufferPtr Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;
};

} // syntax
} // vanillapdf

#endif /* _JPX_DECODE_FILTER_H */
