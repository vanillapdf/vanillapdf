#include <stdio.h>
#include <string.h>
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
    DocumentEncryptionSettingsHandle* settings = NULL;
    BufferHandle* user_password = NULL;
    BufferHandle* owner_password = NULL;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input.pdf> <encrypted.pdf> <user password> <owner password>\n", argv[0]);
        return 1;
    }

    /* The user password opens the document, the owner password also lifts
       the access restrictions */
    CHECK(Buffer_CreateFromData(argv[3], strlen(argv[3]), &user_password));
    CHECK(Buffer_CreateFromData(argv[4], strlen(argv[4]), &owner_password));

    CHECK(DocumentEncryptionSettings_Create(&settings));
    CHECK(DocumentEncryptionSettings_SetAlgorithm(settings, EncryptionAlgorithmType_AES));
    CHECK(DocumentEncryptionSettings_SetKeyLength(settings, 256));
    CHECK(DocumentEncryptionSettings_SetUserPassword(settings, user_password));
    CHECK(DocumentEncryptionSettings_SetOwnerPassword(settings, owner_password));

    /* Readers opening the document with the user password may only print it */
    CHECK(DocumentEncryptionSettings_SetUserAccessPermissions(settings, UserAccessPermissionFlag_PrintFaithful));

    /* Every object is encrypted when the document is saved */
    CHECK(Document_Open(argv[1], &document));
    CHECK(Document_AddEncryption(document, settings));
    CHECK(Document_Save(document, argv[2]));

    printf("Encrypted %s into %s\n", argv[1], argv[2]);

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char message[256] = { 0 };
        Errors_GetLastErrorMessage(message, sizeof(message));
        fprintf(stderr, "Error: %s\n", message);
    }

    if (document) Document_Release(document);
    if (settings) DocumentEncryptionSettings_Release(settings);
    if (owner_password) Buffer_Release(owner_password);
    if (user_password) Buffer_Release(user_password);

    return (rc == VANILLAPDF_ERROR_SUCCESS) ? 0 : 1;
}
