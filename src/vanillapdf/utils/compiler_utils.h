#ifndef _COMPILER_UTILS_H
#define _COMPILER_UTILS_H

/**
 * @brief Inform the compiler/optimizer that a code path is unreachable.
 *
 * In debug builds, callers should assert before reaching this point.
 * In release builds, this hint allows the optimizer to eliminate the dead
 * branch and avoid propagating its effects (e.g. null pointer values)
 * through inlined call chains.
 */
#if defined(_MSC_VER)
    #define VANILLAPDF_UNREACHABLE() __assume(false)
#else
    #define VANILLAPDF_UNREACHABLE() __builtin_unreachable()
#endif

#endif /* _COMPILER_UTILS_H */
