#ifndef _DCT_DECODE_FILTER_H
#define _DCT_DECODE_FILTER_H

#include "syntax/filters/filter.h"

namespace vanillapdf {
namespace syntax {

class DCTDecodeFilter : public FilterBase {
public:
	virtual Type GetType(void) const noexcept override { return FilterBase::Type::DCTDecode; }

	virtual BufferPtr Encode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
	virtual BufferPtr Decode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;

	virtual BufferPtr Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
	virtual BufferPtr Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const override;
};

} // syntax
} // vanillapdf

#endif /* _FLATE_DECODE_FILTER_H */
