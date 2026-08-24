# Require minimum CMAKE version

# Update 29.04.2025
# Minimum version 3.21 is due vcpkg manifest mode.
# Minimum version 3.28 is due CMakePresets.json file version 8.
cmake_minimum_required(VERSION 3.28)

# Vanilla.PDF is a c++ library built using c++17 standard
# In order to make it portable across all platforms we strictly adhere to the standard by enforcing it and disabling the extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Store information about solution root folder
set(VANILLAPDF_SOLUTION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Build configuration options

# Tests are off by default so that consumers (vcpkg port, Conan, Homebrew,
# FetchContent) get the library alone. The CMake presets turn them on together
# with vanillapdf.tools: building from source means verifying the build.
option(VANILLAPDF_ENABLE_TESTS "Perform test scenarios to ensure stable releases" OFF)
option(VANILLAPDF_ENABLE_BENCHMARK "Include benchmarking project to measure performance" OFF)
option(VANILLAPDF_ENABLE_PACKAGING "Enable packaging features (CPack, DEB, Brew, etc.)" OFF)

# Enable/disable optional features
option(VANILLAPDF_ENABLE_ENCRYPTION "Enable PDF encryption/decryption support" ON)
option(VANILLAPDF_ENABLE_JPEG "Enable JPEG image support" ON)
option(VANILLAPDF_ENABLE_JPEG2000 "Enable JPEG2000 image support" ON)
option(VANILLAPDF_ENABLE_ZLIB "Enable zlib compression support" ON)
option(VANILLAPDF_ENABLE_LICENSING "Enable licensing and JSON metadata support" OFF)

# Option to control internal vcpkg dependency management
# Disable when dependencies are managed by parent project (e.g., vcpkg ports, system packages)
# TODO: Will be enhanced with smart detection in https://github.com/vanillapdf/vanillapdf/issues/138
option(VANILLAPDF_INTERNAL_VCPKG "Enable internal vcpkg dependency management" ON)

# vanillapdf.tools is a repository-local validation utility that depends on
# CLI11. Like the tests it is off by default so that no package-manager build
# picks up the tool or its dependency, and on in the CMake presets
# (cmake/presets/shared.json), where the internal vcpkg manifest provides CLI11.
option(VANILLAPDF_ENABLE_TOOLS "Build the vanillapdf.tools validation utility" OFF)

# Allow linking selected dependencies from the system instead of vcpkg,
# or provided externally
option(VANILLAPDF_EXTERNAL_OPENSSL "Use OpenSSL from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_JPEG "Use libjpeg from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_OPENJPEG "Use openjpeg from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_ZLIB "Use zlib from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_SPDLOG "Use spdlog from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_FMT "Use fmt from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_NLOHMANN_JSON "Use nlohmann-json from the system or a package manager" OFF)

if(VANILLAPDF_ENABLE_TESTS)
  option(VANILLAPDF_EXTERNAL_GTEST "Use GoogleTest from the system or a package manager" OFF)
endif()

if(VANILLAPDF_ENABLE_BENCHMARK)
  option(VANILLAPDF_EXTERNAL_BENCHMARK "Use Google Benchmark from the system or a package manager" OFF)
endif()

# Skip installation of CMake config files (vanillapdfConfig.cmake, etc.)
# Enable when an external package manager (e.g. Conan) generates its own find_package config
option(VANILLAPDF_SKIP_CMAKE_CONFIG_INSTALL "Skip installing CMake config files" OFF)

# Allow build configuration to link using static CRT, dynamic CRT link is default
option(VANILLAPDF_USE_STATIC_CRT "Use static MSVC runtime (/MT) instead of dynamic (/MD)" OFF)

# Option if the build should be static/dynamic link
option(BUILD_SHARED_LIBS "Build vanillapdf as a shared library" ON)

# Allow overriding version components from the command line
# These must be set before including vcpkg_manifest.cmake so version.cmake picks them up
set(VANILLAPDF_VERSION_PATCH_OVERRIDE "${VANILLAPDF_VERSION_PATCH_OVERRIDE}" CACHE STRING "Override patch version (e.g., 999)")
set(VANILLAPDF_VERSION_BUILD_OVERRIDE "${VANILLAPDF_VERSION_BUILD_OVERRIDE}" CACHE STRING "Override build version (e.g., 20251228)")
set(VANILLAPDF_VERSION_BUILD_SUFFIX "${VANILLAPDF_VERSION_BUILD_SUFFIX}" CACHE STRING "Optional build suffix (e.g., -nightly.main)")

# Note: This needs to be included before defining the main project
# Generate vcpkg.json from template with version from cmake/version.cmake
include(cmake/vcpkg_manifest.cmake)

# Check for VCPKG - C++ package management system
include(cmake/vcpkg_init.cmake)

# Define main solution
project(vanillapdf)

# Standard installation directory layout (CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR, ...)
# Included here rather than in cmake/vanillapdf_install.cmake, because the install
# rules in src/vanillapdf/CMakeLists.txt run before that file is included.
# It must come after project(), which GNUInstallDirs relies on to detect the
# platform specific library directory.
include(GNUInstallDirs)

# Prefer files in CMAKE_MODULE_PATH over shipped ones in module directory
# In the future, they may possibly add conflicting search modules
# so let's be cautious and prevent it!
# cmake_policy(SET CMP0017 NEW) # https://cmake.org/cmake/help/v3.0/policy/CMP0017.html

# We can setup custom modules to search for third party dependencies
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Version information is loaded early via cmake/vcpkg_manifest.cmake (before vcpkg_init)
# to generate vcpkg.json with the correct version. Version overrides are set before the include.

# Enable packaging (DEB, Brew, etc.) when explicitly requested
if (VANILLAPDF_ENABLE_PACKAGING)

    # Package management
    include(cmake/packaging.cmake)
    include(CPack)

endif()

# Set extended compilator flags
include(cmake/compiler_flags.cmake)

# Enable compiler feature identification macros
include(cmake/compiler_checks.cmake)

# Allow code coverage configuration
include(cmake/coverage.cmake)

# Optional sanitizer configuration
include(cmake/sanitizers.cmake)

# Optional fuzzer configuration
include(cmake/fuzzer.cmake)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(VANILLAPDF_FORCE_32_BIT CACHE BOOL "OFF")

if(VANILLAPDF_FORCE_32_BIT)
    message("Regardless of architecture, forcing 32-bit output binary")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
endif(VANILLAPDF_FORCE_32_BIT)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Enable testing infrastructure before including source directories
if(VANILLAPDF_ENABLE_TESTS)
  include(cmake/build_name.cmake)
  include(CTest)

  # Fetch the shared test corpus + manifest before any test target consumes it
  include(cmake/testdata.cmake)
endif()

# Include source directories
add_subdirectory(src/vanillapdf)

if(VANILLAPDF_ENABLE_TOOLS)
  add_subdirectory(src/vanillapdf.tools)
endif()

if(VANILLAPDF_ENABLE_TESTS)
  add_subdirectory(src/vanillapdf.test)
  add_subdirectory(src/vanillapdf.unittest)
endif()

if(VANILLAPDF_ENABLE_BENCHMARK)
  add_subdirectory(src/vanillapdf.benchmark)
endif()

if(VANILLAPDF_ENABLE_FUZZING)
  add_subdirectory(src/vanillapdf.fuzzer)
endif()
