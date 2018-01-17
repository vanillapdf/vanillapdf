#include "tools.h"

void print_extract_help() {
	printf("Usage: extract -s [source file]");
}

error_type process_stream(StreamObjectHandle* stream, biguint_type object_number, ushort_type generation_number) {
	BufferHandle* body = NULL;
	DictionaryObjectHandle* dictionary = NULL;

	ObjectType type_object_type;
	ObjectType subtype_object_type;
	ObjectHandle* type_object = NULL;
	ObjectHandle* subtype_object = NULL;
	NameObjectHandle* type_name = NULL;
	NameObjectHandle* subtype_name = NULL;
	boolean_type contains_type = VANILLAPDF_RV_FALSE;
	boolean_type contains_subtype = VANILLAPDF_RV_FALSE;
	boolean_type is_type_xobject = VANILLAPDF_RV_FALSE;
	boolean_type is_subtype_image = VANILLAPDF_RV_FALSE;

	int return_value = 0;
	char output_filename[256] = { 0 };
	IOutputStreamHandle* output_stream = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetHeader(stream, &dictionary));

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(dictionary, NameConstant_Type, &contains_type));
	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(dictionary, NameConstant_Subtype, &contains_subtype));

	if (!contains_type || !contains_subtype) {
		return VANILLAPDF_TOOLS_ERROR_SUCCESS;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(dictionary, NameConstant_Type, &type_object));
	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(dictionary, NameConstant_Subtype, &subtype_object));

	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(type_object, &type_object_type));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(subtype_object, &subtype_object_type));

	if (type_object_type != ObjectType_Name || subtype_object_type != ObjectType_Name) {
		return VANILLAPDF_TOOLS_ERROR_SUCCESS;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Object_ToName(type_object, &type_name));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_ToName(subtype_object, &subtype_name));

	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Equals(type_name, NameConstant_XObject, &is_type_xobject));
	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Equals(subtype_name, NameConstant_Image, &is_subtype_image));

	if (!is_type_xobject || !is_subtype_image) {
		return VANILLAPDF_TOOLS_ERROR_SUCCESS;
	}

	return_value = snprintf(output_filename, sizeof(output_filename), "%llu.%u", object_number, generation_number);
	if (return_value < 0) {
		printf("Could not create destination filename");
		return VANILLAPDF_TOOLS_ERROR_FAILURE;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBody(stream, &body));

	RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_CreateFromFile(output_filename, &output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_WriteBuffer(output_stream, body));
	RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_Flush(output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_Release(output_stream));

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(body));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_object(ObjectHandle* obj, biguint_type object_number, ushort_type generation_number) {

	ObjectType type;

	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(obj, &type));

	if (type == ObjectType_Stream) {
		StreamObjectHandle* stream = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToStream(obj, &stream));
		RETURN_ERROR_IF_NOT_SUCCESS(process_stream(stream, object_number, generation_number));
	}

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_xref(XrefHandle* xref) {
	XrefIteratorHandle* xref_iterator = NULL;
	boolean_type valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_Iterator(xref, &xref_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == XrefIterator_IsValid(xref_iterator, xref, &valid)
		&& VANILLAPDF_RV_TRUE == valid) {

		XrefEntryType type;
		XrefEntryHandle* entry = NULL;
		ObjectHandle* obj = NULL;
		XrefCompressedEntryHandle* compressed_entry = NULL;
		XrefUsedEntryHandle* used_entry = NULL;

		biguint_type object_number = 0;
		ushort_type generation_number = 0;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_GetValue(xref_iterator, &entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Type(entry, &type));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ObjectNumber(entry, &object_number));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GenerationNumber(entry, &generation_number));

		switch (type) {
			case XrefEntryType_Used:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToUsedEntry(entry, &used_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_Reference(used_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, object_number, generation_number));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				break;
			case XrefEntryType_Compressed:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToCompressedEntry(entry, &compressed_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_Reference(compressed_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, object_number, generation_number));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				break;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Release(entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Next(xref_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Release(xref_iterator));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_file(FileHandle* file) {
	XrefChainHandle* chain = NULL;
	XrefChainIteratorHandle* chain_iterator = NULL;
	boolean_type valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(File_XrefChain(file, &chain));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Iterator(chain, &chain_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == XrefChainIterator_IsValid(chain_iterator, chain, &valid)
		&& VANILLAPDF_RV_TRUE == valid) {
		XrefHandle* xref = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_GetValue(chain_iterator, &xref));
		RETURN_ERROR_IF_NOT_SUCCESS(process_xref(xref));
		RETURN_ERROR_IF_NOT_SUCCESS(Xref_Release(xref));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Next(chain_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Release(chain_iterator));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Release(chain));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_contents(ContentsHandle* page_contents, size_type page_number) {
	size_type i = 0;
	size_type contents_size = 0;
	unsigned long long page_number_converted = page_number;

	RETURN_ERROR_IF_NOT_SUCCESS(Contents_GetInstructionsSize(page_contents, &contents_size));

	for (i = 0; i < contents_size; ++i) {
		ContentInstructionType instruction_type;
		ContentObjectType object_type;
		ContentInstructionHandle* content_instruction = NULL;
		ContentObjectHandle* content_object = NULL;
		ContentObjectInlineImageHandle* content_image = NULL;
		DictionaryObjectHandle* content_image_dictionary = NULL;
		BufferHandle* content_image_data = NULL;

		int return_value = 0;
		unsigned long long i_converted = i;
		char output_filename[256] = {0};
		IOutputStreamHandle* output_stream = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(Contents_GetInstructionAt(page_contents, i, &content_instruction));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_GetType(content_instruction, &instruction_type));

		if (instruction_type != ContentInstructionType_Object) {
			RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_Release(content_instruction));
			continue;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_ToObject(content_instruction, &content_object));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_GetType(content_object, &object_type));

		if (object_type != ContentObjectType_InlineImage) {
			RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_Release(content_instruction));
			continue;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_ToInlineImage(content_object, &content_image));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetDictionary(content_image, &content_image_dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetData(content_image, &content_image_data));

		return_value = snprintf(output_filename, sizeof(output_filename), "%llu.%llu", page_number_converted, i_converted);
		if (return_value < 0) {
			printf("Could not create destination filename");
			return VANILLAPDF_TOOLS_ERROR_FAILURE;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_CreateFromFile(output_filename, &output_stream));
		RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_WriteBuffer(output_stream, content_image_data));
		RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_Flush(output_stream));
		RETURN_ERROR_IF_NOT_SUCCESS(IOutputStream_Release(output_stream));

		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(content_image_data));
		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(content_image_dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_Release(content_object));
	}

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

int process_extract(int argc, char *argv[]) {

	const char *filename = NULL;

	size_type i = 0;
	size_type page_count = 0;

	FileHandle* file = NULL;
	DocumentHandle* document = NULL;
	CatalogHandle* catalog = NULL;
	PageTreeHandle* tree = NULL;

	if (argc < 2) {
		print_extract_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[0], "-s")) {
		print_extract_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	filename = argv[1];

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(filename, &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(file));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(file, &document));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &tree));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(tree, &page_count));

	for (i = 0; i < page_count; ++i) {
		ContentsHandle* page_contents = NULL;
		PageObjectHandle* page_object = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(tree, i + 1, &page_object));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetContents(page_object, &page_contents));

		RETURN_ERROR_IF_NOT_SUCCESS(process_contents(page_contents, i + 1));

		RETURN_ERROR_IF_NOT_SUCCESS(Contents_Release(page_contents));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page_object));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(tree));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
