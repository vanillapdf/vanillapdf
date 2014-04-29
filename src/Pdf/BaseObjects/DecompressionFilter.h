#ifndef _DECOMPRESSION_FILTER_H
#define _DECOMPRESSION_FILTER_H

#include "Filter.h"

namespace Pdf
{
	class DecompressionFilter : public Filter
	{
	public:
		enum class Type : unsigned char
		{
			LZWDecode,
			FlateDecode,
			RunLengthDecode,
			CCITTFaxDecode,
			JBIG2Decode,
			DCTDecode,
			JPXDecode,

			//TODO?
			Crypt
		};
	};
}

#endif /* _DECOMPRESSION_FILTER_H */
