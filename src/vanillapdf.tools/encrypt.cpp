#include "tools.h"

namespace {

struct EncryptOptions {
    std::string source_file;
    std::string destination_file;
    std::string owner_password;
    std::string user_password;
    std::string algorithm = "RC4";
    integer_type key_length = 40;
    std::string license_file;
};

int process_encrypt(const EncryptOptions& options) {
    FileGuard source_file;
    DocumentGuard source_document;

    EncryptionAlgorithmType encryption_algorithm = EncryptionAlgorithmType_RC4;

    DocumentEncryptionSettingsGuard encryption_settings;
    BufferGuard owner_password_buffer;
    BufferGuard user_password_buffer;

    // Licensing is opt-in (VANILLAPDF_ENABLE_LICENSING, off by default), so the
    // license file is optional; only apply it when one was supplied.
    if (!options.license_file.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(options.license_file.c_str()));
    }

    if (options.algorithm == "None") {
        encryption_algorithm = EncryptionAlgorithmType_None;
    }

    if (options.algorithm == "RC4") {
        encryption_algorithm = EncryptionAlgorithmType_RC4;
    }

    if (options.algorithm == "AES") {
        encryption_algorithm = EncryptionAlgorithmType_AES;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.source_file.c_str(), source_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, source_document.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_Create(encryption_settings.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetAlgorithm(encryption_settings, encryption_algorithm));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetKeyLength(encryption_settings, options.key_length));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, UserAccessPermissionFlag_None));

    RETURN_ERROR_IF_NOT_SUCCESS(Buffer_CreateFromData(options.owner_password.c_str(), options.owner_password.size(), owner_password_buffer.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Buffer_CreateFromData(options.user_password.c_str(), options.user_password.size(), user_password_buffer.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetOwnerPassword(encryption_settings, owner_password_buffer));
    RETURN_ERROR_IF_NOT_SUCCESS(DocumentEncryptionSettings_SetUserPassword(encryption_settings, user_password_buffer));

    // Add encryption to the document dictionary - the actual encryption is done by each object transparently
    RETURN_ERROR_IF_NOT_SUCCESS(Document_AddEncryption(source_document, encryption_settings));

    // Save the encrypted file into the destination path
    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(source_document, options.destination_file.c_str()));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_encrypt(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<EncryptOptions>();
    auto* command = app.add_subcommand("encrypt", "Encrypt a PDF document");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();
    command->add_option("--owner-password", options->owner_password, "Owner password")->required();
    command->add_option("--user-password", options->user_password, "User password")->required();
    command->add_option("-a,--algorithm", options->algorithm, "Encryption algorithm")
        ->check(CLI::IsMember({"None", "RC4", "AES"}))
        ->capture_default_str();
    command->add_option("-k,--key-length", options->key_length, "Key length in bits")
        ->capture_default_str();
    command->add_option("-l,--license", options->license_file, "License file");

    command->callback([options, &exit_code]() {
        exit_code = process_encrypt(*options);
    });
}
