#ifndef _C_EXPORT_H
#define _C_EXPORT_H

#include "c_platform.h"

/**
* \file c_export.h
* \brief This file contains macro declarations for importing
* and exporting symbols from library boundaries.
*/

/*
* Calling convention extended explanation
* GCC triggers a warning on arm processors "warning: 'cdecl' attribute directive ignored"
* From the documentation they state it is only applicable for Intel 386
* https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html
* On the Intel 386, the cdecl attribute causes the compiler to assume that the calling function
* will pop off the stack space used to pass arguments.This is useful to override the effects of the - mrtd switch.
*/

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO) && defined(PROCESSOR_X86)
    #define CALLING_CONVENTION __cdecl
#elif defined(COMPILER_GCC) && defined(PROCESSOR_X86)
    #define CALLING_CONVENTION __attribute__ ((cdecl))
#else
    #define CALLING_CONVENTION
#endif

#if defined(VANILLAPDF_CONFIGURATION_DLL) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
    #if defined(VANILLAPDF_EXPORTS)
        #define VANILLAPDF_API __declspec(dllexport)
    #else
        #define VANILLAPDF_API __declspec(dllimport)
    #endif
#else
    #define VANILLAPDF_API
#endif

#endif /* _C_EXPORT_H */
