#ifndef _BYTE_ORDER_H
#define _BYTE_ORDER_H

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    #include <bit>
#endif

namespace vanillapdf {

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)

using endian = std::endian;

#else

/**
 * @brief Byte order enumeration, mirroring C++20 std::endian.
 *
 * When compiled with C++20 or later, this is an alias for std::endian.
 */
enum class endian {
#if defined(_MSC_VER) && !defined(__clang__)
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

#endif

} // vanillapdf

#endif /* _BYTE_ORDER_H */
