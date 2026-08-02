#include "tools.h"

void print_generate_help() {
    printf("Usage: generate -d [destination file] -p [page count]");
}

// Creates a new PDF document with the requested number of empty pages, appended
// directly to the root /Pages node (a flat page tree). Useful for generating
// large synthetic documents (e.g. page-tree benchmark fixtures) without
// shipping big real-world files.
int process_generate(int argc, char *argv[]) {
    string_type destination_file_path = NULL;
    integer_type page_count = 0;

    DocumentHandle* document = NULL;
    CatalogHandle* catalog = NULL;
    PageTreeHandle* page_tree = NULL;
    NameObjectHandle* page_index_key = NULL;

    integer_type i = 0;

    for (i = 0; i < argc; ++i) {

        // destination file path
        if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc)) {
            destination_file_path = argv[i + 1];
            i++;

            // number of pages
        } else if (strcmp(argv[i], "-p") == 0 && (i + 1 < argc)) {
            page_count = atoi(argv[i + 1]);
            i++;
        } else {
            print_generate_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (destination_file_path == NULL || page_count <= 0) {
        print_generate_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Create(destination_file_path, &document));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &page_tree));

    // Workaround for https://github.com/vanillapdf/vanillapdf/issues/458: the
    // file writer deduplicates content-identical objects, so byte-identical
    // empty pages would collapse into a single shared page. Tag each page with
    // a unique index under a marker key to keep the pages distinct. Once #458
    // is fixed this per-page key can be dropped.
    RETURN_ERROR_IF_NOT_SUCCESS(NameObject_CreateFromDecodedString("BenchPageIndex", &page_index_key));

    for (i = 0; i < page_count; ++i) {
        PageObjectHandle* page = NULL;
        DictionaryObjectHandle* page_dictionary = NULL;
        IntegerObjectHandle* index_object = NULL;
        ObjectHandle* index_value = NULL;

        RETURN_ERROR_IF_NOT_SUCCESS(PageObject_CreateFromDocument(document, &page));

        RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetBaseObject(page, &page_dictionary));
        RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_CreateFromIntegerValue(i, &index_object));
        RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_ToObject(index_object, &index_value));
        RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Insert(page_dictionary, page_index_key, index_value, VANILLAPDF_RV_TRUE));

        RETURN_ERROR_IF_NOT_SUCCESS(PageTree_AppendPage(page_tree, page));

        RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(index_value));
        RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(index_object));
        RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(page_dictionary));
        RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, destination_file_path));

    RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(page_index_key));
    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(page_tree));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));

    printf("Generated %d-page document: %s\n", page_count, destination_file_path);

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
