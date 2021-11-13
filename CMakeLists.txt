# Require minimum CMAKE version
cmake_minimum_required(VERSION 3.0)

# Minimum version is 2.8.4 due to Cygwin warning:

	# CMake no longer defines WIN32 on Cygwin!

	# (1) If you are just trying to build this project, ignore this warning or
	# quiet it by setting CMAKE_LEGACY_CYGWIN_WIN32=0 in your environment or in
	# the CMake cache.  If later configuration or build errors occur then this
	# project may have been written under the assumption that Cygwin is WIN32.
	# In that case, set CMAKE_LEGACY_CYGWIN_WIN32=1 instead.

	# (2) If you are developing this project, add the line

	  # set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required

	# at the top of your top-level CMakeLists.txt file or set the minimum
	# required version of CMake to 2.8.4 or higher.  Then teach your project to
	# build on Cygwin without WIN32.

# Note: This needs to be included before defining the main project
# Check for VCPKG - C++ package management system
include(cmake/vcpkg_init.cmake)

# Define main solution
project(vanillapdf)

# Prefer files in CMAKE_MODULE_PATH over shipped ones in module directory
# In the future, they may possibly add conflicting search modules
# so let's be cautious and prevent it!
# cmake_policy(SET CMP0017 NEW) # https://cmake.org/cmake/help/v3.0/policy/CMP0017.html

# We can setup custom modules to search for third party dependencies
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set production version
# It is important project version before including cmake/packaging.cmake
set(VANILLAPDF_VERSION_MAJOR 1)
set(VANILLAPDF_VERSION_MINOR 5)
set(VANILLAPDF_VERSION_PATCH 2)
set(VANILLAPDF_VERSION_BUILD 0)

# Package management
include(cmake/packaging.cmake)
include(CPack)

# Allow tests to be run
include(cmake/build_name.cmake)
include(CTest)

# Set extended compilator flags
include(cmake/compiler_flags.cmake)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# TODO Support also static library configuration
# Global compilation definitions
add_definitions("-DVANILLAPDF_CONFIGURATION_DLL")

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

# Include source directories
add_subdirectory(src/vanillapdf)
add_subdirectory(src/vanillapdf.test)
add_subdirectory(src/vanillapdf.unittest)
add_subdirectory(src/vanillapdf.tools)
