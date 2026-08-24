#include "unittest.h"

namespace page_tree {

TEST(PageTree, FindPageIndexNull) {
    size_type result;
    EXPECT_EQ(PageTree_FindPageIndex(nullptr, nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(PageTree, GetPageCountAndAccess) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    size_type page_count = 0;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);

    // Get initial page count (Document_CreateFile creates 1 page)
    ASSERT_EQ(PageTree_GetPageCount(page_tree, &page_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_GE(page_count, 1u);

    // Access first page
    PageObjectHandle* page = nullptr;
    ASSERT_EQ(PageTree_GetPage(page_tree, 1, &page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page, nullptr);

    ASSERT_EQ(PageObject_Release(page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(PageTree, InsertAndRemovePage) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    size_type page_count = 0;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);

    // Get initial page count
    ASSERT_EQ(PageTree_GetPageCount(page_tree, &page_count), VANILLAPDF_ERROR_SUCCESS);
    size_type initial_count = page_count;

    // Create a new page
    PageObjectHandle* new_page = nullptr;
    ASSERT_EQ(PageObject_CreateFromDocument(doc, &new_page), VANILLAPDF_ERROR_SUCCESS);

    // Set media box
    RectangleHandle* media_box = nullptr;
    ASSERT_EQ(Rectangle_Create(&media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftXReal(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftYReal(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightXReal(media_box, 612), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightYReal(media_box, 792), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetMediaBox(new_page, media_box), VANILLAPDF_ERROR_SUCCESS);

    // Insert page at position 1
    ASSERT_EQ(PageTree_InsertPage(page_tree, 1, new_page), VANILLAPDF_ERROR_SUCCESS);

    // Verify count increased
    ASSERT_EQ(PageTree_GetPageCount(page_tree, &page_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(page_count, initial_count + 1);

    // Remove the inserted page
    ASSERT_EQ(PageTree_RemovePage(page_tree, 1), VANILLAPDF_ERROR_SUCCESS);

    // Verify count restored
    ASSERT_EQ(PageTree_GetPageCount(page_tree, &page_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(page_count, initial_count);

    // Cleanup
    ASSERT_EQ(Rectangle_Release(media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(new_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(PageTree, InsertPageZeroIndexIsRejected) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    size_type page_count = 0;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);

    PageObjectHandle* new_page = nullptr;
    ASSERT_EQ(PageObject_CreateFromDocument(doc, &new_page), VANILLAPDF_ERROR_SUCCESS);

    // Page indices are 1-based; index 0 must be rejected, not silently
    // reinterpreted as "insert at the front"
    EXPECT_EQ(PageTree_InsertPage(page_tree, 0, new_page), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageTree_RemovePage(page_tree, 0), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // 1-based index 1 still works, including on the front of the tree
    ASSERT_EQ(PageTree_GetPageCount(page_tree, &page_count), VANILLAPDF_ERROR_SUCCESS);
    size_type initial_count = page_count;
    ASSERT_EQ(PageTree_InsertPage(page_tree, 1, new_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_GetPageCount(page_tree, &page_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(page_count, initial_count + 1);

    // Cleanup
    ASSERT_EQ(PageObject_Release(new_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(PageTree, ToAndFromUnknown) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    IUnknownHandle* unknown = nullptr;
    PageTreeHandle* converted = nullptr;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);

    // Convert to unknown
    ASSERT_EQ(PageTree_ToUnknown(page_tree, &unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown, nullptr);

    // Convert back
    ASSERT_EQ(PageTree_FromUnknown(unknown, &converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted, nullptr);

    // Cleanup
    ASSERT_EQ(PageTree_Release(converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IUnknown_Release(unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(PageTree, FindPageIndex) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    PageObjectHandle* page = nullptr;
    DictionaryObjectHandle* page_dict = nullptr;
    ObjectHandle* page_obj = nullptr;
    size_type found_index = 0;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);

    // Get first page
    ASSERT_EQ(PageTree_GetPage(page_tree, 1, &page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page, nullptr);

    // Get base dictionary object
    ASSERT_EQ(PageObject_GetBaseObject(page, &page_dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page_dict, nullptr);

    // Convert to ObjectHandle
    ASSERT_EQ(DictionaryObject_ToObject(page_dict, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page_obj, nullptr);

    // Find page index
    ASSERT_EQ(PageTree_FindPageIndex(page_tree, page_obj, &found_index), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(found_index, 1u);

    // Cleanup
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(page_dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

} // namespace page_tree
