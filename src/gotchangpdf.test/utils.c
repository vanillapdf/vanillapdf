#include "test.h"

const int GOTCHANG_PDF_TEST_ERROR_SUCCESS = 0;
const int GOTCHANG_PDF_TEST_ERROR_INVALID_PASSWORD = 1;
const int GOTCHANG_PDF_TEST_ERROR_INVALID_PARAMETERS = 2;
const int GOTCHANG_PDF_TEST_ERROR_LOGGING_ENABLED = 3;
const int GOTCHANG_PDF_TEST_ERROR_FAILURE = 255;

void print_spaces(int nested) {
	int i;
	for (i = 0; i < nested; ++i) {
		printf("  ");
	}
}

error_type process_buffer(BufferHandle buffer, int nested) {
	string_type data = NULL;
	char* local_string = NULL;
	size_type size = 0;
	size_type size_converted = 0;
	size_type print_size = 0;

	print_spaces(nested);
	printf("Buffer begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(buffer, &data, &size));

	if (size >= SIZE_MAX) {
		printf("Buffer size is too big: %ld bytes\n", size);
		return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	size_converted = (size_type) size;
	print_size = size_converted > 20 ? 10 : size_converted;

	local_string = (char*) calloc(sizeof(char), print_size + 1);
	if (NULL == local_string) {
		printf("Could not allocate memory: %ld bytes\n", print_size + 1);
		return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	memcpy(local_string, data, print_size);

	print_spaces(nested + 1);
	printf("Size: %ld\n", size_converted);
	print_spaces(nested + 1);
	printf("Data: %s\n", local_string);

	free(local_string);

	print_spaces(nested);
	printf("Buffer end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_version(PDFVersion version, int nested) {
	print_spaces(nested);
	printf("PDF Version: 1.%d \n", version);

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}
