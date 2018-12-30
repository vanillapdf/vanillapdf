#include "filter.h"

const int VANILLAPDF_FILTER_ERROR_SUCCESS = 0;
const int VANILLAPDF_FILTER_ERROR_INVALID_PARAMETERS = 1;
const int VANILLAPDF_FILTER_ERROR_FAILURE = 255;

void print_help() {
	printf("Usage: -f [flate|dct|ascii85|ascii_hex] [encode|decode] -s [source file] -d [destination file]");
}

int main(int argc, char *argv[]) {

	InputStreamHandle* input_stream = NULL;
	OutputStreamHandle* output_stream = NULL;

	BufferHandle* input_data = NULL;

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

#if (defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

	if (argc < 8) {
		print_help();
		return VANILLAPDF_FILTER_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[1], "-f")) {
		print_help();
		return VANILLAPDF_FILTER_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[4], "-s")) {
		print_help();
		return VANILLAPDF_FILTER_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[6], "-d")) {
		print_help();
		return VANILLAPDF_FILTER_ERROR_INVALID_PARAMETERS;
	}

	filter_type = argv[2];
	operation = argv[3];
	source_file = argv[5];
	destination_file = argv[7];

	is_encode = (strcmp(operation, "encode") == 0);
	is_decode = (strcmp(operation, "decode") == 0);

	if (!is_encode && !is_decode) {
		print_help();
		return VANILLAPDF_FILTER_ERROR_INVALID_PARAMETERS;
	}

	is_flate_decode = (strcmp(filter_type, "flate") == 0);
	is_dct_decode = (strcmp(filter_type, "dct") == 0);
	is_ascii85_decode = (strcmp(filter_type, "ascii85") == 0);
	is_ascii_hex_decode = (strcmp(filter_type, "ascii_hex") == 0);

	if (!is_flate_decode && !is_dct_decode && !is_ascii85_decode && !is_ascii_hex_decode) {
		print_help();
		return VANILLAPDF_FILTER_ERROR_INVALID_PARAMETERS;
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

	return VANILLAPDF_FILTER_ERROR_SUCCESS;
}
