#include "tools.h"

const int VANILLAPDF_TOOLS_ERROR_SUCCESS = 0;
const int VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS = 1;
const int VANILLAPDF_TOOLS_ERROR_FAILURE = 255;

void print_help() {
	printf("Usage: [merge|filter|extract] [options]");
}

int main(int argc, char *argv[]) {

#if (defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

	if (argc < 2) {
		print_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 == strcmp(argv[1], "merge")) {
		return process_merge(argc - 2, &argv[2]);
	}

	if (0 == strcmp(argv[1], "filter")) {
		return process_filter(argc - 2, &argv[2]);
	}

	if (0 == strcmp(argv[1], "extract")) {
		return process_extract(argc - 2, &argv[2]);
	}

	print_help();
	return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
}
