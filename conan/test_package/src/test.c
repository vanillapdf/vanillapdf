#include <stdio.h>
#include <vanillapdf/c_vanillapdf_api.h>

int main(void) {
    DocumentHandle* document = NULL;
    error_type result = Document_Create("test_output.pdf", &document);

    if (result != VANILLAPDF_ERROR_SUCCESS) {
        fprintf(stderr, "Failed to create document (error: %d)\n", (int)result);
        return 1;
    }

    if (!document) {
        fprintf(stderr, "Document handle is null\n");
        return 1;
    }

    Document_Release(document);

    printf("vanillapdf Conan package test passed\n");
    return 0;
}
