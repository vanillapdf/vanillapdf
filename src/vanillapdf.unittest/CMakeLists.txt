project(vanillapdf.unittest LANGUAGES CXX)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_UNITTEST_SOURCES
	main.cpp
	document_test.cpp
	filter_test.cpp
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

if(WIN32)
	# Copy DLL dependencies on windows to our build directory
	add_custom_command(
		TARGET vanillapdf.unittest POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"$<TARGET_FILE_DIR:vanillapdf>"
		"$<TARGET_FILE_DIR:vanillapdf.unittest>"
	)
endif(WIN32)

set(VANILLAPDF_UNITTEST_LIBS
	vanillapdf
	gtest
	gtest_main
)

target_link_libraries(vanillapdf.unittest ${VANILLAPDF_UNITTEST_LIBS})

# We do not need unit test project to be shipped
#install(TARGETS vanillapdf.unittest
#	RUNTIME DESTINATION bin
#)

# Specify version identificators
add_definitions(-DVANILLAPDF_UNITTEST_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
add_definitions(-DVANILLAPDF_UNITTEST_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
add_definitions(-DVANILLAPDF_UNITTEST_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
add_definitions(-DVANILLAPDF_UNITTEST_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

add_definitions(-DVANILLAPDF_UNITTEST_ICON="${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico")

# Build GoogleTest from the external repository
add_subdirectory("${CMAKE_SOURCE_DIR}/external/googletest" build EXCLUDE_FROM_ALL)

# Use GoogleTest for unit test execution
include(GoogleTest)

gtest_discover_tests(vanillapdf.unittest)
