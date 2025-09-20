cmake_minimum_required(VERSION 3.20)

# Set up vcpkg toolchain before project() call - standard practice
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # Clone vcpkg for this example
    include(FetchContent)
    FetchContent_Declare(
        vcpkg
        GIT_REPOSITORY https://github.com/Microsoft/vcpkg.git
        GIT_TAG        2025.08.27
        SOURCE_DIR     ${CMAKE_BINARY_DIR}/vcpkg
    )
    FetchContent_MakeAvailable(vcpkg)

    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "Set up vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# Configure which features to enable (before project() for proper vcpkg timing)
option(FETCHCONTENT_EXAMPLE_ENABLE_ENCRYPTION "Enable PDF encryption/decryption support" ON)
option(FETCHCONTENT_EXAMPLE_ENABLE_JPEG "Enable JPEG image support" ON)
option(FETCHCONTENT_EXAMPLE_ENABLE_JPEG2000 "Enable JPEG2000 image support" ON)
option(FETCHCONTENT_EXAMPLE_ENABLE_ZLIB "Enable zlib compression support" ON)
option(FETCHCONTENT_EXAMPLE_ENABLE_TESTS "Enable VanillaPDF tests" OFF)
option(FETCHCONTENT_EXAMPLE_ENABLE_BENCHMARKS "Enable VanillaPDF benchmarks" OFF)

# Configure which dependencies to build via vcpkg vs external sources
option(FETCHCONTENT_EXAMPLE_EXTERNAL_OPENSSL "Use external OpenSSL instead of vcpkg" OFF)
option(FETCHCONTENT_EXAMPLE_EXTERNAL_JPEG "Use external JPEG instead of vcpkg" OFF)
option(FETCHCONTENT_EXAMPLE_EXTERNAL_OPENJPEG "Use external OpenJPEG instead of vcpkg" OFF)
option(FETCHCONTENT_EXAMPLE_EXTERNAL_ZLIB "Use external zlib instead of vcpkg" OFF)
option(FETCHCONTENT_EXAMPLE_EXTERNAL_GTEST "Use external GTest instead of vcpkg" OFF)
option(FETCHCONTENT_EXAMPLE_EXTERNAL_BENCHMARK "Use external Google Benchmark instead of vcpkg" OFF)

# Build vcpkg feature list based on enabled features that use vcpkg
set(VCPKG_FEATURE_FLAGS)
if(FETCHCONTENT_EXAMPLE_ENABLE_ENCRYPTION AND NOT FETCHCONTENT_EXAMPLE_EXTERNAL_OPENSSL)
    list(APPEND VCPKG_FEATURE_FLAGS "encryption")
endif()
if(FETCHCONTENT_EXAMPLE_ENABLE_JPEG AND NOT FETCHCONTENT_EXAMPLE_EXTERNAL_JPEG)
    list(APPEND VCPKG_FEATURE_FLAGS "jpeg")
endif()
if(FETCHCONTENT_EXAMPLE_ENABLE_JPEG2000 AND NOT FETCHCONTENT_EXAMPLE_EXTERNAL_OPENJPEG)
    list(APPEND VCPKG_FEATURE_FLAGS "jpeg2000")
endif()
if(FETCHCONTENT_EXAMPLE_ENABLE_ZLIB AND NOT FETCHCONTENT_EXAMPLE_EXTERNAL_ZLIB)
    list(APPEND VCPKG_FEATURE_FLAGS "zlib")
endif()
if(FETCHCONTENT_EXAMPLE_ENABLE_TESTS AND NOT FETCHCONTENT_EXAMPLE_EXTERNAL_GTEST)
    list(APPEND VCPKG_FEATURE_FLAGS "tests")
endif()
if(FETCHCONTENT_EXAMPLE_ENABLE_BENCHMARKS AND NOT FETCHCONTENT_EXAMPLE_EXTERNAL_BENCHMARK)
    list(APPEND VCPKG_FEATURE_FLAGS "benchmarks")
endif()

# Set vcpkg features based on user options
if(VCPKG_FEATURE_FLAGS)
    set(VCPKG_MANIFEST_FEATURES ${VCPKG_FEATURE_FLAGS} CACHE STRING "vcpkg features to enable")
    message(STATUS "Enabling vcpkg features: ${VCPKG_FEATURE_FLAGS}")
else()
    message(STATUS "Using minimal vcpkg dependencies (only spdlog, nlohmann-json)")
endif()

project(FetchContentTest)

# Test real-world FetchContent integration with VanillaPDF
include(FetchContent)

# Configure which VanillaPDF version to fetch
set(FETCHCONTENT_EXAMPLE_VANILLAPDF_GIT_TAG "main" CACHE STRING "VanillaPDF Git branch, tag, or commit hash to fetch")

# Determine which Git reference to use (priority: configured tag > main)
# Use the configured tag/branch, avoiding GITHUB_SHA which may not exist in target repo
set(GIT_TAG_TO_USE ${FETCHCONTENT_EXAMPLE_VANILLAPDF_GIT_TAG})
message(STATUS "Testing against configured reference: ${GIT_TAG_TO_USE}")

# Use real Git repository with robust tag selection
FetchContent_Declare(
    vanillapdf
    GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
    GIT_TAG        ${GIT_TAG_TO_USE}
    GIT_SUBMODULES ""  # Don't fetch submodules (we have our own vcpkg)
)

# Configure vanillapdf to use parent's vcpkg and pass user feature choices

# Configure vanillapdf to use parent's vcpkg
set(VANILLAPDF_INTERNAL_VCPKG OFF)

# Configure VanillaPDF features based on user choices
set(VANILLAPDF_ENABLE_ENCRYPTION ${FETCHCONTENT_EXAMPLE_ENABLE_ENCRYPTION})
set(VANILLAPDF_ENABLE_JPEG ${FETCHCONTENT_EXAMPLE_ENABLE_JPEG})
set(VANILLAPDF_ENABLE_JPEG2000 ${FETCHCONTENT_EXAMPLE_ENABLE_JPEG2000})
set(VANILLAPDF_ENABLE_ZLIB ${FETCHCONTENT_EXAMPLE_ENABLE_ZLIB})
set(VANILLAPDF_ENABLE_TESTS ${FETCHCONTENT_EXAMPLE_ENABLE_TESTS})
set(VANILLAPDF_ENABLE_BENCHMARK ${FETCHCONTENT_EXAMPLE_ENABLE_BENCHMARKS})

# Configure VanillaPDF external dependencies based on user choices
set(VANILLAPDF_EXTERNAL_OPENSSL ${FETCHCONTENT_EXAMPLE_EXTERNAL_OPENSSL})
set(VANILLAPDF_EXTERNAL_JPEG ${FETCHCONTENT_EXAMPLE_EXTERNAL_JPEG})
set(VANILLAPDF_EXTERNAL_OPENJPEG ${FETCHCONTENT_EXAMPLE_EXTERNAL_OPENJPEG})
set(VANILLAPDF_EXTERNAL_ZLIB ${FETCHCONTENT_EXAMPLE_EXTERNAL_ZLIB})
set(VANILLAPDF_EXTERNAL_GTEST ${FETCHCONTENT_EXAMPLE_EXTERNAL_GTEST})
set(VANILLAPDF_EXTERNAL_BENCHMARK ${FETCHCONTENT_EXAMPLE_EXTERNAL_BENCHMARK})

# Test real-world FetchContent integration
message(STATUS "Testing real-world VanillaPDF FetchContent integration...")
message(STATUS "Fetching VanillaPDF from GitHub repository...")
message(STATUS "Parent project managing dependencies via vcpkg")
FetchContent_MakeAvailable(vanillapdf)

# Create a simple test executable to verify the integration works
add_executable(fetchcontent_test main.cpp)
target_link_libraries(fetchcontent_test PRIVATE vanillapdf::vanillapdf)

# Copy vanillapdf.dll to executable directory on Windows (if building shared)
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET fetchcontent_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:vanillapdf::vanillapdf>
            $<TARGET_FILE_DIR:fetchcontent_test>
        COMMENT "Copying vanillapdf.dll to executable directory"
    )
endif()

# Enable testing and add test
enable_testing()
add_test(NAME fetchcontent_integration_test
         COMMAND fetchcontent_test
         WORKING_DIRECTORY $<TARGET_FILE_DIR:fetchcontent_test>)

message(STATUS "✅ Real-world FetchContent integration successful!")
message(STATUS "   - VanillaPDF fetched from GitHub")
message(STATUS "   - Dependencies auto-managed")
message(STATUS "   - Target linked successfully")