#include "tools.h"

void print_resave_help() {
    printf("Usage: resave -s [source file] -d [destination file] -p [password]");
}

int process_resave(int argc, char *argv[]) {
    FileHandle* source_file = NULL;
    DocumentHandle* source_document = NULL;

    string_type password = NULL;
    string_type source_file_path = NULL;
    string_type destination_file_path = NULL;

    integer_type i = 0;

    for (i = 0; i < argc; ++i) {

        // password
        if (strcmp(argv[i], "-p") == 0 && (i + 1 < argc)) {
            password = argv[i + 1];
            i++;

            // source file path
        } else if (strcmp(argv[i], "-s") == 0 && (i + 1 < argc)) {
            source_file_path = argv[i + 1];
            i++;

            // destination file path
        } else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc)) {
            destination_file_path = argv[i + 1];
            i++;
        } else {
            print_resave_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (source_file_path == NULL) {
        print_resave_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (destination_file_path == NULL) {
        print_resave_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(source_file_path, &source_file));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, &source_document));

    if (password != NULL) {
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(source_file, password));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(source_document, destination_file_path));

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(source_document));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Release(source_file));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
