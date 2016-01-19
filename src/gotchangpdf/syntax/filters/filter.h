#ifndef _FILTER_H
#define _FILTER_H

#include "syntax_fwd.h"

#include "deferred.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class FilterBase : public IUnknown
		{
		public:
			enum class Type : unsigned char
			{
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

			virtual Type GetType(void) const = 0;
		};
	}
}

#endif /* _FILTER_H */
