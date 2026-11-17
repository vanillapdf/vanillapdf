cmake_minimum_required(VERSION 3.26)

# This example consumes vanillapdf from an installed system package, as produced
# by build-deb-package.yml and build-rpm-package.yml, and from the binary
# archives attached to each release (scripts/verify_binary_archive.py points
# CMAKE_PREFIX_PATH at the extracted archive).
#
# The shared library and the CMake config package are installed by separate
# rules, so they can disagree about which directory libraries live in - lib
# versus lib64 on RPM distributions, or the multiarch triplet on Debian. Both
# packages still build and install successfully when they disagree, so only
# consuming the installed package through find_package detects it.
#
# See https://github.com/vanillapdf/vanillapdf/issues/487

project(SystemPackageIntegrationTest LANGUAGES CXX)

# Resolved from the installed package, not from this repository
find_package(vanillapdf CONFIG REQUIRED)

add_executable(system_package_test main.cpp)
target_link_libraries(system_package_test PRIVATE vanillapdf::vanillapdf)

# Windows has no run path: the loader looks for DLLs next to the executable.
# Copy every DLL the executable links next to it. Elsewhere the list is empty,
# copy -t (CMake 3.26) with no files does nothing, and the run path CMake
# records finds the library.
add_custom_command(TARGET system_package_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different -t $<TARGET_FILE_DIR:system_package_test>
            $<TARGET_RUNTIME_DLLS:system_package_test>
    COMMAND_EXPAND_LISTS
)

enable_testing()
add_test(NAME system_package_integration_test
         COMMAND system_package_test
         WORKING_DIRECTORY $<TARGET_FILE_DIR:system_package_test>)

message(STATUS "System package integration test configured successfully")
