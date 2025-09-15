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

# Debug: Print current enable flag values
message(STATUS "DEBUG: VANILLAPDF_ENABLE_ENCRYPTION: ${VANILLAPDF_ENABLE_ENCRYPTION}")
message(STATUS "DEBUG: VANILLAPDF_ENABLE_JPEG: ${VANILLAPDF_ENABLE_JPEG}")
message(STATUS "DEBUG: VANILLAPDF_ENABLE_JPEG2000: ${VANILLAPDF_ENABLE_JPEG2000}")
message(STATUS "DEBUG: VANILLAPDF_ENABLE_ZLIB: ${VANILLAPDF_ENABLE_ZLIB}")
message(STATUS "DEBUG: CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "DEBUG: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

if(VANILLAPDF_ENABLE_ENCRYPTION)
  if(NOT DEFINED VANILLAPDF_EXTERNAL_OPENSSL)
    set(VANILLAPDF_EXTERNAL_OPENSSL OFF)
  endif()
  if(NOT VANILLAPDF_EXTERNAL_OPENSSL)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "encryption")
  endif()
endif()
if(VANILLAPDF_ENABLE_JPEG)
  if(NOT DEFINED VANILLAPDF_EXTERNAL_JPEG)
    set(VANILLAPDF_EXTERNAL_JPEG OFF)
  endif()
  if(NOT VANILLAPDF_EXTERNAL_JPEG)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "jpeg")
  endif()
endif()
if(VANILLAPDF_ENABLE_JPEG2000)
  if(NOT DEFINED VANILLAPDF_EXTERNAL_OPENJPEG)
    set(VANILLAPDF_EXTERNAL_OPENJPEG OFF)
  endif()
  if(NOT VANILLAPDF_EXTERNAL_OPENJPEG)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "jpeg2000")
  endif()
endif()
if(VANILLAPDF_ENABLE_ZLIB)
  if(NOT DEFINED VANILLAPDF_EXTERNAL_ZLIB)
    set(VANILLAPDF_EXTERNAL_ZLIB OFF)
  endif()
  if(NOT VANILLAPDF_EXTERNAL_ZLIB)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "zlib")
  endif()
endif()
# Note: spdlog and nlohmann-json are always needed
if(NOT DEFINED VANILLAPDF_EXTERNAL_SPDLOG)
  set(VANILLAPDF_EXTERNAL_SPDLOG OFF)
endif()
if(NOT VANILLAPDF_EXTERNAL_SPDLOG)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "spdlog")
endif()
if(NOT DEFINED VANILLAPDF_EXTERNAL_NLOHMANN_JSON)
  set(VANILLAPDF_EXTERNAL_NLOHMANN_JSON OFF)
endif()
if(NOT VANILLAPDF_EXTERNAL_NLOHMANN_JSON)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "nlohmann-json")
endif()

if(VANILLAPDF_ENABLE_TESTS)
  if(NOT DEFINED VANILLAPDF_EXTERNAL_GTEST)
    set(VANILLAPDF_EXTERNAL_GTEST OFF)
  endif()
  if(NOT VANILLAPDF_EXTERNAL_GTEST)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "tests")
  endif()
endif()
if(VANILLAPDF_ENABLE_BENCHMARK)
  if(NOT DEFINED VANILLAPDF_EXTERNAL_BENCHMARK)
    set(VANILLAPDF_EXTERNAL_BENCHMARK OFF)
  endif()
  if(NOT VANILLAPDF_EXTERNAL_BENCHMARK)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "benchmarks")
  endif()
endif()

list(JOIN VANILLAPDF_VCPKG_FEATURE_LIST ";" VANILLAPDF_VCPKG_FEATURE_STRING)
set(VCPKG_MANIFEST_FEATURES "${VANILLAPDF_VCPKG_FEATURE_STRING}" CACHE STRING "" FORCE)

# Debug: Print final feature list
message(STATUS "DEBUG: Final VCPKG_MANIFEST_FEATURES: '${VANILLAPDF_VCPKG_FEATURE_STRING}'")
