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
# survive the argv boundary unchanged). /MANIFEST* are MSVC-linker options
# (cl and clang-cl); MinGW would need a .rc-based manifest instead.
if(MSVC)
    target_link_options(vanillapdf.test PRIVATE
        "/MANIFEST:EMBED"
        "/MANIFESTINPUT:${CMAKE_CURRENT_SOURCE_DIR}/vanillapdf.test.manifest")
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

if (CMAKE_COMPILER_IS_GNUCC)

    # C executables does not have access to c++ standard library by default
    # We need to explicitly pull dependencies
    set(VANILLAPDF_TEST_LIBS ${VANILLAPDF_TEST_LIBS} -lstdc++)

endif (CMAKE_COMPILER_IS_GNUCC)

target_link_libraries(vanillapdf.test PRIVATE ${VANILLAPDF_TEST_LIBS})

# In case we have the dynamic link configuration add specific definitions
if(BUILD_SHARED_LIBS)
    target_compile_definitions(vanillapdf.test PRIVATE VANILLAPDF_CONFIGURATION_DLL)
endif()

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
    )

    # OpenSSL 3.x: set provider module search path (see vanillapdf/CMakeLists.txt)
    if(WIN32 AND VANILLAPDF_INTERNAL_VCPKG AND NOT VANILLAPDF_EXTERNAL_OPENSSL
            AND VANILLAPDF_OPENSSL_MODULES_DIR)
        set_tests_properties(${TEST_FILENAME} PROPERTIES
            ENVIRONMENT "OPENSSL_MODULES=${VANILLAPDF_OPENSSL_MODULES_DIR}"
        )
    endif()
endforeach()

# MPK_SLOVLEX.pdf: 9.5 MB, 1,450 pages — takes 600-1500s under sanitizers and
# dominates CI time, so it is disabled.
set_tests_properties(MPK_SLOVLEX.pdf PROPERTIES DISABLED TRUE)
