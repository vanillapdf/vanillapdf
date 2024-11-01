project(vanillapdf.benchmark LANGUAGES CXX)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_BENCHMARK_SOURCES
	objects.cpp
	main.cpp
)

set(VANILLAPDF_BENCHMARK_HEADERS
	benchmark.h
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
find_package(benchmark REQUIRED)

set(VANILLAPDF_BENCHMARK_LIBS
	vanillapdf
	benchmark::benchmark
)

target_link_libraries(vanillapdf.benchmark ${VANILLAPDF_BENCHMARK_LIBS})

# We do not need unit test project to be shipped
#install(TARGETS vanillapdf.benchmark
#	RUNTIME DESTINATION bin
#)

# Specify version identificators
add_definitions(-DVANILLAPDF_BENCHMARK_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
add_definitions(-DVANILLAPDF_BENCHMARK_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
add_definitions(-DVANILLAPDF_BENCHMARK_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
add_definitions(-DVANILLAPDF_BENCHMARK_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

add_definitions(-DVANILLAPDF_BENCHMARK_ICON="${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico")
