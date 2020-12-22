#include "tools.h"

void print_extract_object_help() {
	printf("Usage: extract_object -s [source file] -o [object number] -g [generation number]");
}

error_type eo_process_stream(StreamObjectHandle* stream, biguint_type object_number, ushort_type generation_number) {

	int return_value = 0;
	char output_filename[256] = { 0 };
	BufferHandle* decoded_body = NULL;
	OutputStreamHandle* output_stream = NULL;

	unsigned long long object_number_converted = object_number;
	unsigned long long generation_number_converted = generation_number;

	return_value = snprintf(output_filename, sizeof(output_filename), "%llu.%llu", object_number_converted, generation_number_converted);
	if (return_value < 0) {
		printf("Could not create destination filename");
		return VANILLAPDF_TOOLS_ERROR_FAILURE;
	}


	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBody(stream, &decoded_body));

	RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(output_filename, &output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, decoded_body));
	RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Flush(output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Release(output_stream));

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(decoded_body));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type eo_process_object(ObjectHandle* obj, biguint_type object_number, ushort_type generation_number) {

	ObjectType type;

	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(obj, &type));

	if (type == ObjectType_Stream) {
		StreamObjectHandle* stream = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_FromObject(obj, &stream));
		RETURN_ERROR_IF_NOT_SUCCESS(eo_process_stream(stream, object_number, generation_number));
		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_Release(stream));
	}

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type eo_process_xref(XrefHandle* xref, biguint_type object_number, ushort_type generation_number) {
	XrefIteratorHandle* xref_iterator = NULL;
	boolean_type valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_GetIterator(xref, &xref_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == XrefIterator_IsValid(xref_iterator, &valid)
		&& VANILLAPDF_RV_TRUE == valid) {

		XrefEntryType type;
		XrefEntryHandle* entry = NULL;
		XrefCompressedEntryHandle* compressed_entry = NULL;
		XrefUsedEntryHandle* used_entry = NULL;

		biguint_type current_object_number = 0;
		ushort_type current_generation_number = 0;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_GetValue(xref_iterator, &entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetType(entry, &type));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetObjectNumber(entry, &current_object_number));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetGenerationNumber(entry, &current_generation_number));

		if (current_object_number == object_number && current_generation_number == generation_number) {

			if (type == XrefEntryType_Used) {
				ObjectHandle* obj = NULL;

				RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_FromEntry(entry, &used_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_GetReference(used_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(eo_process_object(obj, object_number, generation_number));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_Release(used_entry));
			}

			if (type == XrefEntryType_Compressed) {
				ObjectHandle* obj = NULL;

				RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_FromEntry(entry, &compressed_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_GetReference(compressed_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(eo_process_object(obj, object_number, generation_number));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_Release(compressed_entry));
			}
		}

		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Release(entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Next(xref_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Release(xref_iterator));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type eo_process_file(FileHandle* file, biguint_type object_number, ushort_type generation_number) {
	XrefChainHandle* chain = NULL;
	XrefChainIteratorHandle* chain_iterator = NULL;
	boolean_type valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(File_XrefChain(file, &chain));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_GetIterator(chain, &chain_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == XrefChainIterator_IsValid(chain_iterator, &valid)
		&& VANILLAPDF_RV_TRUE == valid) {
		XrefHandle* xref = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_GetValue(chain_iterator, &xref));
		RETURN_ERROR_IF_NOT_SUCCESS(eo_process_xref(xref, object_number, generation_number));
		RETURN_ERROR_IF_NOT_SUCCESS(Xref_Release(xref));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Next(chain_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Release(chain_iterator));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Release(chain));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

int process_extract_object(int argc, char *argv[]) {

	const char *filename = NULL;
	int arg_counter = 0;

	FileHandle* file = NULL;

	biguint_type object_number = 0;
	ushort_type generation_number = 0;

	for (arg_counter = 0; arg_counter < argc; ++arg_counter) {

		// source file
		if (strcmp(argv[arg_counter], "-s") == 0 && (arg_counter + 1 < argc)) {
			filename = argv[arg_counter + 1];
			arg_counter++;
		} else if (strcmp(argv[arg_counter], "-o") == 0 && (arg_counter + 1 < argc)) {
			object_number = atoll(argv[arg_counter + 1]);
			arg_counter++;
		} else if (strcmp(argv[arg_counter], "-g") == 0 && (arg_counter + 1 < argc)) {
			generation_number = (ushort_type)atoi(argv[arg_counter + 1]);
			arg_counter++;
		} else {
			print_extract_object_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}
	}

	if (filename == NULL) {
		print_extract_object_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(filename, &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(eo_process_file(file, object_number, generation_number));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
