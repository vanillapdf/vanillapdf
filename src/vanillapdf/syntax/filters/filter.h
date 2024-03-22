#ifndef _FILTER_H
#define _FILTER_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/dictionary_object.h"

#include "utils/streams/input_stream_interface.h"

namespace vanillapdf {
namespace syntax {

class FilterBase : public IUnknown, public IWeakReferenceable<FilterBase> {
public:
	enum class Type : unsigned char {
		Undefined = 0,
		ASCIIHexDecode,
		ASCII85Decode,
		LZWDecode,
		FlateDecode,
		RunLengthDecode,
		CCITTFaxDecode,
		JBIG2Decode,
		DCTDecode,
		JPXDecode
	};

	static FilterBasePtr GetFilterByName(const NameObject& name);

	virtual BufferPtr Encode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const = 0;
	virtual BufferPtr Decode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const = 0;

	virtual BufferPtr Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const = 0;
	virtual BufferPtr Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr(), AttributeListPtr object_attributes = AttributeListPtr()) const = 0;

	virtual Type GetType(void) const noexcept = 0;
};

} // syntax
} // vanillapdf

#endif /* _FILTER_H */
