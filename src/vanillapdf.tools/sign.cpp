#include "tools.h"

namespace {

struct SignOptions {
    std::string source_file;
    std::string destination_file;
    std::string key_file;
    std::string key_password;
    std::string license_file;
};

int process_sign(const SignOptions& options) {
    DocumentGuard source_document;
    FileGuard destination_file;

    PKCS12KeyGuard pkcs12_key;
    SigningKeyGuard signing_key;
    DateGuard signing_time;
    DocumentSignatureSettingsGuard signature_settings;

    if (!options.license_file.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(options.license_file.c_str()));
    }

    // A key file without a password is opened with a null password
    string_type key_password = options.key_password.empty() ? nullptr : options.key_password.c_str();

    RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(options.key_file.c_str(), key_password, pkcs12_key.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(Date_CreateCurrent(signing_time.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Create(signature_settings.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time));

    RETURN_ERROR_IF_NOT_SUCCESS(File_Create(options.destination_file.c_str(), destination_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(options.source_file.c_str(), source_document.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Sign(source_document, destination_file, signature_settings));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_sign(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<SignOptions>();
    auto* command = app.add_subcommand("sign", "Sign a PDF document with a PKCS#12 key");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination (signed) file")->required();
    command->add_option("-k,--key", options->key_file, "PKCS#12 key file containing certificate and private key")->required();
    command->add_option("-p,--password", options->key_password, "Password for the key file");
    command->add_option("-l,--license", options->license_file, "License file");

    command->callback([options, &exit_code]() {
        exit_code = process_sign(*options);
    });
}
