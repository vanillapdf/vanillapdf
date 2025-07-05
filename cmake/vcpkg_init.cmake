# VCPKG - C++ package management system

# For embedded configuration we leave everyting to the parent project
if(NOT VANILLAPDF_STANDALONE)
  message(STATUS "VanillaPDF is being built as a vcpkg port or embedded, skipping automatic package management")
  return()
endif()

# In case we are in STANDALONE configuration and VCPKG_ROOT is not defined, let's try to fallback
if(NOT DEFINED VCPKG_ROOT)
  set(VCPKG_ROOT "${VANILLAPDF_SOLUTION_SOURCE_DIR}/external/vcpkg")
endif()

# Clone vcpkg when requested
if(NOT EXISTS "${VCPKG_ROOT}")
  if(VANILLAPDF_AUTO_VCPKG_CLONE)
    message(STATUS "Cloning vcpkg into ${VCPKG_ROOT}")
    execute_process(
      COMMAND git clone https://github.com/microsoft/vcpkg.git "${VCPKG_ROOT}"
      RESULT_VARIABLE VANILLAPDF_GIT_CLONE_RESULT
    )
    if(NOT VANILLAPDF_GIT_CLONE_RESULT EQUAL 0)
      message(FATAL_ERROR "***** FATAL ERROR: Failed to clone vcpkg *****")
    endif()
  else()
    message(FATAL_ERROR "***** FATAL ERROR: VCPKG_ROOT not found. Set VCPKG_ROOT or enable VANILLAPDF_AUTO_VCPKG_CLONE *****")
  endif()
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

if(NOT VANILLAPDF_EXTERNAL_OPENSSL)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "openssl")
endif()
if(NOT VANILLAPDF_EXTERNAL_JPEG)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "libjpeg-turbo")
endif()
if(NOT VANILLAPDF_EXTERNAL_OPENJPEG)
  list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "openjpeg")
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
