#include "merge.h"

error_type process_contents(ContentsHandle page_contents, integer_type page_number)
{
	integer_type j = 0;
	integer_type contents_size = 0;
	RectangleHandle media_box = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(Contents_GetInstructionsSize(page_contents, &contents_size));

	for (j = 0; j < contents_size; ++j) {
		integer_type k = 0;
		integer_type operations_size = 0;
		ContentInstructionType instruction_type;
		ContentObjectType object_type;
		ContentInstructionHandle content_instruction = NULL;
		ContentObjectHandle content_object = NULL;
		ContentObjectTextHandle content_text = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(Contents_GetInstructionAt(page_contents, j, &content_instruction));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_GetType(content_instruction, &instruction_type));
		if (instruction_type != ContentInstructionType_Object) {
			RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_Release(content_instruction));
			continue;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_ToObject(content_instruction, &content_object));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_GetType(content_object, &object_type));
		if (object_type != ContentObjectType_Text) {
			RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_Release(content_instruction));
			continue;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_ToText(content_object, &content_text));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationsSize(content_text, &operations_size));

		for (k = 0; k < operations_size; ++k) {
			ContentOperationHandle content_operation = NULL;
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationAt(content_text, k, &content_operation));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation(content_operation, page_number));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_Release(content_operation));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_Release(content_object));
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_string_object(StringHandle string_handle, integer_type page_number)
{
	BufferHandle string_buffer = NULL;
	string_type string_data = NULL;
	string_size_type string_size = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_GetValue(string_handle, &string_buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(string_buffer, &string_data, &string_size));

	if (0 != strncmp(string_data, ".page:01234/56789", string_size)) {
		int buffer_size = snprintf(NULL, 0, "%d", page_number);

		assert(buffer_size > 0 && "Could not get page number size");
		if (buffer_size < 0) {
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		char *page_string = malloc(buffer_size + 1);

		assert(NULL != page_string && "Could not allocate memory");
		if (NULL == page_string) {
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		int printed_size = snprintf(page_string, buffer_size + 1, "%d", page_number);

		assert(printed_size > 0 && "Could not print page number");
		if (printed_size < 0) {
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_SetData(string_buffer, page_string, buffer_size));

		free(page_string);
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(string_buffer));

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_operation(ContentOperationHandle content_operation, integer_type page_number)
{
	ContentOperationType operation_type;
	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_GetType(content_operation, &operation_type));
	if (operation_type == ContentOperationType_TextShow) {
		StringHandle text_string = NULL;
		ContentOperationTextShowHandle text_handle = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToTextShow(content_operation, &text_handle));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShow_GetValue(text_handle, &text_string));
		RETURN_ERROR_IF_NOT_SUCCESS(process_string_object(text_string, page_number));
		RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Release(text_string));
	}

	if (operation_type == ContentOperationType_TextShowArray) {
		integer_type l = 0;
		integer_type items_size = 0;
		ArrayHandle text_items = NULL;
		ContentOperationTextShowArrayHandle text_handle = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToTextShowArray(content_operation, &text_handle));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShowArray_GetValue(text_handle, &text_items));
		RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Size(text_items, &items_size));

		for (l = 0; l < items_size; ++l) {
			ObjectType object_type;
			ObjectHandle object_handle = NULL;
			StringHandle string_handle = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_At(text_items, l, &object_handle));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Type(object_handle, &object_type));
			if (object_type != ObjectType_String) {
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(object_handle));
				continue;
			}

			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToString(object_handle, &string_handle));
			RETURN_ERROR_IF_NOT_SUCCESS(process_string_object(string_handle, page_number));
			RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Release(string_handle));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Release(text_items));
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int main(int argc, char *argv[])
{
	FileHandle file1 = NULL;
	FileHandle file2 = NULL;
	DocumentHandle document1 = NULL;
	DocumentHandle document2 = NULL;
	CatalogHandle catalog1 = NULL;
	PageTreeHandle tree1 = NULL;

	integer_type i = 0;
	integer_type page_count = 0;

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open("test/Report.pdf", &file1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Open("test/sample.pdf", &file2));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file2));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file1, &document1));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file2, &document2));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_AppendContent(document1, document2));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document1, &catalog1));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog1, &tree1));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(tree1, &page_count));

	for (i = 0; i < page_count; ++i) {
		PageObjectHandle page_object = NULL;
		ContentsHandle page_contents = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(tree1, i + 1, &page_object));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetContents(page_object, &page_contents));
		RETURN_ERROR_IF_NOT_SUCCESS(process_contents(page_contents, i + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Contents_Release(page_contents));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page_object));
	}

	//for (i = 0; i < page_count; ++i) {
	//	PageObjectHandle page_object = NULL;

	//	RETURN_ERROR_IF_NOT_SUCCESS(PageObject_CreateFromDocument(document1, &page_object));
	//	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_AppendPage(tree1, page_object));
	//	RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page_object));
	//}

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document1, "output.pdf"));

	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog1));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(tree1));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document1));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document2));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file2));

	return GOTCHANG_PDF_ERROR_SUCCES;
}