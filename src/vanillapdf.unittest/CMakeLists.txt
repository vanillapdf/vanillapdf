project(vanillapdf.unittest LANGUAGES CXX)

# Solution root folder
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include/files.cmake")

# Additional directories in the root project
include_directories("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include")

# Include local files in the directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_UNITTEST_SOURCES
    main.cpp
    document_test.cpp
    destinations_test.cpp
    filter_test.cpp
    test_environment.cpp
    attributes_test.cpp
    objects_test.cpp
    utils_test.cpp
    signature_verifier_test.cpp
)

set(VANILLAPDF_UNITTEST_HEADERS
    unittest.h
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
vanillapdf_configure_msvc_runtime(vanillapdf.unittest)

# Optional stack sanitizer
enable_stack_sanitizer_for_target(vanillapdf.unittest)

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

target_link_libraries(vanillapdf.unittest PRIVATE ${VANILLAPDF_UNITTEST_LIBS})

# In case we have the dynamic link configuration add specific definitions
if(BUILD_SHARED_LIBS)
    target_compile_definitions(vanillapdf.unittest PRIVATE VANILLAPDF_CONFIGURATION_DLL)
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
gtest_discover_tests(vanillapdf.unittest DISCOVERY_MODE PRE_TEST)
