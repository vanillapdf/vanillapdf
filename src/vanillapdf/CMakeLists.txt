project(vanillapdf LANGUAGES CXX)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/lib/nlohmann_json/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(VANILLAPDF_PRECOMPILED_SOURCE
	precompiled.cpp
)

set(VANILLAPDF_C_IMPLEMENTATION_SOURCES
	implementation/c_annotations.cpp
	implementation/c_array_object.cpp
	implementation/c_boolean_object.cpp
	implementation/c_buffer.cpp
	implementation/c_catalog.cpp
	implementation/c_character_map.cpp
	implementation/c_contents.cpp
	implementation/c_date.cpp
	implementation/c_destinations.cpp
	implementation/c_developer_extensions.cpp
	implementation/c_dictionary_object.cpp
	implementation/c_digital_signature.cpp
	implementation/c_document.cpp
	implementation/c_document_info.cpp
	implementation/c_document_signature_settings.cpp
	implementation/c_encryption.cpp
	implementation/c_errors.cpp
	implementation/c_fields.cpp
	implementation/c_file.cpp
	implementation/c_file_writer.cpp
	implementation/c_font.cpp
	implementation/c_font_map.cpp
	implementation/c_indirect_object_reference.cpp
	implementation/c_integer_object.cpp
	implementation/c_interactive_forms.cpp
	implementation/c_library_info.cpp
	implementation/c_license_info.cpp
	implementation/c_logging.cpp
	implementation/c_name_object.cpp
	implementation/c_null_object.cpp
	implementation/c_object.cpp
	implementation/c_outline.cpp
	implementation/c_page_labels.cpp
	implementation/c_page_object.cpp
	implementation/c_page_tree.cpp
	implementation/c_pkcs12_key.cpp
	implementation/c_real_object.cpp
	implementation/c_rectangle.cpp
	implementation/c_resource_dictionary.cpp
	implementation/c_signing_key.cpp
	implementation/c_stream_object.cpp
	implementation/c_string_object.cpp
	implementation/c_unknown_interface.cpp
	implementation/c_values.cpp
	implementation/c_viewer_preferences.cpp
	implementation/c_xref.cpp
)

set(VANILLAPDF_CONTENTS_SOURCES
	contents/content_stream_objects.cpp
	contents/content_stream_operations.cpp
	contents/content_stream_parser.cpp
	contents/content_stream_operation_generic.cpp
)

set(VANILLAPDF_SEMANTICS_SOURCES
	semantics/objects/catalog.cpp
	semantics/objects/content_stream.cpp
	semantics/objects/contents.cpp
	semantics/objects/document.cpp
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
	
	syntax/objects/boolean_object.cpp
	syntax/objects/dictionary_object.cpp
	syntax/objects/indirect_object_reference.cpp
	syntax/objects/integer_object.cpp
	syntax/objects/mixed_array_object.cpp
	syntax/objects/name_object.cpp
	syntax/objects/null_object.cpp
	syntax/objects/numeric_object.cpp
	syntax/objects/object.cpp
	syntax/objects/real_object.cpp
	syntax/objects/stream_object.cpp
	syntax/objects/string_object.cpp
	
	syntax/parsers/character_map_data.cpp
	syntax/parsers/lexical_tree.cpp
	syntax/parsers/parser.cpp
	syntax/parsers/reverse_parser.cpp
	syntax/parsers/reverse_tokenizer.cpp
	syntax/parsers/token.cpp
	syntax/parsers/token_dictionary.cpp
	syntax/parsers/tokenizer.cpp
	
	syntax/utils/encryption.cpp
)

set(VANILLAPDF_UTILS_SOURCES
	utils/attribute_interface.cpp
	utils/attribute_list.cpp
	utils/buffer.cpp
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
	utils/zlib_wrapper.cpp
	
	utils/streams/input_reverse_stream.cpp
	utils/streams/input_stream.cpp
	utils/streams/output_stream.cpp
)

set(VANILLAPDF_CONTENTS_HEADERS
	contents/content_fwd.h
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
	semantics/objects/contents.h
	semantics/objects/document.h
	semantics/objects/page_node_base.h
	semantics/objects/page_object.h
	semantics/objects/page_tree.h
	semantics/objects/page_tree_node.h
	semantics/objects/resource_dictionary.h
	
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
	
	syntax/objects/array_object.h
	syntax/objects/mixed_array_object.h
	syntax/objects/boolean_object.h
	syntax/objects/containable.h
	syntax/objects/dictionary_object.h
	syntax/objects/indirect_object_reference.h
	syntax/objects/integer_object.h
	syntax/objects/name_object.h
	syntax/objects/null_object.h
	syntax/objects/numeric_object.h
	syntax/objects/object.h
	syntax/objects/objects.h
	syntax/objects/real_object.h
	syntax/objects/stream_object.h
	syntax/objects/string_object.h
	
	syntax/parsers/character_map_data.h
	syntax/parsers/lexical_tree.h
	syntax/parsers/object_stream_entry.h
	syntax/parsers/parser.h
	syntax/parsers/parser_interface.h
	syntax/parsers/reverse_parser.h
	syntax/parsers/reverse_tokenizer.h
	syntax/parsers/token.h
	syntax/parsers/token_dictionary.h
	syntax/parsers/tokenizer.h
	
	syntax/utils/encryption.h
	syntax/utils/output_pointer.h
	syntax/utils/syntax_fwd.h
	syntax/utils/object_utils.h
)

set(VANILLAPDF_UTILS_HEADERS
	utils/attribute_interface.h
	utils/attribute_list.h
	utils/base_iterator.h
	utils/buffer.h
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
	utils/zlib_wrapper.h
	
	utils/streams/input_stream_interface.h
	utils/streams/input_reverse_stream.h
	utils/streams/input_stream.h
	
	utils/streams/output_stream_interface.h
	utils/streams/output_stream.h
	
	utils/streams/input_output_stream_interface.h
	utils/streams/input_output_stream.h
)

set(VANILLAPDF_INCLUDE_HEADERS
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_annotations.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_array_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_boolean_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_buffer.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_catalog.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_character_map.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_contents.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_date.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_destinations.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_developer_extensions.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_dictionary_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_digital_signature.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_document.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_document_info.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_document_signature_settings.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_encryption.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_errors.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_export.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_fields.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_file.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_file_writer.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_font.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_font_map.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_handles.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_indirect_object_reference.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_integer_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_interactive_forms.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_library_info.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_license_info.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_logging.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_message_digest_algorithm.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_name_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_null_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_outline.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_page_labels.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_page_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_page_tree.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_pdf_version.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_pkcs12_key.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_platform.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_real_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_rectangle.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_resource_dictionary.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_signing_key.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_stream_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_string_object.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_types.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_unknown_interface.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_values.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_viewer_preferences.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_xref.h"
	"${CMAKE_SOURCE_DIR}/include/vanillapdf/c_vanillapdf_api.h"
)

set(VANILLAPDF_IMPLEMENTATION_HELPER_HEADER
	implementation/helper.h
)

set(VANILLAPDF_ADDITIONAL_HEADERS
	"${CMAKE_SOURCE_DIR}/include/unistd.h"
)

set(VANILLAPDF_PRECOMPILED_HEADER
	precompiled.h
)

set(VANILLAPDF_RESOURCE_FILES
  vanillapdf.natvis
  resources.rc
)

source_group("Resource Files" FILES
	${VANILLAPDF_RESOURCE_FILES}
)

source_group("Header Files" FILES
	${VANILLAPDF_PRECOMPILED_HEADER}
	${VANILLAPDF_ADDITIONAL_HEADERS}
)

source_group("Header Files\\include" FILES
	${VANILLAPDF_INCLUDE_HEADERS}
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

source_group("Source Files\\implementation" FILES
	${VANILLAPDF_C_IMPLEMENTATION_SOURCES}
	${VANILLAPDF_IMPLEMENTATION_HELPER_HEADER}
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
	${VANILLAPDF_SEMANTICS_SOURCES}
	${VANILLAPDF_SYNTAX_SOURCES}
	
	${VANILLAPDF_CONTENTS_SOURCES}
	${VANILLAPDF_UTILS_SOURCES}
)

add_library(vanillapdf SHARED
	${VANILLAPDF_C_IMPLEMENTATION_SOURCES}
	${VANILLAPDF_SEMANTICS_SOURCES}
	${VANILLAPDF_SYNTAX_SOURCES}
	${VANILLAPDF_PRECOMPILED_SOURCE}
	${VANILLAPDF_PRECOMPILED_HEADER}
	${VANILLAPDF_INCLUDE_HEADERS}
	${VANILLAPDF_SEMANTICS_HEADERS}
	${VANILLAPDF_SYNTAX_HEADERS}
	
	${VANILLAPDF_CONTENTS_HEADERS}
	${VANILLAPDF_CONTENTS_SOURCES}
	
	${VANILLAPDF_UTILS_HEADERS}
	${VANILLAPDF_UTILS_SOURCES}
	
	${VANILLAPDF_ADDITIONAL_HEADERS}
	
	${VANILLAPDF_RESOURCE_FILES}
)

set_target_properties(vanillapdf PROPERTIES
  PUBLIC_HEADER "${VANILLAPDF_INCLUDE_HEADERS}"
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

target_compile_features(vanillapdf PRIVATE
	cxx_decltype
	cxx_defaulted_functions
	cxx_defaulted_move_initializers
	cxx_deleted_functions
	cxx_noexcept
	cxx_nullptr
	cxx_override
	cxx_rvalue_references
	cxx_static_assert
	cxx_strong_enums
)

# Specify that library should export functions
add_definitions(-DVANILLAPDF_EXPORTS)

# Specify version identificators
add_definitions(-DVANILLAPDF_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
add_definitions(-DVANILLAPDF_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
add_definitions(-DVANILLAPDF_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
add_definitions(-DVANILLAPDF_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

# Specify build time identificators
string(TIMESTAMP VANILLAPDF_BUILD_DAY		"%d" UTC)
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

if(WIN32)

	# TODO packaged openSSL build uses older VC compiler
	# also it requires multiple additional dependencies such as winsock
	
	# Try to use static libraries on windows as they have to be shipped
	#set(OPENSSL_USE_STATIC_LIBS TRUE)
	#set(OPENSSL_MSVC_STATIC_RT TRUE)

endif(WIN32)

find_package(OpenSSL)
find_package(ZLIB)
find_package(JPEG)

if (OPENSSL_FOUND)
	include_directories(${OPENSSL_INCLUDE_DIR})
	set(VANILLAPDF_LIBS ${OPENSSL_LIBRARIES} ${VANILLAPDF_LIBS})
	add_definitions(-DVANILLAPDF_HAVE_OPENSSL)
	
	message(STATUS "OpenSSL ${OPENSSL_VERSION} was found")
else (OPENSSL_FOUND)
	message(WARNING "Compiling without OpenSSL support. Work with encrypted document will be prohibited")
endif (OPENSSL_FOUND)

if (JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIR})
	set(VANILLAPDF_LIBS ${JPEG_LIBRARIES} ${VANILLAPDF_LIBS})
	add_definitions(-DVANILLAPDF_HAVE_JPEG)

	message(STATUS "JPEG library was found")
else (JPEG_FOUND)
	message(STATUS "Compiling without JPEG support. JPEG compression will become unavailable")
endif (JPEG_FOUND)

if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	set(VANILLAPDF_LIBS ${ZLIB_LIBRARIES} ${VANILLAPDF_LIBS})
	add_definitions(-DVANILLAPDF_HAVE_ZLIB)

	message(STATUS "zlib ${ZLIB_VERSION_STRING} was found")
else (ZLIB_FOUND)
	message(STATUS "Compiling without zlib support. zlib decompression will become unavailable")
endif (ZLIB_FOUND)

target_link_libraries(vanillapdf ${VANILLAPDF_LIBS})

# Place for install targets

# Project install targets
install(TARGETS vanillapdf
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION "include\\vanillapdf"
)

# Install dependent libraries on windows
if(WIN32)

	# OpenSSL dependent libraries
	if (OPENSSL_FOUND)
		# TODO search module for binaries
		set(OPENSSL_CRYPTO_BINARY_DEBUG CACHE FILEPATH "")
		set(OPENSSL_CRYPTO_BINARY_RELEASE CACHE FILEPATH "")

		# Set current configuration binary file
		if (CONFIG:debug)
			set(OPENSSL_CRYPTO_BINARY ${OPENSSL_CRYPTO_BINARY_DEBUG})
		else(CONFIG:debug)
			set(OPENSSL_CRYPTO_BINARY ${OPENSSL_CRYPTO_BINARY_RELEASE})
		endif(CONFIG:debug)
		
		# Install dependency into output package
		install(FILES ${OPENSSL_CRYPTO_BINARY} DESTINATION bin)
	endif (OPENSSL_FOUND)

	# libjpeg dependent libraries
	if (JPEG_FOUND)
		# TODO search module for binaries
		set(JPEG_BINARY_DEBUG CACHE FILEPATH "")
		set(JPEG_BINARY_RELEASE CACHE FILEPATH "")

		# Set current configuration binary file
		if (CONFIG:debug)
			set(JPEG_BINARY ${JPEG_BINARY_DEBUG})
		else(CONFIG:debug)
			set(JPEG_BINARY ${JPEG_BINARY_RELEASE})
		endif(CONFIG:debug)
		
		# Install dependency into output package
		install(FILES ${JPEG_BINARY} DESTINATION bin)
	endif (JPEG_FOUND)

	# zlib dependent libraries
	if (ZLIB_FOUND)
		# TODO search module for binaries
		set(ZLIB_BINARY_DEBUG CACHE FILEPATH "")
		set(ZLIB_BINARY_RELEASE CACHE FILEPATH "")
		
		# Set current configuration binary file
		if (CONFIG:debug)
			set(ZLIB_BINARY ${ZLIB_BINARY_DEBUG})
		else(CONFIG:debug)
			set(ZLIB_BINARY ${ZLIB_BINARY_RELEASE})
		endif(CONFIG:debug)
		
		# Install dependency into output package
		install(FILES ${ZLIB_BINARY} DESTINATION bin)
	endif (ZLIB_FOUND)
endif(WIN32)
