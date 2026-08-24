#include "tools.h"

namespace {

struct ResaveOptions {
    std::string source_file;
    std::string destination_file;
    std::string password;
};

int process_resave(const ResaveOptions& options) {
    FileGuard source_file;
    DocumentGuard source_document;

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.source_file.c_str(), source_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, source_document.out()));

    if (!options.password.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(source_file, options.password.c_str()));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(source_document, options.destination_file.c_str()));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_resave(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<ResaveOptions>();
    auto* command = app.add_subcommand("resave", "Re-save a PDF document");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();
    command->add_option("-p,--password", options->password, "Encryption password");

    command->callback([options, &exit_code]() {
        exit_code = process_resave(*options);
    });
}
