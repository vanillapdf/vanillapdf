#include "tools.h"

void print_sign_help() {
	printf("Usage: sign -s [source file] -d [destination file] -k [key file] -p [key password]");
}

int process_sign(int argc, char *argv[]) {
	string_type source_file = NULL;
	string_type destination_file = NULL;
	string_type key_file = NULL;
	string_type key_password = NULL;

	DocumentHandle* document = NULL;

	PKCS12KeyHandle* pkcs12_key = NULL;
	SigningKeyHandle* signing_key = NULL;
	DocumentSignatureSettingsHandle* signature_settings = NULL;

	if (argc < 6) {
		print_sign_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[0], "-s")) {
		print_sign_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[2], "-d")) {
		print_sign_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	if (0 != strcmp(argv[4], "-k")) {
		print_sign_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	source_file = argv[1];
	destination_file = argv[3];
	key_file = argv[5];

	if (argc >= 7) {
		if (0 != strcmp(argv[6], "-p")) {
			print_sign_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}

		key_password = argv[7];
	}

	RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(key_file, key_password, &pkcs12_key));
	RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToSigningKey(pkcs12_key, &signing_key));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Create(&signature_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(source_file, &document));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Sign(document, destination_file, signature_settings));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Release(signature_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
