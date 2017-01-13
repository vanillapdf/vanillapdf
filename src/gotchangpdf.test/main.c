#include "test.h"

// I have used error code from library calls
// to forward them as exit status from application
// as well. Unfortunately, linux does not
// support exit status greater than 255.

// When a program exits, it can return to the parent process
// a small amount of information about the cause of termination,
// using the exit status. This is a value between 0 and 255 that
// the exiting process passes as an argument to exit.

// Therefore on library failures this application
// only returns GOTCHANG_PDF_TEST_ERROR_FAILURE
// and any additional informations have to be
// extracted from library logging facility

int main(int argc, char *argv[])
{
	FileHandle file = NULL;
	DocumentHandle document = NULL;
	string_type password = NULL;
	string_type cert_path = NULL;
	string_type cert_password = NULL;
	EncryptionKeyHandle encryption_key = NULL;
	boolean_type is_encrypted = GOTCHANG_PDF_RV_FALSE;
	boolean_type logging_enabled = GOTCHANG_PDF_RV_FALSE;
	LoggingSeverity logging_severity;
	integer_type library_version_major = 0;
	integer_type library_version_minor = 0;
	integer_type library_version_patch = 0;
	string_type library_author = NULL;

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif

	if (argc < 2) {
		return GOTCHANG_PDF_TEST_ERROR_INVALID_PARAMETERS;
	}

	if (argc >= 4) {
		// password
		if (0 == strcmp(argv[2], "-p")) {
			password = argv[3];
		}
		// key
		else if (0 == strcmp(argv[2], "-k")) {
			cert_path = argv[3];
			if (argc >= 5) {
				cert_password = argv[4];
			}
		}
		else {
			return GOTCHANG_PDF_TEST_ERROR_INVALID_PARAMETERS;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionMajor(&library_version_major));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionMinor(&library_version_minor));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionPatch(&library_version_patch));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetAuthor(&library_author));

	printf("Library gotchangpdf %d.%d.%d by %s\n",
		library_version_major,
		library_version_minor,
		library_version_patch,
		library_author
	);

	// I don't have easy idea how to verify disabled
	// logging or different severity, so just test API
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_GetEnabled(&logging_enabled));

	if (logging_enabled != GOTCHANG_PDF_RV_FALSE) {
		return GOTCHANG_PDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_SetEnabled());

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_GetSeverity(&logging_severity));
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_SetSeverity(logging_severity));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_IsEncrypted(file, &is_encrypted));

	if (is_encrypted == GOTCHANG_PDF_RV_TRUE) {
		// No password entered
		if (password == NULL && cert_path == NULL) {
			// Opening the file with default password
		}

		if (password != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(file, password));
		}

		if (cert_path != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_CreateFromPkcs12File(cert_path, cert_password, &encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(file, encryption_key));
		}
	}
	else {
		// Password for un-encrypted file
		if (password != NULL || cert_path != NULL) {
			return GOTCHANG_PDF_TEST_ERROR_INVALID_PASSWORD;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(file, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file, &document));
	RETURN_ERROR_IF_NOT_SUCCESS(process_document(document, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	if (NULL != encryption_key)
		RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}
