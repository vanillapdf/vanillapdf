# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

# Define main solution
project(gotchangpdf)

# Allow tests to be run
enable_testing()

# Prefer files in CMAKE_MODULE_PATH over shipped ones in module directory
# In the future, they may possibly add conflicting search modules
# so let's be cautious and prevent it!
# cmake_policy(SET CMP0017 NEW) # https://cmake.org/cmake/help/v3.0/policy/CMP0017.html

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# We can setup custom modules to search for third party dependencies
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# TODO Support also static library configuration
# Global compilation definitions
add_definitions("-DGOTCHANG_PDF_CONFIGURATION_DLL")

# Local compilation definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DRELEASE")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DRELEASE")

set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -DRELEASE")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DRELEASE")

if(CMAKE_COMPILER_IS_GNUCXX)

	# Enable maximum warning level
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	
	# GCC warns on pragma region directive, which is only IDE feature
	# It could be solved with #ifdef only for MSVC
	# All other pragmas are only for suppressing specific warnings on MSVC
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
	
	# Variables in constructors should be initialized in order
	# they were declared. I prefer initializing variables
	# in order they make sense and are declared as constructor parameters.
	# This should be harmless unless you take at least a little care.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")
	
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)

	# Enable maximum warning level
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	
endif(MSVC)

# Set production version
set(GOTCHANG_PDF_VERSION_MAJOR 0)
set(GOTCHANG_PDF_VERSION_MINOR 0)
set(GOTCHANG_PDF_VERSION_PATCH 1)
set(GOTCHANG_PDF_VERSION_BUILD 0)

# Include source directories
add_subdirectory(src/gotchangpdf)
add_subdirectory(src/gotchangpdf.test)
add_subdirectory(src/gotchangpdf.merge)

# All dependent libraries
add_subdirectory(lib)