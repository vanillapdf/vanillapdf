#include "tools.h"

void print_sign_custom_help() {
	printf("Usage: sign_custom -s [source file] -d [destination file]");
}

// Custom callbacks
typedef struct {
	BufferHandle* data;
} SignatureData;

error_type sign_init(void* user_data, MessageDigestAlgorithmType algorithm) {
	SignatureData* signature_data = (SignatureData*) user_data;

	const char SAMPLE_DATA[] = "Hello world!";

	UNUSED(algorithm);

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Create(&signature_data->data));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_SetData(signature_data->data, SAMPLE_DATA, sizeof(SAMPLE_DATA)));

	return VANILLAPDF_ERROR_SUCCESS;
}

error_type sign_update(void* user_data, const BufferHandle* data) {
	SignatureData* signature_data = (SignatureData*) user_data;
	string_type buffer_data = NULL;
	size_type buffer_size = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(data, &buffer_data, &buffer_size));

	// Calculate signature

	UNUSED(signature_data);

	return VANILLAPDF_ERROR_SUCCESS;
}

error_type sign_final(void* user_data, BufferHandle** result) {
	SignatureData* signature_data = (SignatureData*) user_data;

	*result = signature_data->data;

	return VANILLAPDF_ERROR_SUCCESS;
}

error_type sign_cleanup(void* user_data) {
	SignatureData* signature_data = (SignatureData*) user_data;

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(signature_data->data));

	return VANILLAPDF_ERROR_SUCCESS;
}

int process_sign_custom(int argc, char *argv[]) {
	string_type source_document_path = NULL;
	string_type destination_file_path = NULL;

	DocumentHandle* source_document = NULL;
	FileHandle* destination_file = NULL;

	DocumentSignatureSettingsHandle* signature_settings = NULL;
	SigningKeyHandle* signing_key = NULL;
	SignatureData user_data = {0};

	integer_type i = 0;

	for (i = 0; i < argc; ++i) {

		// source document path
		if (strcmp(argv[i], "-s") == 0 && (i + 1 < argc)) {
			source_document_path = argv[i + 1];
			i++;

		// destination file path
		} else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc)) {
			destination_file_path = argv[i + 1];
			i++;
		} else {
			print_sign_custom_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}
	}

	if (source_document_path == NULL || destination_file_path == NULL) {
		print_sign_custom_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(SigningKey_CreateCustom(&sign_init, &sign_update, &sign_final, &sign_cleanup, &user_data, &signing_key));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Create(&signature_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Create(destination_file_path, &destination_file));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(source_document_path, &source_document));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Sign(source_document, destination_file, signature_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(source_document));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(destination_file));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Release(signature_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(SigningKey_Release(signing_key));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
