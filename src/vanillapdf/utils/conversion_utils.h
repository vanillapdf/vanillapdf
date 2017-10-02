#ifndef _CONVERSION_UTILS_H
#define _CONVERSION_UTILS_H

#include "utils/exceptions.h"

#include <limits>

namespace vanillapdf {

class ValueConvertUtils {
public:
	template <typename RangeType, typename ValueType>
	static bool IsInRange(ValueType value) {
		return Specializator<RangeType, ValueType>::IsInRange(value);
	}

	template <typename RangeType, typename SourceType>
	static RangeType SafeConvert(SourceType value) {
		if (!IsInRange<RangeType, SourceType>(value)) {
			throw ConversionExceptionFactory<RangeType>::Construct(value);
		}

		return static_cast<RangeType>(value);
	}

private:
	template <
		typename RangeType,
		typename ValueType,

		// This is to enable specialization for the same singedness
		bool = (std::is_signed<ValueType>::value ^ std::is_signed<RangeType>::value),

		// I had to divide logic for different singedness furthermore, because
		// while the compiler sees a body where I do compare signed and unsigned
		// even when the logic does not allow control flow to execute the comparison,
		// the compiler warning was issued
		bool = (std::is_signed<ValueType>::value)
	>
		class Specializator {
		public:
			static bool IsInRange(ValueType value) {

				// TODO I believe this code should be optimized away
				// in cases when it is evaluated to false at compile time.
				// Not quite sure how to verify this hypothesis.
				// It could be otherwise rewritten as another ugly template parameter.
				if (std::is_same<RangeType, ValueType>::value) {
					return true;
				}

				return (value >= std::numeric_limits<RangeType>::lowest()) &&
					(value <= std::numeric_limits<RangeType>::max());
			}
	};

	template <
		typename RangeType,
		typename ValueType
	>
		class Specializator<RangeType, ValueType, true, true> {
		public:
			static_assert(std::is_signed<ValueType>::value, "Value type shall be signed");

		public:
			static bool IsInRange(ValueType value) {
				if (value < 0) {
					return false;
				}

				using unsigned_value_type = typename std::make_unsigned<ValueType>::type;
				unsigned_value_type unsigned_value = static_cast<unsigned_value_type>(value);
				return Specializator<RangeType, unsigned_value_type>::IsInRange(unsigned_value);
			}
	};

	template <
		typename RangeType,
		typename ValueType
	>
		class Specializator<RangeType, ValueType, true, false> {
		public:
			static_assert(std::is_signed<RangeType>::value, "Range type shall be signed");

		public:
			static bool IsInRange(ValueType value) {
				using unsigned_range_type = typename std::make_unsigned<RangeType>::type;
				RangeType range_max = std::numeric_limits<RangeType>::max();

				// This operation shall be safe
				// Converting maximum signed value shall be in range of unsigned type of the same size
				unsigned_range_type unsigned_range_max = static_cast<unsigned_range_type>(range_max);
				return unsigned_range_max > value;
			}
	};
};

template <typename BaseT>
class ConvertUtils {
public:
	template <typename T>
	static T ConvertTo(BaseT obj) {
		return Specializator<T>::ConvertTo(obj);
	}

	template <typename T>
	static bool IsType(BaseT obj) {
		return Specializator<T>::IsType(obj);
	}

private:
	template <typename T>
	class Specializator {
	public:
		static T ConvertTo(BaseT obj) {
			auto ptr = obj.get();
			auto converted = dynamic_cast<typename T::deferred_ptr_type *>(ptr);
			if (nullptr == converted) {
				throw ConversionExceptionFactory<T>::Construct(obj);
			}

			return T(converted);
		}

		static bool IsType(BaseT obj) {
			auto ptr = obj.get();
			auto converted = dynamic_cast<typename T::deferred_ptr_type *>(ptr);
			return (nullptr != converted);
		}
	};
};

} // vanillapdf

#endif /* _CONVERSION_UTILS_H */
