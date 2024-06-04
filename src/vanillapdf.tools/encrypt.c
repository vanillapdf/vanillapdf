#include "tools.h"

void print_encrypt_help() {
	printf("Usage: encrypt -s [source file] -d [destination file] -op [owner password] -up [user password] -a [None|RC4|AES] -l [key length]");
}

int process_encrypt(int argc, char *argv[]) {
	FileHandle* source_file = NULL;
	DocumentHandle* source_document = NULL;

	string_type owner_password = NULL;
	string_type user_password = NULL;
	string_type encryption_algorithm_text = NULL;
	string_type source_file_path = NULL;
	string_type destination_file_path = NULL;

	integer_type key_length = 40;
	EncryptionAlgorithmType encryption_algorithm = EncryptionAlgorithmType_RC4;

	integer_type i = 0;

	DocumentEncryptionSettingsHandle* encryption_settings = NULL;
	BufferHandle* owner_password_buffer = NULL;
	BufferHandle* user_password_buffer = NULL;

	for (i = 0; i < argc; ++i) {

		// owner password
		if (strcmp(argv[i], "-op") == 0 && (i + 1 < argc)) {
			owner_password = argv[i + 1];
			i++;

			// user password
		} else if (strcmp(argv[i], "-up") == 0 && (i + 1 < argc)) {
			user_password = argv[i + 1];
			i++;

			// source file path
		} else if (strcmp(argv[i], "-s") == 0 && (i + 1 < argc)) {
			source_file_path = argv[i + 1];
			i++;

			// destination file path
		} else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc)) {
			destination_file_path = argv[i + 1];
			i++;

			// encryption algorithm
		} else if (strcmp(argv[i], "-a") == 0 && (i + 1 < argc)) {
			encryption_algorithm_text = argv[i + 1];
			i++;

			// key length
		} else if (strcmp(argv[i], "-l") == 0 && (i + 1 < argc)) {
			key_length = atoi(argv[i + 1]);
			i++;
		} else {
			print_encrypt_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}
	}

	if (source_file_path == NULL ||
		destination_file_path == NULL ||
		user_password == NULL ||
		owner_password == NULL) {
		print_encrypt_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (encryption_algorithm_text != NULL) {
		if (strcmp(encryption_algorithm_text, "None") == 0) {
			encryption_algorithm = EncryptionAlgorithmType_None;
		}

		if (strcmp(encryption_algorithm_text, "RC4") == 0) {
			encryption_algorithm = EncryptionAlgorithmType_RC4;
		}

		if (strcmp(encryption_algorithm_text, "AES") == 0) {
			encryption_algorithm = EncryptionAlgorithmType_AES;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(source_file_path, &source_file));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, &source_document));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_Create(&encryption_settings));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetAlgorithm(encryption_settings, encryption_algorithm));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetKeyLength(encryption_settings, key_length));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, UserAccessPermissionFlag_None));

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_CreateFromData(owner_password, strlen(owner_password), &owner_password_buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_CreateFromData(user_password, strlen(user_password), &user_password_buffer));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetOwnerPassword(encryption_settings, owner_password_buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetUserPassword(encryption_settings, user_password_buffer));

	// Add encryption to the document dictionary - the actual encryption is done by each object transparently
	RETURN_ERROR_IF_NOT_SUCCESS(Document_AddEncryption(source_document, encryption_settings));

	// Save the encrypted file into the destination path
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(source_document, destination_file_path));

	// Cleanup

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(owner_password_buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(user_password_buffer));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_Release(encryption_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(source_document));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(source_file));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
