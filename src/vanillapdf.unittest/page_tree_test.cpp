#include "unittest.h"
#include "handle_guard.h"

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
    ASSERT_EQ(Rectangle_SetLowerLeftX(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(media_box, 612), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(media_box, 792), VANILLAPDF_ERROR_SUCCESS);
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


// Root -> [node with pages 1-3, node with pages 4-5]. Flat page numbers count leaf
// pages across the whole tree, while Remove and Insert used to apply them to the
// root /Kids array directly, which only coincides on a completely flat tree.
static const unsigned char NESTED_PAGES[] = {
    0x25, 0x50, 0x44, 0x46, 0x2D, 0x31, 0x2E, 0x37, 0x0A, 0x31, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A,
    0x0A, 0x3C, 0x3C, 0x2F, 0x50, 0x61, 0x67, 0x65, 0x73, 0x20, 0x32, 0x20, 0x30, 0x20, 0x52, 0x20,
    0x2F, 0x54, 0x79, 0x70, 0x65, 0x20, 0x2F, 0x43, 0x61, 0x74, 0x61, 0x6C, 0x6F, 0x67, 0x3E, 0x3E,
    0x0A, 0x65, 0x6E, 0x64, 0x6F, 0x62, 0x6A, 0x0A, 0x32, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A, 0x0A,
    0x3C, 0x3C, 0x2F, 0x43, 0x6F, 0x75, 0x6E, 0x74, 0x20, 0x35, 0x20, 0x2F, 0x4B, 0x69, 0x64, 0x73,
    0x20, 0x5B, 0x33, 0x20, 0x30, 0x20, 0x52, 0x20, 0x34, 0x20, 0x30, 0x20, 0x52, 0x5D, 0x20, 0x2F,
    0x54, 0x79, 0x70, 0x65, 0x20, 0x2F, 0x50, 0x61, 0x67, 0x65, 0x73, 0x3E, 0x3E, 0x0A, 0x65, 0x6E,
    0x64, 0x6F, 0x62, 0x6A, 0x0A, 0x33, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A, 0x0A, 0x3C, 0x3C, 0x2F,
    0x43, 0x6F, 0x75, 0x6E, 0x74, 0x20, 0x33, 0x20, 0x2F, 0x4B, 0x69, 0x64, 0x73, 0x20, 0x5B, 0x35,
    0x20, 0x30, 0x20, 0x52, 0x20, 0x36, 0x20, 0x30, 0x20, 0x52, 0x20, 0x37, 0x20, 0x30, 0x20, 0x52,
    0x5D, 0x20, 0x2F, 0x50, 0x61, 0x72, 0x65, 0x6E, 0x74, 0x20, 0x32, 0x20, 0x30, 0x20, 0x52, 0x20,
    0x2F, 0x54, 0x79, 0x70, 0x65, 0x20, 0x2F, 0x50, 0x61, 0x67, 0x65, 0x73, 0x3E, 0x3E, 0x0A, 0x65,
    0x6E, 0x64, 0x6F, 0x62, 0x6A, 0x0A, 0x34, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A, 0x0A, 0x3C, 0x3C,
    0x2F, 0x43, 0x6F, 0x75, 0x6E, 0x74, 0x20, 0x32, 0x20, 0x2F, 0x4B, 0x69, 0x64, 0x73, 0x20, 0x5B,
    0x38, 0x20, 0x30, 0x20, 0x52, 0x20, 0x39, 0x20, 0x30, 0x20, 0x52, 0x5D, 0x20, 0x2F, 0x50, 0x61,
    0x72, 0x65, 0x6E, 0x74, 0x20, 0x32, 0x20, 0x30, 0x20, 0x52, 0x20, 0x2F, 0x54, 0x79, 0x70, 0x65,
    0x20, 0x2F, 0x50, 0x61, 0x67, 0x65, 0x73, 0x3E, 0x3E, 0x0A, 0x65, 0x6E, 0x64, 0x6F, 0x62, 0x6A,
    0x0A, 0x35, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A, 0x0A, 0x3C, 0x3C, 0x2F, 0x4D, 0x65, 0x64, 0x69,
    0x61, 0x42, 0x6F, 0x78, 0x20, 0x5B, 0x30, 0x20, 0x30, 0x20, 0x36, 0x31, 0x32, 0x20, 0x37, 0x39,
    0x32, 0x5D, 0x20, 0x2F, 0x50, 0x61, 0x72, 0x65, 0x6E, 0x74, 0x20, 0x33, 0x20, 0x30, 0x20, 0x52,
    0x20, 0x2F, 0x54, 0x79, 0x70, 0x65, 0x20, 0x2F, 0x50, 0x61, 0x67, 0x65, 0x3E, 0x3E, 0x0A, 0x65,
    0x6E, 0x64, 0x6F, 0x62, 0x6A, 0x0A, 0x36, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A, 0x0A, 0x3C, 0x3C,
    0x2F, 0x4D, 0x65, 0x64, 0x69, 0x61, 0x42, 0x6F, 0x78, 0x20, 0x5B, 0x30, 0x20, 0x30, 0x20, 0x36,
    0x31, 0x32, 0x20, 0x37, 0x39, 0x32, 0x5D, 0x20, 0x2F, 0x50, 0x61, 0x72, 0x65, 0x6E, 0x74, 0x20,
    0x33, 0x20, 0x30, 0x20, 0x52, 0x20, 0x2F, 0x54, 0x79, 0x70, 0x65, 0x20, 0x2F, 0x50, 0x61, 0x67,
    0x65, 0x3E, 0x3E, 0x0A, 0x65, 0x6E, 0x64, 0x6F, 0x62, 0x6A, 0x0A, 0x37, 0x20, 0x30, 0x20, 0x6F,
    0x62, 0x6A, 0x0A, 0x3C, 0x3C, 0x2F, 0x4D, 0x65, 0x64, 0x69, 0x61, 0x42, 0x6F, 0x78, 0x20, 0x5B,
    0x30, 0x20, 0x30, 0x20, 0x36, 0x31, 0x32, 0x20, 0x37, 0x39, 0x32, 0x5D, 0x20, 0x2F, 0x50, 0x61,
    0x72, 0x65, 0x6E, 0x74, 0x20, 0x33, 0x20, 0x30, 0x20, 0x52, 0x20, 0x2F, 0x54, 0x79, 0x70, 0x65,
    0x20, 0x2F, 0x50, 0x61, 0x67, 0x65, 0x3E, 0x3E, 0x0A, 0x65, 0x6E, 0x64, 0x6F, 0x62, 0x6A, 0x0A,
    0x38, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A, 0x0A, 0x3C, 0x3C, 0x2F, 0x4D, 0x65, 0x64, 0x69, 0x61,
    0x42, 0x6F, 0x78, 0x20, 0x5B, 0x30, 0x20, 0x30, 0x20, 0x36, 0x31, 0x32, 0x20, 0x37, 0x39, 0x32,
    0x5D, 0x20, 0x2F, 0x50, 0x61, 0x72, 0x65, 0x6E, 0x74, 0x20, 0x34, 0x20, 0x30, 0x20, 0x52, 0x20,
    0x2F, 0x54, 0x79, 0x70, 0x65, 0x20, 0x2F, 0x50, 0x61, 0x67, 0x65, 0x3E, 0x3E, 0x0A, 0x65, 0x6E,
    0x64, 0x6F, 0x62, 0x6A, 0x0A, 0x39, 0x20, 0x30, 0x20, 0x6F, 0x62, 0x6A, 0x0A, 0x3C, 0x3C, 0x2F,
    0x4D, 0x65, 0x64, 0x69, 0x61, 0x42, 0x6F, 0x78, 0x20, 0x5B, 0x30, 0x20, 0x30, 0x20, 0x36, 0x31,
    0x32, 0x20, 0x37, 0x39, 0x32, 0x5D, 0x20, 0x2F, 0x50, 0x61, 0x72, 0x65, 0x6E, 0x74, 0x20, 0x34,
    0x20, 0x30, 0x20, 0x52, 0x20, 0x2F, 0x54, 0x79, 0x70, 0x65, 0x20, 0x2F, 0x50, 0x61, 0x67, 0x65,
    0x3E, 0x3E, 0x0A, 0x65, 0x6E, 0x64, 0x6F, 0x62, 0x6A, 0x0A, 0x0A, 0x78, 0x72, 0x65, 0x66, 0x0A,
    0x30, 0x20, 0x31, 0x30, 0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20,
    0x36, 0x35, 0x35, 0x33, 0x35, 0x20, 0x66, 0x20, 0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x39, 0x20, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x6E, 0x20, 0x0A, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x35, 0x36, 0x20, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x6E, 0x20,
    0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x37, 0x20, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x20, 0x6E, 0x20, 0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x39, 0x38, 0x20,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x6E, 0x20, 0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x32, 0x37, 0x33, 0x20, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x6E, 0x20, 0x0A, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x33, 0x34, 0x32, 0x20, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x6E, 0x20,
    0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x31, 0x31, 0x20, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x20, 0x6E, 0x20, 0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 0x38, 0x30, 0x20,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x6E, 0x20, 0x0A, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x35, 0x34, 0x39, 0x20, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x6E, 0x20, 0x0A, 0x74, 0x72, 0x61,
    0x69, 0x6C, 0x65, 0x72, 0x0A, 0x3C, 0x3C, 0x2F, 0x52, 0x6F, 0x6F, 0x74, 0x20, 0x31, 0x20, 0x30,
    0x20, 0x52, 0x20, 0x2F, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x31, 0x30, 0x3E, 0x3E, 0x0A, 0x73, 0x74,
    0x61, 0x72, 0x74, 0x78, 0x72, 0x65, 0x66, 0x0A, 0x36, 0x31, 0x39, 0x0A, 0x25, 0x25, 0x45, 0x4F,
    0x46,
};

class NestedPageTree {
public:
    void Open() {
        ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(NESTED_PAGES), sizeof(NESTED_PAGES), buffer.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_WriteBuffer(io_stream, buffer), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_Flush(io_stream), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_OpenStream(io_stream, "nested-pages", file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_Initialize(file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_OpenFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Catalog_GetPages(catalog, page_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    }

    size_type PageCount() {
        size_type page_count = 0;
        EXPECT_EQ(PageTree_GetPageCount(page_tree, &page_count), VANILLAPDF_ERROR_SUCCESS);
        return page_count;
    }

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<BufferHandle, Buffer_Release> buffer;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> page_tree;
};

// Holds a page and the object handle FindPageIndex looks it up by
class TrackedPage {
public:
    void Take(PageTreeHandle* page_tree, size_type at) {
        ASSERT_EQ(PageTree_GetPage(page_tree, at, page.out()), VANILLAPDF_ERROR_SUCCESS);
        Track();
    }

    void Track() {
        ASSERT_EQ(PageObject_GetBaseObject(page, dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DictionaryObject_ToObject(dictionary, object.out()), VANILLAPDF_ERROR_SUCCESS);
    }

    size_type IndexIn(PageTreeHandle* page_tree) {
        size_type index = 0;
        EXPECT_EQ(PageTree_FindPageIndex(page_tree, object, &index), VANILLAPDF_ERROR_SUCCESS);
        return index;
    }

    HandleGuard<PageObjectHandle, PageObject_Release> page;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary;
    HandleGuard<ObjectHandle, Object_Release> object;
};

TEST(PageTree, RemoveFirstPageOfNestedTree) {
    NestedPageTree tree;
    ASSERT_NO_FATAL_FAILURE(tree.Open());
    ASSERT_EQ(tree.PageCount(), 5u);

    TrackedPage second_page;
    TrackedPage last_page;
    ASSERT_NO_FATAL_FAILURE(second_page.Take(tree.page_tree, 2));
    ASSERT_NO_FATAL_FAILURE(last_page.Take(tree.page_tree, 5));

    // Used to remove the whole first subtree of three pages
    ASSERT_EQ(PageTree_RemovePage(tree.page_tree, 1), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(tree.PageCount(), 4u);
    EXPECT_EQ(second_page.IndexIn(tree.page_tree), 1u);
    EXPECT_EQ(last_page.IndexIn(tree.page_tree), 4u);
}

TEST(PageTree, RemovePageFromSecondSubtree) {
    NestedPageTree tree;
    ASSERT_NO_FATAL_FAILURE(tree.Open());

    TrackedPage third_page;
    TrackedPage last_page;
    ASSERT_NO_FATAL_FAILURE(third_page.Take(tree.page_tree, 3));
    ASSERT_NO_FATAL_FAILURE(last_page.Take(tree.page_tree, 5));

    // Page 4 is the first page of the second subtree. The root /Kids array only
    // has two entries, so removing index 4 from it used to remove nothing at all.
    ASSERT_EQ(PageTree_RemovePage(tree.page_tree, 4), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(tree.PageCount(), 4u);
    EXPECT_EQ(third_page.IndexIn(tree.page_tree), 3u);
    EXPECT_EQ(last_page.IndexIn(tree.page_tree), 4u);
}

TEST(PageTree, RemovePageOutOfRangeIsRejected) {
    NestedPageTree tree;
    ASSERT_NO_FATAL_FAILURE(tree.Open());

    EXPECT_NE(PageTree_RemovePage(tree.page_tree, 6), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(tree.PageCount(), 5u);
}

TEST(PageTree, InsertPageIntoSecondSubtree) {
    NestedPageTree tree;
    ASSERT_NO_FATAL_FAILURE(tree.Open());

    TrackedPage last_page;
    ASSERT_NO_FATAL_FAILURE(last_page.Take(tree.page_tree, 5));

    TrackedPage new_page;
    ASSERT_EQ(PageObject_CreateFromDocument(tree.document, new_page.page.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NO_FATAL_FAILURE(new_page.Track());

    ASSERT_EQ(PageTree_InsertPage(tree.page_tree, 4, new_page.page), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(tree.PageCount(), 6u);
    EXPECT_EQ(new_page.IndexIn(tree.page_tree), 4u);
    EXPECT_EQ(last_page.IndexIn(tree.page_tree), 6u);
}

} // namespace page_tree
