option(VANILLAPDF_ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)

function(enable_coverage_for_target target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Code coverage enabled for target: ${target}")
        target_compile_options(${target} PRIVATE --coverage)
        target_link_options(${target} PRIVATE --coverage)
        # Use atomic profile updates to prevent race conditions in multi-threaded tests
        # Without this, gcov counters can overflow/corrupt when threads execute same code paths
        # See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=68080
        target_compile_options(${target} PRIVATE -fprofile-update=atomic)
    else()
        message(WARNING "Code coverage is only supported with GCC or Clang")
    endif()
endfunction()
