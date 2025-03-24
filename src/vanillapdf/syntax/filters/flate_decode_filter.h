#ifndef _FLATE_DECODE_FILTER_H
#define _FLATE_DECODE_FILTER_H

#include "syntax/filters/filter.h"

namespace vanillapdf {
namespace syntax {

class FlateDecodeFilter : public FilterBase {
public:
    virtual Type GetType(void) const noexcept override { return FilterBase::Type::FlateDecode; }

    virtual BufferPtr Encode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;
    virtual BufferPtr Decode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;

    virtual BufferPtr Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;
    virtual BufferPtr Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const override;

    BufferPtr ApplyPredictor(BufferPtr src, DictionaryObjectPtr parameters) const;
    BufferPtr ApplyPredictor(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters) const;
};

} // syntax
} // vanillapdf

#endif /* _FLATE_DECODE_FILTER_H */
