project(vanillapdf.tools LANGUAGES C)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_TOOLS_SOURCES
	main.c
	extract.c
	extract_object.c
	filter.c
	merge.c
	sign.c
	sign_custom.c
	decrypt.c
	write_custom.c
)

set(VANILLAPDF_TOOLS_HEADERS
	tools.h
)

set(VANILLAPDF_TOOLS_RESOURCE_FILES
	resources.rc
)

# Resource files
source_group("Resource Files" FILES
	${VANILLAPDF_TOOLS_RESOURCE_FILES}
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
	${VANILLAPDF_TOOLS_RESOURCE_FILES}
)

if(WIN32)

	# Copy DLL dependencies on windows to our build directory
	add_custom_command(
		TARGET vanillapdf.tools POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"$<TARGET_FILE_DIR:vanillapdf>"
		"$<TARGET_FILE_DIR:vanillapdf.tools>"
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

# Project install targets
install(TARGETS vanillapdf.tools
	RUNTIME DESTINATION bin
)

# Specify version identificators
add_definitions(-DVANILLAPDF_TOOLS_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
add_definitions(-DVANILLAPDF_TOOLS_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
add_definitions(-DVANILLAPDF_TOOLS_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
add_definitions(-DVANILLAPDF_TOOLS_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

add_definitions(-DVANILLAPDF_TOOLS_ICON="${CMAKE_SOURCE_DIR}/cmake/vanilla_logo.ico")
