##### set CPack properties #####
#
# Good doc/tutorial/example:
# - http://www.cmake.org/Wiki/CMake:CPackPackageGenerators
# - http://www.cmake.org/cmake/help/v3.3/module/CPack.html
# - http://sourceforge.net/p/klusters/klusters/ci/master/tree/CMakeLists.txt

#set(CPACK_STRIP_FILES					ON)
set(CPACK_PACKAGE_NAME					"Vanilla PDF")
set(CPACK_PACKAGE_VERSION_MAJOR			${VANILLAPDF_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR			${VANILLAPDF_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH			${VANILLAPDF_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT				"info@vanillapdf.com")
set(CPACK_PACKAGE_VENDOR				"Vanilla PDF")
set(CPACK_PACKAGE_INSTALL_DIRECTORY		"Vanilla PDF Labs/Vanilla PDF")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY	"Cross-platform toolkit for creating and manipulating existing PDF documents")
set(CPACK_PACKAGE_DESCRIPTION			"Cross-platform toolkit for creating and manipulating existing PDF documents")
set(CPACK_PACKAGE_FILE_NAME 			${PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_SYSTEM_NAME})

set(CPACK_RESOURCE_FILE_LICENSE			${CMAKE_SOURCE_DIR}/cmake/LICENSE.txt)
set(CPACK_RESOURCE_FILE_README			${CMAKE_SOURCE_DIR}/cmake/README.txt)
set(CPACK_RESOURCE_FILE_WELCOME			${CMAKE_SOURCE_DIR}/cmake/WELCOME.txt)
set(CPACK_PACKAGE_ICON					${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico)
set(CPACK_PACKAGE_CHECKSUM				SHA256)

# Variables specific to CPack RPM generator
set(CPACK_RPM_PACKAGE_LICENSE			"Proprietary")
set(CPACK_RPM_PACKAGE_GROUP				"Development/Tools")
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
set(CPACK_WIX_PRODUCT_GUID				"{4873D236-CABD-43BD-B95F-07AD54DB177F}")
set(CPACK_WIX_UPGRADE_GUID				"{88E3B5A2-C8B2-4F7C-A7E5-A2B764A9B0E6}")

set(CPACK_WIX_PRODUCT_ICON				${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico)
set(CPACK_WIX_PROGRAM_MENU_FOLDER		"Vanilla PDF")
set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY	"VanillaPDF")

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
		set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})

	elseif(distribution MATCHES "RedHat.*")
		# extract the major version from RedHat full version (e.g. 6.7 --> 6)
		execute_process(COMMAND lsb_release -sr COMMAND sed s/[.].*//  OUTPUT_VARIABLE redhat_version_major OUTPUT_STRIP_TRAILING_WHITESPACE)
		set(CPACK_GENERATOR "RPM;TGZ")
		set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.el${redhat_version_major}.${CPACK_RPM_PACKAGE_ARCHITECTURE})

	elseif(distribution STREQUAL "Fedora")
		set(CPACK_GENERATOR "RPM;TGZ")
		set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}.fc${release}.${CPACK_RPM_PACKAGE_ARCHITECTURE})
	  
	else()
		set(CPACK_GENERATOR "RPM;TGZ")
		set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${release}.${CPACK_RPM_PACKAGE_ARCHITECTURE})
	endif()

else()
	# other operating system (not Windows/Apple/Unix)
endif()
