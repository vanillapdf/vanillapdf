#include "test.h"

int main(int argc, char *argv[])
{
	FileHandle file = NULL;
	DocumentHandle document = NULL;

	if (argc != 2)
		return GOTCHANG_PDF_ERROR_GENERAL;

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(process_file(file, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file, &document));
	RETURN_ERROR_IF_NOT_SUCCESS(process_document(document, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	return GOTCHANG_PDF_ERROR_SUCCES;
}
