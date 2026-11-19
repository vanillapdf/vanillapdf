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

int main(void) {
    DocumentHandle* document = NULL;
    CatalogHandle* catalog = NULL;
    PageTreeHandle* page_tree = NULL;
    PageObjectHandle* page = NULL;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;

    CHECK(Document_Create("output.pdf", &document));
    CHECK(Document_GetCatalog(document, &catalog));
    CHECK(Catalog_GetPages(catalog, &page_tree));
    CHECK(PageObject_CreateFromDocument(document, &page));
    CHECK(PageTree_AppendPage(page_tree, page));
    CHECK(Document_Save(document, "output.pdf"));

    printf("Created output.pdf\n");

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char message[256] = { 0 };
        Errors_GetLastErrorMessage(message, sizeof(message));
        fprintf(stderr, "Error: %s\n", message);
    }

    if (page) PageObject_Release(page);
    if (page_tree) PageTree_Release(page_tree);
    if (catalog) Catalog_Release(catalog);
    if (document) Document_Release(document);

    return (rc == VANILLAPDF_ERROR_SUCCESS) ? 0 : 1;
}
