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

/* Verify one signature and print the outcome */
static error_type verify_signature(DocumentHandle* document, DigitalSignatureHandle* signature,
    TrustedCertificateStoreHandle* trust_store, boolean_type* valid) {
    SignatureVerificationResultHandle* result = NULL;
    SignatureVerificationStatusType status = SignatureStatus_Invalid;
    boolean_type document_intact = VANILLAPDF_RV_FALSE;
    BufferHandle* signer = NULL;
    string_type signer_name = NULL;
    size_type signer_name_size = 0;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;

    /* NULL settings use the defaults: full chain validation, no weak algorithms */
    CHECK(DigitalSignatureExtensions_Verify(signature, document, trust_store, NULL, &result));

    CHECK(SignatureVerificationResult_GetStatus(result, &status));
    CHECK(SignatureVerificationResult_IsDocumentIntact(result, &document_intact));

    /*
    * The signer is unknown when the certificate has no common name, or the
    * signature is too damaged to identify it.
    */
    rc = SignatureVerificationResult_GetSignerCommonName(result, &signer);
    if (rc == VANILLAPDF_ERROR_SUCCESS) {
        CHECK(Buffer_GetData(signer, &signer_name, &signer_name_size));
        printf("Signer: %.*s\n", (int) signer_name_size, signer_name);
    } else if (rc == VANILLAPDF_ERROR_OBJECT_MISSING) {
        rc = VANILLAPDF_ERROR_SUCCESS;
        printf("Signer: unknown\n");
    } else {
        goto cleanup;
    }

    printf("Document intact: %s\n", document_intact ? "yes" : "no");
    printf("Status: %s\n", status == SignatureStatus_Valid ? "valid" : "not valid");

    *valid = (status == SignatureStatus_Valid) ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;

cleanup:
    if (signer) Buffer_Release(signer);
    if (result) SignatureVerificationResult_Release(result);

    return rc;
}

int main(int argc, char* argv[]) {
    DocumentHandle* document = NULL;
    CatalogHandle* catalog = NULL;
    InteractiveFormHandle* form = NULL;
    FieldTreeHandle* fields = NULL;
    TrustedCertificateStoreHandle* trust_store = NULL;
    FieldHandle* field = NULL;
    SignatureFieldHandle* signature_field = NULL;
    DigitalSignatureHandle* signature = NULL;
    size_type field_count = 0;
    size_type signature_count = 0;
    boolean_type all_valid = VANILLAPDF_RV_TRUE;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;
    size_type i = 0;

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s <signed.pdf> [trusted certificates directory]\n", argv[0]);
        return 1;
    }

    /* Certificates the signer's chain has to lead to */
    CHECK(TrustedCertificateStore_Create(&trust_store));
    if (argc == 3) {
        CHECK(TrustedCertificateStore_LoadFromDirectory(trust_store, argv[2]));
    } else {
        CHECK(TrustedCertificateStore_LoadSystemDefaults(trust_store));
    }

    /* Signatures are the values of the signature fields of the interactive form */
    CHECK(Document_Open(argv[1], &document));
    CHECK(Document_GetCatalog(document, &catalog));

    /* A document without an interactive form has no signatures */
    rc = Catalog_GetAcroForm(catalog, &form);
    if (rc == VANILLAPDF_ERROR_OBJECT_MISSING) {
        rc = VANILLAPDF_ERROR_SUCCESS;
        printf("Verified 0 signature(s)\n");
        goto cleanup;
    }

    CHECK(rc);
    CHECK(InteractiveForm_GetFieldTree(form, &fields));
    CHECK(FieldTree_GetFieldCount(fields, &field_count));

    for (i = 0; i < field_count; i++) {
        boolean_type valid = VANILLAPDF_RV_FALSE;

        CHECK(FieldTree_GetField(fields, i, &field));

        /* Fields of other types, and signature fields that were not signed yet, are skipped */
        if (SignatureField_FromField(field, &signature_field) == VANILLAPDF_ERROR_SUCCESS
            && SignatureField_GetValue(signature_field, &signature) == VANILLAPDF_ERROR_SUCCESS) {
            signature_count += 1;

            CHECK(verify_signature(document, signature, trust_store, &valid));
            if (!valid) {
                all_valid = VANILLAPDF_RV_FALSE;
            }
        }

        if (signature) DigitalSignature_Release(signature);
        if (signature_field) SignatureField_Release(signature_field);
        Field_Release(field);

        signature = NULL;
        signature_field = NULL;
        field = NULL;
    }

    printf("Verified %llu signature(s)\n", (unsigned long long) signature_count);

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char message[256] = { 0 };
        Errors_GetLastErrorMessage(message, sizeof(message));
        fprintf(stderr, "Error: %s\n", message);
    }

    if (signature) DigitalSignature_Release(signature);
    if (signature_field) SignatureField_Release(signature_field);
    if (field) Field_Release(field);
    if (fields) FieldTree_Release(fields);
    if (form) InteractiveForm_Release(form);
    if (catalog) Catalog_Release(catalog);
    if (document) Document_Release(document);
    if (trust_store) TrustedCertificateStore_Release(trust_store);

    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        return 1;
    }

    /* Exit code 2 tells a script that a signature did not verify */
    return (all_valid && signature_count > 0) ? 0 : 2;
}
