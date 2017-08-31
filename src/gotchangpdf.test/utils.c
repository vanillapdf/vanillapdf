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
	size_type print_size = 0;
	unsigned long long size_converted = 0;

	print_spaces(nested);
	printf("Buffer begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(buffer, &data, &size));

	if (size >= SIZE_MAX) {
		unsigned long long size_long_long = size;
		printf("Buffer size is too big: %llu bytes\n", size_long_long);
		return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	print_size = size > 20 ? 10 : size;
	size_converted = size;

	local_string = (char*) calloc(sizeof(char), print_size + 1);
	if (NULL == local_string) {
		unsigned long long print_size_converted = print_size;
		printf("Could not allocate memory: %llu bytes\n", print_size_converted + 1);
		return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	memcpy(local_string, data, print_size);

	print_spaces(nested + 1);
	printf("Size: %llu\n", size_converted);
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

error_type process_logging() {
	boolean_type logging_enabled = GOTCHANG_PDF_RV_TRUE;
	LoggingSeverity logging_severity;

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_IsEnabled(&logging_enabled));

	if (logging_enabled != GOTCHANG_PDF_RV_FALSE) {
		return GOTCHANG_PDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_Enable());
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_IsEnabled(&logging_enabled));

	if (logging_enabled != GOTCHANG_PDF_RV_TRUE) {
		return GOTCHANG_PDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_Disable());
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_IsEnabled(&logging_enabled));

	if (logging_enabled != GOTCHANG_PDF_RV_FALSE) {
		return GOTCHANG_PDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_GetSeverity(&logging_severity));
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_SetSeverity(logging_severity));

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_library_info() {
	string_type library_author = NULL;
	integer_type library_version_major = 0;
	integer_type library_version_minor = 0;
	integer_type library_version_patch = 0;
	integer_type library_version_build = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionMajor(&library_version_major));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionMinor(&library_version_minor));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionPatch(&library_version_patch));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionPatch(&library_version_build));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetAuthor(&library_author));

	printf("Library gotchangpdf %d.%d.%d.%d by %s\n",
		library_version_major,
		library_version_minor,
		library_version_patch,
		library_version_build,
		library_author
	);

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type print_last_error() {
	char* message = NULL;
	error_type error = 0;
	size_type length = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetLastError(&error));
	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetLastErrorMessageLength(&length));

	if (length >= SIZE_MAX) {
		unsigned long long length_converted = length;
		printf("Buffer size is too big: %llu bytes\n", length_converted);
		return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	message = (char*) calloc(sizeof(char), length + 1);
	if (NULL == message) {
		unsigned long long length_converted = length;
		printf("Could not allocate memory: %llu bytes\n", length_converted + 1);
		return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetLastErrorMessage(message, length));

	printf("Error %u: %s\n", error, message);

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}