#include "tools.h"

void print_sign_help() {
    printf("Usage: sign -s [source file] -d [destination file] -k [key file] -p [key password] -l [license file]");
}

int process_sign(int argc, char *argv[]) {

    int arg_counter = 0;
    string_type license_file = NULL;
    string_type source_document_path = NULL;
    string_type destination_file_path = NULL;
    string_type key_file = NULL;
    string_type key_password = NULL;

    DocumentHandle* source_document = NULL;
    FileHandle* destination_file = NULL;

    PKCS12KeyHandle* pkcs12_key = NULL;
    SigningKeyHandle* signing_key = NULL;
    DateHandle* signing_time = NULL;
    DocumentSignatureSettingsHandle* signature_settings = NULL;

    for (arg_counter = 0; arg_counter < argc; ++arg_counter) {

        // source file
        if (strcmp(argv[arg_counter], "-s") == 0 && (arg_counter + 1 < argc)) {
            source_document_path = argv[arg_counter + 1];
            arg_counter++;

        // destination file
        } else if (strcmp(argv[arg_counter], "-d") == 0 && (arg_counter + 1 < argc)) {
            destination_file_path = argv[arg_counter + 1];
            arg_counter++;

        // key file
        } else if (strcmp(argv[arg_counter], "-k") == 0 && (arg_counter + 1 < argc)) {
            key_file = argv[arg_counter + 1];
            arg_counter++;

        // key file password
        } else if (strcmp(argv[arg_counter], "-p") == 0 && (arg_counter + 1 < argc)) {
            key_password = argv[arg_counter + 1];
            arg_counter++;

        // license
        } else if (strcmp(argv[arg_counter], "-l") == 0 && (arg_counter + 1 < argc)) {
            license_file = argv[arg_counter + 1];
            arg_counter++;

        } else {
            print_sign_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (source_document_path == NULL) {
        print_sign_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (destination_file_path == NULL) {
        print_sign_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (key_file == NULL) {
        print_sign_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (license_file != NULL) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(license_file));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(key_file, key_password, &pkcs12_key));
    RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToSigningKey(pkcs12_key, &signing_key));

    RETURN_ERROR_IF_NOT_SUCCESS(Date_CreateCurrent(&signing_time));

    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Create(&signature_settings));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time));

    RETURN_ERROR_IF_NOT_SUCCESS(File_Create(destination_file_path, &destination_file));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(source_document_path, &source_document));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Sign(source_document, destination_file, signature_settings));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(source_document));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Release(destination_file));

    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Release(signature_settings));
    RETURN_ERROR_IF_NOT_SUCCESS(Date_Release(signing_time));

    RETURN_ERROR_IF_NOT_SUCCESS(SigningKey_Release(signing_key));
    RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
