project(vanillapdf.fuzzer LANGUAGES CXX)

# Solution root folder
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include/files.cmake")

# Additional directories in the root project
include_directories("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include")

# Include local files in the directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_FUZZER_HEADERS
    fuzzer.h
    handle_guard.h
)

source_group("Header Files" FILES
    ${VANILLAPDF_FUZZER_HEADERS}
)

# Helper function to add a fuzz target with standard configuration
function(add_fuzz_target TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE} ${VANILLAPDF_FUZZER_HEADERS})

    # Configure default compilation parameters
    vanillapdf_target_compile_defaults(${TARGET_NAME})

    # Configure MSVC runtime library
    # Skip if already set externally (e.g. vcpkg toolchain, user, parent project)
    if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
        vanillapdf_configure_msvc_runtime(${TARGET_NAME})
    endif()

    # Apply fuzzer instrumentation and ASan to the executable
    enable_fuzzer_for_target(${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} PRIVATE vanillapdf)

    # In case we have the dynamic link configuration add specific definitions
    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${TARGET_NAME} PRIVATE VANILLAPDF_CONFIGURATION_DLL)
    endif()
endfunction()

# Always-available fuzz targets
add_fuzz_target(vanillapdf.fuzz.flate_decode      fuzz_flate_decode.cpp)
add_fuzz_target(vanillapdf.fuzz.flate_predictor    fuzz_flate_predictor.cpp)
add_fuzz_target(vanillapdf.fuzz.ascii85_decode     fuzz_ascii85_decode.cpp)
add_fuzz_target(vanillapdf.fuzz.asciihex_decode    fuzz_asciihex_decode.cpp)
add_fuzz_target(vanillapdf.fuzz.file_parse         fuzz_file_parse.cpp)
add_fuzz_target(vanillapdf.fuzz.content_stream     fuzz_content_stream.cpp)

# Conditional fuzz targets based on feature availability
if(VANILLAPDF_ENABLE_JPEG)
    add_fuzz_target(vanillapdf.fuzz.dct_decode     fuzz_dct_decode.cpp)
endif()

if(VANILLAPDF_ENABLE_JPEG2000)
    add_fuzz_target(vanillapdf.fuzz.jpx_decode     fuzz_jpx_decode.cpp)
endif()
