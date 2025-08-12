# Version information for Vanilla.PDF

# Set production version
set(VANILLAPDF_VERSION_MAJOR 2)
set(VANILLAPDF_VERSION_MINOR 1)
set(VANILLAPDF_VERSION_PATCH 0)
set(VANILLAPDF_VERSION_BUILD 0)

# Optional build suffix, used for pre-release builds
if(NOT DEFINED VANILLAPDF_VERSION_BUILD_SUFFIX)
    set(VANILLAPDF_VERSION_BUILD_SUFFIX "")
endif()

# Sanitize the build suffix to be a valid SemVer pre-release (NuGet) fragment
# Allow only [0-9A-Za-z.-], collapse multiple dashes, and trim leading/trailing dashes
if(NOT "${VANILLAPDF_VERSION_BUILD_SUFFIX}" STREQUAL "")
    set(_suffix "${VANILLAPDF_VERSION_BUILD_SUFFIX}")
    # Replace any invalid characters with '-'
    string(REGEX REPLACE "[^0-9A-Za-z\.-]+" "-" _suffix "${_suffix}")
    # Collapse consecutive dashes
    string(REGEX REPLACE "-{2,}" "-" _suffix "${_suffix}")
    # Trim leading and trailing dashes
    string(REGEX REPLACE "^-+" "" _suffix "${_suffix}")
    string(REGEX REPLACE "-+$" "" _suffix "${_suffix}")
    # Guard against empty result
    if("${_suffix}" STREQUAL "")
        set(VANILLAPDF_VERSION_BUILD_SUFFIX "-prerelease")
    else()
        set(VANILLAPDF_VERSION_BUILD_SUFFIX "-${_suffix}")
    endif()
endif()

# Compose full version string for configuration files
set(VANILLAPDF_VERSION "${VANILLAPDF_VERSION_MAJOR}.${VANILLAPDF_VERSION_MINOR}.${VANILLAPDF_VERSION_PATCH}${VANILLAPDF_VERSION_BUILD_SUFFIX}")
