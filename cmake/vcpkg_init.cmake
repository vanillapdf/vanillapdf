# VCPKG - C++ package management system

# Check if internal vcpkg dependency management is enabled
# Disable when dependencies are managed by parent project (e.g., vcpkg ports, system packages)
# TODO: Will be enhanced with smart detection in https://github.com/vanillapdf/vanillapdf/issues/138
if(NOT VANILLAPDF_INTERNAL_VCPKG)
  message(STATUS "vanillapdf internal vcpkg disabled via VANILLAPDF_INTERNAL_VCPKG=OFF")
  return()
endif()

message(STATUS "vanillapdf managing dependencies via internal vcpkg")

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
# Note: spdlog and fmt are always needed; nlohmann-json only when licensing is enabled
if(NOT DEFINED VANILLAPDF_EXTERNAL_SPDLOG)
  set(VANILLAPDF_EXTERNAL_SPDLOG OFF)
endif()
if(NOT VANILLAPDF_EXTERNAL_SPDLOG)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "spdlog")
endif()
if(NOT DEFINED VANILLAPDF_EXTERNAL_FMT)
  set(VANILLAPDF_EXTERNAL_FMT OFF)
endif()
if(NOT VANILLAPDF_EXTERNAL_FMT)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "fmt")
endif()
if(VANILLAPDF_ENABLE_LICENSING)
  if(NOT DEFINED VANILLAPDF_EXTERNAL_NLOHMANN_JSON)
    set(VANILLAPDF_EXTERNAL_NLOHMANN_JSON OFF)
  endif()
  if(NOT VANILLAPDF_EXTERNAL_NLOHMANN_JSON)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "nlohmann-json")
  endif()
endif()

if(VANILLAPDF_ENABLE_TOOLS)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "tools")
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

