# Generate documentation configuration
# Usage: cmake -P cmake/docs.cmake

include(${CMAKE_CURRENT_LIST_DIR}/minimum_version.cmake)
cmake_minimum_required(VERSION ${VANILLAPDF_MINIMUM_CMAKE_VERSION})

# The script assumes it is run from the project root
set(CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyfile.in
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyfile
    @ONLY
)
