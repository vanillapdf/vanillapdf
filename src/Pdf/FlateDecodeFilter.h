#ifndef _FLATE_DECODE_FILTER_H
#define _FLATE_DECODE_FILTER_H

#include "BaseObjects/Filter.h"
#include "Buffer.h"

namespace Pdf
{
	namespace Filters
	{
		class FlateDecodeFilter : Filter
		{
		public:
			FlateDecodeFilter() : Filter(Type::FlateDecode) {}

			virtual Buffer Apply(Buffer src) const override
			{
			}
		};
	}
}

#endif /* _FLATE_DECODE_FILTER_H */
