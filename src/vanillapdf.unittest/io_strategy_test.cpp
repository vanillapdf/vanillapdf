#include "unittest.h"
#include "handle_guard.h"
#include "temp_file.h"

namespace io_strategy {

TEST(IOStrategy, FileOpenWithUndefinedStrategyReturnsInvalidParameter) {
    HandleGuard<FileHandle, File_Release> file;

    auto result = File_OpenWithStrategy("nonexistent.pdf", IOStrategy_Undefined, file.out());
    ASSERT_EQ(result, VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(IOStrategy, FileCreateWithUndefinedStrategyReturnsInvalidParameter) {
    HandleGuard<FileHandle, File_Release> file;

    auto result = File_CreateWithStrategy("nonexistent.pdf", IOStrategy_Undefined, file.out());
    ASSERT_EQ(result, VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(IOStrategy, FileOpenWithMemoryMappedStrategyReturnsNotSupported) {
    HandleGuard<FileHandle, File_Release> file;

    // MemoryMapped strategy is not yet supported
    auto result = File_OpenWithStrategy("nonexistent.pdf", IOStrategy_MemoryMapped, file.out());
    ASSERT_EQ(result, VANILLAPDF_ERROR_NOT_SUPPORTED);
}

TEST(IOStrategy, FileCreateWithMemoryMappedStrategyReturnsNotSupported) {
    HandleGuard<FileHandle, File_Release> file;

    // MemoryMapped strategy is not yet supported
    auto result = File_CreateWithStrategy("nonexistent.pdf", IOStrategy_MemoryMapped, file.out());
    ASSERT_EQ(result, VANILLAPDF_ERROR_NOT_SUPPORTED);
}

TEST(IOStrategy, DocumentCreateWithMemoryStrategy) {
    TempFile tmp("vanillapdf_test_create_memory.pdf");

    HandleGuard<DocumentHandle, Document_Release> doc;

    auto result = Document_CreateWithStrategy(tmp.c_str(), IOStrategy_Memory, doc.out());
    ASSERT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(doc.get(), nullptr);
}

TEST(IOStrategy, DocumentCreateWithFileStreamStrategy) {
    TempFile tmp("vanillapdf_test_create_filestream.pdf");

    HandleGuard<DocumentHandle, Document_Release> doc;

    auto result = Document_CreateWithStrategy(tmp.c_str(), IOStrategy_FileStream, doc.out());
    ASSERT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(doc.get(), nullptr);
}

TEST(IOStrategy, FileCreateAndOpenWithFileStreamStrategy) {
    TempFile tmp("vanillapdf_test_roundtrip_filestream.pdf");

    {
        HandleGuard<DocumentHandle, Document_Release> doc;
        auto result = Document_CreateWithStrategy(tmp.c_str(), IOStrategy_FileStream, doc.out());
        ASSERT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(doc.get(), nullptr);

        // Save the document
        ASSERT_EQ(Document_Save(doc, tmp.c_str()), VANILLAPDF_ERROR_SUCCESS);
    }

    // Reopen with FileStream strategy
    {
        HandleGuard<DocumentHandle, Document_Release> doc;
        auto result = Document_OpenWithStrategy(tmp.c_str(), IOStrategy_FileStream, doc.out());
        ASSERT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(doc.get(), nullptr);

        // Verify we can read the catalog
        HandleGuard<CatalogHandle, Catalog_Release> catalog;
        ASSERT_EQ(Document_GetCatalog(doc, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(catalog.get(), nullptr);
    }
}

TEST(IOStrategy, FileCreateAndOpenWithMemoryStrategy) {
    TempFile tmp("vanillapdf_test_roundtrip_memory.pdf");

    {
        HandleGuard<DocumentHandle, Document_Release> doc;
        auto result = Document_CreateWithStrategy(tmp.c_str(), IOStrategy_FileStream, doc.out());
        ASSERT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(doc.get(), nullptr);

        // Save the document
        ASSERT_EQ(Document_Save(doc, tmp.c_str()), VANILLAPDF_ERROR_SUCCESS);
    }

    // Reopen with Memory strategy
    {
        HandleGuard<DocumentHandle, Document_Release> doc;
        auto result = Document_OpenWithStrategy(tmp.c_str(), IOStrategy_Memory, doc.out());
        ASSERT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(doc.get(), nullptr);

        // Verify we can read the catalog
        HandleGuard<CatalogHandle, Catalog_Release> catalog;
        ASSERT_EQ(Document_GetCatalog(doc, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(catalog.get(), nullptr);
    }
}

TEST(IOStrategy, NullParameterChecks) {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> doc;

    ASSERT_EQ(File_OpenWithStrategy(nullptr, IOStrategy_FileStream, file.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    ASSERT_EQ(File_OpenWithStrategy("test.pdf", IOStrategy_FileStream, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    ASSERT_EQ(File_CreateWithStrategy(nullptr, IOStrategy_FileStream, file.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    ASSERT_EQ(File_CreateWithStrategy("test.pdf", IOStrategy_FileStream, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    ASSERT_EQ(Document_OpenWithStrategy(nullptr, IOStrategy_FileStream, doc.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    ASSERT_EQ(Document_OpenWithStrategy("test.pdf", IOStrategy_FileStream, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    ASSERT_EQ(Document_CreateWithStrategy(nullptr, IOStrategy_FileStream, doc.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    ASSERT_EQ(Document_CreateWithStrategy("test.pdf", IOStrategy_FileStream, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // io_strategy
