# Version information for Vanilla.PDF

# Set production version
set(VANILLAPDF_VERSION_MAJOR 2)
set(VANILLAPDF_VERSION_MINOR 3)
set(VANILLAPDF_VERSION_PATCH 0)
set(VANILLAPDF_VERSION_BUILD 0)
set(VANILLAPDF_VERSION_BUILD_STRING "")

# Allow CI to override patch version (e.g., 999 for nightly builds)
if(VANILLAPDF_VERSION_PATCH_OVERRIDE)
    set(VANILLAPDF_VERSION_PATCH ${VANILLAPDF_VERSION_PATCH_OVERRIDE})
endif()

# Allow CI to override build version (e.g., 20251228 for nightly builds)
if(VANILLAPDF_VERSION_BUILD_OVERRIDE)
    set(VANILLAPDF_VERSION_BUILD ${VANILLAPDF_VERSION_BUILD_OVERRIDE})
    set(VANILLAPDF_VERSION_BUILD_STRING ".${VANILLAPDF_VERSION_BUILD_OVERRIDE}")
endif()

# Optional build suffix for pre-release builds (without leading separator).
# The separator is added per format: '-' for semver/NuGet, '~' for DEB/RPM.
# Examples: dev, alpha.1, beta.1, rc.1, nightly.main
if(NOT DEFINED VANILLAPDF_VERSION_BUILD_SUFFIX)
    set(VANILLAPDF_VERSION_BUILD_SUFFIX "")
endif()

# Compose full version string for configuration files
# Examples: 2.3.0, 2.3.0-dev, 2.3.0-alpha.1, 2.3.0-rc.1, 2.3.999.20251228-nightly.main
set(VANILLAPDF_VERSION "${VANILLAPDF_VERSION_MAJOR}.${VANILLAPDF_VERSION_MINOR}.${VANILLAPDF_VERSION_PATCH}${VANILLAPDF_VERSION_BUILD_STRING}")
if(VANILLAPDF_VERSION_BUILD_SUFFIX)
    set(VANILLAPDF_VERSION "${VANILLAPDF_VERSION}-${VANILLAPDF_VERSION_BUILD_SUFFIX}")
endif()
