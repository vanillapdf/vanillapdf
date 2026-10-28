#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

#include "utils/conversion_utils.h"

#include <cassert>
#include <limits>
#include <type_traits>

#include <fmt/core.h>

namespace vanillapdf {

template <typename Result, typename ValueFirst, typename ValueSecond = ValueFirst>
Result SafeAddition(ValueFirst number, ValueSecond addend) {
    Result result = number + addend;

    if (number < 0) {
        if (addend < 0) {
            if (result > number) {
                throw std::out_of_range(
                    fmt::format("Could not compute sum of {} and {}", number, addend));
            }
        }

        return result;
    }

    if (result < number) {
        throw std::out_of_range(
            fmt::format("Could not compute sum of {} and {}", number, addend));
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
        throw fmt::format("Could not multiply {} by {}.The operation would overflow", converted_number, converted_multiplier);
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

template <
    typename T,
    typename = typename std::enable_if<std::is_integral<T>::value>::type
>
inline auto MostSignificantBit(T value) -> decltype(sizeof(T)) {

    decltype(MostSignificantBit(value)) result = 0;
    while (value > 0) {
        value = (value >> 1);
        result++;
    }

    return result;
}

/**
 * @brief Reverses the byte order of an unsigned \p value, mirroring C++23 std::byteswap.
 */
template <
    typename T,
    typename = typename std::enable_if<std::is_unsigned<T>::value>::type
>
inline T ByteSwap(T value) {
    T result = 0;
    for (decltype(sizeof(T)) i = 0; i < sizeof(T); ++i) {
        result = static_cast<T>((result << 8) | (value & 0xFF));
        value = static_cast<T>(value >> 8);
    }

    return result;
}

/**
 * @brief Rotates the bits of an unsigned \p value left by \p count positions.
 *
 * \p count is taken modulo the bit width of \p value.
 */
template <
    typename T,
    typename = typename std::enable_if<std::is_unsigned<T>::value>::type
>
inline T RotateLeft(T value, unsigned int count) {
    constexpr unsigned int bit_width = std::numeric_limits<T>::digits;

    // Shifting by the full bit width is undefined behavior
    count %= bit_width;
    if (count == 0) {
        return value;
    }

    return static_cast<T>((value << count) | (value >> (bit_width - count)));
}

} // vanillapdf

#endif /* _MATH_UTILS_H */
