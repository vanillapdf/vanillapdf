#include "tools.h"

namespace {

struct MergeOptions {
    std::string source_file;
    std::string destination_file;
    std::vector<std::string> merge_files;
};

int process_merge(const MergeOptions& options) {
    DocumentGuard document;

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(options.source_file.c_str(), document.out()));

    for (const auto& merge_file : options.merge_files) {
        DocumentGuard other_document;

        RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(merge_file.c_str(), other_document.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(Document_AppendDocument(document, other_document));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, options.destination_file.c_str()));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_merge(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<MergeOptions>();
    auto* command = app.add_subcommand("merge", "Merge multiple PDF files");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();
    command->add_option("-f,--files", options->merge_files, "Files appended to the source, in order")->required();

    command->callback([options, &exit_code]() {
        exit_code = process_merge(*options);
    });
}
