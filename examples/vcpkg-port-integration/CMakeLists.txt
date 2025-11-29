cmake_minimum_required(VERSION 3.20)

# This example tests the vcpkg port overlay from ports/vanillapdf
#
# For --head mode testing (uses HEAD_REF main), first install vanillapdf manually:
#   cd <repo-root>
#   ./external/vcpkg/vcpkg install vanillapdf --head --overlay-ports=./ports
#
# Then configure and build:
#   cmake --preset linux-x64-debug
#   cmake --build --preset linux-x64-debug
#   ctest --preset linux-x64-debug
#
# Note: --head only works in classic mode, not manifest mode. The CI workflow
# handles the two-step process (vcpkg install first, then cmake configure).

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

# Disable manifest mode - we use classic mode with pre-installed packages
set(VCPKG_MANIFEST_MODE OFF CACHE BOOL "")

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
