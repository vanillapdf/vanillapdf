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

# There is a little trick going on
# By default, the CPACK_PACKAGE_FILE_NAME is set to
# "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}."
# However! Variable CPACK_PACKAGE_VERSION is only used internally a is not yet set!
# Therefore we have to create a different variable for storing the version string
set(PACKAGE_VERSION_NAME				"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(PACKAGE_SYSTEM_NAME					"${CMAKE_SYSTEM_NAME}")

# Special naming convention for windows
if(${PACKAGE_SYSTEM_NAME} MATCHES Windows)
	if(CMAKE_CL_64)
		set(PACKAGE_SYSTEM_NAME win64-x64)
	else()
		set(PACKAGE_SYSTEM_NAME win32-x86)
	endif()
endif()

# Override the default package version name with PROJECT_NAME instead of CPACK_PACKAGE_NAME
# Note: The CPACK_SYSTEM_NAME is not set either, so we have to use CMAKE_SYSTEM_NAME instead
set(CPACK_PACKAGE_FILE_NAME 			"${PROJECT_NAME}_${PACKAGE_VERSION_NAME}_${PACKAGE_SYSTEM_NAME}")

# External resources
set(CPACK_RESOURCE_FILE_LICENSE			${CMAKE_SOURCE_DIR}/cmake/LICENSE.txt)
set(CPACK_RESOURCE_FILE_README			${CMAKE_SOURCE_DIR}/cmake/README.txt)
#set(CPACK_RESOURCE_FILE_WELCOME			${CMAKE_SOURCE_DIR}/cmake/WELCOME.txt)
set(CPACK_PACKAGE_ICON					${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico)

# Include checksum file
set(CPACK_PACKAGE_CHECKSUM				SHA256)

# Variables specific to CPack RPM generator
set(CPACK_RPM_PACKAGE_LICENSE			"Proprietary")
set(CPACK_RPM_PACKAGE_GROUP				"Development/Libraries/C and C++")
set(CPACK_RPM_PACKAGE_URL				"https://vanillapdf.com/")
set(CPACK_RPM_PACKAGE_AUTOREQ			1)
set(CPACK_RPM_PACKAGE_AUTOPROV			1)

# Variables specific to CPack DEB generator
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE		"https://vanillapdf.com/")
set(CPACK_DEBIAN_PACKAGE_SECTION		"devel")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS		YES)

# Variables specific to CPack FreeBSD generator
set(CPACK_FREEBSD_PACKAGE_MAINTAINER	"info@vanillapdf.com")

# Variables specific to CPack WiX generator
set(CPACK_WIX_UPGRADE_GUID				"{88E3B5A2-C8B2-4F7C-A7E5-A2B764A9B0E6}")

set(CPACK_WIX_PRODUCT_ICON				${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico)
set(CPACK_WIX_PROGRAM_MENU_FOLDER		"Vanilla.PDF")
set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY	"Vanilla.PDF")

if(WIN32)
	set(CPACK_GENERATOR		"WIX;ZIP")
	
elseif(APPLE)
	set(CPACK_GENERATOR		"ZIP")
	set(CPACK_SYSTEM_NAME	"OSX")
	
elseif(UNIX)
	# Determine distribution and release
	execute_process(COMMAND lsb_release -si	OUTPUT_VARIABLE distribution					OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND lsb_release -sc	OUTPUT_VARIABLE release							OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND uname -m		OUTPUT_VARIABLE CPACK_RPM_PACKAGE_ARCHITECTURE	OUTPUT_STRIP_TRAILING_WHITESPACE)

	if(distribution STREQUAL "Debian" OR distribution STREQUAL "Ubuntu")
		set(CPACK_GENERATOR "DEB;TGZ")
		execute_process(COMMAND dpkg --print-architecture OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE OUTPUT_STRIP_TRAILING_WHITESPACE)
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PACKAGE_VERSION_NAME}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

	elseif(distribution MATCHES "RedHat.*")
		# extract the major version from RedHat full version (e.g. 6.7 --> 6)
		execute_process(COMMAND lsb_release -sr COMMAND sed s/[.].*//  OUTPUT_VARIABLE redhat_version_major OUTPUT_STRIP_TRAILING_WHITESPACE)
		set(CPACK_GENERATOR "RPM;TGZ")
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PACKAGE_VERSION_NAME}-${CPACK_RPM_PACKAGE_RELEASE}.el${redhat_version_major}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")

	elseif(distribution STREQUAL "Fedora")
		set(CPACK_GENERATOR "RPM;TGZ")
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PACKAGE_VERSION_NAME}.fc${release}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")
	  
	else()
		set(CPACK_GENERATOR "RPM;TGZ")
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PACKAGE_VERSION_NAME}-${release}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")
	endif()

else()
	# other operating system (not Windows/Apple/Unix)
endif()
