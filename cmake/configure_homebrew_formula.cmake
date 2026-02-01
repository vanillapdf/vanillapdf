# Configure the Homebrew formula from its template.
#
# Usage:
#   cmake -P cmake/configure_homebrew_formula.cmake
#   cmake -DVANILLAPDF_RELEASE_TAG=v2.3.0 -DVANILLAPDF_SHA256=abc123 -P cmake/configure_homebrew_formula.cmake
#   cmake -DVANILLAPDF_HEAD_BRANCH=feature/xyz -P cmake/configure_homebrew_formula.cmake

if(NOT VANILLAPDF_RELEASE_TAG)
    include("${CMAKE_CURRENT_LIST_DIR}/version.cmake")
    set(VANILLAPDF_RELEASE_TAG "v${VANILLAPDF_VERSION_MAJOR}.${VANILLAPDF_VERSION_MINOR}.${VANILLAPDF_VERSION_PATCH}")
endif()

if(NOT VANILLAPDF_SHA256)
    set(VANILLAPDF_SHA256 "0000000000000000000000000000000000000000000000000000000000000000")
endif()

if(NOT VANILLAPDF_HEAD_BRANCH)
    set(VANILLAPDF_HEAD_BRANCH "main")
endif()

message(STATUS "Configuring Homebrew formula: tag=${VANILLAPDF_RELEASE_TAG}, branch=${VANILLAPDF_HEAD_BRANCH}")

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/../homebrew/vanillapdf.rb.in"
    "${CMAKE_CURRENT_LIST_DIR}/../homebrew/vanillapdf.rb"
    @ONLY
)
