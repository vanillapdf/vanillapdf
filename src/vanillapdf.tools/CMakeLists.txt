project(vanillapdf.tools LANGUAGES C)

# Solution root folder
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include/files.cmake")

# Additional directories in the root project
include_directories("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include")

# Include local files in the directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_TOOLS_SOURCES
    main.c
    extract.c
    extract_object.c
    filter.c
    merge.c
    sign.c
    sign_custom.c
    verify.c
    encrypt.c
    decrypt.c
    write_custom.c
)

set(VANILLAPDF_TOOLS_HEADERS
    tools.h
)

set(VANILLAPDF_TOOLS_RESOURCE_FILES
    resources.rc
)

# Resource files
source_group("Resource Files" FILES
    ${VANILLAPDF_TOOLS_RESOURCE_FILES}
)

source_group("Header Files" FILES
    ${VANILLAPDF_TOOLS_HEADERS}
)

source_group("Source Files" FILES
    ${VANILLAPDF_TOOLS_SOURCES}
)

add_executable(vanillapdf.tools
    ${VANILLAPDF_TOOLS_HEADERS}
    ${VANILLAPDF_TOOLS_SOURCES}
    ${VANILLAPDF_TOOLS_RESOURCE_FILES}
)

# Configure default compilation parameters, strict warnings, config based definition for DEBUG/RELEASE, etc.
vanillapdf_target_compile_defaults(vanillapdf.tools)

# Configure MSVC runtime library for tools target
# Skip if already set externally (e.g. vcpkg toolchain, user, parent project)
if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    vanillapdf_configure_msvc_runtime(vanillapdf.tools)
endif()

# Optional stack sanitizer
enable_stack_sanitizer_for_target(vanillapdf.tools)

if(WIN32)

    # Copy DLL dependencies on windows to our build directory
    add_custom_command(
        TARGET vanillapdf.tools POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_FILE_DIR:vanillapdf>"
        "$<TARGET_FILE_DIR:vanillapdf.tools>"
    )
endif(WIN32)

set(VANILLAPDF_TOOLS_LIBS
    vanillapdf
)

if (CMAKE_COMPILER_IS_GNUCC)

    # C executables does not have access to c++ standard library by default
    # We need to explicitly pull dependencies
    set(VANILLAPDF_TOOLS_LIBS ${VANILLAPDF_TOOLS_LIBS} -lstdc++)

endif (CMAKE_COMPILER_IS_GNUCC)

target_link_libraries(vanillapdf.tools PRIVATE ${VANILLAPDF_TOOLS_LIBS})

# In case we have the dynamic link configuration add specific definitions
if(BUILD_SHARED_LIBS)
    target_compile_definitions(vanillapdf.tools PRIVATE VANILLAPDF_CONFIGURATION_DLL)

    # CMake strips build-time RPATHs during install, so the installed binary
    # has no RPATH entries and cannot find libvanillapdf in ../lib.
    # Only set if CMAKE_INSTALL_RPATH was not provided externally (e.g. by a
    # package manager), so their value is respected when present.
    if(NOT CMAKE_INSTALL_RPATH)
        if(UNIX AND NOT APPLE)
            set_target_properties(vanillapdf.tools PROPERTIES
                INSTALL_RPATH "$ORIGIN/../lib"
            )
        elseif(APPLE)
            set_target_properties(vanillapdf.tools PROPERTIES
                INSTALL_RPATH "@loader_path/../lib"
            )
        endif()
    endif()
endif()

# Project install targets
install(TARGETS vanillapdf.tools
    RUNTIME DESTINATION bin
)

# Specify version identificators
target_compile_definitions(vanillapdf.tools PRIVATE -DVANILLAPDF_TOOLS_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
target_compile_definitions(vanillapdf.tools PRIVATE -DVANILLAPDF_TOOLS_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
target_compile_definitions(vanillapdf.tools PRIVATE -DVANILLAPDF_TOOLS_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
target_compile_definitions(vanillapdf.tools PRIVATE -DVANILLAPDF_TOOLS_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

target_compile_definitions(vanillapdf.tools PRIVATE -DVANILLAPDF_TOOLS_ICON="${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake/vanilla_logo.ico")

# Add tests for vanillapdf-tools CLI
if(VANILLAPDF_ENABLE_TESTS)
    # Verify test for Granizo-signed.PDF (self-signed certificate) - should succeed with flag
    # Uses --skip-certificate-validation to bypass untrusted root check
    add_test(
        NAME "Tools.verify.Granizo-signed.with-flag"
        COMMAND $<TARGET_FILE:vanillapdf.tools> verify -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/test/custom/Granizo-signed.PDF" --skip-certificate-validation
    )

    # Verify test for Granizo-signed.PDF without flag - should fail (untrusted root)
    add_test(
        NAME "Tools.verify.Granizo-signed.without-flag"
        COMMAND $<TARGET_FILE:vanillapdf.tools> verify -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/test/custom/Granizo-signed.PDF"
    )
    set_tests_properties("Tools.verify.Granizo-signed.without-flag" PROPERTIES WILL_FAIL TRUE)
endif()
