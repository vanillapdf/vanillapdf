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

# Consumers without CMake, such as the vcpkg MSBuild integration, only add the
# include directory and link the import library. The installed headers alone
# have to select the linkage, so this test deliberately avoids the
# vanillapdf::vanillapdf target and the compile definitions it carries.
# Windows only: the dynamic triplet is where linkage matters, the static
# triplets of the other platforms would need every dependency listed by hand.
if(WIN32)
    set(VANILLAPDF_TRIPLET_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

    find_path(VANILLAPDF_DIRECT_INCLUDE_DIR
        NAMES vanillapdf/c_vanillapdf_api.h
        PATHS "${VANILLAPDF_TRIPLET_DIR}/include"
        NO_DEFAULT_PATH
        REQUIRED
    )

    find_library(VANILLAPDF_DIRECT_LIBRARY_RELEASE
        NAMES vanillapdf libvanillapdf
        PATHS "${VANILLAPDF_TRIPLET_DIR}/lib"
        NO_DEFAULT_PATH
        REQUIRED
    )

    find_library(VANILLAPDF_DIRECT_LIBRARY_DEBUG
        NAMES vanillapdf libvanillapdf
        PATHS "${VANILLAPDF_TRIPLET_DIR}/debug/lib"
        NO_DEFAULT_PATH
        REQUIRED
    )

    add_executable(vcpkg_port_direct_link_test direct_link.c)
    target_include_directories(vcpkg_port_direct_link_test PRIVATE "${VANILLAPDF_DIRECT_INCLUDE_DIR}")
    target_link_libraries(vcpkg_port_direct_link_test PRIVATE
        optimized "${VANILLAPDF_DIRECT_LIBRARY_RELEASE}"
        debug "${VANILLAPDF_DIRECT_LIBRARY_DEBUG}"
    )

    # The vcpkg toolchain copies the DLLs next to the executable after the build
    add_test(NAME vcpkg_port_direct_link_test
             COMMAND vcpkg_port_direct_link_test
             WORKING_DIRECTORY $<TARGET_FILE_DIR:vcpkg_port_direct_link_test>)
endif()

message(STATUS "vcpkg port integration test configured successfully")
