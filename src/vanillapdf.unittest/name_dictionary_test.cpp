#include <gtest/gtest.h>

#include <vanillapdf/c_vanillapdf_api.h>

#include "test_data.h"

// Test fixture for tests that require a document context with indirect objects
// Creates a document programmatically with an indirect page - no disk files needed
class DestinationNameTreeWithDocument : public ::testing::Test {
protected:
    InputOutputStreamHandle* stream = nullptr;
    FileHandle* file = nullptr;
    DocumentHandle* document = nullptr;
    PageObjectHandle* page = nullptr;

    void SetUp() override {
        // Create in-memory stream for the document
        ASSERT_EQ(InputOutputStream_CreateFromMemory(&stream), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(stream, nullptr);

        // Create file on the stream
        ASSERT_EQ(File_CreateStream(stream, "test_doc.pdf", &file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(file, nullptr);

        // Create new document (this is an empty document)
        ASSERT_EQ(Document_CreateFile(file, &document), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(document, nullptr);

        // Create a page from the document - this creates an INDIRECT page object
        ASSERT_EQ(PageObject_CreateFromDocument(document, &page), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page, nullptr);

        // Add the page to the document's page tree
        CatalogHandle* catalog = nullptr;
        ASSERT_EQ(Document_GetCatalog(document, &catalog), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(catalog, nullptr);

        PageTreeHandle* page_tree = nullptr;
        ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page_tree, nullptr);

        // Note: PageTree_InsertPage(tree, 0, page) is broken - see issue #226
        // Use AppendPage as workaround
        ASSERT_EQ(PageTree_AppendPage(page_tree, page), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    }

    void TearDown() override {
        if (page) PageObject_Release(page);
        if (document) Document_Release(document);
        if (file) File_Release(file);
        if (stream) InputOutputStream_Release(stream);
    }

    // Helper to create a Fit destination with an indirect array
    // The array must be made indirect for NameTree::Insert to work
    void CreateFitDestinationWithPage(DestinationHandle** result, ArrayObjectHandle** out_array) {
        ArrayObjectHandle* arr = nullptr;
        ASSERT_EQ(ArrayObject_Create(&arr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(arr, nullptr);

        // Add page number (0) as IntegerObject
        IntegerObjectHandle* page_num = nullptr;
        ASSERT_EQ(IntegerObject_CreateFromIntegerValue(0, &page_num), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page_num, nullptr);
        ObjectHandle* page_obj = nullptr;
        ASSERT_EQ(IntegerObject_ToObject(page_num, &page_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page_obj, nullptr);
        ASSERT_EQ(ArrayObject_Append(arr, page_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_Release(page_num), VANILLAPDF_ERROR_SUCCESS);

        // Add /Fit name
        NameObjectHandle* fit_name = nullptr;
        ASSERT_EQ(NameObject_CreateFromDecodedString("Fit", &fit_name), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(fit_name, nullptr);
        ObjectHandle* fit_obj = nullptr;
        ASSERT_EQ(NameObject_ToObject(fit_name, &fit_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(fit_obj, nullptr);
        ASSERT_EQ(ArrayObject_Append(arr, fit_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Object_Release(fit_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(NameObject_Release(fit_name), VANILLAPDF_ERROR_SUCCESS);

        // Make the array indirect by allocating an xref entry
        ObjectHandle* arr_obj = nullptr;
        ASSERT_EQ(ArrayObject_ToObject(arr, &arr_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(arr_obj, nullptr);

        XrefUsedEntryHandle* xref_entry = nullptr;
        ASSERT_EQ(File_AllocateNewEntry(file, &xref_entry), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_entry, nullptr);
        ASSERT_EQ(XrefUsedEntry_SetReference(xref_entry, arr_obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_Release(xref_entry), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Object_Release(arr_obj), VANILLAPDF_ERROR_SUCCESS);

        // Create destination from array (now indirect)
        ASSERT_EQ(Destination_CreateFromArray(arr, result), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(*result, nullptr);
        *out_array = arr;
    }
};

// Test creating and releasing NameDictionary
TEST(NameDictionary, CreateAndRelease) {
    NameDictionaryHandle* dict = nullptr;
    ASSERT_EQ(NameDictionary_Create(&dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict, nullptr);

    ASSERT_EQ(NameDictionary_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

// Test ContainsDestinations on empty NameDictionary
TEST(NameDictionary, ContainsDestinations_Empty) {
    NameDictionaryHandle* dict = nullptr;
    ASSERT_EQ(NameDictionary_Create(&dict), VANILLAPDF_ERROR_SUCCESS);

    boolean_type contains = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(NameDictionary_ContainsDestinations(dict, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(NameDictionary_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

// Test GetDestinations returns OBJECT_MISSING on empty NameDictionary
TEST(NameDictionary, GetDestinations_Empty) {
    NameDictionaryHandle* dict = nullptr;
    ASSERT_EQ(NameDictionary_Create(&dict), VANILLAPDF_ERROR_SUCCESS);

    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(NameDictionary_GetDestinations(dict, &tree), VANILLAPDF_ERROR_OBJECT_MISSING);

    ASSERT_EQ(NameDictionary_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

// Test ToUnknown and FromUnknown
TEST(NameDictionary, ToAndFromUnknown) {
    NameDictionaryHandle* dict = nullptr;
    ASSERT_EQ(NameDictionary_Create(&dict), VANILLAPDF_ERROR_SUCCESS);

    IUnknownHandle* unknown = nullptr;
    ASSERT_EQ(NameDictionary_ToUnknown(dict, &unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown, nullptr);

    NameDictionaryHandle* dict2 = nullptr;
    ASSERT_EQ(NameDictionary_FromUnknown(unknown, &dict2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict2, nullptr);

    ASSERT_EQ(NameDictionary_Release(dict2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IUnknown_Release(unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameDictionary_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

// Test creating and releasing DestinationNameTree
TEST(DestinationNameTree, CreateAndRelease) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree, nullptr);

    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test Contains on empty tree
TEST(DestinationNameTree, Contains_Empty) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree, nullptr);

    LiteralStringObjectHandle* name = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("test", &name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name, nullptr);

    StringObjectHandle* name_str = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, &name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str, nullptr);

    boolean_type contains = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTree_Contains(tree, name_str, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(StringObject_Release(name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test TryFind on empty tree
TEST(DestinationNameTree, TryFind_Empty) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree, nullptr);

    LiteralStringObjectHandle* name = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("test", &name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name, nullptr);

    StringObjectHandle* name_str = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, &name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str, nullptr);

    DestinationHandle* dest = nullptr;
    boolean_type found = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTree_TryFind(tree, name_str, &dest, &found), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(found, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(StringObject_Release(name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test GetIterator on empty tree
TEST(DestinationNameTree, Iterator_Empty) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);

    DestinationNameTreeIteratorHandle* iter = nullptr;
    ASSERT_EQ(DestinationNameTree_GetIterator(tree, &iter), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(iter, nullptr);

    boolean_type valid = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTreeIterator_IsValid(iter, &valid), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(valid, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(DestinationNameTreeIterator_Release(iter), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test ToUnknown and FromUnknown for DestinationNameTree
TEST(DestinationNameTree, ToAndFromUnknown) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);

    IUnknownHandle* unknown = nullptr;
    ASSERT_EQ(DestinationNameTree_ToUnknown(tree, &unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown, nullptr);

    DestinationNameTreeHandle* tree2 = nullptr;
    ASSERT_EQ(DestinationNameTree_FromUnknown(unknown, &tree2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree2, nullptr);

    ASSERT_EQ(DestinationNameTree_Release(tree2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IUnknown_Release(unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test SetDestinations and GetDestinations
TEST(NameDictionary, SetAndGetDestinations) {
    NameDictionaryHandle* dict = nullptr;
    ASSERT_EQ(NameDictionary_Create(&dict), VANILLAPDF_ERROR_SUCCESS);

    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);

    // Set the tree
    ASSERT_EQ(NameDictionary_SetDestinations(dict, tree), VANILLAPDF_ERROR_SUCCESS);

    // Now it should contain destinations
    boolean_type contains = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(NameDictionary_ContainsDestinations(dict, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_TRUE);

    // Get the tree back
    DestinationNameTreeHandle* tree2 = nullptr;
    ASSERT_EQ(NameDictionary_GetDestinations(dict, &tree2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree2, nullptr);

    ASSERT_EQ(DestinationNameTree_Release(tree2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameDictionary_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

// Test Insert and Find (requires document context for indirect objects)
TEST_F(DestinationNameTreeWithDocument, InsertAndFind) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree, nullptr);

    // Create a Fit destination using the page from document
    DestinationHandle* dest = nullptr;
    ArrayObjectHandle* dest_arr = nullptr;
    CreateFitDestinationWithPage(&dest, &dest_arr);

    // Create a name
    LiteralStringObjectHandle* name = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("TestDest", &name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name, nullptr);

    StringObjectHandle* name_str = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, &name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str, nullptr);

    // Insert the destination
    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str, dest), VANILLAPDF_ERROR_SUCCESS);

    // Now Contains should return true
    boolean_type contains = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTree_Contains(tree, name_str, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_TRUE);

    // Find should return the destination
    DestinationHandle* found_dest = nullptr;
    ASSERT_EQ(DestinationNameTree_Find(tree, name_str, &found_dest), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_dest, nullptr);

    // Verify it's a Fit destination
    DestinationType dest_type = DestinationType_Undefined;
    ASSERT_EQ(Destination_GetDestinationType(found_dest, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_Fit);

    ASSERT_EQ(Destination_Release(found_dest), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(dest_arr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test TryFind with existing item (requires document context for indirect objects)
TEST_F(DestinationNameTreeWithDocument, TryFind_Found) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree, nullptr);

    // Create and insert a destination using the page from document
    DestinationHandle* dest = nullptr;
    ArrayObjectHandle* dest_arr = nullptr;
    CreateFitDestinationWithPage(&dest, &dest_arr);

    LiteralStringObjectHandle* name = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("MyDest", &name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name, nullptr);

    StringObjectHandle* name_str = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, &name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str, nullptr);

    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str, dest), VANILLAPDF_ERROR_SUCCESS);

    // TryFind should return true and set the destination
    DestinationHandle* found_dest = nullptr;
    boolean_type found = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTree_TryFind(tree, name_str, &found_dest, &found), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(found, VANILLAPDF_RV_TRUE);
    ASSERT_NE(found_dest, nullptr);

    ASSERT_EQ(Destination_Release(found_dest), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(dest_arr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test Remove (requires document context)
TEST_F(DestinationNameTreeWithDocument, Remove) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree, nullptr);

    // Create and insert a destination using the page from document
    DestinationHandle* dest = nullptr;
    ArrayObjectHandle* dest_arr = nullptr;
    CreateFitDestinationWithPage(&dest, &dest_arr);

    LiteralStringObjectHandle* name = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("ToRemove", &name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name, nullptr);

    StringObjectHandle* name_str = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, &name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str, nullptr);

    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str, dest), VANILLAPDF_ERROR_SUCCESS);

    // Verify it exists
    boolean_type contains = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTree_Contains(tree, name_str, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_TRUE);

    // Remove it
    boolean_type removed = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTree_Remove(tree, name_str, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_TRUE);

    // Verify it no longer exists
    contains = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTree_Contains(tree, name_str, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_FALSE);

    // Removing again should return false
    removed = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTree_Remove(tree, name_str, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(StringObject_Release(name_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(dest_arr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}

// Test iterator with single item (requires document context)
// Note: Multiple inserts fail due to "Limits key already present" bug - see issue #227
TEST_F(DestinationNameTreeWithDocument, Iterator_WithItems) {
    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(DestinationNameTree_Create(&tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree, nullptr);

    // Create one destination (multiple inserts blocked by issue #227)
    DestinationHandle* dest1 = nullptr;
    ArrayObjectHandle* dest_arr1 = nullptr;
    CreateFitDestinationWithPage(&dest1, &dest_arr1);

    LiteralStringObjectHandle* name1 = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("TestDest", &name1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name1, nullptr);
    StringObjectHandle* name_str1 = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name1, &name_str1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str1, nullptr);

    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str1, dest1), VANILLAPDF_ERROR_SUCCESS);

    // Get iterator
    DestinationNameTreeIteratorHandle* iter = nullptr;
    ASSERT_EQ(DestinationNameTree_GetIterator(tree, &iter), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(iter, nullptr);

    // Count items
    int count = 0;
    boolean_type valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTreeIterator_IsValid(iter, &valid), VANILLAPDF_ERROR_SUCCESS);
    while (valid == VANILLAPDF_RV_TRUE) {
        // Get key
        StringObjectHandle* key = nullptr;
        ASSERT_EQ(DestinationNameTreeIterator_GetKey(iter, &key), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(key, nullptr);
        ASSERT_EQ(StringObject_Release(key), VANILLAPDF_ERROR_SUCCESS);

        // Get value
        DestinationHandle* value = nullptr;
        ASSERT_EQ(DestinationNameTreeIterator_GetValue(iter, &value), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(value, nullptr);
        ASSERT_EQ(Destination_Release(value), VANILLAPDF_ERROR_SUCCESS);

        count++;
        ASSERT_EQ(DestinationNameTreeIterator_Next(iter), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DestinationNameTreeIterator_IsValid(iter, &valid), VANILLAPDF_ERROR_SUCCESS);
    }

    EXPECT_EQ(count, 1);

    ASSERT_EQ(DestinationNameTreeIterator_Release(iter), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(name_str1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(name1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(dest_arr1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DestinationNameTree_Release(tree), VANILLAPDF_ERROR_SUCCESS);
}
