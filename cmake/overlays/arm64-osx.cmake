set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

# Force consistent JPEG library usage on macOS
# This ensures we use libjpeg-turbo instead of system libjpeg
set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_OSX_ARCHITECTURES arm64)

# Ensure libjpeg-turbo is used instead of system libjpeg
# This helps avoid version mismatch issues on macOS
if(PORT MATCHES "libjpeg-turbo")
    set(VCPKG_LIBRARY_LINKAGE static)
endif()