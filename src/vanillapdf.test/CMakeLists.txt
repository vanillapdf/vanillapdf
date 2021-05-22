project(vanillapdf.test LANGUAGES C)

include_directories("${CMAKE_SOURCE_DIR}/include")
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

if(WIN32)
	# Copy DLL dependencies on windows to our build directory
	add_custom_command(
		TARGET vanillapdf.test POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		$<TARGET_FILE_DIR:vanillapdf>
		$<TARGET_FILE_DIR:vanillapdf.test>
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

target_link_libraries(vanillapdf.test ${VANILLAPDF_TEST_LIBS})

# Project install targets
install(TARGETS vanillapdf.test
	RUNTIME DESTINATION bin
)

# Specify version identificators
add_definitions(-DVANILLAPDF_TEST_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
add_definitions(-DVANILLAPDF_TEST_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
add_definitions(-DVANILLAPDF_TEST_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
add_definitions(-DVANILLAPDF_TEST_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

add_definitions(-DVANILLAPDF_TEST_ICON="${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico")

# Run tests
include(FindPythonInterp)

# Case insensitive
file(GLOB_RECURSE TEST_FILES "${CMAKE_SOURCE_DIR}/test/*.[Pp][Dd][Ff]")

foreach(TEST_FILE ${TEST_FILES})
	# Get filename from full path
	get_filename_component(TEST_FILENAME ${TEST_FILE} NAME)

	# Add every test file as separate entry
	add_test(
		NAME ${TEST_FILENAME}							# test name same with file name
		COMMAND ${PYTHON_EXECUTABLE}					# reference to python executable
		"${CMAKE_SOURCE_DIR}/scripts/run_test.py"		# path to run_test python script
		"$<TARGET_FILE:vanillapdf.test>"				# destination of the test executable
		"${TEST_FILE}"									# destination of the source file
		"${CMAKE_SOURCE_DIR}/scripts/vanilla_test.cfg"	# path to additional configuration file
		"${CMAKE_SOURCE_DIR}/scripts/VanillaPDF.lic"	# path to license file for test purposes
        "${CMAKE_SOURCE_DIR}"							# path to root of the source directory
	)
endforeach(TEST_FILE)
