#include "tools.h"

void print_decrypt_help() {
	printf("Usage: decrypt -s [source file] -p [password] -k [path to PKCS#12]");
}

int process_decrypt(int argc, char *argv[]) {
	FileHandle* file = NULL;
	string_type password = NULL;
	string_type cert_path = NULL;
	string_type file_path = NULL;

	integer_type i = 0;

	for (i = 0; i < argc; ++i) {

		// password
		if (strcmp(argv[i], "-p") == 0 && (i + 1 < argc)) {
			password = argv[i + 1];
			i++;

			// certificate
		} else if (strcmp(argv[i], "-k") == 0 && (i + 1 < argc)) {
			cert_path = argv[i + 1];
			i++;
			// source file path
		} else if (strcmp(argv[i], "-s") == 0 && (i + 1 < argc)) {
			file_path = argv[i + 1];
			i++;
		} else {
			print_decrypt_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}
	}

	if (file_path == NULL) {
		print_decrypt_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(file_path, &file));

	if (password != NULL) {
		RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(file, password));
	}

	if (cert_path != NULL) {
		PKCS12KeyHandle* pkcs12_key = NULL;
		EncryptionKeyHandle* encryption_key = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(cert_path, NULL, &pkcs12_key));
		RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(pkcs12_key, &encryption_key));
		RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(file, encryption_key));

		RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));
		RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));
	}

	// File contents are now accessible

	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));
	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
