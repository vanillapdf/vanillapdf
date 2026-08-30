#include "tools.h"

namespace {

struct GenerateOptions {
    std::string destination_file;
    integer_type page_count = 0;
};

// Creates a new PDF document with the requested number of empty pages, appended
// directly to the root /Pages node (a flat page tree). Useful for generating
// large synthetic documents (e.g. page-tree benchmark fixtures) without
// shipping big real-world files.
int process_generate(const GenerateOptions& options) {
    DocumentGuard document;
    CatalogGuard catalog;
    PageTreeGuard page_tree;
    NameObjectGuard page_index_key;

    integer_type i = 0;

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Create(options.destination_file.c_str(), document.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, catalog.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, page_tree.out()));

    // Workaround for https://github.com/vanillapdf/vanillapdf/issues/458: the
    // file writer deduplicates content-identical objects, so byte-identical
    // empty pages would collapse into a single shared page. Tag each page with
    // a unique index under a marker key to keep the pages distinct. Once #458
    // is fixed this per-page key can be dropped.
    RETURN_ERROR_IF_NOT_SUCCESS(NameObject_CreateFromDecodedString("BenchPageIndex", page_index_key.out()));

    for (i = 0; i < options.page_count; ++i) {
        PageObjectGuard page;
        DictionaryObjectGuard page_dictionary;
        IntegerObjectGuard index_object;
        ObjectGuard index_value;

        RETURN_ERROR_IF_NOT_SUCCESS(PageObject_CreateFromDocument(document, page.out()));

        RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetBaseObject(page, page_dictionary.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_CreateFromIntegerValue(i, index_object.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_ToObject(index_object, index_value.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Insert(page_dictionary, page_index_key, index_value, VANILLAPDF_RV_TRUE));

        RETURN_ERROR_IF_NOT_SUCCESS(PageTree_AppendPage(page_tree, page));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, options.destination_file.c_str()));

    printf("Generated %d-page document: %s\n", options.page_count, options.destination_file.c_str());

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_generate(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<GenerateOptions>();
    auto* command = app.add_subcommand("generate", "Generate a PDF with N empty pages");

    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();
    command->add_option("-p,--pages", options->page_count, "Number of empty pages")
        ->required()
        ->check(CLI::PositiveNumber);

    command->callback([options, &exit_code]() {
        exit_code = process_generate(*options);
    });
}
