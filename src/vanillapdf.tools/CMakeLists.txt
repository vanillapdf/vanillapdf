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
    remove_page.c
    sign.c
    sign_custom.c
    verify.c
    validate.c
    encrypt.c
    decrypt.c
    write_custom.c
    generate.c
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

# Embed a UTF-8 active-code-page manifest so the CLI receives UTF-8 argv and
# treats narrow file paths as UTF-8 on Windows (non-ASCII paths and passwords
# survive the argv boundary unchanged). Adding the .manifest as a target source
# lets CMake embed it through its own manifest-tool invocation, which works with
# both the Visual Studio and Ninja generators. Passing explicit
# "/MANIFEST:EMBED /MANIFESTINPUT" link options instead collides with CMake's
# default "/MANIFESTFILE" under Ninja (LNK4075 -> LNK1218 under the vcpkg port
# build's linker /WX). MSVC only (cl and clang-cl); MinGW would need a .rc manifest.
if(MSVC)
    target_sources(vanillapdf.tools PRIVATE vanillapdf.tools.manifest)
endif()

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
    # The path has to follow CMAKE_INSTALL_LIBDIR, which is lib64 on 64-bit
    # RPM distributions and a multiarch triplet on Debian.
    if(NOT CMAKE_INSTALL_RPATH)
        if(UNIX AND NOT APPLE)
            set_target_properties(vanillapdf.tools PROPERTIES
                INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
            )
        elseif(APPLE)
            set_target_properties(vanillapdf.tools PROPERTIES
                INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
            )
        endif()
    endif()
endif()

# Project install targets
# The CLI tool is an internal/testing utility (weakly documented) and is
# intentionally NOT shipped in the distributable packages. It is assigned to a
# dedicated "Tools" component that is excluded from CPACK_COMPONENTS_ALL, so it
# can still be installed on demand (cmake --install --component Tools) without
# leaking into the deb/rpm/etc. packages.
install(TARGETS vanillapdf.tools
    RUNTIME DESTINATION bin COMPONENT Tools
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
        COMMAND $<TARGET_FILE:vanillapdf.tools> verify -f "${VANILLAPDF_CORPUS_DIR}/custom/Granizo-signed.PDF" --skip-certificate-validation
    )

    # Verify test for Granizo-signed.PDF without flag - should fail (untrusted root)
    add_test(
        NAME "Tools.verify.Granizo-signed.without-flag"
        COMMAND $<TARGET_FILE:vanillapdf.tools> verify -f "${VANILLAPDF_CORPUS_DIR}/custom/Granizo-signed.PDF"
    )
    set_tests_properties("Tools.verify.Granizo-signed.without-flag" PROPERTIES WILL_FAIL TRUE)

    # OpenSSL 3.x: set provider module search path on verify tests (see vanillapdf/CMakeLists.txt)
    if(VANILLAPDF_TOOLS_EXTRA_PROPERTIES)
        set_tests_properties("Tools.verify.Granizo-signed.with-flag" ${VANILLAPDF_TOOLS_EXTRA_PROPERTIES})
        set_tests_properties("Tools.verify.Granizo-signed.without-flag" ${VANILLAPDF_TOOLS_EXTRA_PROPERTIES})
    endif()

    # Validate tests (paths are relative to the fetched corpus)
    set(VALIDATE_TEST_FILES
        "example.pdf"
        "19005-1_FAQ.PDF"
        "custom/minimalist.pdf"
        "pdf-association/pdf20examples/Simple-PDF-2.0-file.pdf"
    )

    foreach(TEST_FILE ${VALIDATE_TEST_FILES})
        string(REPLACE "/" "." TEST_NAME "${TEST_FILE}")
        string(REPLACE "\\" "." TEST_NAME "${TEST_NAME}")
        string(REGEX REPLACE "\\.[pP][dD][fF]$" "" TEST_NAME "${TEST_NAME}")

        add_test(
            NAME "Tools.validate.${TEST_NAME}"
            COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_CORPUS_DIR}/${TEST_FILE}"
        )
        set_tests_properties("Tools.validate.${TEST_NAME}" PROPERTIES
            PASS_REGULAR_EXPRESSION "Issues: 0 \\(0 error"
        )
    endforeach()

    # Validate tests for files with known structural errors (expected to fail validation)

    # issue5874.pdf: 1 xref bad-offset error (code=300), no warnings
    add_test(
        NAME "Tools.validate.pdfjs.issue5874"
        COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_CORPUS_DIR}/pdfjs/issue5874.pdf"
    )
    set_tests_properties("Tools.validate.pdfjs.issue5874" PROPERTIES
        PASS_REGULAR_EXPRESSION "Issues: 1 \\(1 error\\(s\\), 0 warning"
    )

    # request_for_taxpayer.pdf: 1 TrailerSizeMismatch warning (code=202) + 1 xref bad-offset error (code=300)
    add_test(
        NAME "Tools.validate.request_for_taxpayer"
        COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_CORPUS_DIR}/request_for_taxpayer.pdf"
    )
    set_tests_properties("Tools.validate.request_for_taxpayer" PROPERTIES
        PASS_REGULAR_EXPRESSION "Issues: 2 \\(1 error\\(s\\), 1 warning"
    )

    # bug1011159.pdf: 1 TrailerSizeMismatch warning (code=202) only - still reports VALID
    add_test(
        NAME "Tools.validate.pdfjs.bug1011159"
        COMMAND $<TARGET_FILE:vanillapdf.tools> validate -f "${VANILLAPDF_CORPUS_DIR}/pdfjs/bug1011159.pdf"
    )
    set_tests_properties("Tools.validate.pdfjs.bug1011159" PROPERTIES
        PASS_REGULAR_EXPRESSION "Issues: 1 \\(0 error\\(s\\), 1 warning"
    )

    # Read tests using memory IO strategy (paths are relative to the fetched corpus)
    set(MEMORY_STRATEGY_TEST_FILES
        "example.pdf"
        "19005-1_FAQ.PDF"
        "custom/minimalist.pdf"
        "pdf-association/pdf20examples/Simple-PDF-2.0-file.pdf"
    )

    foreach(TEST_FILE ${MEMORY_STRATEGY_TEST_FILES})
        # Derive test name from file path: replace / and \ with . and remove extension
        string(REPLACE "/" "." TEST_NAME "${TEST_FILE}")
        string(REPLACE "\\" "." TEST_NAME "${TEST_NAME}")
        string(REGEX REPLACE "\\.[pP][dD][fF]$" "" TEST_NAME "${TEST_NAME}")

        add_test(
            NAME "Tools.read.${TEST_NAME}"
            COMMAND $<TARGET_FILE:vanillapdf.tools> read -f "${VANILLAPDF_CORPUS_DIR}/${TEST_FILE}"
        )
    endforeach()

    # Non-ASCII path coverage: this CJK-named fixture lives in-repo rather than in
    # the fetched corpus. git checks out a UTF-8 filename correctly on every
    # platform and locale, whereas a non-ASCII name inside the downloaded archive
    # trips libarchive's locale conversion during extraction ("Pathname can't be
    # converted from UTF-8 to current locale" under a C/POSIX locale). Reading it
    # through the CLI exercises the UTF-8 app manifest end to end (argv ->
    # File_Open) — the reason narrow non-ASCII paths survive on Windows.
    add_test(
        NAME "Tools.read.chinese_names_中文的名字-2"
        COMMAND $<TARGET_FILE:vanillapdf.tools> read -f "${VANILLAPDF_SOLUTION_SOURCE_DIR}/fixtures/chinese_names_中文的名字-2.pdf"
    )

    # remove_page sanity tests on the in-repo fixtures. The tool itself checks
    # that the page count dropped by exactly one, so each test only asserts
    # the reported counts.

    # nested-pages.pdf: root -> [3-page node, 2-page node]. Page 4 is the
    # first page of the second subtree - the regression case where the flat
    # page number used to be applied directly to the root Kids array.
    add_test(
        NAME "Tools.remove_page.nested-pages.subtree-boundary"
        COMMAND $<TARGET_FILE:vanillapdf.tools> remove_page -s "${VANILLAPDF_SOLUTION_SOURCE_DIR}/fixtures/nested-pages.pdf" -d "${CMAKE_CURRENT_BINARY_DIR}/remove_page-nested-pages.pdf" -n 4
    )
    set_tests_properties("Tools.remove_page.nested-pages.subtree-boundary" PROPERTIES
        PASS_REGULAR_EXPRESSION "Removed page 4 \\(5 -> 4 pages\\)"
    )

    # Flat single-node tree, removal from the middle of the wide Kids array
    add_test(
        NAME "Tools.remove_page.flat-pages.middle"
        COMMAND $<TARGET_FILE:vanillapdf.tools> remove_page -s "${VANILLAPDF_SOLUTION_SOURCE_DIR}/fixtures/flat-pages.pdf" -d "${CMAKE_CURRENT_BINARY_DIR}/remove_page-flat-pages.pdf" -n 725
    )
    set_tests_properties("Tools.remove_page.flat-pages.middle" PROPERTIES
        PASS_REGULAR_EXPRESSION "Removed page 725 \\(1451 -> 1450 pages\\)"
    )

    # First page of a document with real content
    add_test(
        NAME "Tools.remove_page.sample-document.first"
        COMMAND $<TARGET_FILE:vanillapdf.tools> remove_page -s "${VANILLAPDF_SOLUTION_SOURCE_DIR}/fixtures/sample-document.pdf" -d "${CMAKE_CURRENT_BINARY_DIR}/remove_page-sample-document.pdf" -n 1
    )
    set_tests_properties("Tools.remove_page.sample-document.first" PROPERTIES
        PASS_REGULAR_EXPRESSION "Removed page 1 \\(2 -> 1 pages\\)"
    )

    # Removing the only page leaves a document with an empty page tree
    add_test(
        NAME "Tools.remove_page.chinese_names.only-page"
        COMMAND $<TARGET_FILE:vanillapdf.tools> remove_page -s "${VANILLAPDF_SOLUTION_SOURCE_DIR}/fixtures/chinese_names_中文的名字-2.pdf" -d "${CMAKE_CURRENT_BINARY_DIR}/remove_page-chinese_names.pdf" -n 1
    )
    set_tests_properties("Tools.remove_page.chinese_names.only-page" PROPERTIES
        PASS_REGULAR_EXPRESSION "Removed page 1 \\(1 -> 0 pages\\)"
    )

    # Removing from an empty document reports a clean out-of-range error
    # instead of the historical silent success
    add_test(
        NAME "Tools.remove_page.minimalist.out-of-range"
        COMMAND $<TARGET_FILE:vanillapdf.tools> remove_page -s "${VANILLAPDF_SOLUTION_SOURCE_DIR}/fixtures/minimalist.pdf" -d "${CMAKE_CURRENT_BINARY_DIR}/remove_page-minimalist.pdf" -n 1
    )
    set_tests_properties("Tools.remove_page.minimalist.out-of-range" PROPERTIES
        PASS_REGULAR_EXPRESSION "out of range, the document has 0 pages"
    )
endif()
