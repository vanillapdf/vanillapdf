#include "precompiled.h"

#include "syntax/filters/lzw_decode_filter.h"
#include "utils/character.h"

#include <string>
#include <unordered_map>

namespace vanillapdf {
namespace syntax {

using namespace std;

BufferPtr LZWDecodeFilter::Encode(IInputStreamPtr src, types::stream_size, DictionaryObjectPtr /* = DictionaryObjectPtr() */, AttributeListPtr /* = AttributeListPtr() */) const {
    throw NotSupportedException("LZWDecodeFilter encoding is not supported");
}

BufferPtr LZWDecodeFilter::Decode(IInputStreamPtr src, types::stream_size, DictionaryObjectPtr /* = DictionaryObjectPtr() */, AttributeListPtr /* = AttributeListPtr() */) const {
    throw NotSupportedException("LZWDecodeFilter decoding is not supported");
}

BufferPtr LZWDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {
    auto stream = src->ToInputStream();
    return Encode(stream, src->size(), parameters, object_attributes);
}

BufferPtr LZWDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {
    auto stream = src->ToInputStream();
    return Decode(stream, src->size(), parameters, object_attributes);
}

} // syntax
} // vanillapdf
