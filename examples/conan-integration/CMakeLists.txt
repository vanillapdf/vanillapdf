cmake_minimum_required(VERSION 3.20)

# This example tests the Conan package for vanillapdf
#
# First, configure the example and create the vanillapdf Conan package:
#   cmake -P ../../cmake/configure_conan_example.cmake
#   conan create ../../conan/ --version=<version> --build=missing
#
# Then install dependencies and build:
#   conan install . --build=missing
#   cmake --preset linux-x64-debug       # or other preset
#   cmake --build --preset linux-x64-debug
#   ctest --preset linux-x64-debug
#
# On Windows with Visual Studio:
#   conan install . --build=missing -s build_type=Debug
#   cmake --preset windows-x64-debug
#   cmake --build --preset windows-x64-debug --config Debug
#   ctest --preset windows-x64-debug -C Debug --output-on-failure

project(ConanIntegrationTest)

# Find the vanillapdf package installed via Conan
find_package(vanillapdf CONFIG REQUIRED)

# Create a test executable
add_executable(conan_test main.cpp)
target_link_libraries(conan_test PRIVATE vanillapdf::vanillapdf)
target_include_directories(conan_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../shared/include)

# Enable testing
enable_testing()
add_test(NAME conan_integration_test
         COMMAND conan_test
         WORKING_DIRECTORY $<TARGET_FILE_DIR:conan_test>)

message(STATUS "Conan integration test configured successfully")
