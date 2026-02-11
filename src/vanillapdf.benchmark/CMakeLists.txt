project(vanillapdf.benchmark LANGUAGES CXX)

# Solution root folder
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include/files.cmake")

# Additional directories in the root project
include_directories("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include")

# Include local files in the directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_BENCHMARK_SOURCES
    buffer_hash.cpp
    character_map.cpp
    filters.cpp
    objects.cpp
    main.cpp
)

set(VANILLAPDF_BENCHMARK_HEADERS
    benchmark.h
    handle_guard.h
)

# Resource files
source_group("Resource Files" FILES
    ${VANILLAPDF_BENCHMARK_RESOURCE_FILES}
)

source_group("Header Files" FILES
    ${VANILLAPDF_BENCHMARK_HEADERS}
)

source_group("Source Files" FILES
    ${VANILLAPDF_BENCHMARK_SOURCES}
)

add_executable(vanillapdf.benchmark
    ${VANILLAPDF_BENCHMARK_HEADERS}
    ${VANILLAPDF_BENCHMARK_SOURCES}
    ${VANILLAPDF_BENCHMARK_RESOURCE_FILES}
)

# Configure default compilation parameters, strict warnings, config based definition for DEBUG/RELEASE, etc.
vanillapdf_target_compile_defaults(vanillapdf.benchmark)

# Configure MSVC runtime library for benchmark target
# Skip if already set externally (e.g. vcpkg toolchain, user, parent project)
if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    vanillapdf_configure_msvc_runtime(vanillapdf.benchmark)
endif()

# Optional stack sanitizer
enable_stack_sanitizer_for_target(vanillapdf.benchmark)

if(WIN32)
    # Copy DLL dependencies on windows to our build directory
    add_custom_command(
        TARGET vanillapdf.benchmark POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_FILE_DIR:vanillapdf>"
        "$<TARGET_FILE_DIR:vanillapdf.benchmark>"
    )
endif(WIN32)

# Find google benchmarking library
if (VANILLAPDF_EXTERNAL_BENCHMARK)
    find_package(benchmark CONFIG REQUIRED)
else()
    find_package(benchmark CONFIG REQUIRED)
endif()

set(VANILLAPDF_BENCHMARK_LIBS
    vanillapdf
    benchmark::benchmark
)

target_link_libraries(vanillapdf.benchmark PRIVATE ${VANILLAPDF_BENCHMARK_LIBS})

# In case we have the dynamic link configuration add specific definitions
if(BUILD_SHARED_LIBS)
    target_compile_definitions(vanillapdf.benchmark PRIVATE VANILLAPDF_CONFIGURATION_DLL)
endif()

# We do not need unit test project to be shipped
#install(TARGETS vanillapdf.benchmark
#	RUNTIME DESTINATION bin
#)

# Specify version identificators
target_compile_definitions(vanillapdf.benchmark PRIVATE -DVANILLAPDF_BENCHMARK_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
target_compile_definitions(vanillapdf.benchmark PRIVATE -DVANILLAPDF_BENCHMARK_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
target_compile_definitions(vanillapdf.benchmark PRIVATE -DVANILLAPDF_BENCHMARK_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
target_compile_definitions(vanillapdf.benchmark PRIVATE -DVANILLAPDF_BENCHMARK_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

target_compile_definitions(vanillapdf.benchmark PRIVATE -DVANILLAPDF_BENCHMARK_ICON="${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake/vanilla_logo.ico")
