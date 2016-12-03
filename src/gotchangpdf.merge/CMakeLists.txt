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

target_link_libraries(gotchangpdf.merge gotchangpdf)