#include "tools.h"

void print_verify_help() {
    printf("Usage: verify -f [signed PDF file] [options]\n");
    printf("Options:\n");
    printf("  -f [file]              Signed PDF file to verify (required)\n");
    printf("  -c [certs]             Trusted certificate store directory (optional, uses system defaults if not specified)\n");
    printf("  -l [license]           License file (optional)\n");
    printf("  --allow-expired-certs  Allow expired certificates\n");
    printf("  --allow-weak-algorithms Allow weak cryptographic algorithms (MD5, SHA-1, RSA < 2048 bits)\n");
    printf("  --allow-untrusted-root Allow untrusted root certificates (self-signed)\n");
    printf("  --check-signing-time   Validate certificate at signing time instead of current time\n");
    printf("  --check-revocation     Check certificate revocation (CRL/OCSP)\n");
}

int process_verify(int argc, char *argv[]) {

    int arg_counter = 0;
    string_type license_file = NULL;
    string_type pdf_file = NULL;
    string_type certs_path = NULL;

    boolean_type allow_expired_certs = VANILLAPDF_RV_FALSE;
    boolean_type allow_weak_algorithms = VANILLAPDF_RV_FALSE;
    boolean_type allow_untrusted_root = VANILLAPDF_RV_FALSE;
    boolean_type check_signing_time = VANILLAPDF_RV_FALSE;
    boolean_type check_revocation = VANILLAPDF_RV_FALSE;

    DocumentHandle* document = NULL;
    FileHandle* file = NULL;
    CatalogHandle* catalog = NULL;
    InteractiveFormHandle* acro_form = NULL;
    FieldCollectionHandle* fields = NULL;
    FieldHandle* field = NULL;
    SignatureFieldHandle* sig_field = NULL;
    DigitalSignatureHandle* digital_signature = NULL;
    TrustedCertificateStoreHandle* trust_store = NULL;
    SignatureVerificationSettingsHandle* settings = NULL;
    SignatureVerificationResultHandle* result = NULL;

    for (arg_counter = 0; arg_counter < argc; ++arg_counter) {

        // PDF file
        if (strcmp(argv[arg_counter], "-f") == 0 && (arg_counter + 1 < argc)) {
            pdf_file = argv[arg_counter + 1];
            arg_counter++;

        // Certificates path
        } else if (strcmp(argv[arg_counter], "-c") == 0 && (arg_counter + 1 < argc)) {
            certs_path = argv[arg_counter + 1];
            arg_counter++;

        // License
        } else if (strcmp(argv[arg_counter], "-l") == 0 && (arg_counter + 1 < argc)) {
            license_file = argv[arg_counter + 1];
            arg_counter++;

        // Flags
        } else if (strcmp(argv[arg_counter], "--allow-expired-certs") == 0) {
            allow_expired_certs = VANILLAPDF_RV_TRUE;

        } else if (strcmp(argv[arg_counter], "--allow-weak-algorithms") == 0) {
            allow_weak_algorithms = VANILLAPDF_RV_TRUE;

        } else if (strcmp(argv[arg_counter], "--allow-untrusted-root") == 0) {
            allow_untrusted_root = VANILLAPDF_RV_TRUE;

        } else if (strcmp(argv[arg_counter], "--check-signing-time") == 0) {
            check_signing_time = VANILLAPDF_RV_TRUE;

        } else if (strcmp(argv[arg_counter], "--check-revocation") == 0) {
            check_revocation = VANILLAPDF_RV_TRUE;

        } else {
            print_verify_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (pdf_file == NULL) {
        printf("Error: PDF file is required\n");
        print_verify_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (license_file != NULL) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(license_file));
    }

    // Open PDF document
    printf("Opening PDF file: %s\n", pdf_file);
    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(pdf_file, &file));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(file, &document));

    // Get catalog
    error_type catalog_result = Document_GetCatalog(document, &catalog);
    if (catalog_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Error: Failed to get document catalog (not a valid PDF?)\n");
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    // Get AcroForm
    error_type acroform_result = Catalog_GetAcroForm(catalog, &acro_form);
    if (acroform_result == VANILLAPDF_ERROR_OBJECT_MISSING) {
        printf("Error: No AcroForm found in PDF (document has no form fields)\n");
        Catalog_Release(catalog);
        Document_Release(document);
        File_Release(file);
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }
    if (acroform_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Error: Failed to get AcroForm\n");
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    // Get fields
    error_type fields_result = InteractiveForm_GetFields(acro_form, &fields);
    if (fields_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Error: Failed to get form fields\n");
        InteractiveForm_Release(acro_form);
        Catalog_Release(catalog);
        Document_Release(document);
        File_Release(file);
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    size_type field_count = 0;
    RETURN_ERROR_IF_NOT_SUCCESS(FieldCollection_GetSize(fields, &field_count));

    if (field_count == 0) {
        printf("Error: No form fields found in PDF\n");
        FieldCollection_Release(fields);
        InteractiveForm_Release(acro_form);
        Catalog_Release(catalog);
        Document_Release(document);
        File_Release(file);
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    printf("Found %zu form field(s), searching for signature fields...\n", field_count);

    // Create or load trusted certificate store
    RETURN_ERROR_IF_NOT_SUCCESS(TrustedCertificateStore_Create(&trust_store));

    if (certs_path != NULL) {
        printf("Loading trusted certificates from: %s\n", certs_path);
        RETURN_ERROR_IF_NOT_SUCCESS(TrustedCertificateStore_LoadFromDirectory(trust_store, certs_path));
    } else {
        printf("Loading system default trusted certificates\n");
        RETURN_ERROR_IF_NOT_SUCCESS(TrustedCertificateStore_LoadSystemDefaults(trust_store));
    }

    // Configure verification settings
    RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_Create(&settings));

    if (allow_expired_certs) {
        printf("Allowing expired certificates\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetAllowExpiredCertsFlag(settings, VANILLAPDF_RV_TRUE));
    }

    if (allow_weak_algorithms) {
        printf("Allowing weak cryptographic algorithms\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(settings, VANILLAPDF_RV_TRUE));
    }

    if (allow_untrusted_root) {
        printf("Allowing untrusted root certificates\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetAllowUntrustedRootFlag(settings, VANILLAPDF_RV_TRUE));
    }

    if (check_signing_time) {
        printf("Checking certificate validity at signing time\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetCheckSigningTimeFlag(settings, VANILLAPDF_RV_TRUE));
    }

    if (check_revocation) {
        printf("Checking certificate revocation status\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetCheckRevocationFlag(settings, VANILLAPDF_RV_TRUE));
    }

    // Iterate through all fields and verify signature fields
    size_type signature_count = 0;
    int overall_result = VANILLAPDF_TOOLS_ERROR_SUCCESS;

    for (size_type i = 0; i < field_count; i++) {
        field = NULL;
        sig_field = NULL;
        digital_signature = NULL;
        result = NULL;

        // Get field at index
        error_type field_result = FieldCollection_At(fields, i, &field);
        if (field_result != VANILLAPDF_ERROR_SUCCESS) {
            printf("Warning: Failed to get field at index %zu\n", i);
            continue;
        }

        // Try to convert to signature field
        error_type sig_result = Field_ToSignature(field, &sig_field);
        if (sig_result != VANILLAPDF_ERROR_SUCCESS) {
            // Not a signature field, skip
            Field_Release(field);
            continue;
        }

        // Get digital signature value
        error_type value_result = SignatureField_GetValue(sig_field, &digital_signature);
        if (value_result != VANILLAPDF_ERROR_SUCCESS) {
            printf("Warning: Signature field at index %zu has no value\n", i);
            SignatureField_Release(sig_field);
            Field_Release(field);
            continue;
        }

        signature_count++;
        printf("\n=== Verifying Signature #%zu ===\n", signature_count);

        // Verify signature using DigitalSignatureExtensions
        RETURN_ERROR_IF_NOT_SUCCESS(DigitalSignatureExtensions_Verify(digital_signature, document, trust_store, settings, &result));

        // Get verification status
        SignatureVerificationStatusType status;
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_GetStatus(result, &status));

        // Get verification flags
        boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
        boolean_type is_document_intact = VANILLAPDF_RV_FALSE;
        boolean_type is_cert_trusted = VANILLAPDF_RV_FALSE;

        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_IsSignatureValid(result, &is_signature_valid));
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_IsDocumentIntact(result, &is_document_intact));
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_IsCertificateTrusted(result, &is_cert_trusted));

        // Get signer common name
        BufferHandle* signer_name = NULL;
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_GetSignerCommonName(result, &signer_name));

        // Get verification message
        BufferHandle* message_buffer = NULL;
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_GetMessage(result, &message_buffer));

        // Print results
        printf("Status: ");
        switch (status) {
            case SignatureStatus_Valid:
                printf("VALID\n");
                break;
            case SignatureStatus_Invalid:
                printf("INVALID\n");
                break;
            case SignatureStatus_CertificateExpired:
                printf("CERTIFICATE EXPIRED\n");
                break;
            case SignatureStatus_CertificateNotYetValid:
                printf("CERTIFICATE NOT YET VALID\n");
                break;
            case SignatureStatus_CertificateRevoked:
                printf("CERTIFICATE REVOKED\n");
                break;
            case SignatureStatus_CertificateUntrusted:
                printf("CERTIFICATE UNTRUSTED\n");
                break;
            case SignatureStatus_WeakAlgorithm:
                printf("WEAK CRYPTOGRAPHIC ALGORITHM\n");
                break;
            default:
                printf("UNKNOWN (%d)\n", status);
                break;
        }

        printf("Signature Valid: %s\n", is_signature_valid ? "Yes" : "No");
        printf("Document Intact: %s\n", is_document_intact ? "Yes" : "No");
        printf("Certificate Trusted: %s\n", is_cert_trusted ? "Yes" : "No");

        if (signer_name != NULL) {
            size_t name_size = 0;
            string_type name_data = NULL;
            RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(signer_name, &name_data, &name_size));
            if (name_size > 0) {
                printf("Signer: %s\n", name_data);
            }
        }

        if (message_buffer != NULL) {
            size_t message_size = 0;
            string_type message_data = NULL;
            RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(message_buffer, &message_data, &message_size));
            if (message_size > 0) {
                printf("Message: %s\n", message_data);
            }
        }

        // Track overall result (fail if any signature is invalid)
        if (status != SignatureStatus_Valid) {
            overall_result = VANILLAPDF_TOOLS_ERROR_FAILURE;
        }

        // Cleanup iteration resources
        if (message_buffer) Buffer_Release(message_buffer);
        if (signer_name) Buffer_Release(signer_name);
        if (result) SignatureVerificationResult_Release(result);
        if (digital_signature) DigitalSignature_Release(digital_signature);
        if (sig_field) SignatureField_Release(sig_field);
        if (field) Field_Release(field);
    }

    // Print summary
    printf("\n============================\n");
    if (signature_count == 0) {
        printf("No signature fields found in PDF\n");
        overall_result = VANILLAPDF_TOOLS_ERROR_FAILURE;
    } else {
        printf("Verified %zu signature(s)\n", signature_count);
    }
    printf("============================\n\n");

    // Cleanup (iteration resources already cleaned up in loop)
    if (settings) SignatureVerificationSettings_Release(settings);
    if (trust_store) TrustedCertificateStore_Release(trust_store);
    if (fields) FieldCollection_Release(fields);
    if (acro_form) InteractiveForm_Release(acro_form);
    if (catalog) Catalog_Release(catalog);
    if (document) Document_Release(document);
    if (file) File_Release(file);

    // Return overall result
    return overall_result;
}
