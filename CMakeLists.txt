# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

# Define main solution
project(gotchangpdf)

# Prefer files in CMAKE_MODULE_PATH over shipped ones in module directory
# In the future, they may possibly add conflicting search modules
# so let's be cautious and prevent it!
# cmake_policy(SET CMP0017 NEW) # https://cmake.org/cmake/help/v3.0/policy/CMP0017.html

# We can setup custom modules to search for third party dependencies
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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
add_definitions("-DGOTCHANG_PDF_CONFIGURATION_DLL")

# Set production version
set(GOTCHANG_PDF_VERSION_MAJOR 0)
set(GOTCHANG_PDF_VERSION_MINOR 0)
set(GOTCHANG_PDF_VERSION_PATCH 1)
set(GOTCHANG_PDF_VERSION_BUILD 0)

# Include source directories
add_subdirectory(src/gotchangpdf)
add_subdirectory(src/gotchangpdf.test)
add_subdirectory(src/gotchangpdf.merge)
