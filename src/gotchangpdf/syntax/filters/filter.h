#ifndef _FILTER_H
#define _FILTER_H

#include "syntax/utils/syntax_fwd.h"

#include "syntax/objects/dictionary_object.h"

namespace gotchangpdf {
namespace syntax {

class FilterBase : public virtual IUnknown, public IWeakReferenceable<FilterBase> {
public:
	enum class Type : unsigned char {
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

	static FilterBasePtr GetFilterByName(const NameObjectPtr name);

	virtual BufferPtr Encode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const = 0;
	virtual BufferPtr Decode(BufferPtr src, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const = 0;

	virtual BufferPtr Encode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const = 0;
	virtual BufferPtr Decode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters = DictionaryObjectPtr()) const = 0;

	virtual Type GetType(void) const noexcept = 0;
};

} // syntax
} // gotchangpdf

#endif /* _FILTER_H */
