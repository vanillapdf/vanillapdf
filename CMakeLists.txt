# Require minimum CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Define main project
PROJECT(gotchangpdf)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(src/gotchangpdf)
INCLUDE_DIRECTORIES(src/gotchangpdf/implementation)
INCLUDE_DIRECTORIES(src/gotchangpdf/semantics/documents)
INCLUDE_DIRECTORIES(src/gotchangpdf/semantics/objects)
INCLUDE_DIRECTORIES(src/gotchangpdf/semantics/utils)

INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/exceptions)
INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/files)
INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/filters)
INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/objects)
INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/parsers)
INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/parsers/contents)
INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/streams)
INCLUDE_DIRECTORIES(src/gotchangpdf/syntax/utils)

message("test message")
message(${CMAKE_BUILD_TYPE})

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("debug mode")
	add_definitions(-DDEBUG)
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
    message("release mode")
	add_definitions(-DRELEASE)
	add_definitions(-DNDEBUG)
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

SET(GOTCHANGPDF_PRECOMPILED_SOURCE
	src/gotchangpdf/precompiled.cpp
)

SET(GOTCHANGPDF_C_IMPLEMENTATION_SOURCES
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

SET(GOTCHANGPDF_SEMANTICS_SOURCES
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

SET(GOTCHANGPDF_SYNTAX_SOURCES
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

SET(INCLUDE_HEADERS
	include/gotchangpdf/c_annotations.h
	include/gotchangpdf/c_array_object.h
	include/gotchangpdf/c_boolean_object.h
	include/gotchangpdf/c_buffer.h
	include/gotchangpdf/c_catalog.h
	include/gotchangpdf/c_character_map.h
	include/gotchangpdf/c_contents.h
	include/gotchangpdf/c_date.h
	include/gotchangpdf/c_destinations.h
	include/gotchangpdf/c_developer_extensions.h
	include/gotchangpdf/c_dictionary_object.h
	include/gotchangpdf/c_document.h
	include/gotchangpdf/c_document_info.h
	include/gotchangpdf/c_encryption.h
	include/gotchangpdf/c_export.h
	include/gotchangpdf/c_file.h
	include/gotchangpdf/c_font.h
	include/gotchangpdf/c_font_map.h
	include/gotchangpdf/c_handles.h
	include/gotchangpdf/c_indirect_object_reference.h
	include/gotchangpdf/c_integer_object.h
	include/gotchangpdf/c_name_object.h
	include/gotchangpdf/c_object.h
	include/gotchangpdf/c_outline.h
	include/gotchangpdf/c_page_labels.h
	include/gotchangpdf/c_page_object.h
	include/gotchangpdf/c_page_tree.h
	include/gotchangpdf/c_real_object.h
	include/gotchangpdf/c_rectangle.h
	include/gotchangpdf/c_resource_dictionary.h
	include/gotchangpdf/c_stream_object.h
	include/gotchangpdf/c_string_object.h
	include/gotchangpdf/c_values.h
	include/gotchangpdf/c_version.h
	include/gotchangpdf/c_viewer_preferences.h
	include/gotchangpdf/c_xref.h
	include/gotchangpdf/gotchangpdf.h
)

SET(SEMANTICS_HEADERS
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

SET(SYNTAX_HEADERS
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

SET(IMPLEMENTATION_HELPER_HEADER
	src/gotchangpdf/implementation/helper.h
)

SET(PRECOMPILED_HEADER
	src/gotchangpdf/precompiled.h
)

SOURCE_GROUP("Header Files" FILES
	${PRECOMPILED_HEADER}
)

SOURCE_GROUP("Header Files\\include" FILES
	${INCLUDE_HEADERS}
)

SOURCE_GROUP("Header Files\\semantics" FILES
	${SEMANTICS_HEADERS}
)

SOURCE_GROUP("Header Files\\syntax" FILES
	${SYNTAX_HEADERS}
)

SOURCE_GROUP("Source Files\\implementation" FILES
	${GOTCHANGPDF_C_IMPLEMENTATION_SOURCES}
	${IMPLEMENTATION_HELPER_HEADER}
)

SOURCE_GROUP("Source Files\\syntax" FILES
	${GOTCHANGPDF_SYNTAX_SOURCES}
)

SOURCE_GROUP("Source Files\\semantics" FILES
	${GOTCHANGPDF_SEMANTICS_SOURCES}
)

INSTALL(FILES ${INCLUDE_HEADERS}
  DESTINATION "include\\gotchangpdf"
)

add_library(gotchangpdf
	${GOTCHANGPDF_C_IMPLEMENTATION_SOURCES}
	${GOTCHANGPDF_SEMANTICS_SOURCES}
	${GOTCHANGPDF_SYNTAX_SOURCES}
	${GOTCHANGPDF_PRECOMPILED_SOURCE}
	${PRECOMPILED_HEADER}
	${INCLUDE_HEADERS}
	${SEMANTICS_HEADERS}
	${SYNTAX_HEADERS}
)