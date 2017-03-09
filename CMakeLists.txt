# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

# Define main solution
project(gotchangpdf)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# We can setup custom modules to search for third party dependencies
# SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# TODO Support also static library configuration
# Global compilation definitions
add_definitions("-DGOTCHANG_PDF_CONFIGURATION_DLL")

# Local compilation definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

set(GOTCHANG_PDF_VERSION_MAJOR 0)
set(GOTCHANG_PDF_VERSION_MINOR 0)
set(GOTCHANG_PDF_VERSION_PATCH 1)

add_subdirectory(src/gotchangpdf)
add_subdirectory(src/gotchangpdf.test)
add_subdirectory(src/gotchangpdf.merge)

# All dependent libraries
add_subdirectory(lib)