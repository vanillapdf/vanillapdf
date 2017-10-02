#ifndef _C_VANILLAPDF_API_H
#define _C_VANILLAPDF_API_H

/**
* \file c_vanillapdf_api.h
* This file contains the entire available API merged in to single file.
*
* Additionaly it is a host for all the documentation group declarations.
*/

/* export rules */
#include "c_export.h"

/* return values */
#include "c_values.h"

/* forward declarations */
#include "c_handles.h"

/* files */
#include "c_file.h"
#include "c_file_writer.h"
#include "c_xref.h"
#include "c_encryption.h"

/* documents */
#include "c_document.h"
#include "c_document_info.h"
#include "c_document_signature_settings.h"
#include "c_catalog.h"
#include "c_page_tree.h"
#include "c_page_object.h"
#include "c_developer_extensions.h"
#include "c_page_labels.h"
#include "c_viewer_preferences.h"
#include "c_outline.h"
#include "c_destinations.h"
#include "c_annotations.h"
#include "c_resource_dictionary.h"
#include "c_interactive_forms.h"
#include "c_fields.h"
#include "c_digital_signature.h"
#include "c_message_digest_algorithm.h"

/* fonts */
#include "c_font.h"
#include "c_font_map.h"
#include "c_character_map.h"

/* common data structures */
#include "c_date.h"
#include "c_rectangle.h"

/* contents */
#include "c_contents.h"

/* objects */
#include "c_object.h"
#include "c_array_object.h"
#include "c_boolean_object.h"
#include "c_real_object.h"
#include "c_string_object.h"
#include "c_stream_object.h"
#include "c_dictionary_object.h"
#include "c_indirect_object_reference.h"
#include "c_integer_object.h"
#include "c_name_object.h"

/* utils */
#include "c_unknown_interface.h"
#include "c_buffer.h"
#include "c_logging.h"
#include "c_pdf_version.h"
#include "c_library_info.h"
#include "c_license_info.h"
#include "c_errors.h"
#include "c_pkcs12_key.h"
#include "c_signing_key.h"

#endif /* _C_VANILLAPDF_API_H */
