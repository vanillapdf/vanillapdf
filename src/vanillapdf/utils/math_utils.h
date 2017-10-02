#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

#include "utils/conversion_utils.h"

#include <cassert>
#include <limits>
#include <sstream>

namespace vanillapdf {

template <typename Result, typename ValueFirst, typename ValueSecond = ValueFirst>
Result SafeAddition(ValueFirst number, ValueSecond addend) {
	Result result = number + addend;

	if (number < 0) {
		if (addend < 0) {
			if (result > number) {
				std::stringstream ss;
				ss << "Could not compute sum of " << number << " and " << addend;
				throw std::out_of_range(ss.str());
			}
		}

		return result;
	}

	if (result < number) {
		std::stringstream ss;
		ss << "Could not compute sum of " << number << " and " << addend;
		throw std::out_of_range(ss.str());
	}

	return result;
}

template <typename ResultType, typename ValueType>
ResultType SafeMultiply(ValueType number, ValueType multiplier) {
	// Special case
	if (number == 0 || multiplier == 0) {
		return 0;
	}

	// We need to unite singedness of all operands
	auto converted_number = ValueConvertUtils::SafeConvert<ResultType>(number);
	auto converted_multiplier = ValueConvertUtils::SafeConvert<ResultType>(multiplier);

	// If the maximum representable number divided by one of the operands
	// would result in number greater than the operand means,
	// that the multiply operation would overflow.
	if ((std::numeric_limits<ResultType>::max() / converted_multiplier) < converted_number) {
		std::stringstream ss;
		ss << "Could not multiply " << converted_number << " by " << converted_multiplier << ". The operation would overflow";
		throw std::overflow_error(ss.str());
	}

	// Compute the result
	ResultType result = (converted_number * converted_multiplier);

	// Validate the result
	if (converted_number != 0) {
		assert((result / converted_number) == converted_multiplier);
	}

	if (converted_multiplier != 0) {
		assert((result / converted_multiplier) == converted_number);
	}

	return result;
}

} // vanillapdf

#endif /* _MATH_UTILS_H */
