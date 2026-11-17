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

int main(int argc, char* argv[]) {
    DocumentHandle* document = NULL;
    DocumentHandle* other = NULL;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;
    int i = 0;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <merged.pdf> <first.pdf> <second.pdf> [more.pdf ...]\n", argv[0]);
        return 1;
    }

    CHECK(Document_Open(argv[2], &document));

    /* Append the pages of every other document, in order */
    for (i = 3; i < argc; i++) {
        CHECK(Document_Open(argv[i], &other));
        CHECK(Document_AppendDocument(document, other));

        Document_Release(other);
        other = NULL;
    }

    CHECK(Document_Save(document, argv[1]));

    printf("Merged %d documents into %s\n", argc - 2, argv[1]);

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char message[256] = { 0 };
        Errors_GetLastErrorMessage(message, sizeof(message));
        fprintf(stderr, "Error: %s\n", message);
    }

    if (other) Document_Release(other);
    if (document) Document_Release(document);

    return (rc == VANILLAPDF_ERROR_SUCCESS) ? 0 : 1;
}
