#include "unittest.h"

namespace catalog {

TEST(Catalog, GetOpenActionNull) {
    ObjectHandle* obj = nullptr;
    EXPECT_EQ(Catalog_GetOpenAction(nullptr, &obj), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Catalog, GetOpenActionFreshDocument) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);

    // Fresh document should not have OpenAction
    ObjectHandle* open_action = nullptr;
    EXPECT_EQ(Catalog_GetOpenAction(catalog, &open_action), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Cleanup
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Catalog, ToAndFromUnknown) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    IUnknownHandle* unknown = nullptr;
    CatalogHandle* converted = nullptr;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);

    // Convert to unknown
    ASSERT_EQ(Catalog_ToUnknown(catalog, &unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown, nullptr);

    // Convert back
    ASSERT_EQ(Catalog_FromUnknown(unknown, &converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted, nullptr);

    // Cleanup
    ASSERT_EQ(Catalog_Release(converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IUnknown_Release(unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Catalog, GetOptionalEntriesFreshDocument) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);

    // Optional entries should return OBJECT_MISSING on fresh document
    PageLabelsHandle* labels = nullptr;
    EXPECT_EQ(Catalog_GetPageLabels(catalog, &labels), VANILLAPDF_ERROR_OBJECT_MISSING);

    PageLayout layout;
    EXPECT_EQ(Catalog_GetPageLayout(catalog, &layout), VANILLAPDF_ERROR_OBJECT_MISSING);

    PDFVersion version;
    EXPECT_EQ(Catalog_GetVersion(catalog, &version), VANILLAPDF_ERROR_OBJECT_MISSING);

    DeveloperExtensionsHandle* extensions = nullptr;
    EXPECT_EQ(Catalog_GetExtensions(catalog, &extensions), VANILLAPDF_ERROR_OBJECT_MISSING);

    ViewerPreferencesHandle* prefs = nullptr;
    EXPECT_EQ(Catalog_GetViewerPreferences(catalog, &prefs), VANILLAPDF_ERROR_OBJECT_MISSING);

    OutlineHandle* outlines = nullptr;
    EXPECT_EQ(Catalog_GetOutlines(catalog, &outlines), VANILLAPDF_ERROR_OBJECT_MISSING);

    NamedDestinationsHandle* dests = nullptr;
    EXPECT_EQ(Catalog_GetDestinations(catalog, &dests), VANILLAPDF_ERROR_OBJECT_MISSING);

    NameDictionaryHandle* names = nullptr;
    EXPECT_EQ(Catalog_GetNames(catalog, &names), VANILLAPDF_ERROR_OBJECT_MISSING);

    InteractiveFormHandle* form = nullptr;
    EXPECT_EQ(Catalog_GetAcroForm(catalog, &form), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Cleanup
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

} // namespace catalog
