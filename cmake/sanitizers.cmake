option(VANILLAPDF_ENABLE_ASAN "Enable AddressSanitizer instrumentation (use with Debug builds)" OFF)
option(VANILLAPDF_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer instrumentation (use with Debug builds)" OFF)
option(VANILLAPDF_ENABLE_TSAN "Enable ThreadSanitizer instrumentation (use with Debug builds)" OFF)

# ASan and TSan instrument the memory subsystem in incompatible ways and
# cannot be loaded in the same process.
if(VANILLAPDF_ENABLE_ASAN AND VANILLAPDF_ENABLE_TSAN)
    message(FATAL_ERROR
        "VANILLAPDF_ENABLE_ASAN (AddressSanitizer) and "
        "VANILLAPDF_ENABLE_TSAN (ThreadSanitizer) cannot be enabled simultaneously.")
endif()

function(enable_asan_for_target target)
    if(VANILLAPDF_ENABLE_ASAN)
        if(MSVC)
            message(STATUS "ASan enabled for target: ${target}")
            target_compile_options(${target} PRIVATE /fsanitize=address /Zi)
            target_link_options(${target} PRIVATE /fsanitize=address)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
            message(STATUS "ASan enabled for target: ${target}")
            target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=address)
        else()
            message(WARNING "AddressSanitizer is not supported for this compiler")
        endif()
    endif()
endfunction()

function(enable_ubsan_for_target target)
    if(VANILLAPDF_ENABLE_UBSAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
            message(STATUS "UBSan enabled for target: ${target}")
            # -fno-sanitize-recover=all: abort on first UB instead of continuing.
            target_compile_options(${target} PRIVATE
                -fsanitize=undefined
                -fno-sanitize-recover=all
                -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=undefined)
        else()
            message(WARNING "UndefinedBehaviorSanitizer is not supported for this compiler")
        endif()
    endif()
endfunction()

function(enable_tsan_for_target target)
    if(VANILLAPDF_ENABLE_TSAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
            message(STATUS "TSan enabled for target: ${target}")
            target_compile_options(${target} PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=thread)
        else()
            message(WARNING "ThreadSanitizer is not supported for this compiler")
        endif()
    endif()
endfunction()
