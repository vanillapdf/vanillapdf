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
    FileHandle* destination = NULL;
    PKCS12KeyHandle* key_file = NULL;
    SigningKeyHandle* signing_key = NULL;
    DateHandle* signing_time = NULL;
    DocumentSignatureSettingsHandle* settings = NULL;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input.pdf> <signed.pdf> <key.p12> <password>\n", argv[0]);
        return 1;
    }

    /* The PKCS#12 file holds the signer's certificate and private key */
    CHECK(PKCS12Key_CreateFromFile(argv[3], argv[4], &key_file));
    CHECK(PKCS12Key_ToSigningKey(key_file, &signing_key));

    CHECK(Date_CreateCurrent(&signing_time));

    CHECK(DocumentSignatureSettings_Create(&settings));
    CHECK(DocumentSignatureSettings_SetSigningKey(settings, signing_key));
    CHECK(DocumentSignatureSettings_SetDigest(settings, MessageDigestAlgorithmType_SHA256));
    CHECK(DocumentSignatureSettings_SetSigningTime(settings, signing_time));

    /* The signed document is written to a new file, the input stays unchanged */
    CHECK(Document_Open(argv[1], &document));
    CHECK(File_Create(argv[2], &destination));
    CHECK(Document_Sign(document, destination, settings));

    printf("Signed %s into %s\n", argv[1], argv[2]);

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char message[256] = { 0 };
        Errors_GetLastErrorMessage(message, sizeof(message));
        fprintf(stderr, "Error: %s\n", message);
    }

    if (destination) File_Release(destination);
    if (document) Document_Release(document);
    if (settings) DocumentSignatureSettings_Release(settings);
    if (signing_time) Date_Release(signing_time);
    if (signing_key) SigningKey_Release(signing_key);
    if (key_file) PKCS12Key_Release(key_file);

    return (rc == VANILLAPDF_ERROR_SUCCESS) ? 0 : 1;
}
