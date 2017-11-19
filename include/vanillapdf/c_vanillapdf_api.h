#ifndef _C_VANILLAPDF_API_H
#define _C_VANILLAPDF_API_H

/**
* \file c_vanillapdf_api.h
* This file contains the entire available API merged in to single file.
*/

/* general utility */
#include "vanillapdf/c_export.h"
#include "vanillapdf/c_values.h"
#include "vanillapdf/c_handles.h"

/* syntax */
#include "vanillapdf/syntax/c_file.h"
#include "vanillapdf/syntax/c_file_writer.h"
#include "vanillapdf/syntax/c_xref.h"
#include "vanillapdf/syntax/c_filter.h"

#include "vanillapdf/syntax/c_object.h"
#include "vanillapdf/syntax/c_array_object.h"
#include "vanillapdf/syntax/c_boolean_object.h"
#include "vanillapdf/syntax/c_real_object.h"
#include "vanillapdf/syntax/c_string_object.h"
#include "vanillapdf/syntax/c_stream_object.h"
#include "vanillapdf/syntax/c_dictionary_object.h"
#include "vanillapdf/syntax/c_indirect_object_reference.h"
#include "vanillapdf/syntax/c_integer_object.h"
#include "vanillapdf/syntax/c_name_object.h"

/* semantics */
#include "vanillapdf/semantics/c_document.h"
#include "vanillapdf/semantics/c_document_info.h"
#include "vanillapdf/semantics/c_document_signature_settings.h"
#include "vanillapdf/semantics/c_catalog.h"
#include "vanillapdf/semantics/c_page_tree.h"
#include "vanillapdf/semantics/c_page_object.h"
#include "vanillapdf/semantics/c_developer_extensions.h"
#include "vanillapdf/semantics/c_page_labels.h"
#include "vanillapdf/semantics/c_viewer_preferences.h"
#include "vanillapdf/semantics/c_outline.h"
#include "vanillapdf/semantics/c_destinations.h"
#include "vanillapdf/semantics/c_annotations.h"
#include "vanillapdf/semantics/c_resource_dictionary.h"
#include "vanillapdf/semantics/c_interactive_forms.h"
#include "vanillapdf/semantics/c_fields.h"
#include "vanillapdf/semantics/c_digital_signature.h"
#include "vanillapdf/semantics/c_font.h"
#include "vanillapdf/semantics/c_font_map.h"
#include "vanillapdf/semantics/c_character_map.h"
#include "vanillapdf/semantics/c_date.h"
#include "vanillapdf/semantics/c_rectangle.h"
#include "vanillapdf/semantics/c_contents.h"

/* contents */
#include "vanillapdf/contents/c_content_object.h"
#include "vanillapdf/contents/c_content_operator.h"
#include "vanillapdf/contents/c_content_operation.h"
#include "vanillapdf/contents/c_content_instruction.h"

/* utils */
#include "vanillapdf/utils/c_unknown_interface.h"
#include "vanillapdf/utils/c_encryption.h"
#include "vanillapdf/utils/c_buffer.h"
#include "vanillapdf/utils/c_logging.h"
#include "vanillapdf/utils/c_pdf_version.h"
#include "vanillapdf/utils/c_library_info.h"
#include "vanillapdf/utils/c_license_info.h"
#include "vanillapdf/utils/c_errors.h"
#include "vanillapdf/utils/c_pkcs12_key.h"
#include "vanillapdf/utils/c_signing_key.h"
#include "vanillapdf/utils/c_input_stream_interface.h"
#include "vanillapdf/utils/c_output_stream_interface.h"
#include "vanillapdf/utils/c_message_digest_algorithm.h"
#include "vanillapdf/utils/c_name_constants.h"

#endif /* _C_VANILLAPDF_API_H */
