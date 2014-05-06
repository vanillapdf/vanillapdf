#ifndef _FILTER_H
#define _FILTER_H

#include "Fwd.h"

namespace Pdf
{
	namespace Filters
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

			virtual Buffer Apply(Buffer& src) const = 0;

		protected:
			Type _type;
		};
	}
}

#endif /* _FILTER_H */
