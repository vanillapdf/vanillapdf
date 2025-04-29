# 30.04.2025
# We are switching to vcpkg manifest

##  # VCPKG - C++ package management system
##  
##  if(WIN32)
##    # Note:
##    # For the sake of ease of use, I am overriding default vcpkg triplets (x86-windows, x64-windows)
##    # It is possible to keep default triplets untouched and use custom naming (x86-windows-custom)
##    # Unfortunately this is not easy to do and lead to incorrect include paths
##    set(VCPKG_TRIPLET_OVERLAY "--overlay-triplets=${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake")
##    
##    set(VANILLPDF_INCLUDE_DIR ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)
##    set(VANILLPDF_BINARY_DIR ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/bin)
##  endif()
##  
##  # Set VCPKG_TARGET_TRIPLET automatically if not set
##  if(NOT DEFINED VCPKG_TARGET_TRIPLET)
##  
##    if(WIN32)
##      if(CMAKE_SIZEOF_VOID_P EQUAL 8)
##          set(VCPKG_TARGET_TRIPLET "x64-windows")
##      else()
##          set(VCPKG_TARGET_TRIPLET "x86-windows")
##      endif()
##    elseif(APPLE)
##      if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
##          set(VCPKG_TARGET_TRIPLET "arm64-osx")
##      else()
##          set(VCPKG_TARGET_TRIPLET "x64-osx")
##      endif()
##    elseif(UNIX)
##      if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
##        set(VCPKG_TARGET_TRIPLET "arm64-linux")
##      else()
##        set(VCPKG_TARGET_TRIPLET "x64-linux")
##      endif()
##    endif()
##  
##    if(NOT DEFINED VCPKG_TARGET_TRIPLET)
##      message(FATAL "VCPKG_TARGET_TRIPLET is not defined and could not be auto-detected")
##    endif()
##  
##    message(STATUS "Auto-detected VCPKG_TARGET_TRIPLET=${VCPKG_TARGET_TRIPLET}")
##  endif()
##  
##  message(STATUS "Installing dependencies for ${VCPKG_TARGET_TRIPLET}")
##  
##  execute_process(COMMAND ${VCPKG_EXEC} install "openssl:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY})
##  execute_process(COMMAND ${VCPKG_EXEC} install "libjpeg-turbo:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY})
##  execute_process(COMMAND ${VCPKG_EXEC} install "openjpeg:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY})
##  execute_process(COMMAND ${VCPKG_EXEC} install "zlib:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY})
##  execute_process(COMMAND ${VCPKG_EXEC} install "benchmark:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY})
##  execute_process(COMMAND ${VCPKG_EXEC} install "spdlog:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY})
##  
##  if (COMPILE_SKIA)
##    execute_process(COMMAND ${VCPKG_EXEC} install "skia:${VCPKG_TARGET_TRIPLET}" ${VCPKG_TRIPLET_OVERLAY})
##  endif (COMPILE_SKIA)
