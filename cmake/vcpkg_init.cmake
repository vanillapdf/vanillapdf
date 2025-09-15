# VCPKG - C++ package management system

# Check if VanillaPDF should manage its own dependencies
if(NOT VANILLAPDF_MANAGE_DEPS)
  message(STATUS "VanillaPDF dependency management disabled, skipping automatic vcpkg setup")
  return()
endif()

# In case we are in STANDALONE configuration and VCPKG_ROOT is not defined, let's try to fallback
if(NOT DEFINED VCPKG_ROOT)
  set(VCPKG_ROOT "${VANILLAPDF_SOLUTION_SOURCE_DIR}/external/vcpkg")
endif()

# In case we are in STANDALONE configuration and CMAKE_TOOLCHAIN_FILE is not defined, let's try to fallback
# This only works in case it is defined before the first call to project(), which is our case
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED VCPKG_ROOT)
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Configure path to vcpkg bootstrap executables
if(WIN32)
  set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe)
  set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
else()
  set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
  set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
endif()

# In case the executable is not there invoke bootstrapping process
if(NOT EXISTS ${VCPKG_EXEC})
  message(STATUS "Bootstrapping vcpkg in ${VCPKG_ROOT}")
  execute_process(COMMAND ${VCPKG_BOOTSTRAP})
endif()

# Ensure the bootstrap process has succeeded
if(NOT EXISTS ${VCPKG_EXEC})
  message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
endif()

# Configure vcpkg manifest features based on build options
set(VANILLAPDF_VCPKG_FEATURE_LIST "")

if(VANILLAPDF_ENABLE_ENCRYPTION AND NOT VANILLAPDF_EXTERNAL_OPENSSL)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "encryption")
endif()
if(VANILLAPDF_ENABLE_JPEG AND NOT VANILLAPDF_EXTERNAL_JPEG)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "jpeg")
endif()
if(VANILLAPDF_ENABLE_JPEG2000 AND NOT VANILLAPDF_EXTERNAL_OPENJPEG)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "jpeg2000")
endif()
if(NOT VANILLAPDF_EXTERNAL_ZLIB)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "zlib")
endif()
if(NOT VANILLAPDF_EXTERNAL_SPDLOG)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "spdlog")
endif()
if(NOT VANILLAPDF_EXTERNAL_NLOHMANN_JSON)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "nlohmann-json")
endif()

if(VANILLAPDF_ENABLE_TESTS AND NOT VANILLAPDF_EXTERNAL_GTEST)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "tests")
endif()
if(VANILLAPDF_ENABLE_BENCHMARK AND NOT VANILLAPDF_EXTERNAL_BENCHMARK)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "benchmarks")
endif()

list(JOIN VANILLAPDF_VCPKG_FEATURE_LIST ";" VANILLAPDF_VCPKG_FEATURE_STRING)
set(VCPKG_MANIFEST_FEATURES "${VANILLAPDF_VCPKG_FEATURE_STRING}" CACHE STRING "" FORCE)
