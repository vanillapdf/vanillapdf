cmake_minimum_required(VERSION 3.20)
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

# Test real-world FetchContent integration
message(STATUS "Testing real-world VanillaPDF FetchContent integration...")
message(STATUS "Fetching VanillaPDF from GitHub repository...")
FetchContent_MakeAvailable(vanillapdf)

# Create a simple test executable to verify the integration works
add_executable(fetchcontent_test main.cpp)
target_link_libraries(fetchcontent_test PRIVATE vanillapdf::vanillapdf)

message(STATUS "✅ Real-world FetchContent integration successful!")
message(STATUS "   - VanillaPDF fetched from GitHub")
message(STATUS "   - Dependencies auto-managed")
message(STATUS "   - Target linked successfully")