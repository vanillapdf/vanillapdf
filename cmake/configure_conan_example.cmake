# Configure the Conan integration example conanfile from its template.
#
# Usage:
#   cmake -DVANILLAPDF_VERSION=2.3.0 -P cmake/configure_conan_example.cmake
#
# Or omit VANILLAPDF_VERSION to auto-detect from cmake/version.cmake.

if(NOT VANILLAPDF_VERSION)
    include("${CMAKE_CURRENT_LIST_DIR}/version.cmake")
    set(VANILLAPDF_VERSION "${VANILLAPDF_VERSION_MAJOR}.${VANILLAPDF_VERSION_MINOR}.${VANILLAPDF_VERSION_PATCH}")
endif()

message(STATUS "Configuring Conan example with version: ${VANILLAPDF_VERSION}")

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/../examples/conan-integration/conanfile.py.in"
    "${CMAKE_CURRENT_LIST_DIR}/../examples/conan-integration/conanfile.py"
    @ONLY
)
