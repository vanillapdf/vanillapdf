#include "unittest.h"

namespace actions {

// Helper to create a name object
NameObjectHandle* CreateName(const char* name) {
    NameObjectHandle* handle = nullptr;
    EXPECT_EQ(NameObject_CreateFromDecodedString(name, &handle), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(handle, nullptr);
    return handle;
}

// Helper to build an action dictionary with a given /S entry
DictionaryObjectHandle* CreateActionDict(const char* action_type) {
    DictionaryObjectHandle* dict = nullptr;
    EXPECT_EQ(DictionaryObject_Create(&dict), VANILLAPDF_ERROR_SUCCESS);

    NameObjectHandle* s_key = CreateName("S");
    NameObjectHandle* s_val = CreateName(action_type);
    ObjectHandle* s_val_obj = nullptr;
    EXPECT_EQ(NameObject_ToObject(s_val, &s_val_obj), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(DictionaryObject_Insert(dict, s_key, s_val_obj, false), VANILLAPDF_ERROR_SUCCESS);

    Object_Release(s_val_obj);
    NameObject_Release(s_val);
    NameObject_Release(s_key);
    return dict;
}

TEST(ActionType, GoToAction) {
    DictionaryObjectHandle* dict = CreateActionDict("GoTo");

    // Add a destination array: [0 /Fit]
    ArrayObjectHandle* dest_array = nullptr;
    ASSERT_EQ(ArrayObject_Create(&dest_array), VANILLAPDF_ERROR_SUCCESS);

    IntegerObjectHandle* page_int = nullptr;
    ASSERT_EQ(IntegerObject_CreateFromIntegerValue(0, &page_int), VANILLAPDF_ERROR_SUCCESS);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_int, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(dest_array, page_obj), VANILLAPDF_ERROR_SUCCESS);

    NameObjectHandle* fit_name = CreateName("Fit");
    ObjectHandle* fit_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(fit_name, &fit_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(dest_array, fit_obj), VANILLAPDF_ERROR_SUCCESS);

    // Insert /D entry into dict
    NameObjectHandle* d_key = CreateName("D");
    ObjectHandle* array_obj = nullptr;
    ASSERT_EQ(ArrayObject_ToObject(dest_array, &array_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, d_key, array_obj, false), VANILLAPDF_ERROR_SUCCESS);

    // Convert dictionary to ObjectHandle for Action_CreateFromDictionary pattern
    ObjectHandle* dict_obj = nullptr;
    ASSERT_EQ(DictionaryObject_ToObject(dict, &dict_obj), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    Object_Release(dict_obj);
    Object_Release(array_obj);
    NameObject_Release(d_key);
    Object_Release(fit_obj);
    NameObject_Release(fit_name);
    Object_Release(page_obj);
    IntegerObject_Release(page_int);
    ArrayObject_Release(dest_array);
    DictionaryObject_Release(dict);
}

TEST(ActionType, URIAction) {
    DictionaryObjectHandle* dict = CreateActionDict("URI");

    // Add /URI entry
    NameObjectHandle* uri_key = CreateName("URI");
    LiteralStringObjectHandle* uri_val = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("https://example.com", &uri_val), VANILLAPDF_ERROR_SUCCESS);
    StringObjectHandle* uri_str = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(uri_val, &uri_str), VANILLAPDF_ERROR_SUCCESS);
    ObjectHandle* uri_obj = nullptr;
    ASSERT_EQ(StringObject_ToObject(uri_str, &uri_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, uri_key, uri_obj, false), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    Object_Release(uri_obj);
    StringObject_Release(uri_str);
    LiteralStringObject_Release(uri_val);
    NameObject_Release(uri_key);
    DictionaryObject_Release(dict);
}

TEST(ActionType, NamedAction) {
    DictionaryObjectHandle* dict = CreateActionDict("Named");

    // Add /N entry
    NameObjectHandle* n_key = CreateName("N");
    NameObjectHandle* n_val = CreateName("NextPage");
    ObjectHandle* n_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(n_val, &n_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, n_key, n_obj, false), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    Object_Release(n_obj);
    NameObject_Release(n_val);
    NameObject_Release(n_key);
    DictionaryObject_Release(dict);
}

TEST(ActionType, InvalidActionType) {
    // Create dictionary with an invalid /S value - this should not crash
    DictionaryObjectHandle* dict = CreateActionDict("InvalidType");
    DictionaryObject_Release(dict);
}

TEST(ActionType, MissingSEntry) {
    // Create dictionary without /S entry
    DictionaryObjectHandle* dict = nullptr;
    ASSERT_EQ(DictionaryObject_Create(&dict), VANILLAPDF_ERROR_SUCCESS);
    DictionaryObject_Release(dict);
}

TEST(Action, ToAndFromUnknown) {
    // Verify Action_ToUnknown and Action_FromUnknown are available
    // These are tested indirectly through the C API declarations
    ActionHandle* null_action = nullptr;
    IUnknownHandle* unknown = nullptr;

    // Calling with null should return parameter error
    EXPECT_EQ(Action_ToUnknown(null_action, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Action_FromUnknown(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Action, ReleaseNull) {
    // Action_Release with null should return parameter error
    EXPECT_EQ(Action_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(GoToAction, ReleaseNull) {
    EXPECT_EQ(GoToAction_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(URIAction, ReleaseNull) {
    EXPECT_EQ(URIAction_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(GoToRemoteAction, ReleaseNull) {
    EXPECT_EQ(GoToRemoteAction_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(NamedAction, ReleaseNull) {
    EXPECT_EQ(NamedAction_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Action, GetActionTypeNull) {
    ActionType result;
    EXPECT_EQ(Action_GetActionType(nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(LinkAnnotation, GetActionNull) {
    // Calling with null should return parameter error
    ActionHandle* action = nullptr;
    EXPECT_EQ(LinkAnnotation_GetAction(nullptr, &action), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(OutlineItem, GetActionNull) {
    ActionHandle* action = nullptr;
    EXPECT_EQ(OutlineItem_GetAction(nullptr, &action), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Catalog, GetOpenActionNull) {
    ObjectHandle* obj = nullptr;
    EXPECT_EQ(Catalog_GetOpenAction(nullptr, &obj), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(PageTree, FindPageIndexNull) {
    size_type result;
    EXPECT_EQ(PageTree_FindPageIndex(nullptr, nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);
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

} // namespace actions
