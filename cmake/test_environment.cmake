# Runtime environment of the test executables.
#
# Without VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER the tests get no environment
# at all, so they run exactly as an application consuming the library would.
#
# With it, the library loads the OpenSSL legacy provider at initialization. A
# shared OpenSSL needs the location of the provider module (see
# cmake/openssl_legacy_module.cmake), which the tests receive through
# OPENSSL_MODULES - the same thing an application shipping a shared OpenSSL
# has to do. The release module serves every configuration, providers talk to
# libcrypto through OpenSSL's dispatch tables rather than the C runtime.
#
# Included from the top-level CMakeLists before the test directories. Provides:
#   vanillapdf_apply_test_environment() - applies the environment to every test
#                                         added so far in the calling directory
#   VANILLAPDF_TEST_ENVIRONMENT         - the environment, for gtest_discover_tests

set(VANILLAPDF_TEST_ENVIRONMENT "")

if(VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER AND VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR)
    message(STATUS "Tests load the OpenSSL legacy provider from ${VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR}")
    set(VANILLAPDF_TEST_ENVIRONMENT "OPENSSL_MODULES=${VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR}")
endif()

function(vanillapdf_apply_test_environment)
    if(NOT VANILLAPDF_TEST_ENVIRONMENT)
        return()
    endif()

    get_property(VANILLAPDF_DIRECTORY_TESTS DIRECTORY PROPERTY TESTS)
    if(VANILLAPDF_DIRECTORY_TESTS)
        set_tests_properties(${VANILLAPDF_DIRECTORY_TESTS} PROPERTIES ENVIRONMENT "${VANILLAPDF_TEST_ENVIRONMENT}")
    endif()
endfunction()
