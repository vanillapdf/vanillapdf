#ifndef _GOTCHANGPDF_TEST_H
#define _GOTCHANGPDF_TEST_H

#include "gotchangpdf/gotchangpdf.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

void print_spaces(int nested);

error_type process_object(ObjectHandle obj, int nested);
error_type process_content_instruction(ContentInstructionHandle obj, int nested);
error_type process_content_object(ContentObjectHandle obj, int nested);
error_type process_content_operation(ContentOperationHandle obj, int nested);
error_type process_contents(ContentsHandle obj, int nested);
error_type process_page(PageObjectHandle obj, int nested);
error_type process_buffer(BufferHandle buffer, int nested);
error_type process_name(NameHandle name, int nested);
error_type process_lit_string(LiteralStringHandle string, int nested);
error_type process_hex_string(HexadecimalStringHandle string, int nested);
error_type process_dictionary(DictionaryHandle dictionary, int nested);
error_type process_xref(XrefHandle xref, int nested);

error_type process_content_object_text(ContentObjectTextHandle obj, int nested);
error_type process_content_operator(ContentOperatorHandle obj, int nested);
error_type process_content_operation_generic(ContentOperationGenericHandle obj, int nested);

#define RETURN_ERROR_IF_NOT_SUCCESS(var) do { int __result__ = (var);  if (GOTCHANG_PDF_ERROR_SUCCES != __result__) return __result__; } while(0)

#endif /* _GOTCHANGPDF_TEST_H */
