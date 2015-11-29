#ifndef _UTIL_H
#define _UTIL_H

#include "exception.h"

#include <climits>

namespace gotchangpdf
{
	template <typename ValueType, typename RangeType>
	bool IsInRange(ValueType value) _NOEXCEPT {
		return (value >= std::numeric_limits<RangeType>::min()) &&
			(value <= std::numeric_limits<RangeType>::max());
	}

	template <typename DestType, typename SourceType>
	DestType SafeConvert(SourceType value)
	{
		if (!IsInRange<SourceType, DestType>(value))
			throw syntax::Exception("Integer value is out of range");

		return static_cast<DestType>(value);
	}
}

#endif /* _UTIL_H */
