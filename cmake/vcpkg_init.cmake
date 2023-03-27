# VCPKG - C++ package management system

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/external/vcpkg")

if(WIN32)
  set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe)
  set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
else()
  set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
  set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
endif()

if(NOT EXISTS ${VCPKG_EXEC})
  message("Bootstrapping vcpkg in ${VCPKG_ROOT}")
  execute_process(COMMAND ${VCPKG_BOOTSTRAP} WORKING_DIRECTORY ${VCPKG_ROOT})
endif()

if(NOT EXISTS ${VCPKG_EXEC})
  message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
endif()

# VCPKG creates abnormal warning, when there are no installed packages
# We are installing packages during the bootstrap process
set(VCPKG_SUPPRESS_INSTALLED_LIBRARIES_WARNING "1")

# Setup VCPKG toolchain
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
