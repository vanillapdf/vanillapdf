project(vanillapdf.tools LANGUAGES C)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_TOOLS_SOURCES
	main.c
)

set(VANILLAPDF_TOOLS_HEADERS
	tools.h
)

source_group("Header Files" FILES
	${VANILLAPDF_TOOLS_HEADERS}
)

source_group("Source Files" FILES
	${VANILLAPDF_TOOLS_SOURCES}
)

add_executable(vanillapdf.tools
	${VANILLAPDF_TOOLS_HEADERS}
	${VANILLAPDF_TOOLS_SOURCES}
)

if(WIN32)
	# Copy DLL dependencies on windows to our build directory
	add_custom_command(
		TARGET vanillapdf.tools POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		$<TARGET_FILE_DIR:vanillapdf>
		$<TARGET_FILE_DIR:vanillapdf.tools>
	)
endif(WIN32)

set(VANILLAPDF_TOOLS_LIBS
	vanillapdf
)

if (CMAKE_COMPILER_IS_GNUCC)

	# C executables does not have access to c++ standard library by default
	# We need to explicitly pull dependencies
	set(VANILLAPDF_TOOLS_LIBS ${VANILLAPDF_TOOLS_LIBS} -lstdc++)

endif (CMAKE_COMPILER_IS_GNUCC)

target_link_libraries(vanillapdf.tools ${VANILLAPDF_TOOLS_LIBS})
