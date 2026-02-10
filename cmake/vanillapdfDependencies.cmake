# This file is included *before* vanillapdfTargets.cmake
# to ensure all required targets are defined for INTERFACE_LINK_LIBRARIES

include(CMakeFindDependencyMacro)

# Only resolve dependencies if vanillapdf is not already defined
if(NOT TARGET vanillapdf)
  find_dependency(OpenSSL REQUIRED)        # Used for PDF encryption/signing
  find_dependency(ZLIB REQUIRED)           # Required for FlateDecode filter
  find_dependency(JPEG REQUIRED)           # Required for DCTDecode (JPEG images)
  find_dependency(OpenJPEG REQUIRED)       # Required for JPXDecode (JPEG2000 codec)
  find_dependency(fmt CONFIG REQUIRED)            # High-performance string formatting
  find_dependency(spdlog CONFIG REQUIRED)  # Internal logging/debugging
endif()
