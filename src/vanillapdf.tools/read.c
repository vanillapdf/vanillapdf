#include "tools.h"

void print_read_help() {
    printf("Usage: read -f [PDF file]\n");
    printf("Options:\n");
    printf("  -f [file]  PDF file to read using memory IO strategy (required)\n");
}

int process_read(int argc, char *argv[]) {

    int arg_counter = 0;
    const char *filename = NULL;

    size_type page_count = 0;

    FileHandle* file = NULL;
    DocumentHandle* document = NULL;
    CatalogHandle* catalog = NULL;
    PageTreeHandle* pages = NULL;

    for (arg_counter = 0; arg_counter < argc; ++arg_counter) {

        // PDF file
        if (strcmp(argv[arg_counter], "-f") == 0 && (arg_counter + 1 < argc)) {
            filename = argv[arg_counter + 1];
            arg_counter += 1;
        } else {
            print_read_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (filename == NULL) {
        print_read_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(File_OpenWithStrategy(filename, IOStrategy_Memory, &file));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));

    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(file, &document));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &pages));
    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(pages, &page_count));

    printf("Successfully read %llu pages from %s using memory IO strategy\n",
        (unsigned long long) page_count, filename);

    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(pages));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
