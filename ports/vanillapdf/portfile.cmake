set(_vanillapdf_github_token "")
if(DEFINED ENV{GITHUB_TOKEN} AND NOT "$ENV{GITHUB_TOKEN}" STREQUAL "")
    set(_vanillapdf_github_token "$ENV{GITHUB_TOKEN}")
elseif(DEFINED ENV{GH_TOKEN} AND NOT "$ENV{GH_TOKEN}" STREQUAL "")
    set(_vanillapdf_github_token "$ENV{GH_TOKEN}")
endif()

if(_vanillapdf_github_token)
    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO vanillapdf/vanillapdf
        REF "v${VERSION}"
        SHA512 f15d9a290de0eebac9073503ac555cbf389484aa3ff6385697ba879c336ed9cd4277af180f9d842b5bd8cca69bf6ef4dcfbedba07a6a76014e3974fe09fc6190
        HEAD_REF main
        AUTHORIZATION_TOKEN "${_vanillapdf_github_token}"
    )
else()
    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO vanillapdf/vanillapdf
        REF "v${VERSION}"
        SHA512 f15d9a290de0eebac9073503ac555cbf389484aa3ff6385697ba879c336ed9cd4277af180f9d842b5bd8cca69bf6ef4dcfbedba07a6a76014e3974fe09fc6190
        HEAD_REF main
    )
endif()

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
      -DVANILLAPDF_INTERNAL_VCPKG=OFF
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
