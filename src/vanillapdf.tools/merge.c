#include "tools.h"

void print_merge_help() {
	printf("Usage: merge -s [source file] -d [destination file] -f [array of merged files]");
}

int process_merge(int argc, char *argv[]) {
	const int MERGE_FILES_START_INDEX = 5;

	const char *source_file = NULL;
	const char *destination_file = NULL;
	int merge_files_count = 0;

	DocumentHandle* document = NULL;

	integer_type i = 0;

	if (argc < 6) {
		print_merge_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[0], "-s")) {
		print_merge_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[2], "-d")) {
		print_merge_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[4], "-f")) {
		print_merge_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	source_file = argv[1];
	destination_file = argv[3];
	merge_files_count = argc - MERGE_FILES_START_INDEX;

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(source_file, &document));

	for (i = 0; i < merge_files_count; ++i) {
		DocumentHandle* other_document = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(argv[MERGE_FILES_START_INDEX + i], &other_document));
		RETURN_ERROR_IF_NOT_SUCCESS(Document_AppendDocument(document, other_document));
		RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(other_document));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, destination_file));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
