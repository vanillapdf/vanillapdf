#include "tools.h"

void print_extract_help() {
	printf("Usage: extract -s [source file]");
}

error_type process_stream(StreamObjectHandle* stream, biguint_type object_number, ushort_type generation_number) {
	DictionaryObjectHandle* stream_dictionary = NULL;

	ObjectType type_object_type;
	ObjectType subtype_object_type;

	ObjectHandle* type_object = NULL;
	ObjectHandle* subtype_object = NULL;

	NameObjectHandle* type_name = NULL;
	NameObjectHandle* subtype_name = NULL;

	boolean_type contains_type = VANILLAPDF_RV_FALSE;
	boolean_type contains_subtype = VANILLAPDF_RV_FALSE;
	boolean_type contains_width = VANILLAPDF_RV_FALSE;
	boolean_type contains_height = VANILLAPDF_RV_FALSE;
	boolean_type contains_colorspace = VANILLAPDF_RV_FALSE;

	boolean_type is_type_xobject = VANILLAPDF_RV_FALSE;
	boolean_type is_subtype_image = VANILLAPDF_RV_FALSE;

	boolean_type processed_with_params = VANILLAPDF_RV_FALSE;

	unsigned long long object_number_converted = 0;
	unsigned int generation_number_converted = 0;

	int return_value = 0;
	char output_filename[256] = { 0 };

	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetHeader(stream, &stream_dictionary));

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Type, &contains_type));
	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Subtype, &contains_subtype));

	if (!contains_type || !contains_subtype) {
		goto err;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Type, &type_object));
	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Subtype, &subtype_object));

	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(type_object, &type_object_type));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(subtype_object, &subtype_object_type));

	if (type_object_type != ObjectType_Name || subtype_object_type != ObjectType_Name) {
		goto err;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_FromObject(type_object, &type_name));
	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_FromObject(subtype_object, &subtype_name));

	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Equals(type_name, NameConstant_XObject, &is_type_xobject));
	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Equals(subtype_name, NameConstant_Image, &is_subtype_image));

	if (!is_type_xobject || !is_subtype_image) {
		goto err;
	}

	object_number_converted = object_number;
	generation_number_converted = generation_number;

	return_value = snprintf(output_filename, sizeof(output_filename), "%llu.%u.jpeg", object_number_converted, generation_number_converted);
	if (return_value < 0) {
		printf("Could not create destination filename");
		return VANILLAPDF_TOOLS_ERROR_FAILURE;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Width, &contains_width));
	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Height, &contains_height));
	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_ColorSpace, &contains_colorspace));

	if (contains_width && contains_height && contains_colorspace) {
		ObjectType width_object_type;
		ObjectType height_object_type;
		ObjectType colorspace_object_type;

		ObjectHandle* width_object = NULL;
		ObjectHandle* height_object = NULL;
		ObjectHandle* colorspace_object = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Width, &width_object));
		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Height, &height_object));
		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_ColorSpace, &colorspace_object));

		RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(width_object, &width_object_type));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(height_object, &height_object_type));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(colorspace_object, &colorspace_object_type));

		if (width_object_type == ObjectType_Integer && height_object_type == ObjectType_Integer && colorspace_object_type == ObjectType_Name) {
			OutputStreamHandle* output_stream = NULL;

			DCTDecodeFilterHandle* encoding_filter = NULL;
			DictionaryObjectHandle* encoding_dictionary = NULL;
			BufferHandle* decoded_body = NULL;
			BufferHandle* encoded_body = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Create(&encoding_dictionary));
			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_InsertConst(encoding_dictionary, NameConstant_Width, width_object));
			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_InsertConst(encoding_dictionary, NameConstant_Height, height_object));
			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_InsertConst(encoding_dictionary, NameConstant_ColorSpace, colorspace_object));

			RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBody(stream, &decoded_body));

			RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_Create(&encoding_filter));
			RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_EncodeParams(encoding_filter, decoded_body, encoding_dictionary, &encoded_body));
			RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_Release(encoding_filter));

			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(output_filename, &output_stream));
			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, encoded_body));
			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Flush(output_stream));
			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Release(output_stream));

			RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(encoded_body));
			RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(decoded_body));
			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(encoding_dictionary));

			processed_with_params = VANILLAPDF_RV_TRUE;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(width_object));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(height_object));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(colorspace_object));
	}

	if (processed_with_params != VANILLAPDF_RV_TRUE) {
		OutputStreamHandle* output_stream = NULL;
		BufferHandle* encoded_body = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBodyRaw(stream, &encoded_body));

		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(output_filename, &output_stream));
		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, encoded_body));
		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Flush(output_stream));
		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Release(output_stream));

		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(encoded_body));
	}

err:
	if (type_object != NULL) {
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(type_object));
		type_object = NULL;
	}

	if (subtype_object != NULL) {
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(subtype_object));
		subtype_object = NULL;
	}

	if (stream_dictionary != NULL) {
		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(stream_dictionary));
		stream_dictionary = NULL;
	}

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_object(ObjectHandle* obj, biguint_type object_number, ushort_type generation_number) {

	ObjectType type;

	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(obj, &type));

	if (type == ObjectType_Stream) {
		StreamObjectHandle* stream = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_FromObject(obj, &stream));
		RETURN_ERROR_IF_NOT_SUCCESS(process_stream(stream, object_number, generation_number));
	}

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_xref(XrefHandle* xref) {
	XrefIteratorHandle* xref_iterator = NULL;
	boolean_type valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_GetIterator(xref, &xref_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == XrefIterator_IsValid(xref_iterator, &valid)
		&& VANILLAPDF_RV_TRUE == valid) {

		XrefEntryType type;
		XrefEntryHandle* entry = NULL;
		XrefCompressedEntryHandle* compressed_entry = NULL;
		XrefUsedEntryHandle* used_entry = NULL;

		biguint_type object_number = 0;
		ushort_type generation_number = 0;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_GetValue(xref_iterator, &entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetType(entry, &type));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetObjectNumber(entry, &object_number));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetGenerationNumber(entry, &generation_number));

		if (type == XrefEntryType_Used) {
			ObjectHandle* obj = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_FromEntry(entry, &used_entry));
			RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_GetReference(used_entry, &obj));
			RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, object_number, generation_number));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
		}

		if (type == XrefEntryType_Compressed) {
			ObjectHandle* obj = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_FromEntry(entry, &compressed_entry));
			RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_GetReference(compressed_entry, &obj));
			RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, object_number, generation_number));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
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
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_GetIterator(chain, &chain_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == XrefChainIterator_IsValid(chain_iterator, &valid)
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
		OutputStreamHandle* output_stream = NULL;

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

		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(output_filename, &output_stream));
		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, content_image_data));
		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Flush(output_stream));
		RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Release(output_stream));

		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(content_image_data));
		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(content_image_dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_Release(content_object));
	}

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

int process_extract(int argc, char *argv[]) {

	const char *filename = NULL;
	int arg_counter = 0;

	size_type i = 0;
	size_type page_count = 0;

	FileHandle* file = NULL;
	DocumentHandle* document = NULL;
	CatalogHandle* catalog = NULL;
	PageTreeHandle* tree = NULL;

	for (arg_counter = 0; arg_counter < argc; ++arg_counter) {

		// source file
		if (strcmp(argv[arg_counter], "-s") == 0 && (arg_counter + 1 < argc)) {
			filename = argv[arg_counter + 1];
			arg_counter++;
		} else {
			print_extract_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}
	}

	if (filename == NULL) {
		print_extract_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

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
