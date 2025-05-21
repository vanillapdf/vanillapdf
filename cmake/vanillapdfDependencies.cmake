# This file is included *before* vanillapdfTargets.cmake
# to ensure all required targets are defined for INTERFACE_LINK_LIBRARIES

include(CMakeFindDependencyMacro)

# Only resolve dependencies if vanillapdf is not already defined
if(NOT TARGET vanillapdf)
  find_dependency(OpenSSL REQUIRED)        # Used for PDF encryption/signing
  find_dependency(ZLIB REQUIRED)           # Required for FlateDecode filter
  find_dependency(JPEG REQUIRED)           # Required for DCTDecode (JPEG images)
  find_dependency(nlohmann_json CONFIG REQUIRED)  # Used internally for structured metadata
  find_dependency(spdlog CONFIG REQUIRED)  # Internal logging/debugging
endif()
