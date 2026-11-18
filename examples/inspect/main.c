#include <stdio.h>
#include <vanillapdf/c_vanillapdf_api.h>

/* Store the result of a call and jump to the cleanup when it failed */
#define CHECK(call) \
    do { \
        rc = (call); \
        if (rc != VANILLAPDF_ERROR_SUCCESS) { \
            goto cleanup; \
        } \
    } while (0)

/*
* Print a text entry of the document information dictionary. The bytes are
* printed as stored, in the encoding of the text string: PDFDocEncoding,
* UTF-16BE or UTF-8.
*/
static error_type print_entry(const char* label, StringObjectHandle* value) {
    BufferHandle* buffer = NULL;
    string_type data = NULL;
    size_type size = 0;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;

    CHECK(StringObject_GetValue(value, &buffer));
    CHECK(Buffer_GetData(buffer, &data, &size));

    printf("%s: %.*s\n", label, (int) size, data);

cleanup:
    if (buffer) Buffer_Release(buffer);

    return rc;
}

int main(int argc, char* argv[]) {
    DocumentHandle* document = NULL;
    CatalogHandle* catalog = NULL;
    PageTreeHandle* page_tree = NULL;
    DocumentInfoHandle* info = NULL;
    StringObjectHandle* title = NULL;
    StringObjectHandle* producer = NULL;
    size_type page_count = 0;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.pdf>\n", argv[0]);
        return 1;
    }

    CHECK(Document_Open(argv[1], &document));

    CHECK(Document_GetCatalog(document, &catalog));
    CHECK(Catalog_GetPages(catalog, &page_tree));
    CHECK(PageTree_GetPageCount(page_tree, &page_count));

    printf("Pages: %llu\n", (unsigned long long) page_count);

    /*
    * The information dictionary and each of its entries are optional,
    * a missing one is reported as VANILLAPDF_ERROR_OBJECT_MISSING.
    */
    rc = Document_GetDocumentInfo(document, &info);
    if (rc == VANILLAPDF_ERROR_OBJECT_MISSING) {
        rc = VANILLAPDF_ERROR_SUCCESS;
        goto cleanup;
    }

    CHECK(rc);

    rc = DocumentInfo_GetTitle(info, &title);
    if (rc == VANILLAPDF_ERROR_SUCCESS) {
        CHECK(print_entry("Title", title));
    } else if (rc != VANILLAPDF_ERROR_OBJECT_MISSING) {
        goto cleanup;
    }

    rc = DocumentInfo_GetProducer(info, &producer);
    if (rc == VANILLAPDF_ERROR_SUCCESS) {
        CHECK(print_entry("Producer", producer));
    } else if (rc != VANILLAPDF_ERROR_OBJECT_MISSING) {
        goto cleanup;
    }

    rc = VANILLAPDF_ERROR_SUCCESS;

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char message[256] = { 0 };
        Errors_GetLastErrorMessage(message, sizeof(message));
        fprintf(stderr, "Error: %s\n", message);
    }

    if (producer) StringObject_Release(producer);
    if (title) StringObject_Release(title);
    if (info) DocumentInfo_Release(info);
    if (page_tree) PageTree_Release(page_tree);
    if (catalog) Catalog_Release(catalog);
    if (document) Document_Release(document);

    return (rc == VANILLAPDF_ERROR_SUCCESS) ? 0 : 1;
}
