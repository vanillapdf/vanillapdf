# Generate vcpkg.json from template with version information
#
# This script generates vcpkg.json from vcpkg.json.in, substituting the version
# from cmake/version.cmake. This ensures the version is defined in a single place.
#
# Must be included BEFORE vcpkg_init.cmake (which sets up the toolchain)

# Include version information first
include(${CMAKE_CURRENT_LIST_DIR}/version.cmake)

# Skip vcpkg.json generation when built as a vcpkg port (VANILLAPDF_INTERNAL_VCPKG=OFF)
# In that case, vcpkg manages dependencies externally and the source may be read-only
if(DEFINED VANILLAPDF_INTERNAL_VCPKG AND NOT VANILLAPDF_INTERNAL_VCPKG)
  message(STATUS "Skipping vcpkg.json generation (external vcpkg build)")
  return()
endif()

# Generate root vcpkg.json from template
set(VCPKG_JSON_IN_PATH "${VANILLAPDF_SOLUTION_SOURCE_DIR}/vcpkg.json.in")
set(VCPKG_JSON_PATH "${VANILLAPDF_SOLUTION_SOURCE_DIR}/vcpkg.json")

if(EXISTS "${VCPKG_JSON_IN_PATH}")
  configure_file(
    "${VCPKG_JSON_IN_PATH}"
    "${VCPKG_JSON_PATH}"
    @ONLY
  )
  message(STATUS "Generated vcpkg.json with version ${VANILLAPDF_VERSION}")
endif()
