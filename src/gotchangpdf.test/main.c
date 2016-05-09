#include "test.h"

int main(int argc, char *argv[])
{
	FileHandle file = NULL;
	DocumentHandle document = NULL;
	string_type password = NULL;
	string_type cert_path = NULL;
	string_type cert_password = NULL;
	EncryptionKeyHandle encryption_key = NULL;
	boolean_type is_encrypted = GOTCHANG_PDF_FALSE;

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif

	if (argc < 2)
		return GOTCHANG_PDF_ERROR_GENERAL;

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
			return GOTCHANG_PDF_ERROR_GENERAL;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_IsEncrypted(file, &is_encrypted));

	if (is_encrypted == GOTCHANG_PDF_TRUE) {
		// No password entered
		if (password == NULL && cert_path == NULL) {
			return GOTCHANG_PDF_ERROR_INVALID_PASSWORD;
		}

		if (password != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(file, password));
		}

		if (cert_path != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(Pkcs12EncryptionKey_CreateFromFile(cert_path, cert_password, &encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(file, encryption_key));
		}
	}
	else {
		// Password for un-encrypted file
		if (password != NULL || cert_path != NULL) {
			return GOTCHANG_PDF_ERROR_INVALID_PASSWORD;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(file, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file, &document));
	RETURN_ERROR_IF_NOT_SUCCESS(process_document(document, 0));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	if (NULL != encryption_key)
		RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));

	return GOTCHANG_PDF_ERROR_SUCCES;
}
