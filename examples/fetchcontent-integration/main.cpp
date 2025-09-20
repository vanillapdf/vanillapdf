#include <iostream>
#include <vanillapdf/c_vanillapdf_api.h>
#include "scope_guard.h"

int main() {
    std::cout << "FetchContent Integration Test\n";
    std::cout << "============================\n";

    // Create a new PDF document
    DocumentHandle* document = nullptr;
    error_type result = Document_Create("test_output.pdf", &document);

    if (result != VANILLAPDF_ERROR_SUCCESS) {
        std::cerr << "❌ Failed to create PDF document (error: " << result << ")\n";
        return 1;
    }

    if (!document) {
        std::cerr << "❌ Document handle is null\n";
        return 1;
    }

    // Automatic cleanup for document
    SCOPE_GUARD([&]() { if (document) Document_Release(document); });

    // Get document catalog to access page tree
    CatalogHandle* catalog = nullptr;
    result = Document_GetCatalog(document, &catalog);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        std::cerr << "❌ Failed to get document catalog (error: " << result << ")\n";
        return 1;
    }

    // Automatic cleanup for catalog
    SCOPE_GUARD([&]() { if (catalog) Catalog_Release(catalog); });

    // Get page tree from catalog
    PageTreeHandle* page_tree = nullptr;
    result = Catalog_GetPages(catalog, &page_tree);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        std::cerr << "❌ Failed to get page tree (error: " << result << ")\n";
        return 1;
    }

    // Automatic cleanup for page tree
    SCOPE_GUARD([&]() { if (page_tree) PageTree_Release(page_tree); });

    // Create a new page
    PageObjectHandle* page = nullptr;
    result = PageObject_CreateFromDocument(document, &page);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        std::cerr << "❌ Failed to create page (error: " << result << ")\n";
        return 1;
    }

    // Automatic cleanup for page
    SCOPE_GUARD([&]() { if (page) PageObject_Release(page); });

    // Add page to document
    result = PageTree_AppendPage(page_tree, page);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        std::cerr << "❌ Failed to add page to document (error: " << result << ")\n";
        return 1;
    }

    // Save the document
    result = Document_Save(document, "test_output.pdf");
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        std::cerr << "❌ Failed to save PDF document (error: " << result << ")\n";
        return 1;
    }

    std::cout << "✅ Successfully created PDF document with page\n";
    std::cout << "✅ VanillaPDF is properly linked and functional\n";
    std::cout << "✅ Document creation and page management working\n";
    std::cout << "✅ FetchContent integration successful\n";

    return 0;
}