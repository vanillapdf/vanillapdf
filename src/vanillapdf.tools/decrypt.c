#include "tools.h"

void print_decrypt_help() {
    printf("Usage: decrypt -s [source file] -d [destination file] -p [password] -k [path to PKCS#12] -l [license file]");
}

int process_decrypt(int argc, char *argv[]) {
    FileHandle* source_file = NULL;
    DocumentHandle* source_document = NULL;

    string_type password = NULL;
    string_type cert_path = NULL;

    string_type license_file_path = NULL;
    string_type source_file_path = NULL;
    string_type destination_file_path = NULL;

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
            source_file_path = argv[i + 1];
            i++;
            // destination file path
        } else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc)) {
            destination_file_path = argv[i + 1];
            i++;
            // license file path
        } else if (strcmp(argv[i], "-l") == 0 && (i + 1 < argc)) {
            license_file_path = argv[i + 1];
            i++;
        }
        else {
            print_decrypt_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (source_file_path == NULL) {
        print_decrypt_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (destination_file_path == NULL) {
        print_decrypt_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (license_file_path == NULL) {
        print_decrypt_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (license_file_path != NULL) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(license_file_path));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(source_file_path, &source_file));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, &source_document));

    if (password != NULL) {
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(source_file, password));
    }

    if (cert_path != NULL) {
        PKCS12KeyHandle* pkcs12_key = NULL;
        EncryptionKeyHandle* encryption_key = NULL;
        RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(cert_path, NULL, &pkcs12_key));
        RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(pkcs12_key, &encryption_key));
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(source_file, encryption_key));

        RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));
        RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));
    }

    // Remove file encryption
    RETURN_ERROR_IF_NOT_SUCCESS(Document_RemoveEncryption(source_document));

    // Save the decrypted file into the destination path
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(source_document, destination_file_path));

    // Cleanup
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(source_document));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Release(source_file));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
