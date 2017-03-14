# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

project(gotchangpdf.merge)

include_directories("${CMAKE_SOURCE_DIR}/include")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(GOTCHANGPDF_MERGE_SOURCES
	main.c
)

set(GOTCHANGPDF_MERGE_HEADERS
	merge.h
)

source_group("Header Files" FILES
	${GOTCHANGPDF_MERGE_HEADERS}
)

source_group("Source Files" FILES
	${GOTCHANGPDF_MERGE_SOURCES}
)

add_executable(gotchangpdf.merge
	${GOTCHANGPDF_MERGE_HEADERS}
	${GOTCHANGPDF_MERGE_SOURCES}
)

set(GOTCHANGPDF_MERGE_LIBS
	gotchangpdf
)

if (CMAKE_COMPILER_IS_GNUCC)
	# C executables does not have access to c++ standard library by default
	# We need to explicitly pull dependencies
	set(GOTCHANGPDF_MERGE_LIBS ${GOTCHANGPDF_MERGE_LIBS} -lstdc++)
endif (CMAKE_COMPILER_IS_GNUCC)

add_definitions("-DGOTCHANG_PDF_MERGE_VERSION_MAJOR=${GOTCHANG_PDF_VERSION_MAJOR}")
add_definitions("-DGOTCHANG_PDF_MERGE_VERSION_MINOR=${GOTCHANG_PDF_VERSION_MINOR}")
add_definitions("-DGOTCHANG_PDF_MERGE_VERSION_PATCH=${GOTCHANG_PDF_VERSION_PATCH}")
add_definitions("-DGOTCHANG_PDF_MERGE_VERSION_BUILD=${GOTCHANG_PDF_VERSION_BUILD}")

target_link_libraries(gotchangpdf.merge ${GOTCHANGPDF_MERGE_LIBS})

add_custom_command(
	TARGET gotchangpdf.merge POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:gotchangpdf>
	$<TARGET_FILE_DIR:gotchangpdf.merge>
)

add_custom_command(
	TARGET gotchangpdf.merge POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:zlib>
	$<TARGET_FILE_DIR:gotchangpdf.test>
)