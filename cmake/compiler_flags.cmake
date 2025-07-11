# =====================================================================================
# \file compiler_flags.cmake
# \brief Compiler configuration defaults for Vanilla.PDF
#
# This file configures optimal compilation behavior for Vanilla.PDF across supported
# platforms and compilers. It avoids global flag pollution by applying settings
# per-target through `vanillapdf_target_compile_defaults()`.
#
# Features:
# - Applies DEBUG/RELEASE macros based on build configuration.
# - Enables `/WX` or `-Werror` to treat warnings as errors for all build types.
# - Enables strict warning levels for GCC/Clang.
# - Disables known noisy or non-critical warnings for improved developer ergonomics.
# - Configures MSVC CRT linkage via `CMAKE_MSVC_RUNTIME_LIBRARY`.
#
# To apply these defaults:
#   vanillapdf_target_compile_defaults(<target>)
#
# This module should be included after target creation.
# =====================================================================================

if(MSVC)

    # We would like to allow explicit configuration of the CMAKE_MSVC_RUNTIME_LIBRARY
    if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
        if(VANILLAPDF_USE_STATIC_CRT)
            message(STATUS "Using static MSVC runtime (/MT or /MTd)")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
        else()
            message(STATUS "Using dynamic MSVC runtime (/MD or /MDd)")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
        endif()
    endif()
endif()

# Utility functions that change the options on per-targer basis

function(vanillapdf_target_compile_defaults TARGET)

    # Define config-specific macros
    target_compile_definitions(${TARGET} PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:RELEASE>
    )

    # Warnings as errors for MSVC
    if(CMAKE_C_COMPILER_ID MATCHES "MSVC" OR CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${TARGET} PRIVATE /WX)
    endif()

    # Warnings as errors for GCC, Clang, AppleClang (C and C++)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${TARGET} PRIVATE -Werror)
    endif()

    # GCC/Clang warnings
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

        # Apply to both C and C++
        target_compile_options(${TARGET} PRIVATE

            # Enable maximum warning level
            -Wall
            -Wextra

            # GCC warns on pragma region directive, which is only IDE feature
            # It could be solved with #ifdef only for MSVC
            # All other pragmas are only for suppressing specific warnings on MSVC
            -Wno-unknown-pragmas

            # On Linux arm GCC the entire log is filled with notes:

            # parameter passing for argument of type 'std::_Rb_tree<vanillapdf::syntax::IndirectReferenceObject,
            # std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool>,
            # std::_Select1st<std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool> >,
            # std::less<vanillapdf::syntax::IndirectReferenceObject>,
            # std::allocator<std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool> > >::const_iterator' changed in GCC 7.1

            # That warning is telling you that there was a subtle ABI change
            # (actually a conformance fix) between 6 and 7.1,
            # such that libraries built with 6.x or earlier may not work properly when called
            # from code built with 7.x (and vice-versa).
            # As long as all your C++ code is built with GCC 7.1 or later, you can safely ignore this warning.
            # To disable it, pass -Wno-psabi to the compiler.
            # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=77728
            -Wno-psabi

            # There are quite some parameters defined in functions, however they are not used.
            # I do not really consider this an issue, as even having the name of the parameter gives you some insights.
            # Having just the type even if it is currently not used is not a bug for me.
            -Wno-unused-parameter
        )

        # Apply only to C++
        target_compile_options(${TARGET} PRIVATE

            # Variables in constructors should be initialized in order
            # they were declared. I prefer initializing variables
            # in order they make sense and are declared as constructor parameters.
            # This should be harmless unless you take at least a little care.
            $<$<COMPILE_LANGUAGE:CXX>:-Wno-reorder>
        )
    endif()
endfunction()
