#ifndef _VANILLAPDF_TEST_H
#define _VANILLAPDF_TEST_H

#include "vanillapdf/c_vanillapdf_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

extern const int VANILLAPDF_TEST_ERROR_SUCCESS;
extern const int VANILLAPDF_TEST_ERROR_INVALID_PASSWORD;
extern const int VANILLAPDF_TEST_ERROR_INVALID_PARAMETERS;
extern const int VANILLAPDF_TEST_ERROR_LOGGING_ENABLED;
extern const int VANILLAPDF_TEST_ERROR_FAILURE;

/* Support functions */
boolean_type is_quiet_mode();
error_type set_quiet_mode(boolean_type value);
void print_spaces(int nested);
void print_text(const char * const format, ...);
error_type print_last_error();

/* Utilities */
error_type process_buffer(BufferHandle* buffer, int nested);
error_type process_version(PDFVersion version, int nested);
error_type process_library_info();
error_type process_license_info(string_type license_file);
error_type process_logging();
error_type process_constants();

/* Files */
error_type process_file(FileHandle* file, int nested);
error_type process_xref(XrefHandle* xref, int nested);

/* Objects */
error_type process_object(ObjectHandle* obj, int nested);
error_type process_name(NameObjectHandle* name, int nested);
error_type process_array(ArrayObjectHandle* arr, int nested);
error_type process_integer(IntegerObjectHandle* integer, int nested);
error_type process_boolean(BooleanObjectHandle* boolean, int nested);
error_type process_reference(IndirectReferenceObjectHandle* reference, int nested);
error_type process_real(RealObjectHandle* real, int nested);
error_type process_null(NullObjectHandle* obj, int nested);
error_type process_string(StringObjectHandle* string, int nested);
error_type process_dictionary(DictionaryObjectHandle* dictionary, int nested);
error_type process_stream(StreamObjectHandle* stream, int nested);

error_type process_lit_string(LiteralStringObjectHandle* string, int nested);
error_type process_hex_string(HexadecimalStringObjectHandle* string, int nested);

/* Contents */
error_type process_content_instruction(ContentInstructionHandle* obj, int nested);
error_type process_content_object(ContentObjectHandle* obj, int nested);
error_type process_content_operation(ContentOperationHandle* obj, int nested);
error_type process_page_contents(PageContentsHandle* obj, int nested);

error_type process_content_object_text(ContentObjectTextHandle* obj, int nested);
error_type process_content_object_inline_image(ContentObjectInlineImageHandle* obj, int nested);
error_type process_content_operator(ContentOperatorHandle* obj, int nested);
error_type process_content_operation_generic(ContentOperationGenericHandle* obj, int nested);
error_type process_content_operation_textshow(ContentOperationTextShowHandle* obj, int nested);
error_type process_content_operation_textfont(ContentOperationTextFontHandle* obj, int nested);
error_type process_content_operation_textshowarray(ContentOperationTextShowArrayHandle* obj, int nested);
error_type process_content_operation_endtext(ContentOperationEndTextHandle* obj, int nested);

/* Documents */
error_type process_document(DocumentHandle* document, int nested);
error_type process_catalog(CatalogHandle* obj, int nested);
error_type process_document_info(DocumentInfoHandle* obj, int nested);
error_type process_trapped(DocumentTrappedType trapped, int nested);
error_type process_page(PageObjectHandle* obj, int nested);
error_type process_extensions(DeveloperExtensionsHandle* extensions, int nested);
error_type process_extension(DeveloperExtensionHandle* extensions, int nested);
error_type process_page_labels(PageLabelsHandle* labels, size_type size, int nested);
error_type process_page_label(PageLabelHandle* label, int nested);
error_type process_page_layout(PageLayout page_layout, int nested);
error_type process_viewer_preferences(ViewerPreferencesHandle* preferences, int nested);
error_type process_page_range(PageRangeHandle* range, int nested);
error_type proces_nonfullscreen_page_mode(NonFullScreenPageMode page_mode, int nested);
error_type proces_reading_order(ReadingOrder order, int nested);
error_type proces_numbering_style(NumberingStyle style, int nested);
error_type proces_print_scaling(PrintScaling scaling, int nested);
error_type process_duplex(Duplex duplex, int nested);
error_type process_outline(OutlineHandle* outline, int nested);
error_type process_outline_base(OutlineBaseHandle* outline, int nested);
error_type process_outline_item(OutlineItemHandle* outline, int nested);
error_type process_outline_item_color(OutlineItemColorHandle* obj, int nested);
error_type process_outline_item_flags(OutlineItemFlagsHandle* obj, int nested);
error_type process_named_destinations(NamedDestinationsHandle* obj, int nested);
error_type process_destination(DestinationHandle* obj, int nested);
error_type process_page_annotations(PageAnnotationsHandle* obj, int nested);
error_type process_annotation(AnnotationHandle* obj, int nested);
error_type process_link_annotation(LinkAnnotationHandle* obj, int nested);
error_type process_resource_dictionary(ResourceDictionaryHandle* obj, int nested);
error_type process_font_map(FontMapHandle* obj, int nested);
error_type process_interactive_form(InteractiveFormHandle* obj, int nested);
error_type process_field_collection(FieldCollectionHandle* obj, int nested);
error_type process_field(FieldHandle* obj, int nested);
error_type process_signature_field(SignatureFieldHandle* obj, int nested);
error_type process_digital_signature(DigitalSignatureHandle* obj, int nested);
error_type process_byte_range_collection(ByteRangeCollectionHandle* obj, int nested);
error_type process_byte_range(ByteRangeHandle* obj, int nested);

/* Common data structures */
error_type process_rectangle(RectangleHandle* obj, int nested);
error_type process_date(DateHandle* obj, int nested);

/* Test document save */
error_type process_document_save(
	DocumentHandle* document,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested);

error_type process_document_save_incremental(
	DocumentHandle* document,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested);

error_type process_document_merge(
	DocumentHandle* document,
	string_type merge_file,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested);

error_type process_document_sign(DocumentHandle* document,
	string_type key_file,
	string_type key_password,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested);

// Some parameters to functions are unused
#define UNUSED(x) (void)(x)

#define RETURN_ERROR_IF_NOT_SUCCESS(fn) \
do { \
	error_type __result__ = (fn); \
	if (VANILLAPDF_ERROR_SUCCESS != __result__) \
	{ \
		print_text("Function call \"%s\" has failed with result %u { %s:%d }\n", \
		#fn, __result__, __FILE__, __LINE__); \
		print_last_error(); \
		assert(!"Operation failed"); \
		return VANILLAPDF_TEST_ERROR_FAILURE; \
	} \
} while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, release) \
do { \
	error_type __result__ = (eval); \
	if (VANILLAPDF_ERROR_SUCCESS == __result__) \
	{ \
		RETURN_ERROR_IF_NOT_SUCCESS(call); \
		RETURN_ERROR_IF_NOT_SUCCESS(release); \
	} \
	else if (VANILLAPDF_ERROR_OBJECT_MISSING == __result__) \
	{ \
		/* Do nothing */ \
	} \
	else \
	{ \
		print_text("Function call \"%s\" has failed with result %u { %s:%d }\n", \
		#eval, __result__, __FILE__, __LINE__); \
		print_last_error(); \
		assert(!"Operation failed"); \
		return VANILLAPDF_TEST_ERROR_FAILURE; \
	} \
} while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(eval, call) \
RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, VANILLAPDF_TEST_ERROR_SUCCESS)

#endif /* _VANILLAPDF_TEST_H */
