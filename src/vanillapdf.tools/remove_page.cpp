#include "tools.h"

namespace {

struct RemovePageOptions {
    std::string source_file;
    std::string destination_file;
    std::string password;
    integer_type page_number = 0;
};

// Removes the page at the given 1-based page number and saves the result
// into the destination file. Page numbers count leaf pages across the whole
// page tree, matching PageTree_GetPage, so nested intermediate nodes are
// handled transparently.
int process_remove_page(const RemovePageOptions& options) {
    FileGuard source_file;
    DocumentGuard document;
    CatalogGuard catalog;
    PageTreeGuard page_tree;

    size_type page_count_before = 0;
    size_type page_count_after = 0;

    unsigned long long page_count_before_converted = 0;
    unsigned long long page_count_after_converted = 0;

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.source_file.c_str(), source_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(source_file, document.out()));

    if (!options.password.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(source_file, options.password.c_str()));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, catalog.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, page_tree.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(page_tree, &page_count_before));

    page_count_before_converted = page_count_before;

    if ((size_type) options.page_number > page_count_before) {
        printf("Page number %d is out of range, the document has %llu pages\n",
            options.page_number, page_count_before_converted);
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_RemovePage(page_tree, (size_type) options.page_number));
    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(page_tree, &page_count_after));

    page_count_after_converted = page_count_after;

    if (page_count_after != page_count_before - 1) {
        printf("Page count mismatch after removal: %llu -> %llu\n",
            page_count_before_converted, page_count_after_converted);
        return VANILLAPDF_TOOLS_ERROR_FAILURE;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, options.destination_file.c_str()));

    printf("Removed page %d (%llu -> %llu pages): %s\n",
        options.page_number, page_count_before_converted, page_count_after_converted, options.destination_file.c_str());

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_remove_page(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<RemovePageOptions>();
    auto* command = app.add_subcommand("remove_page", "Remove a page from a PDF document");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();
    command->add_option("-n,--page-number", options->page_number, "1-based number of the page to remove")
        ->required()
        ->check(CLI::PositiveNumber);
    command->add_option("-p,--password", options->password, "Encryption password");

    command->callback([options, &exit_code]() {
        exit_code = process_remove_page(*options);
    });
}
