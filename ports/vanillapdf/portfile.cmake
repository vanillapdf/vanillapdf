vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO vanillapdf/vanillapdf
    REF "v${VERSION}"
    SHA512 f15d9a290de0eebac9073503ac555cbf389484aa3ff6385697ba879c336ed9cd4277af180f9d842b5bd8cca69bf6ef4dcfbedba07a6a76014e3974fe09fc6190
)

vcpkg_check_features(
    OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    encryption   -DVANILLAPDF_ENABLE_ENCRYPTION=ON
    jpeg         -DVANILLAPDF_ENABLE_JPEG=ON
    jpeg2000     -DVANILLAPDF_ENABLE_JPEG2000=ON
    tests        -DVANILLAPDF_ENABLE_TESTS=ON
    benchmarks   -DVANILLAPDF_ENABLE_BENCHMARK=ON
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
      -DVANILLAPDF_MANAGE_DEPS=OFF
      -DVANILLAPDF_ENABLE_PACKAGING=OFF
      ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()

# Ensure debug symbols are copied for proper installation
vcpkg_copy_pdbs()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME "vanillapdf"
    CONFIG_PATH "lib/cmake/vanillapdf"
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage"
     DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

vcpkg_install_copyright(
    FILE_LIST
        "${SOURCE_PATH}/LICENSE.txt"
        "${SOURCE_PATH}/NOTICE.md"
)
