#include "merge.h"

int main(int argc, char *argv[])
{
	FileHandle file1 = NULL;
	FileHandle file2 = NULL;
	DocumentHandle document1 = NULL;
	DocumentHandle document2 = NULL;
	CatalogHandle catalog1 = NULL;
	CatalogHandle catalog2 = NULL;
	PageTreeHandle tree1 = NULL;
	PageTreeHandle tree2 = NULL;

	integer_type i = 0;
	integer_type page_count = 0;

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open("test/example.pdf", &file1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Open("test/Granizo.pdf", &file2));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file2));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file1, &document1));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file2, &document2));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document1, &catalog1));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document2, &catalog2));

	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog1, &tree1));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog2, &tree2));

	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(tree2, &page_count));

	for (i = 1; i < page_count; ++i) {
		integer_type j = 0;
		integer_type contents_size = 0;
		PageObjectHandle page_object = NULL;
		RectangleHandle media_box = NULL;
		ContentsHandle page_contents = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(tree2, i, &page_object));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetContents(page_object, &page_contents));

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
				continue;
			}

			RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_ToObject(content_instruction, &content_object));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_GetType(content_object, &object_type));
			if (object_type != ContentObjectType_Text) {
				continue;
			}

			RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_ToText(content_object, &content_text));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationsSize(content_text, &operations_size));

			for (k = 0; k < operations_size; ++k) {
				ContentOperationType operation_type;
				ContentOperationHandle content_operation = NULL;
				RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationAt(content_text, k, &content_operation));
				RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_GetType(content_operation, &operation_type));
				if (operation_type == ContentOperationType_TextShow) {
					StringHandle text_string = NULL;
					BufferHandle string_data = NULL;
					ContentOperationTextShowHandle text_handle = NULL;
					RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToTextShow(content_operation, &text_handle));
					RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShow_GetValue(text_handle, &text_string));
					RETURN_ERROR_IF_NOT_SUCCESS(StringObject_GetValue(text_string, &string_data));
					RETURN_ERROR_IF_NOT_SUCCESS(Buffer_SetData(string_data, "test", 5));
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
						BufferHandle string_data = NULL;

						RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_At(text_items, l, &object_handle));
						RETURN_ERROR_IF_NOT_SUCCESS(Object_Type(object_handle, &object_type));
						if (object_type != ObjectType_String) {
							continue;
						}

						RETURN_ERROR_IF_NOT_SUCCESS(Object_ToString(object_handle, &string_handle));
						RETURN_ERROR_IF_NOT_SUCCESS(StringObject_GetValue(string_handle, &string_data));
						RETURN_ERROR_IF_NOT_SUCCESS(Buffer_SetData(string_data, "test", 5));
					}
				}
			}
		}

		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_CreateFromDocument(document1, &page_object));
		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_AppendPage(tree1, page_object));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document1, "output.pdf"));

	return GOTCHANG_PDF_ERROR_SUCCES;
}