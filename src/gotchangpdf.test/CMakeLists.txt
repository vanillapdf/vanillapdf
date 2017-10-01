project(gotchangpdf.test LANGUAGES C)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(GOTCHANGPDF_TEST_SOURCES
	documents.c
	files.c
	objects.c
	main.c
	utils.c
)

set(GOTCHANGPDF_TEST_HEADERS
	test.h
)

source_group("Header Files" FILES
	${GOTCHANGPDF_TEST_HEADERS}
)

source_group("Source Files" FILES
	${GOTCHANGPDF_TEST_SOURCES}
)

add_executable(gotchangpdf.test
	${GOTCHANGPDF_TEST_HEADERS}
	${GOTCHANGPDF_TEST_SOURCES}
)

if(WIN32)
	# Copy DLL dependencies on windows to our build directory
	add_custom_command(
		TARGET gotchangpdf.test POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		$<TARGET_FILE_DIR:gotchangpdf>
		$<TARGET_FILE_DIR:gotchangpdf.test>
	)
endif(WIN32)

set(GOTCHANGPDF_TEST_LIBS
	gotchangpdf
)

if (CMAKE_COMPILER_IS_GNUCC)

	# C executables does not have access to c++ standard library by default
	# We need to explicitly pull dependencies
	set(GOTCHANGPDF_TEST_LIBS ${GOTCHANGPDF_TEST_LIBS} -lstdc++)

endif (CMAKE_COMPILER_IS_GNUCC)

add_definitions("-DGOTCHANG_PDF_TEST_VERSION_MAJOR=${GOTCHANG_PDF_VERSION_MAJOR}")
add_definitions("-DGOTCHANG_PDF_TEST_VERSION_MINOR=${GOTCHANG_PDF_VERSION_MINOR}")
add_definitions("-DGOTCHANG_PDF_TEST_VERSION_PATCH=${GOTCHANG_PDF_VERSION_PATCH}")
add_definitions("-DGOTCHANG_PDF_TEST_VERSION_BUILD=${GOTCHANG_PDF_VERSION_BUILD}")

target_link_libraries(gotchangpdf.test ${GOTCHANGPDF_TEST_LIBS})

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
		"$<TARGET_FILE:gotchangpdf.test>"				# destination of the test executable
		"${TEST_FILE}"									# destination of the source file
		"${CMAKE_SOURCE_DIR}/scripts/encryption.cfg"	# path to configuration file for encrypted files
		"${CMAKE_SOURCE_DIR}/scripts/VanillaPDF.lic"	# path to license file for test purposes
	)
endforeach(TEST_FILE)
