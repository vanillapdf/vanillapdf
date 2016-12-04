# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

project(gotchangpdf CXX)

include_directories("${CMAKE_SOURCE_DIR}/include")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(implementation)
include_directories(semantics/documents)
include_directories(semantics/objects)
include_directories(semantics/utils)
                    
include_directories(syntax/exceptions)
include_directories(syntax/files)
include_directories(syntax/filters)
include_directories(syntax/objects)
include_directories(syntax/parsers)
include_directories(syntax/parsers/contents)
include_directories(syntax/streams)
include_directories(syntax/utils)

add_definitions("-DPDF_EXPORTS")

set(GOTCHANGPDF_PRECOMPILED_SOURCE
	precompiled.cpp
)

set(GOTCHANGPDF_C_IMPLEMENTATION_SOURCES
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
	implementation/c_document.cpp
	implementation/c_document_info.cpp
	implementation/c_encryption.cpp
	implementation/c_file.cpp
	implementation/c_font.cpp
	implementation/c_font_map.cpp
	implementation/c_indirect_object_reference.cpp
	implementation/c_integer_object.cpp
	implementation/c_name_object.cpp
	implementation/c_object.cpp
	implementation/c_outline.cpp
	implementation/c_page_labels.cpp
	implementation/c_page_object.cpp
	implementation/c_page_tree.cpp
	implementation/c_real_object.cpp
	implementation/c_rectangle.cpp
	implementation/c_resource_dictionary.cpp
	implementation/c_stream_object.cpp
	implementation/c_string_object.cpp
	implementation/c_values.cpp
	implementation/c_viewer_preferences.cpp
	implementation/c_xref.cpp
)

set(GOTCHANGPDF_SEMANTICS_SOURCES
	semantics/documents/catalog.cpp
	semantics/documents/content_stream.cpp
	semantics/documents/contents.cpp
	semantics/documents/document.cpp
	semantics/documents/page_node_utils.cpp
	semantics/documents/page_object.cpp
	semantics/documents/page_tree.cpp
	semantics/documents/page_tree_node.cpp
	semantics/documents/resource_dictionary.cpp
	semantics/objects/annotations.cpp
	semantics/objects/character_map.cpp
	semantics/objects/date.cpp
	semantics/objects/destinations.cpp
	semantics/objects/developer_extensions.cpp
	semantics/objects/document_info.cpp
	semantics/objects/font.cpp
	semantics/objects/outline.cpp
	semantics/objects/rectangle.cpp
	semantics/utils/semantic_utils.cpp
)

set(GOTCHANGPDF_SYNTAX_SOURCES
	syntax/exceptions/exception.cpp
	
	syntax/files/file.cpp
	syntax/files/file_writer.cpp
	syntax/files/xref.cpp
	syntax/files/xref_chain.cpp
	
	syntax/filters/ascii_85_decode_filter.cpp
	syntax/filters/ascii_hex_decode_filter.cpp
	syntax/filters/dct_decode_filter.cpp
	syntax/filters/filter.cpp
	syntax/filters/flate_decode_filter.cpp
	
	syntax/objects/array_object.cpp
	syntax/objects/dictionary_object.cpp
	syntax/objects/indirect_object_reference.cpp
	syntax/objects/integer_object.cpp
	syntax/objects/name_object.cpp
	syntax/objects/numeric_object.cpp
	syntax/objects/object.cpp
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
	
	syntax/parsers/contents/content_stream_objects.cpp
	syntax/parsers/contents/content_stream_operations.cpp
	
	syntax/streams/raw_reverse_stream.cpp
	syntax/streams/raw_stream.cpp
	
	syntax/utils/buffer.cpp
	syntax/utils/encryption.cpp
	syntax/utils/log.cpp
	syntax/utils/modify_observer_interface.cpp
	syntax/utils/pkcs12_key.cpp
	syntax/utils/zlib_wrapper.cpp
)

set(SEMANTICS_HEADERS
	semantics/documents/catalog.h
	semantics/documents/content_stream.h
	semantics/documents/contents.h
	semantics/documents/document.h
	semantics/documents/page_node.h
	semantics/documents/page_node_utils.h
	semantics/documents/page_object.h
	semantics/documents/page_tree.h
	semantics/documents/page_tree_node.h
	semantics/documents/resource_dictionary.h
	
	semantics/objects/annotations.h
	semantics/objects/character_map.h
	semantics/objects/date.h
	semantics/objects/destinations.h
	semantics/objects/developer_extensions.h
	semantics/objects/document_info.h
	semantics/objects/font.h
	semantics/objects/high_level_object.h
	semantics/objects/name_dictionary.h
	semantics/objects/outline.h
	semantics/objects/page_labels.h
	semantics/objects/rectangle.h
	semantics/objects/tree.h
	semantics/objects/viewer_preferences.h
	
	semantics/utils/semantic_exceptions.h
	semantics/utils/semantic_utils.h
	semantics/utils/semantics_fwd.h
)

set(SYNTAX_HEADERS
	syntax/exceptions/exception.h
	
	syntax/files/file.h
	syntax/files/file_writer.h
	syntax/files/header.h
	syntax/files/xref.h
	syntax/files/xref_chain.h
	
	syntax/filters/ascii_85_decode_filter.h
	syntax/filters/ascii_hex_decode_filter.h
	syntax/filters/dct_decode_filter.h
	syntax/filters/filter.h
	syntax/filters/flate_decode_filter.h
	
	syntax/objects/array_object.h
	syntax/objects/boolean_object.h
	syntax/objects/containable.h
	syntax/objects/dictionary_object.h
	syntax/objects/indirect_object_reference.h
	syntax/objects/integer_object.h
	syntax/objects/name_object.h
	syntax/objects/null_object.h
	syntax/objects/numeric_object.h
	syntax/objects/object.h
	syntax/objects/object_utils.h
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
	
	syntax/parsers/contents/content_stream_instruction_base.h
	syntax/parsers/contents/content_stream_objects.h
	syntax/parsers/contents/content_stream_operation_base.h
	syntax/parsers/contents/content_stream_operation_generic.h
	syntax/parsers/contents/content_stream_operations.h
	syntax/parsers/contents/content_stream_operators.h
	syntax/parsers/contents/content_utils.h
	
	syntax/streams/raw_base_stream.h
	syntax/streams/raw_reverse_stream.h
	syntax/streams/raw_stream.h
	
	syntax/utils/buffer.h
	syntax/utils/character.h
	syntax/utils/constants.h
	syntax/utils/deferred.h
	syntax/utils/encryption.h
	syntax/utils/file_device.h
	syntax/utils/log.h
	syntax/utils/modify_observer_interface.h
	syntax/utils/output_pointer.h
	syntax/utils/pkcs12_key.h
	syntax/utils/syntax_fwd.h
	syntax/utils/unknown_interface.h
	syntax/utils/util.h
	syntax/utils/version.h
	syntax/utils/zlib_wrapper.h
)

set(INCLUDE_HEADERS
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_annotations.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_array_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_boolean_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_buffer.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_catalog.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_character_map.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_contents.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_date.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_destinations.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_developer_extensions.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_dictionary_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_document.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_document_info.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_encryption.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_export.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_file.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_font.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_font_map.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_handles.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_indirect_object_reference.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_integer_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_name_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_outline.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_page_labels.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_page_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_page_tree.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_real_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_rectangle.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_resource_dictionary.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_stream_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_string_object.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_values.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_version.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_viewer_preferences.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/c_xref.h"
	"${CMAKE_SOURCE_DIR}/include/gotchangpdf/gotchangpdf.h"
)

set(IMPLEMENTATION_HELPER_HEADER
	implementation/helper.h
)

set(PRECOMPILED_HEADER
	precompiled.h
)

source_group("Header Files" FILES
	${PRECOMPILED_HEADER}
)

source_group("Header Files\\include" FILES
	${INCLUDE_HEADERS}
)

source_group("Header Files\\semantics" FILES
	${SEMANTICS_HEADERS}
)

source_group("Header Files\\syntax" FILES
	${SYNTAX_HEADERS}
)

source_group("Source Files\\implementation" FILES
	${GOTCHANGPDF_C_IMPLEMENTATION_SOURCES}
	${IMPLEMENTATION_HELPER_HEADER}
)

source_group("Source Files\\syntax" FILES
	${GOTCHANGPDF_SYNTAX_SOURCES}
)

source_group("Source Files\\semantics" FILES
	${GOTCHANGPDF_SEMANTICS_SOURCES}
)

install(FILES ${INCLUDE_HEADERS}
  DESTINATION "include\\gotchangpdf"
)

add_library(gotchangpdf SHARED
	${GOTCHANGPDF_C_IMPLEMENTATION_SOURCES}
	${GOTCHANGPDF_SEMANTICS_SOURCES}
	${GOTCHANGPDF_SYNTAX_SOURCES}
	${GOTCHANGPDF_PRECOMPILED_SOURCE}
	${PRECOMPILED_HEADER}
	${INCLUDE_HEADERS}
	${SEMANTICS_HEADERS}
	${SYNTAX_HEADERS}
)

target_compile_features(gotchangpdf PRIVATE
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

find_package(OpenSSL REQUIRED)
find_package(ZLIB)
find_package(JPEG)

if (OPENSSL_FOUND)
	include_directories(${OPENSSL_INCLUDE_DIR})
	set(GOTCHANGPDF_LIBS ${OPENSSL_LIBRARIES} ${GOTCHANGPDF_LIBS})
	
	message(STATUS "OpenSSL ${OPENSSL_VERSION} was found in ${OPENSSL_ROOT_DIR}")
endif (OPENSSL_FOUND)

if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	set(GOTCHANGPDF_LIBS ${ZLIB_LIBRARIES} ${GOTCHANGPDF_LIBS})
	
	message(STATUS "Zlib ${ZLIB_VERSION_STRING} was found in ${ZLIB_LIBRARIES}")
endif (ZLIB_FOUND)

if (JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIR})
	set(GOTCHANGPDF_LIBS ${JPEG_LIBRARIES} ${GOTCHANGPDF_LIBS})
	
	message(STATUS "JPEG library was found")
endif (JPEG_FOUND)

target_link_libraries(gotchangpdf ${GOTCHANGPDF_LIBS})