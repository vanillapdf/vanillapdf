#ifndef _FLATE_DECODE_FILTER_H
#define _FLATE_DECODE_FILTER_H

#include "filter.h"
#include "buffer.h"

namespace gotchangpdf
{
	namespace filters
	{
		class FlateDecodeFilter : Filter
		{
		public:
			virtual inline Type GetType(void) const override { return Filter::Type::FlateDecode; }

			virtual BufferPtr Encode(BufferPtr src) const override;
			virtual BufferPtr Decode(BufferPtr src) const override;
		};
	}
}

#endif /* _FLATE_DECODE_FILTER_H */
