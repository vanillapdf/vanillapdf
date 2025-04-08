# VCPKG - C++ package management system

if(WIN32)
  # Note:
  # For the sake of ease of use, I am overriding default vcpkg triplets (x86-windows, x64-windows)
  # It is possible to keep default triplets untouched and use custom naming (x86-windows-custom)
  # Unfortunately this is not easy to do and lead to incorrect include paths
  set(VCPKG_TRIPLET_OVERLAY "--overlay-triplets=${CMAKE_SOURCE_DIR}/cmake")
  
  set(VANILLPDF_INCLUDE_DIR ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)
  set(VANILLPDF_BINARY_DIR ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/bin)
endif()

message(STATUS "Installing dependencies for ${VCPKG_TARGET_TRIPLET} in ${VCPKG_ROOT}")

execute_process(COMMAND ${VCPKG_EXEC} install "openssl:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY} WORKING_DIRECTORY ${VCPKG_ROOT})
execute_process(COMMAND ${VCPKG_EXEC} install "libjpeg-turbo:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY} WORKING_DIRECTORY ${VCPKG_ROOT})
execute_process(COMMAND ${VCPKG_EXEC} install "openjpeg:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY} WORKING_DIRECTORY ${VCPKG_ROOT})
execute_process(COMMAND ${VCPKG_EXEC} install "zlib:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY} WORKING_DIRECTORY ${VCPKG_ROOT})
execute_process(COMMAND ${VCPKG_EXEC} install "benchmark:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY} WORKING_DIRECTORY ${VCPKG_ROOT})
execute_process(COMMAND ${VCPKG_EXEC} install "spdlog:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY} WORKING_DIRECTORY ${VCPKG_ROOT})

if (COMPILE_SKIA)
    execute_process(COMMAND ${VCPKG_EXEC} install "skia:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY} WORKING_DIRECTORY ${VCPKG_ROOT})
endif (COMPILE_SKIA)
