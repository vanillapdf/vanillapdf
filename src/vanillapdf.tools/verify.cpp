#include "tools.h"

namespace {

struct VerifyOptions {
    std::string file;
    std::string certificates_path;
    std::string license_file;
    bool skip_certificate_validation = false;
    bool allow_weak_algorithms = false;
    bool check_signing_time = false;
    /* TODO: CRL/OCSP revocation checking (https://github.com/vanillapdf/vanillapdf/issues/157) */
    /* bool check_revocation = false; */
};

int process_verify(const VerifyOptions& options) {
    DocumentGuard document;
    FileGuard file;
    CatalogGuard catalog;
    InteractiveFormGuard acro_form;
    FieldCollectionGuard fields;
    TrustedCertificateStoreGuard trust_store;
    SignatureVerificationSettingsGuard settings;

    if (!options.license_file.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(options.license_file.c_str()));
    }

    // Open PDF document
    printf("Opening PDF file: %s\n", options.file.c_str());
    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.file.c_str(), file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(file, document.out()));

    // Get catalog
    error_type catalog_result = Document_GetCatalog(document, catalog.out());
    if (catalog_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Error: Failed to get document catalog (not a valid PDF?)\n");
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    // Get AcroForm
    error_type acroform_result = Catalog_GetAcroForm(catalog, acro_form.out());
    if (acroform_result == VANILLAPDF_ERROR_OBJECT_MISSING) {
        printf("Error: No AcroForm found in PDF (document has no form fields)\n");
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }
    if (acroform_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Error: Failed to get AcroForm\n");
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    // Get fields
    error_type fields_result = InteractiveForm_GetFields(acro_form, fields.out());
    if (fields_result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Error: Failed to get form fields\n");
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    size_type field_count = 0;
    RETURN_ERROR_IF_NOT_SUCCESS(FieldCollection_GetSize(fields, &field_count));

    if (field_count == 0) {
        printf("Error: No form fields found in PDF\n");
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    printf("Found %llu form field(s), searching for signature fields...\n", (unsigned long long) field_count);

    //! [Create trust store]
    // Create or load trusted certificate store
    RETURN_ERROR_IF_NOT_SUCCESS(TrustedCertificateStore_Create(trust_store.out()));

    if (!options.certificates_path.empty()) {
        printf("Loading trusted certificates from: %s\n", options.certificates_path.c_str());
        RETURN_ERROR_IF_NOT_SUCCESS(TrustedCertificateStore_LoadFromDirectory(trust_store, options.certificates_path.c_str()));
    } else {
        printf("Loading system default trusted certificates\n");
        RETURN_ERROR_IF_NOT_SUCCESS(TrustedCertificateStore_LoadSystemDefaults(trust_store));
    }
    //! [Create trust store]

    //! [Configure verification settings]
    // Configure verification settings
    RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_Create(settings.out()));

    if (options.skip_certificate_validation) {
        printf("WARNING: Skipping certificate chain validation (insecure)\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetSkipCertificateValidation(settings, VANILLAPDF_RV_TRUE));
    }

    if (options.allow_weak_algorithms) {
        printf("Allowing weak cryptographic algorithms\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(settings, VANILLAPDF_RV_TRUE));
    }

    if (options.check_signing_time) {
        printf("Checking certificate validity at signing time\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetCheckSigningTimeFlag(settings, VANILLAPDF_RV_TRUE));
    }

    /* TODO: CRL/OCSP revocation checking (https://github.com/vanillapdf/vanillapdf/issues/157)
    if (options.check_revocation) {
        printf("Checking certificate revocation status\n");
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationSettings_SetCheckRevocationFlag(settings, VANILLAPDF_RV_TRUE));
    }
    */
    //! [Configure verification settings]

    // Iterate through all fields and verify signature fields
    size_type signature_count = 0;
    int overall_result = VANILLAPDF_TOOLS_ERROR_SUCCESS;

    for (size_type i = 0; i < field_count; i++) {
        FieldGuard field;
        SignatureFieldGuard sig_field;
        DigitalSignatureGuard digital_signature;
        SignatureVerificationResultGuard result;

        // Get field at index
        error_type field_result = FieldCollection_At(fields, i, field.out());
        if (field_result != VANILLAPDF_ERROR_SUCCESS) {
            printf("Warning: Failed to get field at index %llu\n", (unsigned long long) i);
            continue;
        }

        // Try to convert to signature field
        error_type sig_result = SignatureField_FromField(field, sig_field.out());
        if (sig_result != VANILLAPDF_ERROR_SUCCESS) {
            // Not a signature field, skip
            continue;
        }

        // Get digital signature value
        error_type value_result = SignatureField_GetValue(sig_field, digital_signature.out());
        if (value_result != VANILLAPDF_ERROR_SUCCESS) {
            printf("Warning: Signature field at index %llu has no value\n", (unsigned long long) i);
            continue;
        }

        signature_count += 1;
        printf("\n=== Verifying Signature #%llu ===\n", (unsigned long long) signature_count);

        //! [Verify signature]
        // Verify signature using DigitalSignatureExtensions
        RETURN_ERROR_IF_NOT_SUCCESS(DigitalSignatureExtensions_Verify(digital_signature, document, trust_store, settings, result.out()));

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
        BufferGuard signer_name;
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_GetSignerCommonName(result, signer_name.out()));

        // Get verification message
        BufferGuard message_buffer;
        RETURN_ERROR_IF_NOT_SUCCESS(SignatureVerificationResult_GetMessage(result, message_buffer.out()));
        //! [Verify signature]

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

        if (signer_name) {
            size_type name_size = 0;
            string_type name_data = nullptr;
            RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(signer_name, &name_data, &name_size));
            if (name_size > 0) {
                printf("Signer: %s\n", name_data);
            }
        }

        if (message_buffer) {
            size_type message_size = 0;
            string_type message_data = nullptr;
            RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(message_buffer, &message_data, &message_size));
            if (message_size > 0) {
                printf("Message: %s\n", message_data);
            }
        }

        // Track overall result (fail if any signature is invalid)
        if (status != SignatureStatus_Valid) {
            overall_result = VANILLAPDF_TOOLS_ERROR_FAILURE;
        }
    }

    // Print summary
    printf("\n============================\n");
    if (signature_count == 0) {
        printf("No signature fields found in PDF\n");
        overall_result = VANILLAPDF_TOOLS_ERROR_FAILURE;
    } else {
        printf("Verified %llu signature(s)\n", (unsigned long long) signature_count);
    }
    printf("============================\n\n");

    // Return overall result
    return overall_result;
}

} // namespace

void register_verify(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<VerifyOptions>();
    auto* command = app.add_subcommand("verify", "Verify PDF signatures");

    command->add_option("-f,--file", options->file, "Signed PDF file to verify")->required();
    command->add_option("-c,--certificates", options->certificates_path, "Trusted certificate store directory (uses system defaults if not specified)");
    command->add_option("-l,--license", options->license_file, "License file");
    command->add_flag("--skip-certificate-validation", options->skip_certificate_validation, "Skip X509 certificate chain validation (insecure, for testing only)");
    command->add_flag("--allow-weak-algorithms", options->allow_weak_algorithms, "Allow weak cryptographic algorithms (MD5, SHA-1, RSA < 2048 bits)");
    command->add_flag("--check-signing-time", options->check_signing_time, "Validate certificate at signing time instead of current time");
    /* TODO: CRL/OCSP revocation checking (https://github.com/vanillapdf/vanillapdf/issues/157)
    command->add_flag("--check-revocation", options->check_revocation, "Check certificate revocation (CRL/OCSP)");
    */

    command->callback([options, &exit_code]() {
        exit_code = process_verify(*options);
    });
}
