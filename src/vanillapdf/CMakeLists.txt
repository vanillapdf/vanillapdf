project(vanillapdf LANGUAGES CXX)

# Include listing of all interface files
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/include/files.cmake")

# Include local files in the directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

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
    implementation/utils/c_misc_utils.cpp
    implementation/utils/c_object_diagnostics.cpp
    implementation/utils/c_output_stream.cpp
    implementation/utils/c_memory_buffer_output_stream.cpp
    implementation/utils/c_pkcs12_key.cpp
    implementation/utils/c_signature_verifier.cpp
    implementation/utils/c_signature_verification_settings.cpp
    implementation/utils/c_signing_key.cpp
    implementation/utils/c_text_string_encoding.cpp
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
    implementation/syntax/c_file_structure_validator.cpp
    implementation/syntax/c_object_attributes.cpp
    implementation/syntax/c_object_attribute_list.cpp
)

set(VANILLAPDF_C_IMPLEMENTATION_SEMANTICS_SOURCES
    implementation/semantics/c_actions.cpp
    implementation/semantics/c_annotations.cpp
    implementation/semantics/c_appearance_characteristics.cpp
    implementation/semantics/c_byte_range.cpp
    implementation/semantics/c_catalog.cpp
    implementation/semantics/c_character_map.cpp
    implementation/semantics/c_color.cpp
    implementation/semantics/c_date.cpp
    implementation/semantics/c_destination_name_tree.cpp
    implementation/semantics/c_destinations.cpp
    implementation/semantics/c_developer_extensions.cpp
    implementation/semantics/c_digital_signature.cpp
    implementation/semantics/c_digital_signature_extensions.cpp
    implementation/semantics/c_document.cpp
    implementation/semantics/c_document_encryption_settings.cpp
    implementation/semantics/c_document_info.cpp
    implementation/semantics/c_document_signature_settings.cpp
    implementation/semantics/c_fields.cpp
    implementation/semantics/c_field_tree.cpp
    implementation/semantics/c_font.cpp
    implementation/semantics/c_font_map.cpp
    implementation/semantics/c_interactive_forms.cpp
    implementation/semantics/c_matrix.cpp
    implementation/semantics/c_name_dictionary.cpp
    implementation/semantics/c_outline.cpp
    implementation/semantics/c_page_contents.cpp
    implementation/semantics/c_page_labels.cpp
    implementation/semantics/c_page_object.cpp
    implementation/semantics/c_page_tree.cpp
    implementation/semantics/c_rectangle.cpp
    implementation/semantics/c_resource_dictionary.cpp
    implementation/semantics/c_signature_flags.cpp
    implementation/semantics/c_viewer_preferences.cpp
    implementation/semantics/c_xobject.cpp
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

    semantics/objects/actions.cpp
    semantics/objects/annotations.cpp
    semantics/objects/appearance_characteristics.cpp
    semantics/objects/character_map.cpp
    semantics/objects/color.cpp
    semantics/objects/date.cpp
    semantics/objects/destinations.cpp
    semantics/objects/developer_extensions.cpp
    semantics/objects/digital_signature.cpp
    semantics/objects/document_info.cpp
    semantics/objects/font.cpp
    semantics/objects/outline.cpp
    semantics/objects/page_labels.cpp
    semantics/objects/matrix.cpp
    semantics/objects/rectangle.cpp
    semantics/objects/tree.cpp
    semantics/objects/xobject.cpp
    semantics/objects/name_dictionary.cpp
    semantics/objects/fields.cpp
    semantics/objects/field_tree.cpp
    semantics/objects/interactive_forms.cpp
    semantics/objects/viewer_preferences.cpp
    semantics/objects/page_node_base.cpp

    semantics/extensions/digital_signature_extensions.cpp

    semantics/utils/byte_range.cpp
    semantics/utils/document_encryption_settings.cpp

    semantics/utils/document_signer.cpp
    semantics/utils/document_signature_settings.cpp
    semantics/utils/semantic_utils.cpp
)

set(VANILLAPDF_SYNTAX_SOURCES
    syntax/exceptions/syntax_exceptions.cpp

    syntax/files/file.cpp
    syntax/files/file_writer.cpp
    syntax/files/file_structure_issue.cpp
    syntax/files/file_structure_validation_result.cpp
    syntax/files/file_structure_validator.cpp
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
    utils/crypto_utils.cpp
    utils/errors.cpp
    utils/exceptions.cpp
    utils/library_info.cpp
    utils/license_info.cpp
    utils/log.cpp
    utils/misc_utils.cpp
    utils/pkcs12_key.cpp
    utils/resource.cpp
    utils/signature_verification_result.cpp
    utils/signature_verifier.cpp
    utils/text_string_encoding.cpp
    utils/time_utils.cpp
    utils/trusted_certificate_store.cpp
    utils/unknown_interface.cpp
    utils/versionable.cpp
    utils/windows_utils.cpp
    utils/zlib_wrapper.cpp

    utils/streams/input_reverse_stream.cpp
    utils/streams/input_stream.cpp
    utils/streams/output_stream.cpp
    utils/streams/stream_utils.cpp
    utils/streams/memory_buffer_input_stream.cpp
    utils/streams/memory_buffer_output_stream.cpp
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

    semantics/objects/actions.h
    semantics/objects/annotations.h
    semantics/objects/appearance_characteristics.h
    semantics/objects/character_map.h
    semantics/objects/color.h
    semantics/objects/date.h
    semantics/objects/destinations.h
    semantics/objects/developer_extensions.h
    semantics/objects/digital_signature.h
    semantics/objects/document_info.h
    semantics/objects/fields.h
    semantics/objects/field_tree.h
    semantics/objects/font.h
    semantics/objects/interactive_forms.h
    semantics/objects/high_level_object.h
    semantics/objects/name_dictionary.h
    semantics/objects/outline.h
    semantics/objects/page_labels.h
    semantics/objects/matrix.h
    semantics/objects/rectangle.h
    semantics/objects/tree.h
    semantics/objects/xobject.h
    semantics/objects/viewer_preferences.h

    semantics/extensions/digital_signature_extensions.h

    semantics/utils/byte_range.h
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
    syntax/files/file_structure_issue.h
    syntax/files/file_structure_validation_result.h
    syntax/files/file_structure_validator.h
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
    utils/byte_order.h
    utils/cached_value.h
    utils/character.h
    utils/compiler_utils.h
    utils/constants.h
    utils/conversion_utils.h
    utils/crypto_utils.h
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
    utils/observable_interface.h
    utils/versionable.h
    utils/misc_utils.h
    utils/pdf_version.h
    utils/pkcs12_key.h
    utils/resource.h
    utils/signature_verification_result.h
    utils/signature_verifier.h
    utils/signature_verifier_interface.h
    utils/stack_allocator.h
    utils/template_utils.h
    utils/text_string_encoding.h
    utils/time_utils.h
    utils/trusted_certificate_store.h
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
    utils/streams/memory_buffer_input_stream.h
    utils/streams/memory_buffer_output_stream.h
    utils/streams/memory_buffer_input_output_stream.h

    utils/streams/input_output_stream_interface.h
    utils/streams/input_output_stream.h

    utils/streams/io_strategy.h
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

# vanillapdf supports both SHARED and STATIC builds,
# which is being configured using BUILD_SHARED_LIBS.
add_library(vanillapdf
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

# Create alias target for FetchContent and sub-project usage
# TODO: Remove this alias and rename target to vanillapdf::vanillapdf for release 3.0
# See GitHub issue #140: https://github.com/vanillapdf/vanillapdf/issues/140
add_library(vanillapdf::vanillapdf ALIAS vanillapdf)

set_target_properties(vanillapdf PROPERTIES
  PREFIX "lib"
)

# Add precompiled header to speed up the compilation process
if(VANILLAPDF_PRECOMPILED_HEADER)
    target_precompile_headers(vanillapdf PRIVATE ${VANILLAPDF_PRECOMPILED_HEADER})
endif()

if(MSVC)

    # Add natvis file embedding
    target_link_options(vanillapdf PRIVATE "/NATVIS:${CMAKE_CURRENT_SOURCE_DIR}/public.natvis")

    # Disable warning about unused parameters
    target_compile_options(vanillapdf PRIVATE "/wd4100")

endif(MSVC)

if (MSVC AND BUILD_SHARED_LIBS)

    # Enable PDB generation for debug and release configurations
    target_compile_options(vanillapdf PRIVATE "/Zi")
    target_link_options(vanillapdf PRIVATE "/DEBUG")

endif()

# Add support for static linking also with GCC and clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(VANILLAPDF_USE_STATIC_CRT)
        target_link_options(vanillapdf PRIVATE -static-libstdc++ -static-libgcc)
    endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" AND "${CXX_COMPILER_VERSION}" VERSION_LESS "9.1")
    # C++17 filesystem implementation exception for GCC
    # https://en.cppreference.com/w/cpp/filesystem#Notes

    # Quote from link above:
    # Using this library may require additional compiler/linker options.
    # GNU implementation prior to 9.1 requires linking with -lstdc++fs and LLVM implementation prior to LLVM 9.0 requires linking with -lc++fs.

    target_link_libraries(vanillapdf PRIVATE stdc++fs)
endif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" AND "${CXX_COMPILER_VERSION}" VERSION_LESS "9.1")

# In case we have the dynamic link configuration add specific definitions
if(BUILD_SHARED_LIBS)

    target_compile_definitions(vanillapdf
        PUBLIC VANILLAPDF_CONFIGURATION_DLL
        PRIVATE VANILLAPDF_EXPORTS)

endif()

# Specify version identificators
target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_VERSION_MAJOR=${VANILLAPDF_VERSION_MAJOR})
target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_VERSION_MINOR=${VANILLAPDF_VERSION_MINOR})
target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_VERSION_PATCH=${VANILLAPDF_VERSION_PATCH})
target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_VERSION_BUILD=${VANILLAPDF_VERSION_BUILD})

# Specify build time identificators
string(TIMESTAMP VANILLAPDF_BUILD_DAY	"%d" UTC)
string(TIMESTAMP VANILLAPDF_BUILD_MONTH	"%m" UTC)
string(TIMESTAMP VANILLAPDF_BUILD_YEAR	"%Y" UTC)

# Strip leading zeros from date values
# GCC throws an error having month "09" instead of "9"
string(REGEX REPLACE "^0*([0-9]+)" "\\1" VANILLAPDF_BUILD_DAY		"${VANILLAPDF_BUILD_DAY}")
string(REGEX REPLACE "^0*([0-9]+)" "\\1" VANILLAPDF_BUILD_MONTH		"${VANILLAPDF_BUILD_MONTH}")
string(REGEX REPLACE "^0*([0-9]+)" "\\1" VANILLAPDF_BUILD_YEAR		"${VANILLAPDF_BUILD_YEAR}")

target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_BUILD_DAY=${VANILLAPDF_BUILD_DAY})
target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_BUILD_MONTH=${VANILLAPDF_BUILD_MONTH})
target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_BUILD_YEAR=${VANILLAPDF_BUILD_YEAR})

# Check for compiler features and include compile definitions
vanillapdf_check_standard_features(vanillapdf)

# Configure default compilation parameters, strict warnings, config based definition for DEBUG/RELEASE, etc.
vanillapdf_target_compile_defaults(vanillapdf)

# Configure MSVC runtime library for vanillapdf target only
# Skip if already set externally (e.g. vcpkg toolchain, user, parent project)
if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    vanillapdf_configure_msvc_runtime(vanillapdf)
endif()

# In case we would like to extract code coverage report enable the flag
if(VANILLAPDF_ENABLE_COVERAGE)
    enable_coverage_for_target(vanillapdf)
endif()

# Optional sanitizers
enable_asan_for_target(vanillapdf)
enable_ubsan_for_target(vanillapdf)
enable_tsan_for_target(vanillapdf)

# Optional fuzzer coverage instrumentation
enable_fuzzer_coverage_for_target(vanillapdf)

# Update 30.04.2025
# We have switched to vcpkg manifest mode, which makes sure all dependencies are automatically installed

# Prevent POSIX thread detection on MSVC
# LINK : fatal error LNK1104: cannot open file 'pthread.lib'
if (MSVC AND NOT BUILD_SHARED_LIBS)
    set(CMAKE_THREAD_PREFER_PTHREAD OFF)
    set(THREADS_PREFER_PTHREAD_FLAG OFF)
endif()

if (VANILLAPDF_ENABLE_ENCRYPTION)
    if (VANILLAPDF_EXTERNAL_OPENSSL)
        find_package(OpenSSL MODULE REQUIRED)
    else ()
        find_package(OpenSSL REQUIRED)
    endif()
endif()

if (VANILLAPDF_ENABLE_ZLIB)
    if (VANILLAPDF_EXTERNAL_ZLIB)
        find_package(ZLIB MODULE REQUIRED)
    else ()
        find_package(ZLIB REQUIRED)
    endif()
endif()

if (VANILLAPDF_ENABLE_JPEG)
    if (VANILLAPDF_EXTERNAL_JPEG)
        find_package(JPEG MODULE REQUIRED)
    else ()
        find_package(JPEG REQUIRED)
    endif()
endif()

if (VANILLAPDF_ENABLE_JPEG2000)
    if (VANILLAPDF_EXTERNAL_OPENJPEG)
        find_package(OpenJPEG CONFIG REQUIRED)
    else ()
        find_package(OpenJPEG CONFIG REQUIRED)
    endif()
endif()

if (VANILLAPDF_EXTERNAL_SPDLOG)
    find_package(spdlog CONFIG REQUIRED)
else ()
    find_package(spdlog CONFIG REQUIRED)
endif()

if (VANILLAPDF_ENABLE_LICENSING)
    if (VANILLAPDF_EXTERNAL_NLOHMANN_JSON)
        find_package(nlohmann_json CONFIG REQUIRED)
    else ()
        find_package(nlohmann_json CONFIG REQUIRED)
    endif()
endif()

if (COMPILE_SKIA)
    find_package(unofficial-skia CONFIG REQUIRED)
    target_link_libraries(vanillapdf PRIVATE unofficial::skia::skia)
endif (COMPILE_SKIA)

if (VANILLAPDF_ENABLE_ENCRYPTION AND OPENSSL_FOUND)
    target_link_libraries(vanillapdf PRIVATE OpenSSL::Crypto)
    target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_HAVE_OPENSSL)

    message(STATUS "OpenSSL ${OPENSSL_VERSION} was found")
elseif (VANILLAPDF_ENABLE_ENCRYPTION)
    message(FATAL_ERROR "Encryption enabled but OpenSSL not found. Please install OpenSSL or disable encryption with -DVANILLAPDF_ENABLE_ENCRYPTION=OFF")
else()
    message(STATUS "Encryption support disabled. Encrypted documents will not be supported")
endif ()

if (VANILLAPDF_ENABLE_JPEG AND JPEG_FOUND)
    target_link_libraries(vanillapdf PRIVATE JPEG::JPEG)
    target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_HAVE_JPEG)

    message(STATUS "JPEG library ${JPEG_VERSION} was found")
elseif (VANILLAPDF_ENABLE_JPEG)
    message(FATAL_ERROR "JPEG enabled but JPEG library not found. Please install JPEG library or disable JPEG support with -DVANILLAPDF_ENABLE_JPEG=OFF")
else()
    message(STATUS "JPEG support disabled. JPEG images will not be supported")
endif ()

if (VANILLAPDF_ENABLE_ZLIB AND ZLIB_FOUND)
    target_link_libraries(vanillapdf PRIVATE ZLIB::ZLIB)
    target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_HAVE_ZLIB)
    message(STATUS "zlib ${ZLIB_VERSION_STRING} was found")
elseif (VANILLAPDF_ENABLE_ZLIB)
    message(FATAL_ERROR "ZLIB enabled but not found. Please install ZLIB or disable zlib support with -DVANILLAPDF_ENABLE_ZLIB=OFF")
else()
    message(STATUS "zlib support disabled. zlib decompression will be unavailable")
endif()

if (VANILLAPDF_ENABLE_JPEG2000 AND TARGET openjp2)
    target_link_libraries(vanillapdf PRIVATE openjp2)
    target_include_directories(vanillapdf PRIVATE ${OPENJPEG_INCLUDE_DIRS})
    target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_HAVE_OPENJPEG)
    message(STATUS "OpenJPEG ${OPENJPEG_MAJOR_VERSION}.${OPENJPEG_MINOR_VERSION} found and linked")
elseif (VANILLAPDF_ENABLE_JPEG2000)
    message(FATAL_ERROR "JPEG2000 enabled but OpenJPEG not found. Please install OpenJPEG or disable JPEG2000 support with -DVANILLAPDF_ENABLE_JPEG2000=OFF")
else()
    message(STATUS "JPEG2000 support disabled. JPEG2000 images will not be supported")
endif ()

# fmtlib is used directly in source files for string formatting.
# Do NOT link fmt::fmt explicitly here. spdlog built with SPDLOG_FMT_EXTERNAL
# declares fmt::fmt as a PUBLIC interface dependency (by design — spdlog headers
# include fmt headers, so consumers need fmt in scope). Linking fmt explicitly
# alongside spdlog::spdlog duplicates it on the linker command line, which
# Apple ld >= 15 warns about and -Wl,-fatal_warnings turns into an error.
#
# TODO: if spdlog is ever made optional or removed, add an explicit
# target_link_libraries(vanillapdf PRIVATE fmt::fmt) here so that the direct
# fmt usage in vanillapdf source files remains covered.
if (VANILLAPDF_EXTERNAL_FMT)
    find_package(fmt CONFIG REQUIRED)
else ()
    find_package(fmt CONFIG REQUIRED)
endif()

target_link_libraries(vanillapdf PRIVATE spdlog::spdlog)

if (VANILLAPDF_ENABLE_LICENSING)
    target_link_libraries(vanillapdf PRIVATE nlohmann_json::nlohmann_json)
    target_compile_definitions(vanillapdf PRIVATE VANILLAPDF_ENABLE_LICENSING)
    message(STATUS "Licensing support enabled (nlohmann-json)")
else()
    message(STATUS "Licensing support disabled")
endif()

# This includes the public headers in the project as well as publishing them outside to be consumed
target_include_directories(vanillapdf PUBLIC
    $<BUILD_INTERFACE:${VANILLAPDF_SOLUTION_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Project install targets
install(TARGETS vanillapdf
    EXPORT vanillapdfTargets
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT Runtime
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT Runtime
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT Development
)

if (MSVC AND BUILD_SHARED_LIBS)

    # The PDB belongs next to the DLL it describes. Package managers that keep
    # debug and release side by side in a single tree (vcpkg, conan) produce that
    # split by installing each configuration into its own prefix, so the
    # destination must not encode it. Hardcoding "debug/bin" here made vcpkg
    # place the debug PDB in "debug/debug/bin"
    # See https://github.com/vanillapdf/vanillapdf/issues/484
    #
    # $<TARGET_PDB_FILE:...> resolves per configuration, so a single rule covers
    # all of them, including custom ones. The rule is deliberately not OPTIONAL,
    # a missing PDB means an incomplete package and has to fail the installation
    # instead of shipping silently.
    # Own component so consumers that reject debug symbols can skip them.
    install(FILES
        "$<TARGET_PDB_FILE:vanillapdf>"
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Symbols
    )

endif()

install(FILES ${VANILLAPDF_INCLUDE_HEADERS} DESTINATION "include/vanillapdf" COMPONENT Development)
install(FILES ${VANILLAPDF_INCLUDE_UTILS_HEADERS} DESTINATION "include/vanillapdf/utils" COMPONENT Development)
install(FILES ${VANILLAPDF_INCLUDE_SYNTAX_HEADERS} DESTINATION "include/vanillapdf/syntax" COMPONENT Development)
install(FILES ${VANILLAPDF_INCLUDE_SEMANTICS_HEADERS} DESTINATION "include/vanillapdf/semantics" COMPONENT Development)
install(FILES ${VANILLAPDF_INCLUDE_CONTENTS_HEADERS} DESTINATION "include/vanillapdf/contents" COMPONENT Development)

# Split because Apache-2.0 section 4(d) requires LICENSE and NOTICE to be
# redistributed, while the readme is informational. The destination is
# overridable for packagers that expect them elsewhere (Conan uses licenses/).
set(VANILLAPDF_INSTALL_LICENSEDIR "share/vanillapdf" CACHE STRING
    "Destination for LICENSE.txt and NOTICE.md, relative to the install prefix")

install(FILES
    "${VANILLAPDF_SOLUTION_SOURCE_DIR}/LICENSE.txt"
    "${VANILLAPDF_SOLUTION_SOURCE_DIR}/NOTICE.md"
    DESTINATION "${VANILLAPDF_INSTALL_LICENSEDIR}"
    COMPONENT Licenses
)

install(FILES
    "${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake/README.txt"
    DESTINATION "share/vanillapdf"
    COMPONENT Documentation
)

# Define installation files and procedures
include("${VANILLAPDF_SOLUTION_SOURCE_DIR}/cmake/vanillapdf_install.cmake")

# vcpkg's VCPKG_APPLOCAL_DEPS (ON by default) automatically copies linked
# DLL dependencies to the build output directory. Enable its install
# counterpart so that cmake --install also includes them.
if(WIN32 AND VANILLAPDF_INTERNAL_VCPKG)
    set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON CACHE BOOL
        "Automatically copy vcpkg dependencies into install target directory" FORCE)
endif()

# OpenSSL 3.x legacy provider module — runtime-loaded via OSSL_PROVIDER_load,
# not a linked dependency, so VCPKG_APPLOCAL_DEPS does not handle it.
# Neither vcpkg nor CMake's FindOpenSSL expose whether OpenSSL was built as
# shared or static (VCPKG_LIBRARY_LINKAGE is not available to consumer projects,
# and imported targets are created as UNKNOWN). The most reliable check is to
# look for the provider file directly. On static vcpkg triplets, providers are
# compiled into the library and no separate DLL exists, so find_file fails
# silently.
if(WIN32 AND VANILLAPDF_INTERNAL_VCPKG
        AND OPENSSL_FOUND AND NOT VANILLAPDF_EXTERNAL_OPENSSL
        AND OPENSSL_VERSION VERSION_GREATER_EQUAL "3.0.0")

    find_file(OPENSSL_LEGACY_PROVIDER
        NAMES legacy.dll
        PATHS "${OPENSSL_MODULES_DIR}"
              "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin"
        NO_DEFAULT_PATH
    )

    if(EXISTS "${OPENSSL_LEGACY_PROVIDER}")
        message(STATUS "OpenSSL legacy provider found at ${OPENSSL_LEGACY_PROVIDER}")

        add_custom_command(
            TARGET vanillapdf POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENSSL_LEGACY_PROVIDER}"
            "$<TARGET_FILE_DIR:vanillapdf>"
            COMMENT "Copying OpenSSL legacy provider module"
        )
        install(FILES "${OPENSSL_LEGACY_PROVIDER}" DESTINATION bin COMPONENT Runtime)

        # Set the modules directory so test targets can set OPENSSL_MODULES env var.
        # OpenSSL checks this env var at runtime to locate provider modules.
        # Users can override via -DVANILLAPDF_OPENSSL_MODULES_DIR=<path>.
        set(VANILLAPDF_OPENSSL_MODULES_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin"
            CACHE PATH "Directory containing OpenSSL provider modules")
    else()
        message(STATUS "OpenSSL legacy provider not found — assuming static linkage (providers compiled in)")
    endif()
endif()

# Add export verification test for shared library builds
# Only add the test if testing is enabled and we're building shared libraries
# TODO: Re-enable after fixing test infrastructure (https://github.com/vanillapdf/vanillapdf/issues/162)
# - Windows: dumpbin not available in CI runner PATH
# - Linux/macOS: Missing symbol visibility attributes cause C++ symbols to leak
if(FALSE AND VANILLAPDF_ENABLE_TESTS AND BUILD_SHARED_LIBS)

    # Find Python3 executable
    find_package(Python3 COMPONENTS Interpreter REQUIRED)

    # Add test that verifies exports after building vanillapdf
    add_test(
        NAME export_verification
        COMMAND ${Python3_EXECUTABLE} "${VANILLAPDF_SOLUTION_SOURCE_DIR}/scripts/verify_exports.py" "$<TARGET_FILE:vanillapdf>"
        WORKING_DIRECTORY ${VANILLAPDF_SOLUTION_SOURCE_DIR}
    )

    # Make sure the test runs after vanillapdf is built
    set_tests_properties(export_verification PROPERTIES
        DEPENDS vanillapdf
        LABELS "export;verification"
    )

    message(STATUS "Added export verification test using Python3: ${Python3_EXECUTABLE}")

elseif(VANILLAPDF_ENABLE_TESTS AND NOT BUILD_SHARED_LIBS)
    message(STATUS "Export verification test skipped for static builds")
endif()
