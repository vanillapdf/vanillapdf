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
if(VANILLAPDF_ENABLE_TESTS OR VANILLAPDF_ENABLE_BENCHMARK)
  set(VANILLAPDF_VCPKG_FEATURE_LIST "")
  if(VANILLAPDF_ENABLE_TESTS)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "tests")
  endif()
  if(VANILLAPDF_ENABLE_BENCHMARK)
    list(APPEND VANILLAPDF_VCPKG_FEATURE_LIST "benchmarks")
  endif()
  list(JOIN VANILLAPDF_VCPKG_FEATURE_LIST ";" VANILLAPDF_VCPKG_FEATURE_STRING)
  set(VCPKG_MANIFEST_FEATURES "${VANILLAPDF_VCPKG_FEATURE_STRING}" CACHE STRING "" FORCE)
endif()
