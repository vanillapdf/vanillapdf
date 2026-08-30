#include "tools.h"

namespace {

struct SignCustomOptions {
    std::string source_file;
    std::string destination_file;
    std::string license_file;
};

// Custom callbacks

// The buffer is handed over to the library in sign_final and released again
// in sign_cleanup, so it is kept as a raw handle rather than a guard
struct SignatureData {
    BufferHandle* data = nullptr;
};

error_type CALLING_CONVENTION sign_init(void* user_data, MessageDigestAlgorithmType algorithm) {
    SignatureData* signature_data = (SignatureData*) user_data;

    const char SAMPLE_DATA[] = "Hello world!";

    UNUSED(algorithm);

    RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Create(&signature_data->data));
    RETURN_ERROR_IF_NOT_SUCCESS(Buffer_SetData(signature_data->data, SAMPLE_DATA, sizeof(SAMPLE_DATA)));

    return VANILLAPDF_ERROR_SUCCESS;
}

error_type CALLING_CONVENTION sign_update(void* user_data, const BufferHandle* data) {
    SignatureData* signature_data = (SignatureData*) user_data;
    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(data, &buffer_data, &buffer_size));

    // Calculate signature

    UNUSED(signature_data);

    return VANILLAPDF_ERROR_SUCCESS;
}

error_type CALLING_CONVENTION sign_final(void* user_data, BufferHandle** result) {
    SignatureData* signature_data = (SignatureData*) user_data;

    *result = signature_data->data;

    return VANILLAPDF_ERROR_SUCCESS;
}

error_type CALLING_CONVENTION sign_cleanup(void* user_data) {
    SignatureData* signature_data = (SignatureData*) user_data;

    RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(signature_data->data));

    return VANILLAPDF_ERROR_SUCCESS;
}

int process_sign_custom(const SignCustomOptions& options) {
    DocumentGuard source_document;
    FileGuard destination_file;

    DocumentSignatureSettingsGuard signature_settings;
    SigningKeyGuard signing_key;
    SignatureData user_data;

    if (!options.license_file.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(options.license_file.c_str()));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(SigningKey_CreateCustom(&sign_init, &sign_update, &sign_final, &sign_cleanup, &user_data, signing_key.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Create(signature_settings.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256));

    RETURN_ERROR_IF_NOT_SUCCESS(File_Create(options.destination_file.c_str(), destination_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(options.source_file.c_str(), source_document.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Sign(source_document, destination_file, signature_settings));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_sign_custom(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<SignCustomOptions>();
    auto* command = app.add_subcommand("sign_custom", "Sign a PDF document with custom signing callbacks");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination (signed) file")->required();
    command->add_option("-l,--license", options->license_file, "License file");

    command->callback([options, &exit_code]() {
        exit_code = process_sign_custom(*options);
    });
}
