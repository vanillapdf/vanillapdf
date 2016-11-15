# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

# Define main project
project(gotchangpdf)

include_directories("include")
include_directories("include/gotchangpdf")

include_directories(src/gotchangpdf)
include_directories(src/gotchangpdf/implementation)
include_directories(src/gotchangpdf/semantics/documents)
include_directories(src/gotchangpdf/semantics/objects)
include_directories(src/gotchangpdf/semantics/utils)

include_directories(src/gotchangpdf/syntax/exceptions)
include_directories(src/gotchangpdf/syntax/files)
include_directories(src/gotchangpdf/syntax/filters)
include_directories(src/gotchangpdf/syntax/objects)
include_directories(src/gotchangpdf/syntax/parsers)
include_directories(src/gotchangpdf/syntax/parsers/contents)
include_directories(src/gotchangpdf/syntax/streams)
include_directories(src/gotchangpdf/syntax/utils)

set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-DRELEASE")

set(GOTCHANGPDF_PRECOMPILED_SOURCE
	src/gotchangpdf/precompiled.cpp
)

set(GOTCHANGPDF_C_IMPLEMENTATION_SOURCES
	src/gotchangpdf/implementation/c_annotations.cpp
	src/gotchangpdf/implementation/c_array_object.cpp
	src/gotchangpdf/implementation/c_boolean_object.cpp
	src/gotchangpdf/implementation/c_buffer.cpp
	src/gotchangpdf/implementation/c_catalog.cpp
	src/gotchangpdf/implementation/c_character_map.cpp
	src/gotchangpdf/implementation/c_contents.cpp
	src/gotchangpdf/implementation/c_date.cpp
	src/gotchangpdf/implementation/c_destinations.cpp
	src/gotchangpdf/implementation/c_developer_extensions.cpp
	src/gotchangpdf/implementation/c_dictionary_object.cpp
	src/gotchangpdf/implementation/c_document.cpp
	src/gotchangpdf/implementation/c_document_info.cpp
	src/gotchangpdf/implementation/c_encryption.cpp
	src/gotchangpdf/implementation/c_file.cpp
	src/gotchangpdf/implementation/c_font.cpp
	src/gotchangpdf/implementation/c_font_map.cpp
	src/gotchangpdf/implementation/c_indirect_object_reference.cpp
	src/gotchangpdf/implementation/c_integer_object.cpp
	src/gotchangpdf/implementation/c_name_object.cpp
	src/gotchangpdf/implementation/c_object.cpp
	src/gotchangpdf/implementation/c_outline.cpp
	src/gotchangpdf/implementation/c_page_labels.cpp
	src/gotchangpdf/implementation/c_page_object.cpp
	src/gotchangpdf/implementation/c_page_tree.cpp
	src/gotchangpdf/implementation/c_real_object.cpp
	src/gotchangpdf/implementation/c_rectangle.cpp
	src/gotchangpdf/implementation/c_resource_dictionary.cpp
	src/gotchangpdf/implementation/c_stream_object.cpp
	src/gotchangpdf/implementation/c_string_object.cpp
	src/gotchangpdf/implementation/c_values.cpp
	src/gotchangpdf/implementation/c_viewer_preferences.cpp
	src/gotchangpdf/implementation/c_xref.cpp
)

set(GOTCHANGPDF_SEMANTICS_SOURCES
	src/gotchangpdf/semantics/documents/catalog.cpp
	src/gotchangpdf/semantics/documents/content_stream.cpp
	src/gotchangpdf/semantics/documents/contents.cpp
	src/gotchangpdf/semantics/documents/document.cpp
	src/gotchangpdf/semantics/documents/page_node_utils.cpp
	src/gotchangpdf/semantics/documents/page_object.cpp
	src/gotchangpdf/semantics/documents/page_tree.cpp
	src/gotchangpdf/semantics/documents/page_tree_node.cpp
	src/gotchangpdf/semantics/documents/resource_dictionary.cpp
	src/gotchangpdf/semantics/objects/annotations.cpp
	src/gotchangpdf/semantics/objects/character_map.cpp
	src/gotchangpdf/semantics/objects/date.cpp
	src/gotchangpdf/semantics/objects/destinations.cpp
	src/gotchangpdf/semantics/objects/developer_extensions.cpp
	src/gotchangpdf/semantics/objects/document_info.cpp
	src/gotchangpdf/semantics/objects/font.cpp
	src/gotchangpdf/semantics/objects/outline.cpp
	src/gotchangpdf/semantics/objects/rectangle.cpp
	src/gotchangpdf/semantics/utils/semantic_utils.cpp
)

set(GOTCHANGPDF_SYNTAX_SOURCES
	src/gotchangpdf/syntax/exceptions/exception.cpp
	
	src/gotchangpdf/syntax/files/file.cpp
	src/gotchangpdf/syntax/files/file_writer.cpp
	src/gotchangpdf/syntax/files/xref.cpp
	src/gotchangpdf/syntax/files/xref_chain.cpp
	
	src/gotchangpdf/syntax/filters/ascii_85_decode_filter.cpp
	src/gotchangpdf/syntax/filters/ascii_hex_decode_filter.cpp
	src/gotchangpdf/syntax/filters/dct_decode_filter.cpp
	src/gotchangpdf/syntax/filters/filter.cpp
	src/gotchangpdf/syntax/filters/flate_decode_filter.cpp
	
	src/gotchangpdf/syntax/objects/array_object.cpp
	src/gotchangpdf/syntax/objects/dictionary_object.cpp
	src/gotchangpdf/syntax/objects/indirect_object_reference.cpp
	src/gotchangpdf/syntax/objects/integer_object.cpp
	src/gotchangpdf/syntax/objects/name_object.cpp
	src/gotchangpdf/syntax/objects/numeric_object.cpp
	src/gotchangpdf/syntax/objects/object.cpp
	src/gotchangpdf/syntax/objects/stream_object.cpp
	src/gotchangpdf/syntax/objects/string_object.cpp
	
	src/gotchangpdf/syntax/parsers/character_map_data.cpp
	src/gotchangpdf/syntax/parsers/lexical_tree.cpp
	src/gotchangpdf/syntax/parsers/parser.cpp
	src/gotchangpdf/syntax/parsers/reverse_parser.cpp
	src/gotchangpdf/syntax/parsers/reverse_tokenizer.cpp
	src/gotchangpdf/syntax/parsers/token.cpp
	src/gotchangpdf/syntax/parsers/token_dictionary.cpp
	src/gotchangpdf/syntax/parsers/tokenizer.cpp
	
	src/gotchangpdf/syntax/parsers/contents/content_stream_objects.cpp
	src/gotchangpdf/syntax/parsers/contents/content_stream_operations.cpp
	
	src/gotchangpdf/syntax/streams/raw_reverse_stream.cpp
	src/gotchangpdf/syntax/streams/raw_stream.cpp
	
	src/gotchangpdf/syntax/utils/buffer.cpp
	src/gotchangpdf/syntax/utils/encryption.cpp
	src/gotchangpdf/syntax/utils/log.cpp
	src/gotchangpdf/syntax/utils/modify_observer_interface.cpp
	src/gotchangpdf/syntax/utils/pkcs12_key.cpp
	src/gotchangpdf/syntax/utils/zlib_wrapper.cpp
)

set(INCLUDE_HEADERS
	"include/gotchangpdf/c_annotations.h"
	"include/gotchangpdf/c_array_object.h"
	"include/gotchangpdf/c_boolean_object.h"
	"include/gotchangpdf/c_buffer.h"
	"include/gotchangpdf/c_catalog.h"
	"include/gotchangpdf/c_character_map.h"
	"include/gotchangpdf/c_contents.h"
	"include/gotchangpdf/c_date.h"
	"include/gotchangpdf/c_destinations.h"
	"include/gotchangpdf/c_developer_extensions.h"
	"include/gotchangpdf/c_dictionary_object.h"
	"include/gotchangpdf/c_document.h"
	"include/gotchangpdf/c_document_info.h"
	"include/gotchangpdf/c_encryption.h"
	"include/gotchangpdf/c_export.h"
	"include/gotchangpdf/c_file.h"
	"include/gotchangpdf/c_font.h"
	"include/gotchangpdf/c_font_map.h"
	"include/gotchangpdf/c_handles.h"
	"include/gotchangpdf/c_indirect_object_reference.h"
	"include/gotchangpdf/c_integer_object.h"
	"include/gotchangpdf/c_name_object.h"
	"include/gotchangpdf/c_object.h"
	"include/gotchangpdf/c_outline.h"
	"include/gotchangpdf/c_page_labels.h"
	"include/gotchangpdf/c_page_object.h"
	"include/gotchangpdf/c_page_tree.h"
	"include/gotchangpdf/c_real_object.h"
	"include/gotchangpdf/c_rectangle.h"
	"include/gotchangpdf/c_resource_dictionary.h"
	"include/gotchangpdf/c_stream_object.h"
	"include/gotchangpdf/c_string_object.h"
	"include/gotchangpdf/c_values.h"
	"include/gotchangpdf/c_version.h"
	"include/gotchangpdf/c_viewer_preferences.h"
	"include/gotchangpdf/c_xref.h"
	"include/gotchangpdf/gotchangpdf.h"
)

set(SEMANTICS_HEADERS
	src/gotchangpdf/semantics/documents/catalog.h
	src/gotchangpdf/semantics/documents/content_stream.h
	src/gotchangpdf/semantics/documents/contents.h
	src/gotchangpdf/semantics/documents/document.h
	src/gotchangpdf/semantics/documents/page_node.h
	src/gotchangpdf/semantics/documents/page_node_utils.h
	src/gotchangpdf/semantics/documents/page_object.h
	src/gotchangpdf/semantics/documents/page_tree.h
	src/gotchangpdf/semantics/documents/page_tree_node.h
	src/gotchangpdf/semantics/documents/resource_dictionary.h
	
	src/gotchangpdf/semantics/objects/annotations.h
	src/gotchangpdf/semantics/objects/character_map.h
	src/gotchangpdf/semantics/objects/date.h
	src/gotchangpdf/semantics/objects/destinations.h
	src/gotchangpdf/semantics/objects/developer_extensions.h
	src/gotchangpdf/semantics/objects/document_info.h
	src/gotchangpdf/semantics/objects/font.h
	src/gotchangpdf/semantics/objects/high_level_object.h
	src/gotchangpdf/semantics/objects/name_dictionary.h
	src/gotchangpdf/semantics/objects/outline.h
	src/gotchangpdf/semantics/objects/page_labels.h
	src/gotchangpdf/semantics/objects/rectangle.h
	src/gotchangpdf/semantics/objects/tree.h
	src/gotchangpdf/semantics/objects/viewer_preferences.h
	
	src/gotchangpdf/semantics/utils/semantic_exceptions.h
	src/gotchangpdf/semantics/utils/semantic_utils.h
	src/gotchangpdf/semantics/utils/semantics_fwd.h
)

set(SYNTAX_HEADERS
	src/gotchangpdf/syntax/exceptions/exception.h
	
	src/gotchangpdf/syntax/files/file.h
	src/gotchangpdf/syntax/files/file_writer.h
	src/gotchangpdf/syntax/files/header.h
	src/gotchangpdf/syntax/files/xref.h
	src/gotchangpdf/syntax/files/xref_chain.h
	
	src/gotchangpdf/syntax/filters/ascii_85_decode_filter.h
	src/gotchangpdf/syntax/filters/ascii_hex_decode_filter.h
	src/gotchangpdf/syntax/filters/dct_decode_filter.h
	src/gotchangpdf/syntax/filters/filter.h
	src/gotchangpdf/syntax/filters/flate_decode_filter.h
	
	src/gotchangpdf/syntax/objects/array_object.h
	src/gotchangpdf/syntax/objects/boolean_object.h
	src/gotchangpdf/syntax/objects/containable.h
	src/gotchangpdf/syntax/objects/dictionary_object.h
	src/gotchangpdf/syntax/objects/indirect_object_reference.h
	src/gotchangpdf/syntax/objects/integer_object.h
	src/gotchangpdf/syntax/objects/name_object.h
	src/gotchangpdf/syntax/objects/null_object.h
	src/gotchangpdf/syntax/objects/numeric_object.h
	src/gotchangpdf/syntax/objects/object.h
	src/gotchangpdf/syntax/objects/object_utils.h
	src/gotchangpdf/syntax/objects/objects.h
	src/gotchangpdf/syntax/objects/real_object.h
	src/gotchangpdf/syntax/objects/stream_object.h
	src/gotchangpdf/syntax/objects/string_object.h
	
	src/gotchangpdf/syntax/parsers/character_map_data.h
	src/gotchangpdf/syntax/parsers/lexical_tree.h
	src/gotchangpdf/syntax/parsers/object_stream_entry.h
	src/gotchangpdf/syntax/parsers/parser.h
	src/gotchangpdf/syntax/parsers/parser_interface.h
	src/gotchangpdf/syntax/parsers/reverse_parser.h
	src/gotchangpdf/syntax/parsers/reverse_tokenizer.h
	src/gotchangpdf/syntax/parsers/token.h
	src/gotchangpdf/syntax/parsers/token_dictionary.h
	src/gotchangpdf/syntax/parsers/tokenizer.h
	
	src/gotchangpdf/syntax/parsers/contents/content_stream_instruction_base.h
	src/gotchangpdf/syntax/parsers/contents/content_stream_objects.h
	src/gotchangpdf/syntax/parsers/contents/content_stream_operation_base.h
	src/gotchangpdf/syntax/parsers/contents/content_stream_operation_generic.h
	src/gotchangpdf/syntax/parsers/contents/content_stream_operations.h
	src/gotchangpdf/syntax/parsers/contents/content_stream_operators.h
	src/gotchangpdf/syntax/parsers/contents/content_utils.h
	
	src/gotchangpdf/syntax/streams/raw_base_stream.h
	src/gotchangpdf/syntax/streams/raw_reverse_stream.h
	src/gotchangpdf/syntax/streams/raw_stream.h
	
	src/gotchangpdf/syntax/utils/buffer.h
	src/gotchangpdf/syntax/utils/character.h
	src/gotchangpdf/syntax/utils/constants.h
	src/gotchangpdf/syntax/utils/deferred.h
	src/gotchangpdf/syntax/utils/encryption.h
	src/gotchangpdf/syntax/utils/file_device.h
	src/gotchangpdf/syntax/utils/log.h
	src/gotchangpdf/syntax/utils/modify_observer_interface.h
	src/gotchangpdf/syntax/utils/output_pointer.h
	src/gotchangpdf/syntax/utils/pkcs12_key.h
	src/gotchangpdf/syntax/utils/syntax_fwd.h
	src/gotchangpdf/syntax/utils/unknown_interface.h
	src/gotchangpdf/syntax/utils/util.h
	src/gotchangpdf/syntax/utils/version.h
	src/gotchangpdf/syntax/utils/zlib_wrapper.h
)

set(IMPLEMENTATION_HELPER_HEADER
	src/gotchangpdf/implementation/helper.h
)

set(PRECOMPILED_HEADER
	src/gotchangpdf/precompiled.h
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

find_package (openssl REQUIRED)
find_package (zlib)
find_package (jpeg)

SET(GOTCHANGPDF_LIBS kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32)

if (OPENSSL_FOUND)
	include_directories(${OPENSSL_INCLUDE_DIR})
	set (GOTCHANGPDF_LIBS ${OPENSSL_LIBRARIES} ${GOTCHANGPDF_LIBS})
	
	message(STATUS "OpenSSL ${OPENSSL_VERSION} was found in ${OPENSSL_ROOT_DIR}")
endif (OPENSSL_FOUND)

if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
	set (GOTCHANGPDF_LIBS ${ZLIB_LIBRARIES} ${GOTCHANGPDF_LIBS})
	
	message(STATUS "Zlib ${ZLIB_VERSION_STRING} was found in ${ZLIB_LIBRARIES}")
endif (ZLIB_FOUND)

if (JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIR})
	set (GOTCHANGPDF_LIBS ${JPEG_LIBRARIES} ${GOTCHANGPDF_LIBS})
	
	message(STATUS "Jpeg library was found")
endif (JPEG_FOUND)

target_link_libraries (gotchangpdf ${GOTCHANGPDF_LIBS})