#ifndef _MISC_UTILS_H
#define _MISC_UTILS_H

#include "utils/buffer.h"
#include "utils/crypto_utils.h"
#include "utils/message_digest_algorithm.h"

#include <string>
#include <charconv>
#include <system_error>

namespace vanillapdf {

class MiscUtils {
public:
    static BufferPtr ToBase64(const Buffer& value);
    static BufferPtr FromBase64(const Buffer& value);

    static BufferPtr CalculateHash(const Buffer& data, MessageDigestAlgorithm digest_algorithm);

    static std::string ExtractFilename(const std::string& path);
    static bool CaseInsensitiveCompare(const std::string& left, const std::string& right);

    template <typename T>
    static T FromChars(const char* const first, const char* const last, const int base = 10) {

        T value;

        std::from_chars_result result;

        if constexpr (std::is_floating_point_v<T>) {
            result = std::from_chars(first, last, value);
        }

        if constexpr (std::is_integral_v<T>) {
            result = std::from_chars(first, last, value, base);
        }

        if (result.ec != std::errc()) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "Could not parse {} from {}: {}",
                typeid(T).name(),
                first,
                std::make_error_code(result.ec).message());
        }

        return value;
    }

    template <typename T>
    static T FromChars(std::string_view str, const int base = 10) {
        return FromChars<T>(str.data(), str.data() + str.size(), base);
    }

private:
    MiscUtils();
};

// AppleClang does not support std::from_chars for double, so let's have a fallback
// error: call to deleted function 'from_chars'

#if VANILLAPDF_NO_FROM_CHARS_FLOAT

template <>
inline double MiscUtils::FromChars<double>(const char* const first, const char* const last, const int) {

    auto string_size = last - first;
    auto string_size_converted = ValueConvertUtils::SafeConvert<size_t>(string_size);
    std::string_view str(first, string_size_converted);

    char* endptr = nullptr;
    double value = std::strtod(str.data(), &endptr);
    if (endptr != last) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Could not parse double from '{}'", str);
    }

    return value;
}

#endif /* VANILLAPDF_NO_FROM_CHARS_FLOAT */


} // vanillapdf

#endif /* _MISC_UTILS_H */
