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

# Optional stack sanitizer
enable_stack_sanitizer_for_target(vanillapdf.test)

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

# Copy vcpkg shared libraries to test executable directory to prevent version conflicts
if(BUILD_SHARED_LIBS AND APPLE)
    # macOS: Copy each vcpkg dylib file to the same directory as the test executable

    # Copy JPEG library if using vcpkg
    if(NOT VANILLAPDF_EXTERNAL_JPEG AND JPEG_FOUND)
        add_custom_command(TARGET vanillapdf.test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libjpeg.dylib"
                "$<TARGET_FILE_DIR:vanillapdf.test>/libjpeg.dylib"
            COMMENT "Copying vcpkg libjpeg for vanillapdf.test"
        )
    endif()

    # Copy OpenJPEG library if using vcpkg
    if(NOT VANILLAPDF_EXTERNAL_OPENJPEG AND OPENJPEG_LIBRARIES)
        add_custom_command(TARGET vanillapdf.test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libopenjp2.dylib"
                "$<TARGET_FILE_DIR:vanillapdf.test>/libopenjp2.dylib"
            COMMENT "Copying vcpkg openjpeg for vanillapdf.test"
        )
    endif()

    # Copy zlib library if using vcpkg
    if(NOT VANILLAPDF_EXTERNAL_ZLIB AND ZLIB_FOUND)
        add_custom_command(TARGET vanillapdf.test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libz.dylib"
                "$<TARGET_FILE_DIR:vanillapdf.test>/libz.dylib"
            COMMENT "Copying vcpkg zlib for vanillapdf.test"
        )
    endif()

    # Copy OpenSSL library if using vcpkg
    if(NOT VANILLAPDF_EXTERNAL_OPENSSL AND OPENSSL_FOUND)
        add_custom_command(TARGET vanillapdf.test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libcrypto.dylib"
                "$<TARGET_FILE_DIR:vanillapdf.test>/libcrypto.dylib"
            COMMENT "Copying vcpkg openssl for vanillapdf.test"
        )
    endif()
endif()

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

# Case insensitive
file(GLOB_RECURSE TEST_FILES "${VANILLAPDF_SOLUTION_SOURCE_DIR}/test/*.[Pp][Dd][Ff]")

foreach(TEST_FILE ${TEST_FILES})
    # Get filename from full path
    get_filename_component(TEST_FILENAME ${TEST_FILE} NAME)

    # Add every test file as separate entry
    add_test(
        NAME ${TEST_FILENAME}							# test name same with file name
        COMMAND ${Python3_EXECUTABLE}					# reference to python executable
        "${VANILLAPDF_SOLUTION_SOURCE_DIR}/scripts/run_test.py"		# path to run_test python script
        "$<TARGET_FILE:vanillapdf.test>"				# destination of the test executable
        "${TEST_FILE}"									# destination of the source file
        "${VANILLAPDF_SOLUTION_SOURCE_DIR}/scripts/vanilla_test.cfg"	# path to additional configuration file
        "${VANILLAPDF_SOLUTION_SOURCE_DIR}/scripts/VanillaPDF.lic"	# path to license file for test purposes
        "${VANILLAPDF_SOLUTION_SOURCE_DIR}"							# path to root of the source directory
    )
endforeach(TEST_FILE)
