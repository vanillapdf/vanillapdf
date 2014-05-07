#ifndef _FILTER_H
#define _FILTER_H

#include "Fwd.h"

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

			Filter(Type type) : _type(type) {}

			virtual Buffer Encode(Buffer& src) const = 0;
			virtual Buffer Decode(Buffer& src) const = 0;

		protected:
			Type _type;
		};
	}
}

#endif /* _FILTER_H */
