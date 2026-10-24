# Locates the OpenSSL legacy provider module of the vcpkg OpenSSL.
#
# A shared OpenSSL loads the legacy provider at runtime as a separate module
# (legacy.dll, legacy.so) rather than linking it, from a directory fixed when
# OpenSSL was built. vcpkg leaves that directory pointing outside its
# installation, so the module has to be located explicitly: the tests point
# OPENSSL_MODULES at it, and the installation ships it next to the bundled
# OpenSSL. A static OpenSSL compiles the provider into libcrypto and has no
# module file, in which case nothing is found.
#
# Included from the top-level CMakeLists before the source directories. Sets:
#   VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR       - directory of the release module
#   VANILLAPDF_OPENSSL_LEGACY_MODULE_DEBUG_DIR - directory of the debug module

if(VANILLAPDF_ENABLE_ENCRYPTION AND VANILLAPDF_INTERNAL_VCPKG AND NOT VANILLAPDF_EXTERNAL_OPENSSL)

    # Windows keeps the module next to the DLLs, other platforms in lib/ossl-modules
    find_path(VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR
        NAMES legacy.dll legacy.so legacy.dylib
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin"
              "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/ossl-modules"
        NO_DEFAULT_PATH
    )

    find_path(VANILLAPDF_OPENSSL_LEGACY_MODULE_DEBUG_DIR
        NAMES legacy.dll legacy.so legacy.dylib
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin"
              "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib/ossl-modules"
        NO_DEFAULT_PATH
    )

    if(VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR)
        message(STATUS "OpenSSL legacy provider module found in ${VANILLAPDF_OPENSSL_LEGACY_MODULE_DIR}")
    else()
        message(STATUS "OpenSSL legacy provider has no module file, assuming static OpenSSL")
    endif()
endif()
