project(vanillapdf LANGUAGES CXX)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/lib/nlohmann_json/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

include("${CMAKE_SOURCE_DIR}/include/files.cmake")

set(VANILLAPDF_PRECOMPILED_SOURCE
	precompiled.cpp
)

set(VANILLAPDF_C_IMPLEMENTATION_SOURCES
	implementation/c_values.cpp
)

set(VANILLAPDF_C_IMPLEMENTATION_UTILS_SOURCES
	implementation/utils/c_buffer.cpp
	implementation/utils/c_buffer_array.cpp
	implementation/utils/c_encryption.cpp
	implementation/utils/c_errors.cpp
	implementation/utils/c_input_stream.cpp
	implementation/utils/c_input_output_stream.cpp
	implementation/utils/c_library_info.cpp
	implementation/utils/c_license_info.cpp
	implementation/utils/c_logging.cpp
	implementation/utils/c_output_stream.cpp
	implementation/utils/c_pkcs12_key.cpp
	implementation/utils/c_signing_key.cpp
	implementation/utils/c_unknown_interface.cpp
)

set(VANILLAPDF_C_IMPLEMENTATION_SYNTAX_SOURCES
	implementation/syntax/c_array_object.cpp
	implementation/syntax/c_boolean_object.cpp
	implementation/syntax/c_dictionary_object.cpp
	implementation/syntax/c_file.cpp
	implementation/syntax/c_file_writer.cpp
	implementation/syntax/c_file_writer_observer.cpp
	implementation/syntax/c_filter.cpp
	implementation/syntax/c_indirect_reference_object.cpp
	implementation/syntax/c_integer_object.cpp
	implementation/syntax/c_name_object.cpp
	implementation/syntax/c_null_object.cpp
	implementation/syntax/c_object.cpp
	implementation/syntax/c_real_object.cpp
	implementation/syntax/c_stream_object.cpp
	implementation/syntax/c_string_object.cpp
	implementation/syntax/c_xref.cpp
	implementation/syntax/c_object_attributes.cpp
	implementation/syntax/c_object_attribute_list.cpp
)

set(VANILLAPDF_C_IMPLEMENTATION_SEMANTICS_SOURCES
	implementation/semantics/c_annotations.cpp
	implementation/semantics/c_catalog.cpp
	implementation/semantics/c_character_map.cpp
	implementation/semantics/c_date.cpp
	implementation/semantics/c_destinations.cpp
	implementation/semantics/c_developer_extensions.cpp
	implementation/semantics/c_digital_signature.cpp
	implementation/semantics/c_document.cpp
	implementation/semantics/c_document_encryption_settings.cpp
	implementation/semantics/c_document_info.cpp
	implementation/semantics/c_document_signature_settings.cpp
	implementation/semantics/c_fields.cpp
	implementation/semantics/c_font.cpp
	implementation/semantics/c_font_map.cpp
	implementation/semantics/c_interactive_forms.cpp
	implementation/semantics/c_outline.cpp
	implementation/semantics/c_page_contents.cpp
	implementation/semantics/c_page_labels.cpp
	implementation/semantics/c_page_object.cpp
	implementation/semantics/c_page_tree.cpp
	implementation/semantics/c_rectangle.cpp
	implementation/semantics/c_resource_dictionary.cpp
	implementation/semantics/c_viewer_preferences.cpp
)

set(VANILLAPDF_C_IMPLEMENTATION_CONTENTS_SOURCES
	implementation/contents/c_base_font_range.cpp
	implementation/contents/c_content_instruction.cpp
	implementation/contents/c_content_object.cpp
	implementation/contents/c_content_operation.cpp
	implementation/contents/c_content_operator.cpp
	implementation/contents/c_content_parser.cpp
)

set(VANILLAPDF_CONTENTS_SOURCES
	contents/character_map_data.cpp
	contents/character_map_parser.cpp
	contents/content_stream_objects.cpp
	contents/content_stream_operations.cpp
	contents/content_stream_parser.cpp
	contents/content_stream_operation_generic.cpp
)

set(VANILLAPDF_SEMANTICS_SOURCES
	semantics/objects/catalog.cpp
	semantics/objects/content_stream.cpp
	semantics/objects/document.cpp
	semantics/objects/page_contents.cpp
	semantics/objects/page_object.cpp
	semantics/objects/page_tree.cpp
	semantics/objects/page_tree_node.cpp
	semantics/objects/resource_dictionary.cpp
	
	semantics/objects/annotations.cpp
	semantics/objects/character_map.cpp
	semantics/objects/date.cpp
	semantics/objects/destinations.cpp
	semantics/objects/developer_extensions.cpp
	semantics/objects/digital_signature.cpp
	semantics/objects/document_info.cpp
	semantics/objects/font.cpp
	semantics/objects/outline.cpp
	semantics/objects/page_labels.cpp
	semantics/objects/rectangle.cpp
	semantics/objects/tree.cpp
	semantics/objects/name_dictionary.cpp
	semantics/objects/fields.cpp
	semantics/objects/interactive_forms.cpp
	semantics/objects/viewer_preferences.cpp
	semantics/objects/page_node_base.cpp
	
	semantics/utils/document_encryption_settings.cpp
	
	semantics/utils/document_signer.cpp
	semantics/utils/document_signature_settings.cpp
	semantics/utils/semantic_utils.cpp
)

set(VANILLAPDF_SYNTAX_SOURCES
	syntax/exceptions/syntax_exceptions.cpp

	syntax/files/file.cpp
	syntax/files/file_writer.cpp
	syntax/files/xref.cpp
	syntax/files/xref_entry.cpp
	syntax/files/xref_chain.cpp

	syntax/filters/ascii_85_decode_filter.cpp
	syntax/filters/ascii_hex_decode_filter.cpp
	syntax/filters/dct_decode_filter.cpp
	syntax/filters/filter.cpp
	syntax/filters/flate_decode_filter.cpp
	syntax/filters/lzw_decode_filter.cpp
	syntax/filters/jpx_decode_filter.cpp

	syntax/objects/boolean_object.cpp
	syntax/objects/dictionary_object.cpp
	syntax/objects/indirect_reference_object.cpp
	syntax/objects/integer_object.cpp
	syntax/objects/mixed_array_object.cpp
	syntax/objects/name_object.cpp
	syntax/objects/null_object.cpp
	syntax/objects/numeric_object.cpp
	syntax/objects/object.cpp
	syntax/objects/real_object.cpp
	syntax/objects/stream_object.cpp
	syntax/objects/string_object.cpp

	syntax/parsers/lexical_tree.cpp
	syntax/parsers/parser.cpp
	syntax/parsers/parser_utils.cpp
	syntax/parsers/reverse_parser.cpp
	syntax/parsers/reverse_tokenizer.cpp
	syntax/parsers/token.cpp
	syntax/parsers/token_dictionary.cpp
	syntax/parsers/tokenizer.cpp

	syntax/utils/encryption.cpp
	syntax/utils/name_constants.cpp
	syntax/utils/base_object_attribute.cpp
	syntax/utils/object_attribute_list.cpp
)

set(VANILLAPDF_UTILS_SOURCES
	utils/buffer.cpp
	utils/buffer_array.cpp
	utils/constants.cpp
	utils/errors.cpp
	utils/exceptions.cpp
	utils/library_info.cpp
	utils/license_info.cpp
	utils/log.cpp
	utils/misc_utils.cpp
	utils/modify_observer_interface.cpp
	utils/pkcs12_key.cpp
	utils/resource.cpp
	utils/time_utils.cpp
	utils/unknown_interface.cpp
	utils/windows_utils.cpp
	utils/zlib_wrapper.cpp

	utils/streams/input_reverse_stream.cpp
	utils/streams/input_stream.cpp
	utils/streams/output_stream.cpp
	utils/streams/stream_utils.cpp
)

set(VANILLAPDF_CONTENTS_HEADERS
	contents/content_fwd.h
	contents/character_map_data.h
	contents/character_map_parser.h
	contents/content_stream_instruction_base.h
	contents/content_stream_objects.h
	contents/content_stream_operation_base.h
	contents/content_stream_operation_generic.h
	contents/content_stream_operations.h
	contents/content_stream_operator_base.h
	contents/content_stream_operators.h
	contents/content_stream_parser.h
	contents/content_utils.h
)

set(VANILLAPDF_SEMANTICS_HEADERS
	semantics/objects/catalog.h
	semantics/objects/content_stream.h
	semantics/objects/document.h
	semantics/objects/page_contents.h
	semantics/objects/page_node_base.h
	semantics/objects/page_object.h
	semantics/objects/page_tree.h
	semantics/objects/page_tree_node.h
	semantics/objects/resource_dictionary.h
	semantics/objects/signature_flags.h

	semantics/objects/annotations.h
	semantics/objects/character_map.h
	semantics/objects/date.h
	semantics/objects/destinations.h
	semantics/objects/developer_extensions.h
	semantics/objects/digital_signature.h
	semantics/objects/document_info.h
	semantics/objects/fields.h
	semantics/objects/font.h
	semantics/objects/interactive_forms.h
	semantics/objects/high_level_object.h
	semantics/objects/name_dictionary.h
	semantics/objects/outline.h
	semantics/objects/page_labels.h
	semantics/objects/rectangle.h
	semantics/objects/tree.h
	semantics/objects/viewer_preferences.h

	semantics/utils/document_encryption_settings.h

	semantics/utils/document_signer.h
	semantics/utils/document_signature_settings.h
	semantics/utils/semantic_exceptions.h
	semantics/utils/semantic_utils.h
	semantics/utils/semantics_fwd.h
)

set(VANILLAPDF_SYNTAX_HEADERS
	syntax/exceptions/syntax_exceptions.h
	
	syntax/files/file.h
	syntax/files/file_writer.h
	syntax/files/header.h
	syntax/files/xref.h
	syntax/files/xref_entry.h
	syntax/files/xref_chain.h

	syntax/filters/ascii_85_decode_filter.h
	syntax/filters/ascii_hex_decode_filter.h
	syntax/filters/dct_decode_filter.h
	syntax/filters/filter.h
	syntax/filters/flate_decode_filter.h
	syntax/filters/lzw_decode_filter.h
	syntax/filters/jpx_decode_filter.h

	syntax/objects/array_object.h
	syntax/objects/mixed_array_object.h
	syntax/objects/boolean_object.h
	syntax/objects/containable.h
	syntax/objects/dictionary_object.h
	syntax/objects/indirect_reference_object.h
	syntax/objects/integer_object.h
	syntax/objects/name_object.h
	syntax/objects/null_object.h
	syntax/objects/numeric_object.h
	syntax/objects/object.h
	syntax/objects/objects.h
	syntax/objects/real_object.h
	syntax/objects/stream_object.h
	syntax/objects/string_object.h

	syntax/parsers/lexical_tree.h
	syntax/parsers/object_stream_entry.h
	syntax/parsers/parser.h
	syntax/parsers/parser_interface.h
	syntax/parsers/parser_utils.h
	syntax/parsers/reverse_parser.h
	syntax/parsers/reverse_tokenizer.h
	syntax/parsers/token.h
	syntax/parsers/token_dictionary.h
	syntax/parsers/tokenizer.h

	syntax/utils/encryption.h
	syntax/utils/name_constants.h
	syntax/utils/output_pointer.h
	syntax/utils/syntax_fwd.h
	syntax/utils/object_utils.h

	syntax/utils/base_object_attribute.h
	syntax/utils/object_attribute_list.h
	syntax/utils/empty_object_attribute.h
	syntax/utils/serialization_override_object_attribute.h
	syntax/utils/tracking_identifier_object_attribute.h
	syntax/utils/image_metadata_object_attribute.h
	syntax/utils/object_stream_metadata_attribute.h
)

set(VANILLAPDF_UTILS_HEADERS
	utils/base_iterator.h
	utils/buffer.h
	utils/buffer_array.h
	utils/character.h
	utils/constants.h
	utils/conversion_utils.h
	utils/custom_size_allocator.h
	utils/custom_size_vector.h
	utils/deferred.h
	utils/errors.h
	utils/exceptions.h
	utils/library_info.h
	utils/license_info.h
	utils/log.h
	utils/math_utils.h
	utils/message_digest_algorithm.h
	utils/modify_observer_interface.h
	utils/observable_interface.h
	utils/misc_utils.h
	utils/pdf_version.h
	utils/pkcs12_key.h
	utils/resource.h
	utils/stack_allocator.h
	utils/template_utils.h
	utils/time_utils.h
	utils/unknown_interface.h
	utils/util.h
	utils/utils_fwd.h
	utils/windows_utils.h
	utils/zlib_wrapper.h

	utils/streams/input_stream_interface.h
	utils/streams/input_reverse_stream.h
	utils/streams/input_stream.h

	utils/streams/output_stream_interface.h
	utils/streams/output_stream.h

	utils/streams/input_output_stream_interface.h
	utils/streams/input_output_stream.h

	utils/streams/stream_utils.h
	utils/streams/seek_direction.h
)

set(VANILLAPDF_IMPLEMENTATION_HEADER
	implementation/helper.h
)

set(VANILLAPDF_ADDITIONAL_HEADERS
	unistd.h
)

set(VANILLAPDF_PRECOMPILED_HEADER
	precompiled.h
)

set(VANILLAPDF_RESOURCE_FILES
	public.natvis
	vanillapdf.natvis
	resources.rc
)

# Resource files
source_group("Resource Files" FILES
	${VANILLAPDF_RESOURCE_FILES}
)

# Header files
source_group("Header Files" FILES
	${VANILLAPDF_PRECOMPILED_HEADER}
	${VANILLAPDF_ADDITIONAL_HEADERS}
)

source_group("Header Files\\include" FILES
	${VANILLAPDF_INCLUDE_HEADERS}
)

source_group("Header Files\\include\\utils" FILES
	${VANILLAPDF_INCLUDE_UTILS_HEADERS}
)

source_group("Header Files\\include\\syntax" FILES
	${VANILLAPDF_INCLUDE_SYNTAX_HEADERS}
)

source_group("Header Files\\include\\semantics" FILES
	${VANILLAPDF_INCLUDE_SEMANTICS_HEADERS}
)

source_group("Header Files\\include\\contents" FILES
	${VANILLAPDF_INCLUDE_CONTENTS_HEADERS}
)

source_group("Header Files\\contents" FILES
	${VANILLAPDF_CONTENTS_HEADERS}
)

source_group("Header Files\\semantics" FILES
	${VANILLAPDF_SEMANTICS_HEADERS}
)

source_group("Header Files\\syntax" FILES
	${VANILLAPDF_SYNTAX_HEADERS}
)

source_group("Header Files\\utils" FILES
	${VANILLAPDF_UTILS_HEADERS}
)

source_group("Header Files\\implementation" FILES
	${VANILLAPDF_IMPLEMENTATION_HEADER}
)

# Source files
source_group("Source Files\\implementation" FILES
	${VANILLAPDF_C_IMPLEMENTATION_SOURCES}
)

source_group("Source Files\\implementation\\utils" FILES
	${VANILLAPDF_C_IMPLEMENTATION_UTILS_SOURCES}
)

source_group("Source Files\\implementation\\syntax" FILES
	${VANILLAPDF_C_IMPLEMENTATION_SYNTAX_SOURCES}
)

source_group("Source Files\\implementation\\semantics" FILES
	${VANILLAPDF_C_IMPLEMENTATION_SEMANTICS_SOURCES}
)

source_group("Source Files\\implementation\\contents" FILES
	${VANILLAPDF_C_IMPLEMENTATION_CONTENTS_SOURCES}
)

source_group("Source Files\\contents" FILES
	${VANILLAPDF_CONTENTS_SOURCES}
)

source_group("Source Files\\syntax" FILES
	${VANILLAPDF_SYNTAX_SOURCES}
)

source_group("Source Files\\semantics" FILES
	${VANILLAPDF_SEMANTICS_SOURCES}
)

source_group("Source Files\\utils" FILES
	${VANILLAPDF_UTILS_SOURCES}
)

set(VANILLAPDF_ALL_SOURCES
	${VANILLAPDF_C_IMPLEMENTATION_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_UTILS_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_SYNTAX_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_SEMANTICS_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_CONTENTS_SOURCES}
	
	${VANILLAPDF_SEMANTICS_SOURCES}
	${VANILLAPDF_SYNTAX_SOURCES}
	
	${VANILLAPDF_CONTENTS_SOURCES}
	${VANILLAPDF_UTILS_SOURCES}
)

add_library(vanillapdf SHARED
	${VANILLAPDF_INCLUDE_HEADERS}
	${VANILLAPDF_INCLUDE_UTILS_HEADERS}
	${VANILLAPDF_INCLUDE_SYNTAX_HEADERS}
	${VANILLAPDF_INCLUDE_SEMANTICS_HEADERS}
	${VANILLAPDF_INCLUDE_CONTENTS_HEADERS}
	
	${VANILLAPDF_C_IMPLEMENTATION_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_UTILS_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_SYNTAX_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_SEMANTICS_SOURCES}
	${VANILLAPDF_C_IMPLEMENTATION_CONTENTS_SOURCES}
	
	${VANILLAPDF_SEMANTICS_HEADERS}
	${VANILLAPDF_SEMANTICS_SOURCES}
	
	${VANILLAPDF_SYNTAX_HEADERS}
	${VANILLAPDF_SYNTAX_SOURCES}
	
	${VANILLAPDF_PRECOMPILED_SOURCE}
	${VANILLAPDF_PRECOMPILED_HEADER}
	
	${VANILLAPDF_CONTENTS_HEADERS}
	${VANILLAPDF_CONTENTS_SOURCES}
	
	${VANILLAPDF_UTILS_HEADERS}
	${VANILLAPDF_UTILS_SOURCES}
	
	${VANILLAPDF_ADDITIONAL_HEADERS}
	
	${VANILLAPDF_RESOURCE_FILES}
)

set_target_properties(vanillapdf PROPERTIES
  PREFIX "lib"
)

# Add precompiled header optimization
if(MSVC)
	get_filename_component(PrecompiledBasename ${VANILLAPDF_PRECOMPILED_HEADER} NAME_WE)
	set(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")

	set_source_files_properties(${VANILLAPDF_PRECOMPILED_SOURCE}
								PROPERTIES COMPILE_FLAGS "/Yc\"${VANILLAPDF_PRECOMPILED_HEADER}\" /Fp\"${PrecompiledBinary}\""
										   OBJECT_OUTPUTS "${PrecompiledBinary}")
	set_source_files_properties(${VANILLAPDF_ALL_SOURCES}
								PROPERTIES COMPILE_FLAGS "/Yu\"${VANILLAPDF_PRECOMPILED_HEADER}\" /FI\"${VANILLAPDF_PRECOMPILED_HEADER}\" /Fp\"${PrecompiledBinary}\""
										   OBJECT_DEPENDS "${PrecompiledBinary}")
endif(MSVC)

# Add natvis file embedding
if(MSVC)
	set_property(TARGET vanillapdf APPEND_STRING PROPERTY LINK_FLAGS "/NATVIS:public.natvis" )
endif(MSVC)

if(CMAKE_COMPILER_IS_GNUCXX)
	# C++17 filesystem implementation exception for GCC
	# https://en.cppreference.com/w/cpp/filesystem#Notes

	# Quote from link above:
	# Using this library may require additional compiler/linker options.
	# GNU implementation prior to 9.1 requires linking with -lstdc++fs and LLVM implementation prior to LLVM 9.0 requires linking with -lc++fs.

	set_property(TARGET vanillapdf APPEND_STRING PROPERTY LINK_FLAGS "-lstdc++fs" )
endif(CMAKE_COMPILER_IS_GNUCXX)

# Set C++ standard to be 17
# NOTE: C++20 fails on latest GCC, due to error with atomic variables
# NOTE: Similar issue here https://github.com/zephyrproject-rtos/zephyr/issues/40023
set_property(TARGET vanillapdf PROPERTY CXX_STANDARD 17)
set_property(TARGET vanillapdf PROPERTY CXX_STANDARD_REQUIRED ON)

# Specify that library should export functions
add_definitions(-DVANILLAPDF_EXPORTS)

# Specify version identificators
add_definitions(-DVANILLAPDF_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
add_definitions(-DVANILLAPDF_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
add_definitions(-DVANILLAPDF_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
add_definitions(-DVANILLAPDF_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

# Specify build time identificators
string(TIMESTAMP VANILLAPDF_BUILD_DAY	"%d" UTC)
string(TIMESTAMP VANILLAPDF_BUILD_MONTH	"%m" UTC)
string(TIMESTAMP VANILLAPDF_BUILD_YEAR	"%Y" UTC)

# Strip leading zeros from date values
# GCC throws an error having month "09" instead of "9"
string(REGEX REPLACE "^0*([0-9]+)" "\\1" VANILLAPDF_BUILD_DAY		"${VANILLAPDF_BUILD_DAY}")
string(REGEX REPLACE "^0*([0-9]+)" "\\1" VANILLAPDF_BUILD_MONTH		"${VANILLAPDF_BUILD_MONTH}")
string(REGEX REPLACE "^0*([0-9]+)" "\\1" VANILLAPDF_BUILD_YEAR		"${VANILLAPDF_BUILD_YEAR}")

add_definitions(-DVANILLAPDF_BUILD_DAY=${VANILLAPDF_BUILD_DAY})
add_definitions(-DVANILLAPDF_BUILD_MONTH=${VANILLAPDF_BUILD_MONTH})
add_definitions(-DVANILLAPDF_BUILD_YEAR=${VANILLAPDF_BUILD_YEAR})

# This installs required libraries
include("${CMAKE_SOURCE_DIR}/cmake/vcpkg_dependencies.cmake")

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenJPEG REQUIRED)
find_package(spdlog REQUIRED)

if (COMPILE_SKIA)
	find_package(unofficial-skia CONFIG REQUIRED)
	set(VANILLAPDF_LIBS unofficial::skia::skia ${VANILLAPDF_LIBS})
endif (COMPILE_SKIA)

if (OPENSSL_FOUND)
	include_directories(${OPENSSL_INCLUDE_DIR})
	set(VANILLAPDF_LIBS OpenSSL::Crypto ${VANILLAPDF_LIBS})
	add_definitions(-DVANILLAPDF_HAVE_OPENSSL)

	message(STATUS "OpenSSL ${OPENSSL_VERSION} was found")
else (OPENSSL_FOUND)
	message(WARNING "Compiling without OpenSSL support. Work with encrypted document will be prohibited")
endif (OPENSSL_FOUND)

if (JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIR})

	# This is not yet supported on all linux CMake distributions
	# set(VANILLAPDF_LIBS JPEG::JPEG ${VANILLAPDF_LIBS})
	set(VANILLAPDF_LIBS ${JPEG_LIBRARIES} ${VANILLAPDF_LIBS})
	add_definitions(-DVANILLAPDF_HAVE_JPEG)

	message(STATUS "JPEG library ${JPEG_VERSION} was found")
else (JPEG_FOUND)
	message(WARNING "Compiling without JPEG support. JPEG compression will become unavailable")
endif (JPEG_FOUND)

if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	set(VANILLAPDF_LIBS ZLIB::ZLIB ${VANILLAPDF_LIBS})
	add_definitions(-DVANILLAPDF_HAVE_ZLIB)

	message(STATUS "zlib ${ZLIB_VERSION_STRING} was found")
else (ZLIB_FOUND)
	message(WARNING "Compiling without zlib support. zlib decompression will become unavailable")
endif (ZLIB_FOUND)

if (OPENJPEG_LIBRARIES)
	include_directories(${OPENJPEG_INCLUDE_DIRS})
	set(VANILLAPDF_LIBS ${OPENJPEG_LIBRARIES} ${VANILLAPDF_LIBS})
	add_definitions(-DVANILLAPDF_HAVE_OPENJPEG)
	message(STATUS "openjpeg was found without specified version")
else (OPENJPEG_LIBRARIES)
	message(WARNING "Compiling without openjpeg support. jpeg2000 decompression will become unavailable")
endif (OPENJPEG_LIBRARIES)

# Update 30.08.2024:
# fmtlib is part of spdlog which is a required dependency of the solution
# Maybe it can be configured to use an external fmtlib, however I do not see any value in investigating this
# For the time being I am removing the fmtlib dependency and submodule, but keeping the code for tracking purposes

# Add fmtlib dependency as git external submodule

# Use header only version as under linux there are some compilation issues
# relocation R_X86_64_PC32 against symbol `stderr@@GLIBC_2.2.5' can not be used when making a shared object; recompile with -fPIC
# https://stackoverflow.com/questions/38296756/what-is-the-idiomatic-way-in-cmake-to-add-the-fpic-compiler-option

#add_subdirectory("${CMAKE_SOURCE_DIR}/external/fmtlib" build EXCLUDE_FROM_ALL)

set(VANILLAPDF_LIBS spdlog::spdlog ${VANILLAPDF_LIBS})

target_link_libraries(vanillapdf ${VANILLAPDF_LIBS})

# Project install targets
install(TARGETS vanillapdf
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(FILES ${VANILLAPDF_INCLUDE_HEADERS} DESTINATION "include\\vanillapdf")
install(FILES ${VANILLAPDF_INCLUDE_UTILS_HEADERS} DESTINATION "include\\vanillapdf\\utils")
install(FILES ${VANILLAPDF_INCLUDE_SYNTAX_HEADERS} DESTINATION "include\\vanillapdf\\syntax")
install(FILES ${VANILLAPDF_INCLUDE_SEMANTICS_HEADERS} DESTINATION "include\\vanillapdf\\semantics")
install(FILES ${VANILLAPDF_INCLUDE_CONTENTS_HEADERS} DESTINATION "include\\vanillapdf\\contents")

install(FILES "${CMAKE_SOURCE_DIR}/cmake/README.txt" DESTINATION .)
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE.txt" DESTINATION .)
install(FILES "${CMAKE_SOURCE_DIR}/CHANGELOG.txt" DESTINATION .)

# Install dependent libraries on windows
if(WIN32)

	# OpenSSL dependent libraries
	if (OPENSSL_FOUND)
	
		# Find dll runtime binary file
		find_file(LIBEAY_RUNTIME
			NAMES libcrypto-3.dll libcrypto-3-x64.dll legacy.dll
			PATHS ${VANILLPDF_BINARY_DIR}
		)

		if (NOT EXISTS ${LIBEAY_RUNTIME})
			# message(FATAL_ERROR "OpenSSL support is enabled, but the LIBEAY_RUNTIME does not exist")
		endif(NOT EXISTS ${LIBEAY_RUNTIME})
		
		# message(STATUS "LIBEAY runtime was found at ${LIBEAY_RUNTIME}")
		
		# Install dependency into output package
		# install(FILES ${LIBEAY_RUNTIME} DESTINATION bin)

	endif (OPENSSL_FOUND)

	# libjpeg dependent libraries
	if (JPEG_FOUND)

		# Find dll runtime binary file
		find_file(LIBJPEG_RUNTIME
			NAMES turbojpeg.dll
			PATHS ${VANILLPDF_BINARY_DIR}
		)

		if (NOT EXISTS ${LIBJPEG_RUNTIME})
			# message(FATAL_ERROR "JPEG support is enabled, but the LIBJPEG_RUNTIME does not exist")
		endif(NOT EXISTS ${LIBJPEG_RUNTIME})
		
		# message(STATUS "LIBJPEG runtime was found at ${LIBJPEG_RUNTIME}")
		
		# Install dependency into output package
		# install(FILES ${LIBJPEG_RUNTIME} DESTINATION bin)
	endif (JPEG_FOUND)

	# zlib dependent libraries
	if (ZLIB_FOUND)

		# Find dll runtime binary file
		find_file(ZLIB_RUNTIME
			NAMES zlib1.dll
			PATHS ${VANILLPDF_BINARY_DIR}
		)

		if (NOT EXISTS ${ZLIB_RUNTIME})
			# message(FATAL_ERROR "zlib support is enabled, but the ZLIB_RUNTIME does not exist")
		endif(NOT EXISTS ${ZLIB_RUNTIME})
		
		# message(STATUS "ZLIB runtime was found at ${ZLIB_RUNTIME}")
		
		# Install dependency into output package
		# install(FILES ${ZLIB_RUNTIME} DESTINATION bin)
	endif (ZLIB_FOUND)
endif(WIN32)
