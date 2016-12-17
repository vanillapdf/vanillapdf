#include "merge.h"

const int GOTCHANG_PDF_MERGE_ERROR_SUCCESS = 0;
const int GOTCHANG_PDF_MERGE_ERROR_INVALID_PARAMETERS = 1;
const int GOTCHANG_PDF_MERGE_ERROR_FAILURE = 255;

static PageObjectHandle current_page_object = NULL;
static UnicodeCharacterMapHandle current_unicode_map = NULL;

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

	if (current_unicode_map != NULL) {
		RETURN_ERROR_IF_NOT_SUCCESS(UnicodeCharacterMap_Release(current_unicode_map));
		current_unicode_map = NULL;
	}

	return GOTCHANG_PDF_MERGE_ERROR_SUCCESS;
}

error_type process_string_object(StringHandle string_handle, integer_type page_number)
{
	BufferHandle mapped_value = NULL;
	BufferHandle string_buffer = NULL;
	string_type string_data = NULL;
	size_type string_size = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_GetValue(string_handle, &string_buffer));

	if (NULL == current_unicode_map) {
		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(string_buffer, &string_data, &string_size));
	} else {
		RETURN_ERROR_IF_NOT_SUCCESS(UnicodeCharacterMap_GetMappedValue(current_unicode_map, string_buffer, &mapped_value));
		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(mapped_value, &string_data, &string_size));
	}

	if (0 == strncmp(string_data, ".page:01234/56789", string_size)) {
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

	if (NULL != mapped_value) {
		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(mapped_value));
	}

	return GOTCHANG_PDF_MERGE_ERROR_SUCCESS;
}

error_type process_content_operation(ContentOperationHandle content_operation, integer_type page_number)
{
	ContentOperationType operation_type;
	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_GetType(content_operation, &operation_type));
	if (operation_type == ContentOperationType_TextFont) {
		FontType font_type;
		FontHandle font = NULL;
		FontMapHandle font_map = NULL;
		ResourceDictionaryHandle page_resources = NULL;
		NameHandle font_name = NULL;
		ContentOperationTextFontHandle text_font = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToTextFont(content_operation, &text_font));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextFont_GetName(text_font, &font_name));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetResources(current_page_object, &page_resources));
		RETURN_ERROR_IF_NOT_SUCCESS(ResourceDictionary_GetFontMap(page_resources, &font_map));

		RETURN_ERROR_IF_NOT_SUCCESS(FontMap_Find(font_map, font_name, &font));
		RETURN_ERROR_IF_NOT_SUCCESS(Font_Type(font, &font_type));
		if (font_type == FontType_Composite) {
			CompositeFontHandle composite_font = NULL;

			if (NULL != current_unicode_map) {
				RETURN_ERROR_IF_NOT_SUCCESS(UnicodeCharacterMap_Release(current_unicode_map));
				current_unicode_map = NULL;
			}

			RETURN_ERROR_IF_NOT_SUCCESS(Font_ToComposite(font, &composite_font));
			RETURN_ERROR_IF_NOT_SUCCESS(CompositeFont_GetUnicodeMap(composite_font, &current_unicode_map));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(Font_Release(font));
		RETURN_ERROR_IF_NOT_SUCCESS(FontMap_Release(font_map));
		RETURN_ERROR_IF_NOT_SUCCESS(ResourceDictionary_Release(page_resources));
		RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(font_name));
	}

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

	return GOTCHANG_PDF_MERGE_ERROR_SUCCESS;
}

void print_help()
{
	printf("Usage: -s [source file] -d [destination file] -f [array of merged files]");
}

int main(int argc, char *argv[])
{
	const int MERGE_FILES_START_INDEX = 6;

	const char *source_file = NULL;
	const char *destination_file = NULL;
	int merge_files_count = 0;

	FileHandle file = NULL;
	DocumentHandle document = NULL;
	CatalogHandle catalog = NULL;
	PageTreeHandle tree = NULL;

	integer_type i = 0;
	integer_type page_count = 0;

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif

	if (argc < 7) {
		print_help();
		return GOTCHANG_PDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[1], "-s")) {
		print_help();
		return GOTCHANG_PDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[3], "-d")) {
		print_help();
		return GOTCHANG_PDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[5], "-f")) {
		print_help();
		return GOTCHANG_PDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	source_file = argv[2];
	destination_file = argv[4];
	merge_files_count = argc - MERGE_FILES_START_INDEX;

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(source_file, &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file, &document));

	for (i = 0; i < merge_files_count; ++i) {
		FileHandle other_file = NULL;
		DocumentHandle other_document = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(File_Open(argv[MERGE_FILES_START_INDEX + i], &other_file));
		RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(other_file));
		RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(other_file, &other_document));

		RETURN_ERROR_IF_NOT_SUCCESS(Document_AppendContent(document, other_document));

		RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(other_document));
		RETURN_ERROR_IF_NOT_SUCCESS(File_Release(other_file));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &tree));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(tree, &page_count));

	for (i = 0; i < page_count; ++i) {
		ContentsHandle page_contents = NULL;

		current_page_object = NULL;
		current_unicode_map = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(tree, i + 1, &current_page_object));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetContents(current_page_object, &page_contents));
		RETURN_ERROR_IF_NOT_SUCCESS(process_contents(page_contents, i + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Contents_Release(page_contents));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(current_page_object));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, destination_file));

	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(tree));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	return GOTCHANG_PDF_MERGE_ERROR_SUCCESS;
}