cmake_minimum_required(VERSION 3.20)

project(AndroidNdkIntegration LANGUAGES C CXX)

if(NOT ANDROID)
    message(FATAL_ERROR "This example must be configured with the Android NDK toolchain.")
endif()

option(ANDROID_NDK_EXAMPLE_FULL_FEATURES "Build vanillapdf with all optional features on Android." OFF)

# Build-only integration scenario: keep the graph lean for cross-compilation CI.
set(VANILLAPDF_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(VANILLAPDF_ENABLE_BENCHMARK OFF CACHE BOOL "" FORCE)
set(VANILLAPDF_ENABLE_PACKAGING OFF CACHE BOOL "" FORCE)
set(VANILLAPDF_INTERNAL_VCPKG OFF CACHE BOOL "" FORCE)

if(NOT ANDROID_NDK_EXAMPLE_FULL_FEATURES)
    set(VANILLAPDF_ENABLE_ENCRYPTION OFF CACHE BOOL "" FORCE)
    set(VANILLAPDF_ENABLE_JPEG OFF CACHE BOOL "" FORCE)
    set(VANILLAPDF_ENABLE_JPEG2000 OFF CACHE BOOL "" FORCE)
    set(VANILLAPDF_ENABLE_ZLIB OFF CACHE BOOL "" FORCE)
else()
    set(VCPKG_MANIFEST_FEATURES "encryption;jpeg;jpeg2000;zlib" CACHE STRING "" FORCE)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../.." "${CMAKE_CURRENT_BINARY_DIR}/vanillapdf")

add_library(android_ndk_smoke SHARED main.cpp)
target_link_libraries(android_ndk_smoke PRIVATE vanillapdf::vanillapdf)

message(STATUS "Configured Android NDK integration for ABI: ${CMAKE_ANDROID_ARCH_ABI}")
