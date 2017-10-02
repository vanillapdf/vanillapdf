project(vanillapdf.merge LANGUAGES C)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_MERGE_SOURCES
	main.c
)

set(VANILLAPDF_MERGE_HEADERS
	merge.h
)

source_group("Header Files" FILES
	${VANILLAPDF_MERGE_HEADERS}
)

source_group("Source Files" FILES
	${VANILLAPDF_MERGE_SOURCES}
)

add_executable(vanillapdf.merge
	${VANILLAPDF_MERGE_HEADERS}
	${VANILLAPDF_MERGE_SOURCES}
)

if(WIN32)
	# Copy DLL dependencies on windows to our build directory
	add_custom_command(
		TARGET vanillapdf.merge POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		$<TARGET_FILE_DIR:vanillapdf>
		$<TARGET_FILE_DIR:vanillapdf.merge>
	)
endif(WIN32)

set(VANILLAPDF_MERGE_LIBS
	vanillapdf
)

if (CMAKE_COMPILER_IS_GNUCC)

	# C executables does not have access to c++ standard library by default
	# We need to explicitly pull dependencies
	set(VANILLAPDF_MERGE_LIBS ${VANILLAPDF_MERGE_LIBS} -lstdc++)

endif (CMAKE_COMPILER_IS_GNUCC)

target_link_libraries(vanillapdf.merge ${VANILLAPDF_MERGE_LIBS})
