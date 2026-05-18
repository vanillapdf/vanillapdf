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
    read.c
    resave.c
    sign.c
    sign_custom.c
    verify.c
    validate.c
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

# Optional sanitizers
enable_asan_for_target(vanillapdf.tools)
enable_ubsan_for_target(vanillapdf.tools)
enable_tsan_for_target(vanillapdf.tools)

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
    # OpenSSL 3.x: set provider module search path (see vanillapdf/CMakeLists.txt)
    set(VANILLAPDF_TOOLS_EXTRA_PROPERTIES "")
    if(WIN32 AND VANILLAPDF_INTERNAL_VCPKG AND NOT VANILLAPDF_EXTERNAL_OPENSSL
            AND VANILLAPDF_OPENSSL_MODULES_DIR)
        set(VANILLAPDF_TOOLS_EXTRA_PROPERTIES PROPERTIES ENVIRONMENT "OPENSSL_MODULES=${VANILLAPDF_OPENSSL_MODULES_DIR}")
    endif()

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

    # OpenSSL 3.x: set provider module search path on verify tests (see vanillapdf/CMakeLists.txt)
    if(VANILLAPDF_TOOLS_EXTRA_PROPERTIES)
        set_tests_properties("Tools.verify.Granizo-signed.with-flag" ${VANILLAPDF_TOOLS_EXTRA_PROPERTIES})
        set_tests_properties("Tools.verify.Granizo-signed.without-flag" ${VANILLAPDF_TOOLS_EXTRA_PROPERTIES})
    endif()

    # Validate tests
    set(VALIDATE_TEST_FILES
        "test/example.pdf"
        "test/19005-1_FAQ.PDF"
        "test/custom/minimalist.pdf"
        "test/pdf-association/pdf20examples/Simple-PDF-2.0-file.pdf"
    )

    foreach(TEST_FILE ${VALIDATE_TEST_FILES})
        string(REPLACE "/" "." TEST_NAME "${TEST_FILE}")
        string(REPLACE "\\" "." TEST_NAME "${TEST_NAME}")
        string(REGEX REPLACE "\\.[pP][dD][fF]$" "" TEST_NAME "${TEST_NAME}")

        add_test(
            NAME "Tools.validate.${TEST_NAME}"
            COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/${TEST_FILE}"
        )
        set_tests_properties("Tools.validate.${TEST_NAME}" PROPERTIES
            PASS_REGULAR_EXPRESSION "Issues: 0 \\(0 error"
        )
    endforeach()

    # Validate tests for files with known structural errors (expected to fail validation)

    # issue5874.pdf: 1 xref bad-offset error (code=300), no warnings
    add_test(
        NAME "Tools.validate.test.pdfjs.issue5874"
        COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/test/pdfjs/issue5874.pdf"
    )
    set_tests_properties("Tools.validate.test.pdfjs.issue5874" PROPERTIES
        PASS_REGULAR_EXPRESSION "Issues: 1 \\(1 error\\(s\\), 0 warning"
    )

    # request_for_taxpayer.pdf: 1 TrailerSizeMismatch warning (code=202) + 1 xref bad-offset error (code=300)
    add_test(
        NAME "Tools.validate.test.request_for_taxpayer"
        COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/test/request_for_taxpayer.pdf"
    )
    set_tests_properties("Tools.validate.test.request_for_taxpayer" PROPERTIES
        PASS_REGULAR_EXPRESSION "Issues: 2 \\(1 error\\(s\\), 1 warning"
    )

    # bug1011159.pdf: 1 TrailerSizeMismatch warning (code=202) only - still reports VALID
    add_test(
        NAME "Tools.validate.test.pdfjs.bug1011159"
        COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/test/pdfjs/bug1011159.pdf"
    )
    set_tests_properties("Tools.validate.test.pdfjs.bug1011159" PROPERTIES
        PASS_REGULAR_EXPRESSION "Issues: 1 \\(0 error\\(s\\), 1 warning"
    )

    # Read tests using memory IO strategy
    set(MEMORY_STRATEGY_TEST_FILES
        "test/example.pdf"
        "test/19005-1_FAQ.PDF"
        "test/custom/minimalist.pdf"
        "test/pdf-association/pdf20examples/Simple-PDF-2.0-file.pdf"
    )

    foreach(TEST_FILE ${MEMORY_STRATEGY_TEST_FILES})
        # Derive test name from file path: replace / and \ with . and remove extension
        string(REPLACE "/" "." TEST_NAME "${TEST_FILE}")
        string(REPLACE "\\" "." TEST_NAME "${TEST_NAME}")
        string(REGEX REPLACE "\\.[pP][dD][fF]$" "" TEST_NAME "${TEST_NAME}")

        add_test(
            NAME "Tools.read.${TEST_NAME}"
            COMMAND $<TARGET_FILE:vanillapdf.tools> read -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/${TEST_FILE}"
        )
    endforeach()
endif()
