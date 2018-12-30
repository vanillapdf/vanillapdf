#include "tools.h"

void print_filter_help() {
	printf("Usage: filter -f [flate|dct|ascii85|ascii_hex] [encode|decode] -s [source file] -d [destination file]");
}

int process_filter(int argc, char *argv[]) {

	InputStreamHandle* input_stream = NULL;
	OutputStreamHandle* output_stream = NULL;

	BufferHandle* input_data = NULL;

	int arg_counter = 0;
	const char *filter_type = NULL;
	const char *operation = NULL;
	const char *source_file = NULL;
	const char *destination_file = NULL;

	boolean_type is_encode = VANILLAPDF_RV_FALSE;
	boolean_type is_decode = VANILLAPDF_RV_FALSE;

	boolean_type is_flate_decode = VANILLAPDF_RV_FALSE;
	boolean_type is_dct_decode = VANILLAPDF_RV_FALSE;
	boolean_type is_ascii85_decode = VANILLAPDF_RV_FALSE;
	boolean_type is_ascii_hex_decode = VANILLAPDF_RV_FALSE;

	for (arg_counter = 0; arg_counter < argc; ++arg_counter) {

		// filter type
		if (strcmp(argv[arg_counter], "-f") == 0 && (arg_counter + 2 < argc)) {
			filter_type = argv[arg_counter + 1];
			operation = argv[arg_counter + 2];
			arg_counter++;
			arg_counter++;

		// source file
		} else if (strcmp(argv[arg_counter], "-s") == 0 && (arg_counter + 1 < argc)) {
			source_file = argv[arg_counter + 1];
			arg_counter++;

		// destination file
		} else if (strcmp(argv[arg_counter], "-d") == 0 && (arg_counter + 1 < argc)) {
			destination_file = argv[arg_counter + 1];
			arg_counter++;
		} else {
			print_filter_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}
	}

	if (filter_type == NULL || operation == NULL || source_file == NULL || destination_file == NULL) {
		print_filter_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	is_encode = (strcmp(operation, "encode") == 0);
	is_decode = (strcmp(operation, "decode") == 0);

	if (!is_encode && !is_decode) {
		print_filter_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	is_flate_decode = (strcmp(filter_type, "flate") == 0);
	is_dct_decode = (strcmp(filter_type, "dct") == 0);
	is_ascii85_decode = (strcmp(filter_type, "ascii85") == 0);
	is_ascii_hex_decode = (strcmp(filter_type, "ascii_hex") == 0);

	if (!is_flate_decode && !is_dct_decode && !is_ascii85_decode && !is_ascii_hex_decode) {
		print_filter_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(destination_file, &output_stream));

	RETURN_ERROR_IF_NOT_SUCCESS(InputStream_CreateFromFile(source_file, &input_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(InputStream_ToBuffer(input_stream, &input_data));

	if (is_flate_decode) {
		FlateDecodeFilterHandle* filter_handle = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(FlateDecodeFilter_Create(&filter_handle));

		if (is_encode) {
			BufferHandle* encoded_data = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(FlateDecodeFilter_Encode(filter_handle, input_data, &encoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, encoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(encoded_data));
		}

		if (is_decode) {
			BufferHandle* decoded_data = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(FlateDecodeFilter_Decode(filter_handle, input_data, &decoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, decoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(decoded_data));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(FlateDecodeFilter_Release(filter_handle));
	}

	if (is_dct_decode) {
		DCTDecodeFilterHandle* filter_handle = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_Create(&filter_handle));

		if (is_encode) {
			BufferHandle* encoded_data = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_Encode(filter_handle, input_data, &encoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, encoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(encoded_data));
		}

		if (is_decode) {
			BufferHandle* decoded_data = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_Decode(filter_handle, input_data, &decoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, decoded_data));
			RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(decoded_data));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_Release(filter_handle));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Release(output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(InputStream_Release(input_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(input_data));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
