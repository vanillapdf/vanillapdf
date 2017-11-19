#include "merge.h"

const int VANILLAPDF_MERGE_ERROR_SUCCESS = 0;
const int VANILLAPDF_MERGE_ERROR_INVALID_PARAMETERS = 1;
const int VANILLAPDF_MERGE_ERROR_FAILURE = 255;

void print_help() {
	printf("Usage: -s [source file] -d [destination file] -f [array of merged files]");
}

int main(int argc, char *argv[]) {
	const int MERGE_FILES_START_INDEX = 6;

	const char *source_file = NULL;
	const char *destination_file = NULL;
	int merge_files_count = 0;

	DocumentHandle* document = NULL;

	integer_type i = 0;

#if (defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

	if (argc < 7) {
		print_help();
		return VANILLAPDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[1], "-s")) {
		print_help();
		return VANILLAPDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[3], "-d")) {
		print_help();
		return VANILLAPDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[5], "-f")) {
		print_help();
		return VANILLAPDF_MERGE_ERROR_INVALID_PARAMETERS;
	}

	source_file = argv[2];
	destination_file = argv[4];
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

	return VANILLAPDF_MERGE_ERROR_SUCCESS;
}
