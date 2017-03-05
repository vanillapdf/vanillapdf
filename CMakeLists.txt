# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

# Define main solution
project(gotchangpdf)

# Set custom module path
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# TODO Support also static library configuration
# Global compilation definitions
add_definitions("-DGOTCHANG_PDF_CONFIGURATION_DLL")

# Local compilation definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

add_subdirectory(src/gotchangpdf)
add_subdirectory(src/gotchangpdf.test)
add_subdirectory(src/gotchangpdf.merge)

# All dependent libraries
add_subdirectory(lib)