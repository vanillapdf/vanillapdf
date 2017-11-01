project(vanillapdf.filter LANGUAGES C)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_FILTER_SOURCES
	main.c
)

set(VANILLAPDF_FILTER_HEADERS
	filter.h
)

source_group("Header Files" FILES
	${VANILLAPDF_FILTER_HEADERS}
)

source_group("Source Files" FILES
	${VANILLAPDF_FILTER_SOURCES}
)

add_executable(vanillapdf.filter
	${VANILLAPDF_FILTER_HEADERS}
	${VANILLAPDF_FILTER_SOURCES}
)

if(WIN32)
	# Copy DLL dependencies on windows to our build directory
	add_custom_command(
		TARGET vanillapdf.filter POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		$<TARGET_FILE_DIR:vanillapdf>
		$<TARGET_FILE_DIR:vanillapdf.filter>
	)
endif(WIN32)

set(VANILLAPDF_FILTER_LIBS
	vanillapdf
)

if (CMAKE_COMPILER_IS_GNUCC)

	# C executables does not have access to c++ standard library by default
	# We need to explicitly pull dependencies
	set(VANILLAPDF_FILTER_LIBS ${VANILLAPDF_FILTER_LIBS} -lstdc++)

endif (CMAKE_COMPILER_IS_GNUCC)

target_link_libraries(vanillapdf.filter ${VANILLAPDF_FILTER_LIBS})
