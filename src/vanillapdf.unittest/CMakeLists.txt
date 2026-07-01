project(vanillapdf.unittest LANGUAGES CXX)

# Solution root folder
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include/files.cmake")

# Additional directories in the root project
include_directories("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include")

# Include local files in the directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# Generate test certificates header from PFX files
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake/generate_test_certificates.cmake")
set(GENERATED_CERTIFICATES_HEADER "${CMAKE_CURRENT_BINARY_DIR}/test_certificates.h")
generate_test_certificates_header(
    OUTPUT_FILE ${GENERATED_CERTIFICATES_HEADER}
    CERTIFICATES_DIR "${VANILLAPDF_SOLUTION_SOURCE_DIR}/test/certificates"
)

# Include directory for generated header
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(VANILLAPDF_UNITTEST_SOURCES
    main.cpp
    actions_test.cpp
    annotations_creation_test.cpp
    byte_range_test.cpp
    catalog_test.cpp
    character_map_test.cpp
    color_test.cpp
    date_test.cpp
    document_test.cpp
    destinations_test.cpp
    fields_test.cpp
    filter_test.cpp
    text_string_encoding_test.cpp
    file_structure_validator_test.cpp
    test_environment.cpp
    attributes_test.cpp
    objects_test.cpp
    page_tree_test.cpp
    utils_test.cpp
    digital_signature_test.cpp
    signature_verifier_test.cpp
    thread_safety_test.cpp
    name_dictionary_test.cpp
    io_strategy_test.cpp
    license_info_test.cpp
    content_stream_test.cpp
)

set(VANILLAPDF_UNITTEST_HEADERS
    unittest.h
    test_data.h
    handle_guard.h
    temp_file.h
)

# Resource files
source_group("Resource Files" FILES
    ${VANILLAPDF_UNITTEST_RESOURCE_FILES}
)

source_group("Header Files" FILES
    ${VANILLAPDF_UNITTEST_HEADERS}
)

source_group("Source Files" FILES
    ${VANILLAPDF_UNITTEST_SOURCES}
)

add_executable(vanillapdf.unittest
    ${VANILLAPDF_UNITTEST_HEADERS}
    ${VANILLAPDF_UNITTEST_SOURCES}
    ${VANILLAPDF_UNITTEST_RESOURCE_FILES}
)

# Configure default compilation parameters, strict warnings, config based definition for DEBUG/RELEASE, etc.
vanillapdf_target_compile_defaults(vanillapdf.unittest)

# Configure MSVC runtime library for unittest target
# Skip if already set externally (e.g. vcpkg toolchain, user, parent project)
if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    vanillapdf_configure_msvc_runtime(vanillapdf.unittest)
endif()

# Optional sanitizers
enable_asan_for_target(vanillapdf.unittest)
enable_ubsan_for_target(vanillapdf.unittest)
enable_tsan_for_target(vanillapdf.unittest)

if(WIN32)
    # Copy DLL dependencies on windows to our build directory
    add_custom_command(
        TARGET vanillapdf.unittest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_FILE_DIR:vanillapdf>"
        "$<TARGET_FILE_DIR:vanillapdf.unittest>"
    )
endif(WIN32)

if (VANILLAPDF_EXTERNAL_GTEST)
    find_package(GTest MODULE REQUIRED)
else()
    find_package(GTest CONFIG REQUIRED)
endif()

set(VANILLAPDF_UNITTEST_LIBS
    vanillapdf
    GTest::gtest
)

# GCC < 9 requires explicit -lstdc++fs for std::filesystem (e.g. Rocky 8)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    list(APPEND VANILLAPDF_UNITTEST_LIBS stdc++fs)
endif()

target_link_libraries(vanillapdf.unittest PRIVATE ${VANILLAPDF_UNITTEST_LIBS})

# In case we have the dynamic link configuration add specific definitions
if(BUILD_SHARED_LIBS)
    target_compile_definitions(vanillapdf.unittest PRIVATE VANILLAPDF_CONFIGURATION_DLL)
endif()

# Pass licensing flag to test code so it can skip license loading when disabled
if(VANILLAPDF_ENABLE_LICENSING)
    target_compile_definitions(vanillapdf.unittest PRIVATE VANILLAPDF_ENABLE_LICENSING)
endif()

# We do not need unit test project to be shipped
#install(TARGETS vanillapdf.unittest
#	RUNTIME DESTINATION bin
#)

# Specify version identificators
target_compile_definitions(vanillapdf.unittest PRIVATE -DVANILLAPDF_UNITTEST_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
target_compile_definitions(vanillapdf.unittest PRIVATE -DVANILLAPDF_UNITTEST_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
target_compile_definitions(vanillapdf.unittest PRIVATE -DVANILLAPDF_UNITTEST_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
target_compile_definitions(vanillapdf.unittest PRIVATE -DVANILLAPDF_UNITTEST_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

target_compile_definitions(vanillapdf.unittest PRIVATE -DVANILLAPDF_UNITTEST_ICON="${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake/vanilla_logo.ico")

# Use GoogleTest for unit test execution
include(GoogleTest)

# Update 01.05.2025
# On GitHub windows runners they are failing with discovering the cases
# CMake Error at C:/Program Files/CMake/share/cmake-3.31/Modules/GoogleTestAddTests.cmake:132 (message): Error running test executable.
# DISCOVERY_MODE PRE_TEST delays test discovery until just prior to test execution.
# This way test discovery occurs in the target environment where the test has a better chance at finding appropriate runtime dependencies.
#
# Update 22.11.2025
# NO_PRETTY_VALUES prevents appending "# GetParam() = <raw bytes>" to test names for parameterized tests.
# Without this flag, test names include raw memory addresses making output unreadable:
#   Before: CertificateTypes/.../40-byte object <B0-4E 79-8B F7-7F 00-00...>
#   After:  CertificateTypes/.../2kDSA_SHA256
# OpenSSL 3.x: set provider module search path (see vanillapdf/CMakeLists.txt)
set(VANILLAPDF_UNITTEST_EXTRA_PROPERTIES "")
if(WIN32 AND VANILLAPDF_INTERNAL_VCPKG AND NOT VANILLAPDF_EXTERNAL_OPENSSL
        AND VANILLAPDF_OPENSSL_MODULES_DIR)
    set(VANILLAPDF_UNITTEST_EXTRA_PROPERTIES PROPERTIES ENVIRONMENT "OPENSSL_MODULES=${VANILLAPDF_OPENSSL_MODULES_DIR}")
endif()

gtest_discover_tests(vanillapdf.unittest DISCOVERY_MODE PRE_TEST NO_PRETTY_VALUES
    ${VANILLAPDF_UNITTEST_EXTRA_PROPERTIES}
)
