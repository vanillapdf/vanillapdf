# Runtime environment of the test executables.
#
# Without VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER the tests get no environment
# at all, so they run exactly as an application consuming the library would.
#
# With it, the library loads the OpenSSL legacy provider at initialization. A
# static OpenSSL compiles the provider into libcrypto. A shared OpenSSL loads it
# as a module from a directory fixed when OpenSSL was built, which vcpkg leaves
# pointing outside its installation, so the tests are directed to the vcpkg copy
# through OPENSSL_MODULES - the same thing an application shipping a shared
# OpenSSL has to do.
#
# Included from the top-level CMakeLists before the test directories. Provides:
#   vanillapdf_apply_test_environment() - applies the environment to every test
#                                         added so far in the calling directory
#   VANILLAPDF_TEST_ENVIRONMENT         - the environment, for gtest_discover_tests

set(VANILLAPDF_TEST_ENVIRONMENT "")

if(VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER AND VANILLAPDF_INTERNAL_VCPKG AND NOT VANILLAPDF_EXTERNAL_OPENSSL)

    # Windows keeps the provider next to the DLLs, other platforms in lib/ossl-modules.
    # A static triplet has no module file, find_path then fails and nothing is set.
    find_path(VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR
        NAMES legacy.dll legacy.so legacy.dylib
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin"
              "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/ossl-modules"
        NO_DEFAULT_PATH
    )

    if(VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR)
        message(STATUS "Tests load the OpenSSL legacy provider from ${VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR}")
        set(VANILLAPDF_TEST_ENVIRONMENT "OPENSSL_MODULES=${VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR}")
    else()
        message(STATUS "OpenSSL legacy provider has no module file, assuming static OpenSSL")
    endif()
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
