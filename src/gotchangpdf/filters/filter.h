#ifndef _FILTER_H
#define _FILTER_H

#include "fwd.h"

namespace gotchangpdf
{
	namespace filters
	{
		class Filter
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

			virtual BufferPtr Encode(BufferPtr src) const = 0;
			virtual BufferPtr Decode(BufferPtr src) const = 0;

			virtual inline Type GetType(void) const = 0;
		};
	}
}

#endif /* _FILTER_H */
