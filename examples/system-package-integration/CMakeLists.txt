cmake_minimum_required(VERSION 3.20)

# This example consumes vanillapdf from an installed system package, as produced
# by build-deb-package.yml and build-rpm-package.yml.
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

enable_testing()
add_test(NAME system_package_integration_test
         COMMAND system_package_test
         WORKING_DIRECTORY $<TARGET_FILE_DIR:system_package_test>)

message(STATUS "System package integration test configured successfully")
