vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO vanillapdf/vanillapdf
    REF "v${VERSION}"
    SHA512 2d076c6acf159e4fd9ea998053afe2600d2261233aa5e9e7275334044bbaf5fdd519f4774751e83ac1c444a378b40eb6a06bb2ba87c12296c0539075bd31930f
    HEAD_REF main
)

string(COMPARE EQUAL "${VCPKG_CRT_LINKAGE}" "static" VANILLAPDF_USE_STATIC_CRT)

# mimalloc serves only the library's own reference counted objects and adds no
# API, so it is always on where the mimalloc port is supported
if(VCPKG_TARGET_IS_UWP)
    set(VANILLAPDF_ENABLE_MIMALLOC OFF)
else()
    set(VANILLAPDF_ENABLE_MIMALLOC ON)
endif()

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
      -DVANILLAPDF_INTERNAL_VCPKG=OFF
      -DVANILLAPDF_USE_STATIC_CRT=${VANILLAPDF_USE_STATIC_CRT}
      -DVANILLAPDF_ENABLE_MIMALLOC=${VANILLAPDF_ENABLE_MIMALLOC}
)

vcpkg_cmake_install()

vcpkg_copy_pdbs()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME "vanillapdf"
    CONFIG_PATH "lib/cmake/vanillapdf"
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

vcpkg_install_copyright(
    FILE_LIST
        "${SOURCE_PATH}/LICENSE.txt"
        "${SOURCE_PATH}/NOTICE.md"
)
