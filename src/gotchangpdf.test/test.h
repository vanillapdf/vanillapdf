#ifndef _GOTCHANGPDF_TEST_H
#define _GOTCHANGPDF_TEST_H

#include "gotchangpdf/gotchangpdf.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

#if defined(DEBUG) && defined(_MSC_VER)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

void print_spaces(int nested);
error_type process_buffer(BufferHandle buffer, int nested);
error_type process_version(PDFVersion version, int nested);

/* Files */
error_type process_file(FileHandle file, int nested);
error_type process_xref(XrefHandle xref, int nested);

/* Objects */
error_type process_object(ObjectHandle obj, int nested);
error_type process_name(NameHandle name, int nested);
error_type process_array(ArrayHandle arr, int nested);
error_type process_integer(IntegerHandle integer, int nested);
error_type process_boolean(BooleanHandle boolean, int nested);
error_type process_reference(IndirectReferenceHandle reference, int nested);
error_type process_real(RealHandle real, int nested);
error_type process_null(NullHandle obj, int nested);
error_type process_string(StringHandle string, int nested);
error_type process_dictionary(DictionaryHandle dictionary, int nested);
error_type process_stream(StreamHandle stream, int nested);

error_type process_lit_string(LiteralStringHandle string, int nested);
error_type process_hex_string(HexadecimalStringHandle string, int nested);

/* Contents */
error_type process_content_instruction(ContentInstructionHandle obj, int nested);
error_type process_content_object(ContentObjectHandle obj, int nested);
error_type process_content_operation(ContentOperationHandle obj, int nested);
error_type process_contents(ContentsHandle obj, int nested);

error_type process_content_object_text(ContentObjectTextHandle obj, int nested);
error_type process_content_object_inline_image(ContentObjectInlineImageHandle obj, int nested);
error_type process_content_operator(ContentOperatorHandle obj, int nested);
error_type process_content_operation_generic(ContentOperationGenericHandle obj, int nested);
error_type process_content_operation_textshow(ContentOperationTextShowHandle obj, int nested);
error_type process_content_operation_endtext(ContentOperationEndTextHandle obj, int nested);


/* Documents */
error_type process_document(DocumentHandle document, int nested);
error_type process_catalog(CatalogHandle obj, int nested);
error_type process_document_info(DocumentInfoHandle obj, int nested);
error_type process_date(DateHandle obj, int nested);
error_type process_trapped(DocumentTrapped trapped, int nested);
error_type process_page(PageObjectHandle obj, int nested);
error_type process_extensions(DeveloperExtensionsHandle extensions, int nested);
error_type process_extension(DeveloperExtensionHandle extensions, int nested);
error_type process_page_labels(PageLabelsHandle labels, integer_type size, int nested);
error_type process_page_label(PageLabelHandle label, int nested);
error_type process_page_layout(PageLayout page_layout, int nested);
error_type process_viewer_preferences(ViewerPreferencesHandle preferences, int nested);
error_type process_page_range(PageRangeHandle range, int nested);
error_type proces_nonfullscreen_page_mode(NonFullScreenPageMode page_mode, int nested);
error_type proces_reading_order(ReadingOrder order, int nested);
error_type proces_numbering_style(NumberingStyle style, int nested);
error_type proces_print_scaling(PrintScaling scaling, int nested);
error_type process_duplex(Duplex duplex, int nested);
error_type process_outline(OutlineHandle outline, int nested);
error_type process_outline_base(OutlineBaseHandle outline, int nested);
error_type process_outline_item(OutlineItemHandle outline, int nested);
error_type process_outline_item_color(OutlineItemColorHandle obj, int nested);
error_type process_outline_item_flags(OutlineItemFlagsHandle obj, int nested);

#define RETURN_ERROR_IF_NOT_SUCCESS(var) do { error_type __result__ = (var);  if (GOTCHANG_PDF_ERROR_SUCCES != __result__) return __result__; } while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, release) \
do { \
	error_type __result__ = (eval); \
	if (GOTCHANG_PDF_ERROR_SUCCES == __result__) { RETURN_ERROR_IF_NOT_SUCCESS(call); RETURN_ERROR_IF_NOT_SUCCESS(release); } \
	else if (GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING == __result__) { /* Do nothing */ } \
	else { return __result__; } \
} while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(eval, call) RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, GOTCHANG_PDF_ERROR_SUCCES)

#endif /* _GOTCHANGPDF_TEST_H */
