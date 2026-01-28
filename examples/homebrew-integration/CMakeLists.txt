cmake_minimum_required(VERSION 3.20)

# This example validates the Homebrew formula build approach works correctly.
# It tests that vanillapdf can be built using Homebrew dependencies and then
# consumed by a downstream project via find_package.
#
# The CI workflow:
#   1. Installs Homebrew dependencies (openssl@3, jpeg-turbo, zlib, openjpeg)
#   2. Builds and installs vanillapdf using the same flags as the formula
#   3. Runs this integration test to verify the installation works
#
# This validates the formula before submitting to Homebrew/homebrew-core.

project(HomebrewIntegrationTest)

# Find the vanillapdf package (installed by CI before running this test)
# Homebrew installs to /opt/homebrew (Apple Silicon) or /usr/local (Intel)
find_package(vanillapdf CONFIG REQUIRED)

# Create a test executable
add_executable(homebrew_test main.cpp)
target_link_libraries(homebrew_test PRIVATE vanillapdf::vanillapdf)
target_include_directories(homebrew_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../shared/include)

# Enable testing
enable_testing()
add_test(NAME homebrew_integration_test
         COMMAND homebrew_test
         WORKING_DIRECTORY $<TARGET_FILE_DIR:homebrew_test>)

message(STATUS "Homebrew integration test configured successfully")
