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
# A missing provider module is not fatal: the library logs a warning, and only
# the legacy algorithms (RC2, MD2/MD4) report NotSupported when used. So only the
# tests that use them need the environment, and a test that is missing it fails
# instead of passing silently.
#
# Included from the top-level CMakeLists before the test directories. Provides
# VANILLAPDF_TEST_ENVIRONMENT, in the syntax of the ENVIRONMENT_MODIFICATION test
# property, which the test directories set on the tests that need it. It is empty
# when the tests need no environment, which leaves the environment unchanged.

set(VANILLAPDF_TEST_ENVIRONMENT "")

if(VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER AND VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR)
    message(STATUS "Tests load the OpenSSL legacy provider from ${VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR}")
    set(VANILLAPDF_TEST_ENVIRONMENT "OPENSSL_MODULES=set:${VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR}")
endif()
