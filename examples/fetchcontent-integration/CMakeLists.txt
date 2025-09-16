cmake_minimum_required(VERSION 3.20)

# Set up vcpkg toolchain before project() call
# This is the typical pattern for FetchContent users
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # Clone vcpkg for this example
    include(FetchContent)
    FetchContent_Declare(
        vcpkg
        GIT_REPOSITORY https://github.com/Microsoft/vcpkg.git
        GIT_TAG        2025.01.31
        SOURCE_DIR     ${CMAKE_BINARY_DIR}/vcpkg
    )
    FetchContent_MakeAvailable(vcpkg)

    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "Set up vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(FetchContentTest)

# Test real-world FetchContent integration with VanillaPDF
include(FetchContent)

# Determine which Git tag/commit to test against
if(DEFINED VANILLAPDF_GIT_TAG)
    set(GIT_TAG_TO_USE ${VANILLAPDF_GIT_TAG})
    message(STATUS "Testing against specified commit: ${GIT_TAG_TO_USE}")
elseif(DEFINED ENV{GITHUB_SHA})
    set(GIT_TAG_TO_USE $ENV{GITHUB_SHA})
    message(STATUS "Testing against CI commit: ${GIT_TAG_TO_USE}")
else()
    set(GIT_TAG_TO_USE "main")  # Fallback to main branch
    message(STATUS "Testing against main branch: ${GIT_TAG_TO_USE}")
endif()

# Use real Git repository with robust tag selection
FetchContent_Declare(
    vanillapdf
    GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
    GIT_TAG        ${GIT_TAG_TO_USE}
)

# Configure vanillapdf to use our vcpkg for some deps, system packages for others
set(VANILLAPDF_INTERNAL_VCPKG OFF CACHE BOOL "Disable vanillapdf internal vcpkg - use parent's vcpkg")

# Use system packages for major dependencies (faster than building through vcpkg)
set(VANILLAPDF_EXTERNAL_OPENSSL ON CACHE BOOL "Use system OpenSSL")
set(VANILLAPDF_EXTERNAL_JPEG ON CACHE BOOL "Use system JPEG")
set(VANILLAPDF_EXTERNAL_OPENJPEG ON CACHE BOOL "Use system OpenJPEG")
set(VANILLAPDF_EXTERNAL_ZLIB ON CACHE BOOL "Use system zlib")

# Use vcpkg for dependencies not available as system packages
set(VANILLAPDF_EXTERNAL_SPDLOG OFF CACHE BOOL "Use vcpkg spdlog")
set(VANILLAPDF_EXTERNAL_NLOHMANN_JSON OFF CACHE BOOL "Use vcpkg nlohmann-json")

# Test real-world FetchContent integration
message(STATUS "Testing real-world VanillaPDF FetchContent integration...")
message(STATUS "Fetching VanillaPDF from GitHub repository...")
message(STATUS "Parent project managing dependencies via vcpkg")
FetchContent_MakeAvailable(vanillapdf)

# Create a simple test executable to verify the integration works
add_executable(fetchcontent_test main.cpp)
target_link_libraries(fetchcontent_test PRIVATE vanillapdf::vanillapdf)

message(STATUS "✅ Real-world FetchContent integration successful!")
message(STATUS "   - VanillaPDF fetched from GitHub")
message(STATUS "   - Dependencies auto-managed")
message(STATUS "   - Target linked successfully")