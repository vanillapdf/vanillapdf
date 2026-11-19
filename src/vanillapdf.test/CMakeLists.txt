project(vanillapdf.test LANGUAGES C)

# Solution root folder
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include/files.cmake")

# Additional directories in the root project
include_directories("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include")

# Include local files in the directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_TEST_SOURCES
    documents.c
    files.c
    objects.c
    main.c
    utils.c
)

set(VANILLAPDF_TEST_HEADERS
    test.h
)

set(VANILLAPDF_TEST_RESOURCE_FILES
    resources.rc
)

# Resource files
source_group("Resource Files" FILES
    ${VANILLAPDF_TEST_RESOURCE_FILES}
)

source_group("Header Files" FILES
    ${VANILLAPDF_TEST_HEADERS}
)

source_group("Source Files" FILES
    ${VANILLAPDF_TEST_SOURCES}
)

add_executable(vanillapdf.test
    ${VANILLAPDF_TEST_HEADERS}
    ${VANILLAPDF_TEST_SOURCES}
    ${VANILLAPDF_TEST_RESOURCE_FILES}
)

# Configure default compilation parameters, strict warnings, config based definition for DEBUG/RELEASE, etc.
vanillapdf_target_compile_defaults(vanillapdf.test)

# Embed a UTF-8 active-code-page manifest so the CLI receives UTF-8 argv and
# treats narrow file paths as UTF-8 on Windows (non-ASCII paths and passwords
# survive the argv boundary unchanged). Adding the .manifest as a target source
# lets CMake embed it through its own manifest-tool invocation, which works with
# both the Visual Studio and Ninja generators. Passing explicit
# "/MANIFEST:EMBED /MANIFESTINPUT" link options instead collides with CMake's
# default "/MANIFESTFILE" under Ninja (LNK4075 -> LNK1218 under the vcpkg port
# build's linker /WX). MSVC only (cl and clang-cl); MinGW would need a .rc manifest.
if(MSVC)
    target_sources(vanillapdf.test PRIVATE vanillapdf.test.manifest)
endif()

# Configure MSVC runtime library for test target
# Skip if already set externally (e.g. vcpkg toolchain, user, parent project)
if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    vanillapdf_configure_msvc_runtime(vanillapdf.test)
endif()

# Optional sanitizers
enable_asan_for_target(vanillapdf.test)
enable_ubsan_for_target(vanillapdf.test)
enable_tsan_for_target(vanillapdf.test)

if(WIN32)
    # Copy DLL dependencies on windows to our build directory
    add_custom_command(
        TARGET vanillapdf.test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_FILE_DIR:vanillapdf>"
        "$<TARGET_FILE_DIR:vanillapdf.test>"
    )
endif(WIN32)

set(VANILLAPDF_TEST_LIBS
    vanillapdf
)

target_link_libraries(vanillapdf.test PRIVATE ${VANILLAPDF_TEST_LIBS})

# The test is written in C, but a static vanillapdf needs the C++ runtime.
# Linking through the C++ driver selects the right one on every toolchain
# (libstdc++ for GCC, libc++ for Clang and Apple Clang).
set_target_properties(vanillapdf.test PROPERTIES LINKER_LANGUAGE CXX)

# We do not need test project to be shipped
#install(TARGETS vanillapdf.test
    #RUNTIME DESTINATION "bin/${PLATFORM_IDENTIFIER}"
#)

# Specify version identificators
target_compile_definitions(vanillapdf.test PRIVATE -DVANILLAPDF_TEST_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
target_compile_definitions(vanillapdf.test PRIVATE -DVANILLAPDF_TEST_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
target_compile_definitions(vanillapdf.test PRIVATE -DVANILLAPDF_TEST_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
target_compile_definitions(vanillapdf.test PRIVATE -DVANILLAPDF_TEST_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

target_compile_definitions(vanillapdf.test PRIVATE -DVANILLAPDF_TEST_ICON="${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake/vanilla_logo.ico")

# Python for running the test cases
find_package (Python3 COMPONENTS Interpreter REQUIRED)

# When cross-compiling (e.g. Android), the test binary cannot execute on the
# host. CMAKE_CROSSCOMPILING_EMULATOR carries the launcher that runs it in the
# target environment (the adb wrapper for Android presets); the Python driver
# itself stays on the host.
set(VANILLAPDF_TEST_LAUNCHER_OPTIONS "")
if(CMAKE_CROSSCOMPILING_EMULATOR)
    set(VANILLAPDF_TEST_LAUNCHER_OPTIONS --launcher "${CMAKE_CROSSCOMPILING_EMULATOR}")
endif()

# The test corpus + manifest are fetched once by cmake/testdata.cmake (included
# from the top-level CMakeLists). Enumerate one ctest per tested corpus fixture
# straight from the manifest — the authoritative fixture list — no globbing.
# All manifest paths resolve against the extracted testdata root.
file(READ "${VANILLAPDF_MANIFEST_FILE}" VANILLAPDF_MANIFEST)

string(JSON VANILLAPDF_MANIFEST_FILES GET "${VANILLAPDF_MANIFEST}" files)
string(JSON VANILLAPDF_MANIFEST_COUNT LENGTH "${VANILLAPDF_MANIFEST_FILES}")
math(EXPR VANILLAPDF_MANIFEST_LAST "${VANILLAPDF_MANIFEST_COUNT} - 1")

foreach(_index RANGE ${VANILLAPDF_MANIFEST_LAST})
    string(JSON TEST_KEY MEMBER "${VANILLAPDF_MANIFEST_FILES}" ${_index})

    # A fixture is a test case only when it is a PDF in valid status. The
    # manifest's orthogonal type/status fields say so directly: certificate
    # inputs fall out by type, broken/analysis fixtures by status. No directory
    # convention required.
    string(JSON TEST_TYPE GET "${VANILLAPDF_MANIFEST_FILES}" "${TEST_KEY}" type)
    string(JSON TEST_STATUS GET "${VANILLAPDF_MANIFEST_FILES}" "${TEST_KEY}" status)
    if(NOT (TEST_TYPE STREQUAL "pdf" AND TEST_STATUS STREQUAL "valid"))
        continue()
    endif()

    # Test name is the bare filename (corpus basenames are collision-free)
    get_filename_component(TEST_FILENAME "${TEST_KEY}" NAME)

    add_test(
        NAME ${TEST_FILENAME}
        COMMAND ${Python3_EXECUTABLE}
        "${VANILLAPDF_SOLUTION_SOURCE_DIR}/scripts/run_test.py"
        --exe            "$<TARGET_FILE:vanillapdf.test>"
        --testdata-root  "${VANILLAPDF_TESTDATA_ROOT}"				# all manifest paths resolve here
        --key            "${TEST_KEY}"								# manifest key for this fixture
        --manifest       "${VANILLAPDF_MANIFEST_FILE}"				# expectations (passwords, skips, merge, signing)
        --license        "${VANILLAPDF_SOLUTION_SOURCE_DIR}/scripts/VanillaPDF.lic"
        ${VANILLAPDF_TEST_LAUNCHER_OPTIONS}
    )

    # The corpus contains documents protected with legacy algorithms, see cmake/test_environment.cmake
    set_tests_properties(${TEST_FILENAME} PROPERTIES ENVIRONMENT_MODIFICATION "${VANILLAPDF_TEST_ENVIRONMENT}")
endforeach()

# MPK_SLOVLEX.pdf: 9.5 MB, 1,450 pages — takes 600-1500s under sanitizers and
# dominates CI time, so it is disabled.
set_tests_properties(MPK_SLOVLEX.pdf PROPERTIES DISABLED TRUE)

# The examples shown in the documentation. They are built and run here so that
# the published code cannot stop compiling or working unnoticed.
function(vanillapdf_add_example EXAMPLE_NAME)
    set(EXAMPLE_TARGET "vanillapdf.example.${EXAMPLE_NAME}")

    add_executable(${EXAMPLE_TARGET} "${VANILLAPDF_SOLUTION_SOURCE_DIR}/examples/${EXAMPLE_NAME}/main.c")
    vanillapdf_target_compile_defaults(${EXAMPLE_TARGET})
    target_link_libraries(${EXAMPLE_TARGET} PRIVATE vanillapdf)

    # A static vanillapdf needs the C++ runtime, see vanillapdf.test above
    set_target_properties(${EXAMPLE_TARGET} PROPERTIES LINKER_LANGUAGE CXX)

    if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
        vanillapdf_configure_msvc_runtime(${EXAMPLE_TARGET})
    endif()

    if(WIN32)
        add_custom_command(
            TARGET ${EXAMPLE_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<TARGET_FILE_DIR:vanillapdf>"
            "$<TARGET_FILE_DIR:${EXAMPLE_TARGET}>"
        )
    endif(WIN32)

    # The examples write their output into the working directory, which only
    # exists on the host. A cross-compiled build (Android) still compiles them.
    if(NOT CMAKE_CROSSCOMPILING)
        add_test(
            NAME Examples.${EXAMPLE_NAME}
            COMMAND ${EXAMPLE_TARGET} ${ARGN}
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        )
    endif()
endfunction()

set(VANILLAPDF_EXAMPLE_FIXTURES "${VANILLAPDF_SOLUTION_SOURCE_DIR}/fixtures")

vanillapdf_add_example(quickstart)
vanillapdf_add_example(create)

vanillapdf_add_example(merge
    "example-merged.pdf"
    "${VANILLAPDF_EXAMPLE_FIXTURES}/sample-document.pdf"
    "${VANILLAPDF_EXAMPLE_FIXTURES}/nested-pages.pdf"
)

# Inspects the merged document, which shows that the merge kept every page
vanillapdf_add_example(inspect "example-merged.pdf")

vanillapdf_add_example(validation "${VANILLAPDF_EXAMPLE_FIXTURES}/sample-document.pdf")

# Signing and encryption need OpenSSL, which the encryption feature brings in
if(VANILLAPDF_ENABLE_ENCRYPTION)
    vanillapdf_add_example(signing
        "${VANILLAPDF_EXAMPLE_FIXTURES}/sample-document.pdf"
        "example-signed.pdf"
        "${VANILLAPDF_EXAMPLE_FIXTURES}/certificates/Test_EC384_SHA512.pfx"
        "test"
    )

    # Verifies the document signed by the signing example
    vanillapdf_add_example(verification "example-signed.pdf")

    vanillapdf_add_example(visible-signature
        "${VANILLAPDF_EXAMPLE_FIXTURES}/sample-document.pdf"
        "example-visible-signed.pdf"
        "${VANILLAPDF_EXAMPLE_FIXTURES}/certificates/Test_EC384_SHA512.pfx"
        "test"
    )

    vanillapdf_add_example(encryption
        "${VANILLAPDF_EXAMPLE_FIXTURES}/sample-document.pdf"
        "example-encrypted.pdf"
        "user"
        "owner"
    )
endif()

if(NOT CMAKE_CROSSCOMPILING)
    set_tests_properties(Examples.merge PROPERTIES FIXTURES_SETUP ExampleMerged)
    set_tests_properties(Examples.inspect PROPERTIES
        FIXTURES_REQUIRED ExampleMerged
        PASS_REGULAR_EXPRESSION "Pages: 7"
    )

    if(VANILLAPDF_ENABLE_ENCRYPTION)
        set_tests_properties(Examples.signing PROPERTIES FIXTURES_SETUP ExampleSigned)

        # The test certificate does not chain to a trusted root, so the example
        # reports the signature as not valid; the signed content must be intact
        set_tests_properties(Examples.verification PROPERTIES
            FIXTURES_REQUIRED ExampleSigned
            PASS_REGULAR_EXPRESSION "Document intact: yes"
        )

        # The verification example finds the one signature, in the visible field
        set_tests_properties(Examples.visible-signature PROPERTIES FIXTURES_SETUP ExampleVisibleSigned)
        add_test(
            NAME Examples.visible-signature.verification
            COMMAND vanillapdf.example.verification "example-visible-signed.pdf"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        )
        set_tests_properties(Examples.visible-signature.verification PROPERTIES
            FIXTURES_REQUIRED ExampleVisibleSigned
            PASS_REGULAR_EXPRESSION "Document intact: yes.*Verified 1 signature"
        )
    endif()
endif()
