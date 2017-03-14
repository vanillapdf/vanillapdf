# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

project(gotchangpdf.test)

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

add_custom_command(
	TARGET gotchangpdf.test POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:gotchangpdf>
	$<TARGET_FILE_DIR:gotchangpdf.test>
)

add_custom_command(
	TARGET gotchangpdf.test POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:zlib>
	$<TARGET_FILE_DIR:gotchangpdf.test>
)