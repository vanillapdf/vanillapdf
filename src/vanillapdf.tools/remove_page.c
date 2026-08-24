#include "tools.h"

void print_remove_page_help() {
    printf("Usage: remove_page -s [source file] -d [destination file] -n [page number] -p [password]");
}

// Removes the page at the given 1-based page number and saves the result
// into the destination file. Page numbers count leaf pages across the whole
// page tree, matching PageTree_GetPage, so nested intermediate nodes are
// handled transparently.
int process_remove_page(int argc, char *argv[]) {
    FileHandle* source_file = NULL;
    DocumentHandle* document = NULL;
    CatalogHandle* catalog = NULL;
    PageTreeHandle* page_tree = NULL;

    string_type password = NULL;
    string_type source_file_path = NULL;
    string_type destination_file_path = NULL;
    integer_type page_number = 0;

    size_type page_count_before = 0;
    size_type page_count_after = 0;

    unsigned long long page_count_before_converted = 0;
    unsigned long long page_count_after_converted = 0;

    integer_type i = 0;

    for (i = 0; i < argc; ++i) {

        // source file path
        if (strcmp(argv[i], "-s") == 0 && (i + 1 < argc)) {
            source_file_path = argv[i + 1];
            i++;

            // destination file path
        } else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc)) {
            destination_file_path = argv[i + 1];
            i++;

            // page number to be removed
        } else if (strcmp(argv[i], "-n") == 0 && (i + 1 < argc)) {
            page_number = atoi(argv[i + 1]);
            i++;

            // encryption password
        } else if (strcmp(argv[i], "-p") == 0 && (i + 1 < argc)) {
            password = argv[i + 1];
            i++;
        } else {
            print_remove_page_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (source_file_path == NULL || destination_file_path == NULL || page_number <= 0) {
        print_remove_page_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(source_file_path, &source_file));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, &document));

    if (password != NULL) {
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(source_file, password));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &page_tree));

    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(page_tree, &page_count_before));

    page_count_before_converted = page_count_before;

    if ((size_type) page_number > page_count_before) {
        printf("Page number %d is out of range, the document has %llu pages\n",
            page_number, page_count_before_converted);

        // Release the handles on this expected error path - a leak here trips
        // LeakSanitizer, whose abort also discards the buffered message above
        PageTree_Release(page_tree);
        Catalog_Release(catalog);
        Document_Release(document);
        File_Release(source_file);
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_RemovePage(page_tree, (size_type) page_number));
    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(page_tree, &page_count_after));

    page_count_after_converted = page_count_after;

    if (page_count_after != page_count_before - 1) {
        printf("Page count mismatch after removal: %llu -> %llu\n",
            page_count_before_converted, page_count_after_converted);

        PageTree_Release(page_tree);
        Catalog_Release(catalog);
        Document_Release(document);
        File_Release(source_file);
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, destination_file_path));

    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(page_tree));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Release(source_file));

    printf("Removed page %d (%llu -> %llu pages): %s\n",
        page_number, page_count_before_converted, page_count_after_converted, destination_file_path);

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
