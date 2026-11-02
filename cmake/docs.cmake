# Generate documentation configuration
# Usage: cmake -P cmake/docs.cmake

cmake_minimum_required(VERSION 3.15)

# The script assumes it is run from the project root
set(CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyfile.in
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyfile
    @ONLY
)

# c_vanillapdf_config.h is generated when the library is configured, which the
# documentation build never does. Document it as the full-featured build.
set(VANILLAPDF_FEATURE_ENCRYPTION ON)
set(VANILLAPDF_FEATURE_JPEG ON)
set(VANILLAPDF_FEATURE_JPEG2000 ON)
set(VANILLAPDF_FEATURE_ZLIB ON)
set(VANILLAPDF_SHARED_LIBRARY ON)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vanillapdf/c_vanillapdf_config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/include/vanillapdf/c_vanillapdf_config.h
)
