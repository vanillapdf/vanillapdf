cmake_minimum_required(VERSION 3.20)

# This example tests the vcpkg port overlay from ports/vanillapdf
# It uses --head mode to test against the current HEAD of the repository
#
# Usage (with --head to test current main branch):
#   cmake --preset linux-x64-debug -DVCPKG_PORT_TEST_USE_HEAD=ON
#   cmake --build --preset linux-x64-debug
#   ctest --preset linux-x64-debug
#
# The --head flag tells vcpkg to use HEAD_REF (main branch) instead of
# the versioned REF+SHA512, allowing us to test uncommitted changes.

# Enable --head mode to use HEAD_REF instead of versioned release
# This tests the current state of the repository
option(VCPKG_PORT_TEST_USE_HEAD "Use vcpkg --head mode to test HEAD_REF" ON)

# Set up vcpkg toolchain before project() call
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # Use vcpkg from the repository root
    set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../external/vcpkg")

    if(NOT EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        message(FATAL_ERROR "vcpkg not found at ${VCPKG_ROOT}. Run from the repository root or set CMAKE_TOOLCHAIN_FILE.")
    endif()

    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# Configure port overlay to use the local port definition
# This points to ports/vanillapdf in the repository root
set(VCPKG_OVERLAY_PORTS "${CMAKE_CURRENT_SOURCE_DIR}/../../ports" CACHE STRING "")
message(STATUS "Using port overlay: ${VCPKG_OVERLAY_PORTS}")

# Pass --head to vcpkg install if enabled
if(VCPKG_PORT_TEST_USE_HEAD)
    list(APPEND VCPKG_INSTALL_OPTIONS "--head")
    message(STATUS "Using vcpkg --head mode (will use HEAD_REF main)")
endif()

project(VcpkgPortIntegrationTest)

# Find the vanillapdf package installed via vcpkg port overlay
find_package(vanillapdf CONFIG REQUIRED)

# Create a test executable
add_executable(vcpkg_port_test main.cpp)
target_link_libraries(vcpkg_port_test PRIVATE vanillapdf::vanillapdf)
target_include_directories(vcpkg_port_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../shared/include)

# Copy DLL on Windows if building shared
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET vcpkg_port_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:vanillapdf::vanillapdf>
            $<TARGET_FILE_DIR:vcpkg_port_test>
        COMMENT "Copying vanillapdf.dll to executable directory"
    )
endif()

# Enable testing
enable_testing()
add_test(NAME vcpkg_port_integration_test
         COMMAND vcpkg_port_test
         WORKING_DIRECTORY $<TARGET_FILE_DIR:vcpkg_port_test>)

message(STATUS "vcpkg port integration test configured successfully")
