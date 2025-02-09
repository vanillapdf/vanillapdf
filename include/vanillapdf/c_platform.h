#ifndef _C_PLATFORM_H
#define _C_PLATFORM_H

/**
* \file c_platform.h
* \brief This file contains macro declarations about current platform and compiler.
*/

#if defined(_MSC_VER)
    #define COMPILER_MICROSOFT_VISUAL_STUDIO

    #ifdef _M_X86
        #define PROCESSOR_X86
    #endif
#endif /* _MSC_VER */

// Check windows
#if defined(_WIN64)
    #define ENVIRONMENT_64_BIT
#endif /* _WIN64 */

// Check GCC
#if defined(__GNUC__) && !defined(__clang__)
    #define COMPILER_GCC

    #ifdef __i386__
        #define PROCESSOR_X86
    #endif

    #if (__x86_64__ || __ppc64__ || __aarch64__)
        #define ENVIRONMENT_64_BIT
    #endif /* __x86_64__ || __ppc64__ || __aarch64__ */
#endif /* defined(__GNUC__) && !defined(__clang__) */

// Check clang
#if defined(__clang__)
    #define COMPILER_CLANG

    #ifdef __i386__
        #define PROCESSOR_X86
    #endif

    #if (__x86_64__ || __ppc64__ || __aarch64__)
        #define ENVIRONMENT_64_BIT
    #endif /* __x86_64__ || __ppc64__ || __aarch64__ */
#endif /* defined(__clang__) */

#if !defined(ENVIRONMENT_64_BIT)
    #define ENVIRONMENT_32_BIT
#endif /* ENVIRONMENT_64_BIT */

#endif /* _C_PLATFORM_H */
