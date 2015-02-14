#ifndef _UTIL_H
#define _UTIL_H

#include <climits>

namespace gotchangpdf
{
	template <typename ValueType, typename RangeType>
	bool IsInRange(ValueType value) _NOEXCEPT {
		return (value >= std::numeric_limits<RangeType>::min()) &&
			(value <= std::numeric_limits<RangeType>::max());
	}
}

#endif /* _UTIL_H */
