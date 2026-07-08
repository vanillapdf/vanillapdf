##### set CPack properties #####
#
# Good doc/tutorial/example:
# - http://www.cmake.org/Wiki/CMake:CPackPackageGenerators
# - http://www.cmake.org/cmake/help/v3.3/module/CPack.html
# - http://sourceforge.net/p/klusters/klusters/ci/master/tree/CMakeLists.txt

#set(CPACK_STRIP_FILES					ON)
set(CPACK_PACKAGE_NAME					"Vanilla.PDF")
set(CPACK_PACKAGE_VERSION_MAJOR			${VANILLAPDF_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR			${VANILLAPDF_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH			${VANILLAPDF_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT				"info@vanillapdf.com")
set(CPACK_PACKAGE_VENDOR				"Vanilla.PDF Labs")
set(CPACK_PACKAGE_INSTALL_DIRECTORY		"Vanilla.PDF Labs/Vanilla.PDF")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY	"Cross-platform toolkit for creating and modifying PDF documents")
set(CPACK_PACKAGE_DESCRIPTION			"Cross-platform toolkit for creating and modifying PDF documents")

# CPack computes CPACK_PACKAGE_VERSION internally as MAJOR.MINOR.PATCH,
# which excludes the build suffix. Override it so pre-release package
# versions match the dependency declarations.
# Separator per format: '-' for semver (ZIP, WiX, DMG), '~' for DEB/RPM.
set(PACKAGE_VERSION_NAME "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
if(VANILLAPDF_VERSION_BUILD_SUFFIX)
    set(PACKAGE_VERSION_NAME "${PACKAGE_VERSION_NAME}-${VANILLAPDF_VERSION_BUILD_SUFFIX}")
    set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION_NAME}")
endif()
set(PACKAGE_SYSTEM_NAME					"${CMAKE_SYSTEM_NAME}")

if (PLATFORM_IDENTIFIER)
    set(PACKAGE_SYSTEM_NAME "${PLATFORM_IDENTIFIER}")
else()

    # Special naming convention for windows
    if(${PACKAGE_SYSTEM_NAME} MATCHES "Windows")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(PACKAGE_SYSTEM_NAME win64-x64)
        else()
            set(PACKAGE_SYSTEM_NAME win32-x86)
        endif()
    elseif(${PACKAGE_SYSTEM_NAME} MATCHES "Darwin")
        set(PACKAGE_SYSTEM_NAME osx)
    elseif(${PACKAGE_SYSTEM_NAME} MATCHES "Linux")
        set(PACKAGE_SYSTEM_NAME linux)
    endif()

    message(STATUS "Platform identifier was not specified, using CMAKE_SYSTEM_NAME: \"${PACKAGE_SYSTEM_NAME}\"")
endif()

# Override the default package version name with PROJECT_NAME instead of CPACK_PACKAGE_NAME
# Note: The CPACK_SYSTEM_NAME is not set either, so we have to use CMAKE_SYSTEM_NAME instead
set(CPACK_PACKAGE_FILE_NAME 			"${PROJECT_NAME}_${PACKAGE_VERSION_NAME}_${PACKAGE_SYSTEM_NAME}")

# External resources
set(CPACK_RESOURCE_FILE_LICENSE			${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt)
set(CPACK_RESOURCE_FILE_README			${CMAKE_CURRENT_SOURCE_DIR}/cmake/README.txt)
#set(CPACK_RESOURCE_FILE_WELCOME			${CMAKE_CURRENT_SOURCE_DIR}/cmake/WELCOME.txt)
set(CPACK_PACKAGE_ICON					${CMAKE_CURRENT_SOURCE_DIR}/cmake/vanilla_logo.ico)

# Include checksum file
set(CPACK_PACKAGE_CHECKSUM				SHA256)

# Component-based packaging (used by DEB and RPM generators)
set(CPACK_COMPONENTS_ALL Runtime Development)
set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Vanilla.PDF Runtime")
set(CPACK_COMPONENT_RUNTIME_DESCRIPTION "Shared library")
set(CPACK_COMPONENT_RUNTIME_REQUIRED ON)
set(CPACK_COMPONENT_DEVELOPMENT_DISPLAY_NAME "Vanilla.PDF Development")
set(CPACK_COMPONENT_DEVELOPMENT_DESCRIPTION "Headers, static libraries, and CMake config files")
set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Runtime)

# DEB and RPM use '~' for pre-release (sorts before release version)
set(PACKAGE_VERSION_NAME_DEB_RPM "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
if(VANILLAPDF_VERSION_BUILD_SUFFIX)
    set(PACKAGE_VERSION_NAME_DEB_RPM "${PACKAGE_VERSION_NAME_DEB_RPM}~${VANILLAPDF_VERSION_BUILD_SUFFIX}")
endif()

# Variables specific to CPack RPM generator
set(CPACK_RPM_PACKAGE_LICENSE			"Apache-2.0")
set(CPACK_RPM_PACKAGE_GROUP				"Development/Libraries/C and C++")
set(CPACK_RPM_PACKAGE_URL				"https://vanillapdf.com/")
set(CPACK_RPM_PACKAGE_AUTOREQ			1)
set(CPACK_RPM_PACKAGE_AUTOPROV			1)
set(CPACK_RPM_COMPONENT_INSTALL			ON)
set(CPACK_RPM_PACKAGE_VERSION			"${PACKAGE_VERSION_NAME_DEB_RPM}")
# Conventional file names: <package>-<version>.<arch>.rpm (no component suffix)
set(CPACK_RPM_FILE_NAME					RPM-DEFAULT)
set(CPACK_RPM_RUNTIME_PACKAGE_NAME		"libvanillapdf")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_NAME	"libvanillapdf-devel")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_REQUIRES "libvanillapdf = ${PACKAGE_VERSION_NAME_DEB_RPM}")

# Variables specific to CPack DEB generator
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE		"https://vanillapdf.com/")
set(CPACK_DEBIAN_PACKAGE_SECTION		"devel")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS		YES)
set(CPACK_DEB_COMPONENT_INSTALL			ON)
set(CPACK_DEBIAN_PACKAGE_VERSION		"${PACKAGE_VERSION_NAME_DEB_RPM}")
# Conventional file names: <package>_<version>_<arch>.deb (no component suffix)
set(CPACK_DEBIAN_FILE_NAME				DEB-DEFAULT)
set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME	"libvanillapdf")
set(CPACK_DEBIAN_RUNTIME_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME "libvanillapdf-dev")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_DEPENDS "libvanillapdf (= ${PACKAGE_VERSION_NAME_DEB_RPM})")

# Variables specific to CPack FreeBSD generator
set(CPACK_FREEBSD_PACKAGE_MAINTAINER	"info@vanillapdf.com")

# Variables specific to CPack WiX generator
# WiX MSI generation is currently disabled (ZIP-only). To re-enable,
# add "WIX" back to CPACK_GENERATOR above and ensure WiX is installed.
if(CPACK_GENERATOR MATCHES "WIX")
    set(CPACK_WIX_UPGRADE_GUID				"{88E3B5A2-C8B2-4F7C-A7E5-A2B764A9B0E6}")
    set(CPACK_WIX_PRODUCT_ICON				${CMAKE_CURRENT_SOURCE_DIR}/cmake/vanilla_logo.ico)
    set(CPACK_WIX_PROGRAM_MENU_FOLDER		"Vanilla.PDF")
    set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY	"Vanilla.PDF")
endif()

# Variables specific to CPack NuGet generator
#set(CPACK_NUGET_PACKAGE_NAME							"vanillapdf.runtime.${PLATFORM_IDENTIFIER}")
#set(CPACK_NUGET_PACKAGE_DESCRIPTION					"Vanilla.PDF is a cross-platform SDK for creating and modifying PDF documents")
#set(CPACK_NUGET_PACKAGE_HOMEPAGE_URL					"https://vanillapdf.com/")
#set(CPACK_NUGET_PACKAGE_LICENSE_FILE_NAME				"common/LICENSE.txt")
#set(CPACK_NUGET_PACKAGE_REQUIRE_LICENSE_ACCEPTANCE		true)
##set(CPACK_NUGET_PACKAGE_ICON							"${CMAKE_CURRENT_SOURCE_DIR}/cmake/vanilla_logo.png")
#set(CPACK_NUGET_PACKAGE_AUTHORS						"Vanilla.PDF Labs s.r.o.")
#set(CPACK_NUGET_PACKAGE_OWNERS							"Vanilla.PDF Labs s.r.o.")
#set(CPACK_NUGET_PACKAGE_TITLE							"vanillapdf.runtime.${PLATFORM_IDENTIFIER}")
#set(CPACK_NUGET_PACKAGE_COPYRIGHT						"Copyright 2018-2022 Vanilla.PDF Labs s.r.o.")
#set(CPACK_NUGET_PACKAGE_TAGS							"pdf parsing sign native")

# I am not able to use NuGet generator since it bundles a lot of unnecessary
# items and the controlling is not sufficient, so let's do it the old way

# Initialize NuGet project files for the current platform
# This script simply configures *.in templates to actual files.
include(${CMAKE_CURRENT_LIST_DIR}/nuget.cmake)

# This could be useful, however at the time of CPack
# I am not able to alter some of the properties,
# such as install target destination

# Allow runtime customizations based on current generator
#set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/packaging_runtime.cmake)

if(WIN32)
    set(CPACK_GENERATOR		"ZIP")

elseif(APPLE)
    set(CPACK_GENERATOR		"DragNDrop;TGZ")
    set(CPACK_SYSTEM_NAME	"OSX")

    # Use retry script for hdiutil to work around XProtect race conditions
    # See: https://github.com/HEXRD/hexrdgui/pull/1768
    set(CPACK_COMMAND_HDIUTIL "${CMAKE_CURRENT_SOURCE_DIR}/scripts/hdiutil_retry.sh")

elseif(UNIX)
    set(CPACK_GENERATOR		"TGZ")
else()
    # other operating system (not Windows/Apple/Unix)
    message(FATAL_ERROR "Unsupported platform")
endif()

# Opt-in to detect and build custom distro packages RHEL/DEB
if (DETECT_CUSTOM_LINUX_DISTRIBUTION)

    message(STATUS "Detecting custom Linux distribution")

    # OS Architecture
    execute_process(COMMAND uname -m		OUTPUT_VARIABLE PACKAGE_ARCHITECTURE					OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(PACKAGE_ARCHITECTURE STREQUAL "x86_64")
        if(VANILLAPDF_FORCE_32_BIT)
            set(PACKAGE_FILENAME_ARCHITECTURE "x86")
        else(VANILLAPDF_FORCE_32_BIT)
            set(PACKAGE_FILENAME_ARCHITECTURE "x64")
        endif(VANILLAPDF_FORCE_32_BIT)
    endif(PACKAGE_ARCHITECTURE STREQUAL "x86_64")

    if(PACKAGE_ARCHITECTURE STREQUAL "arm" OR PACKAGE_ARCHITECTURE STREQUAL "armv7l")
        set(PACKAGE_FILENAME_ARCHITECTURE "arm")
    endif(PACKAGE_ARCHITECTURE STREQUAL "arm" OR PACKAGE_ARCHITECTURE STREQUAL "armv7l")

    if(PACKAGE_ARCHITECTURE STREQUAL "aarch64")
        if(VANILLAPDF_FORCE_32_BIT)
            set(PACKAGE_FILENAME_ARCHITECTURE "arm")
        else(VANILLAPDF_FORCE_32_BIT)
            set(PACKAGE_FILENAME_ARCHITECTURE "arm64")
        endif(VANILLAPDF_FORCE_32_BIT)
    endif(PACKAGE_ARCHITECTURE STREQUAL "aarch64")

    if(NOT DEFINED PACKAGE_FILENAME_ARCHITECTURE)
        message(FATAL_ERROR "Unknown platform architecture: ${PACKAGE_ARCHITECTURE}")
    endif(NOT DEFINED PACKAGE_FILENAME_ARCHITECTURE)

    # Determine distribution and release
    file(STRINGS /etc/os-release PACKAGE_DISTRIBUTION REGEX "^ID=")
    string(REGEX REPLACE "ID=\"?([^\"]*)\"?" "\\1" PACKAGE_DISTRIBUTION "${PACKAGE_DISTRIBUTION}")

    message(STATUS "Packaging for OS: ${PACKAGE_DISTRIBUTION}")

    if(PACKAGE_DISTRIBUTION STREQUAL "debian" OR PACKAGE_DISTRIBUTION STREQUAL "ubuntu")
        string(TOLOWER ${PACKAGE_DISTRIBUTION} PACKAGE_DISTRIBUTION)

        file(STRINGS /etc/os-release OS_VERSION REGEX "^VERSION_ID=")
        string(REGEX REPLACE "VERSION_ID=\"?([^\"]*)\"?" "\\1" OS_VERSION "${OS_VERSION}")

        set(CPACK_GENERATOR "DEB;TGZ")
        set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PACKAGE_VERSION_NAME}-${PACKAGE_DISTRIBUTION}.${OS_VERSION}_${PACKAGE_FILENAME_ARCHITECTURE}")

    elseif(PACKAGE_DISTRIBUTION STREQUAL "redhat" OR PACKAGE_DISTRIBUTION STREQUAL "centos" OR PACKAGE_DISTRIBUTION STREQUAL "rocky")

        # Determine distribution and release
        file(STRINGS /etc/os-release PLATFORM_ID REGEX "^PLATFORM_ID=")
        string(REGEX REPLACE "PLATFORM_ID=\"platform:([^\"]*)\"" "\\1" PLATFORM_ID "${PLATFORM_ID}")

        set(CPACK_GENERATOR "RPM;TGZ")
        set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PACKAGE_VERSION_NAME}.${PLATFORM_ID}.${PACKAGE_ARCHITECTURE}")

    else()
        message(FATAL_ERROR "Unknown UNIX distribution: ${PACKAGE_DISTRIBUTION}")
    endif()
endif(DETECT_CUSTOM_LINUX_DISTRIBUTION)
