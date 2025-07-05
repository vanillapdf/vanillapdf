# CMake module to locate the OpenJPEG library
#
#  OPENJPEG_FOUND      - True if OpenJPEG was found
#  OPENJPEG_LIBRARIES  - Libraries to link against
#  OPENJPEG_INCLUDE_DIRS - Directories containing headers
#  OPENJPEG_VERSION    - Version string if detected
#
# The module first tries pkg-config and then falls back to
# standard library and header searches.

find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_OPENJPEG openjp2)
endif()

find_path(OPENJPEG_INCLUDE_DIR
    NAMES openjpeg.h
    HINTS ${PC_OPENJPEG_INCLUDEDIR} ${PC_OPENJPEG_INCLUDE_DIRS}
)

find_library(OPENJPEG_LIBRARY
    NAMES openjp2
    HINTS ${PC_OPENJPEG_LIBDIR} ${PC_OPENJPEG_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenJPEG
    DEFAULT_MSG
    OPENJPEG_LIBRARY OPENJPEG_INCLUDE_DIR
)

if(OPENJPEG_FOUND)
    set(OPENJPEG_LIBRARIES ${OPENJPEG_LIBRARY})
    set(OPENJPEG_INCLUDE_DIRS ${OPENJPEG_INCLUDE_DIR})
    if(PC_OPENJPEG_VERSION)
        set(OPENJPEG_VERSION ${PC_OPENJPEG_VERSION})
    endif()
endif()

mark_as_advanced(OPENJPEG_INCLUDE_DIR OPENJPEG_LIBRARY)
