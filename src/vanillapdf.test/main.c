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
// only returns VANILLAPDF_TEST_ERROR_FAILURE
// and any additional informations have to be
// extracted from library logging facility

int main(int argc, char *argv[]) {
	FileHandle file = NULL;
	DocumentHandle document = NULL;
	string_type license_file = NULL;
	string_type password = NULL;
	string_type cert_path = NULL;
	string_type cert_password = NULL;
	PKCS12KeyHandle pkcs12_key = NULL;
	boolean_type is_encrypted = VANILLAPDF_RV_FALSE;

#if (defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

	if (argc < 2) {
		return VANILLAPDF_TEST_ERROR_INVALID_PARAMETERS;
	}

	for (int i = 2; i < argc; ++i) {

		// password
		if (strcmp(argv[i], "-p") == 0 && (i + 1 < argc)) {
			password = argv[i + 1];
			i++;

		// certificate
		} else if (strcmp(argv[i], "-k") == 0 && (i + 1 < argc)) {
			cert_path = argv[i + 1];
			i++;

		// license
		} else if (strcmp(argv[i], "-l") == 0 && (i + 1 < argc)) {
			license_file = argv[i + 1];
			i++;
		} else {
			return VANILLAPDF_TEST_ERROR_INVALID_PARAMETERS;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_library_info());
	RETURN_ERROR_IF_NOT_SUCCESS(process_license_info(license_file));
	RETURN_ERROR_IF_NOT_SUCCESS(process_logging());

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_IsEncrypted(file, &is_encrypted));

	//RETURN_ERROR_IF_NOT_SUCCESS(Document_Create("input.pdf", &document));
	//RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, "output.pdf"));
	//RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));

	if (is_encrypted == VANILLAPDF_RV_TRUE) {
		// No password entered
		if (password == NULL && cert_path == NULL) {
			// Opening the file with default password
		}

		if (password != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(file, password));
		}

		if (cert_path != NULL) {
			EncryptionKeyHandle encryption_key = NULL;
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(cert_path, cert_password, &pkcs12_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(pkcs12_key, &encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(file, encryption_key));
		}
	} else {
		// Password for un-encrypted file
		if (password != NULL || cert_path != NULL) {
			return VANILLAPDF_TEST_ERROR_INVALID_PASSWORD;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(file, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(file, &document));
	RETURN_ERROR_IF_NOT_SUCCESS(process_document(document, 0));
	//RETURN_ERROR_IF_NOT_SUCCESS(process_document_save(document, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	if (NULL != pkcs12_key) {
		RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));
	}

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
