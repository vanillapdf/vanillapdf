#include "tools.h"

namespace {

struct DecryptOptions {
    std::string source_file;
    std::string destination_file;
    std::string password;
    std::string key_file;
    std::string license_file;
};

int process_decrypt(const DecryptOptions& options) {
    FileGuard source_file;
    DocumentGuard source_document;

    // Licensing is opt-in (VANILLAPDF_ENABLE_LICENSING, off by default), so the
    // license file is optional; only apply it when one was supplied.
    if (!options.license_file.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(options.license_file.c_str()));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.source_file.c_str(), source_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, source_document.out()));

    if (!options.password.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(source_file, options.password.c_str()));
    }

    if (!options.key_file.empty()) {
        PKCS12KeyGuard pkcs12_key;
        EncryptionKeyGuard encryption_key;

        RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(options.key_file.c_str(), nullptr, pkcs12_key.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(pkcs12_key, encryption_key.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(source_file, encryption_key));
    }

    // Remove file encryption
    RETURN_ERROR_IF_NOT_SUCCESS(Document_RemoveEncryption(source_document));

    // Save the decrypted file into the destination path
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(source_document, options.destination_file.c_str()));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_decrypt(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<DecryptOptions>();
    auto* command = app.add_subcommand("decrypt", "Decrypt a PDF document");

    command->add_option("-s,--source", options->source_file, "Encrypted source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();
    command->add_option("-p,--password", options->password, "User or owner password");
    command->add_option("-k,--key", options->key_file, "PKCS#12 file for certificate-based decryption");
    command->add_option("-l,--license", options->license_file, "License file");

    command->callback([options, &exit_code]() {
        exit_code = process_decrypt(*options);
    });
}
