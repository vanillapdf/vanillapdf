#include "tools.h"

namespace {

struct ReadOptions {
    std::string file;
};

int process_read(const ReadOptions& options) {
    size_type page_count = 0;

    FileGuard file;
    DocumentGuard document;
    CatalogGuard catalog;
    PageTreeGuard pages;

    RETURN_ERROR_IF_NOT_SUCCESS(File_OpenWithStrategy(options.file.c_str(), IOStrategy_Memory, file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));

    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(file, document.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, catalog.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, pages.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(pages, &page_count));

    printf("Successfully read %llu pages from %s using memory IO strategy\n",
        (unsigned long long) page_count, options.file.c_str());

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_read(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<ReadOptions>();
    auto* command = app.add_subcommand("read", "Read a PDF using memory IO strategy");

    command->add_option("-f,--file", options->file, "PDF file to read using memory IO strategy")->required();

    command->callback([options, &exit_code]() {
        exit_code = process_read(*options);
    });
}
