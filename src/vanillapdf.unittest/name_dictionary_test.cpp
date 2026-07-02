#include <gtest/gtest.h>

#include <vanillapdf/c_vanillapdf_api.h>

#include "test_data.h"
#include "handle_guard.h"

// Test fixture for tests that require a document context with indirect objects
// Creates a document programmatically with an indirect page - no disk files needed
class DestinationNameTreeWithDocument : public ::testing::Test {
protected:
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;

    void SetUp() override {
        // Create in-memory stream for the document
        ASSERT_EQ(InputOutputStream_CreateFromMemory(stream.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(stream.get(), nullptr);

        // Create file on the stream
        ASSERT_EQ(File_CreateStream(stream, "test_doc.pdf", file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(file.get(), nullptr);

        // Create new document (this is an empty document)
        ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(document.get(), nullptr);

        // Create a page from the document - this creates an INDIRECT page object
        ASSERT_EQ(PageObject_CreateFromDocument(document, page.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page.get(), nullptr);

        // Add the page to the document's page tree
        HandleGuard<CatalogHandle, Catalog_Release> catalog;
        ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(catalog.get(), nullptr);

        HandleGuard<PageTreeHandle, PageTree_Release> page_tree;
        ASSERT_EQ(Catalog_GetPages(catalog, page_tree.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page_tree.get(), nullptr);

        // Note: PageTree_InsertPage(tree, 0, page) is broken - see issue #226
        // Use AppendPage as workaround
        ASSERT_EQ(PageTree_AppendPage(page_tree, page), VANILLAPDF_ERROR_SUCCESS);
    }

    // Helper to create a Fit destination with an indirect array
    // The array must be made indirect for NameTree::Insert to work
    // Note: arr ownership is transferred to caller via out_array, so it stays as raw pointer
    void CreateFitDestinationWithPage(DestinationHandle** result, ArrayObjectHandle** out_array) {
        ArrayObjectHandle* arr = nullptr;
        ASSERT_EQ(ArrayObject_Create(&arr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(arr, nullptr);

        // Add page number (0) as IntegerObject
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> page_num;
        ASSERT_EQ(IntegerObject_CreateFromIntegerValue(0, page_num.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page_num.get(), nullptr);
        HandleGuard<ObjectHandle, Object_Release> page_obj;
        ASSERT_EQ(IntegerObject_ToObject(page_num, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(page_obj.get(), nullptr);
        ASSERT_EQ(ArrayObject_Append(arr, page_obj), VANILLAPDF_ERROR_SUCCESS);

        // Add /Fit name
        HandleGuard<NameObjectHandle, NameObject_Release> fit_name;
        ASSERT_EQ(NameObject_CreateFromDecodedString("Fit", fit_name.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(fit_name.get(), nullptr);
        HandleGuard<ObjectHandle, Object_Release> fit_obj;
        ASSERT_EQ(NameObject_ToObject(fit_name, fit_obj.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(fit_obj.get(), nullptr);
        ASSERT_EQ(ArrayObject_Append(arr, fit_obj), VANILLAPDF_ERROR_SUCCESS);

        // Make the array indirect by allocating an xref entry
        HandleGuard<ObjectHandle, Object_Release> arr_obj;
        ASSERT_EQ(ArrayObject_ToObject(arr, arr_obj.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(arr_obj.get(), nullptr);

        HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> xref_entry;
        ASSERT_EQ(File_AllocateNewEntry(file, xref_entry.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_entry.get(), nullptr);
        ASSERT_EQ(XrefUsedEntry_SetReference(xref_entry, arr_obj), VANILLAPDF_ERROR_SUCCESS);

        // Create destination from array (now indirect)
        ASSERT_EQ(Destination_CreateFromArray(arr, result), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(*result, nullptr);
        *out_array = arr;
    }
};

// Test creating and releasing NameDictionary
TEST(NameDictionary, CreateAndRelease) {
    HandleGuard<NameDictionaryHandle, NameDictionary_Release> dict;
    ASSERT_EQ(NameDictionary_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict.get(), nullptr);
}

// Test ContainsDestinations on empty NameDictionary
TEST(NameDictionary, ContainsDestinations_Empty) {
    HandleGuard<NameDictionaryHandle, NameDictionary_Release> dict;
    ASSERT_EQ(NameDictionary_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type contains = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(NameDictionary_ContainsDestinations(dict, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_FALSE);
}

// Test GetDestinations returns OBJECT_MISSING on empty NameDictionary
TEST(NameDictionary, GetDestinations_Empty) {
    HandleGuard<NameDictionaryHandle, NameDictionary_Release> dict;
    ASSERT_EQ(NameDictionary_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);

    DestinationNameTreeHandle* tree = nullptr;
    ASSERT_EQ(NameDictionary_GetDestinations(dict, &tree), VANILLAPDF_ERROR_OBJECT_MISSING);
}

// Test ToUnknown and FromUnknown
TEST(NameDictionary, ToAndFromUnknown) {
    HandleGuard<NameDictionaryHandle, NameDictionary_Release> dict;
    ASSERT_EQ(NameDictionary_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(NameDictionary_ToUnknown(dict, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    HandleGuard<NameDictionaryHandle, NameDictionary_Release> dict2;
    ASSERT_EQ(NameDictionary_FromUnknown(unknown, dict2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict2.get(), nullptr);
}

// Test creating and releasing DestinationNameTree
TEST(DestinationNameTree, CreateAndRelease) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree.get(), nullptr);
}

// Test Contains on empty tree
TEST(DestinationNameTree, Contains_Empty) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree.get(), nullptr);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("test", name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name.get(), nullptr);

    HandleGuard<StringObjectHandle, StringObject_Release> name_str;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, name_str.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str.get(), nullptr);

    boolean_type contains = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTree_Contains(tree, name_str, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_FALSE);
}

// Test TryFind on empty tree
TEST(DestinationNameTree, TryFind_Empty) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree.get(), nullptr);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("test", name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name.get(), nullptr);

    HandleGuard<StringObjectHandle, StringObject_Release> name_str;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, name_str.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str.get(), nullptr);

    DestinationHandle* dest = nullptr;
    boolean_type found = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTree_TryFind(tree, name_str, &dest, &found), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(found, VANILLAPDF_RV_FALSE);
}

// Test GetIterator on empty tree
TEST(DestinationNameTree, Iterator_Empty) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DestinationNameTreeIteratorHandle, DestinationNameTreeIterator_Release> iter;
    ASSERT_EQ(DestinationNameTree_GetIterator(tree, iter.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(iter.get(), nullptr);

    boolean_type valid = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(DestinationNameTreeIterator_IsValid(iter, &valid), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(valid, VANILLAPDF_RV_FALSE);
}

// Test ToUnknown and FromUnknown for DestinationNameTree
TEST(DestinationNameTree, ToAndFromUnknown) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(DestinationNameTree_ToUnknown(tree, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree2;
    ASSERT_EQ(DestinationNameTree_FromUnknown(unknown, tree2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree2.get(), nullptr);
}

// Test SetDestinations and GetDestinations
TEST(NameDictionary, SetAndGetDestinations) {
    HandleGuard<NameDictionaryHandle, NameDictionary_Release> dict;
    ASSERT_EQ(NameDictionary_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);

    // Set the tree
    ASSERT_EQ(NameDictionary_SetDestinations(dict, tree), VANILLAPDF_ERROR_SUCCESS);

    // Now it should contain destinations
    boolean_type contains = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(NameDictionary_ContainsDestinations(dict, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_TRUE);

    // Get the tree back
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree2;
    ASSERT_EQ(NameDictionary_GetDestinations(dict, tree2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree2.get(), nullptr);
}

// Test Insert and Find (requires document context for indirect objects)
TEST_F(DestinationNameTreeWithDocument, InsertAndFind) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree.get(), nullptr);

    // Create a Fit destination using the page from document
    DestinationHandle* dest = nullptr;
    ArrayObjectHandle* dest_arr = nullptr;
    CreateFitDestinationWithPage(&dest, &dest_arr);

    // Create a name
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("TestDest", name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name.get(), nullptr);

    HandleGuard<StringObjectHandle, StringObject_Release> name_str;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, name_str.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str.get(), nullptr);

    // Insert the destination
    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str, dest), VANILLAPDF_ERROR_SUCCESS);

    // Now Contains should return true
    boolean_type contains = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTree_Contains(tree, name_str, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_TRUE);

    // Find should return the destination
    HandleGuard<DestinationHandle, Destination_Release> found_dest;
    ASSERT_EQ(DestinationNameTree_Find(tree, name_str, found_dest.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_dest.get(), nullptr);

    // Verify it's a Fit destination
    DestinationType dest_type = DestinationType_Undefined;
    ASSERT_EQ(Destination_GetDestinationType(found_dest, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_Fit);

    Destination_Release(dest);
    ArrayObject_Release(dest_arr);
}

// Test TryFind with existing item (requires document context for indirect objects)
TEST_F(DestinationNameTreeWithDocument, TryFind_Found) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree.get(), nullptr);

    // Create and insert a destination using the page from document
    DestinationHandle* dest = nullptr;
    ArrayObjectHandle* dest_arr = nullptr;
    CreateFitDestinationWithPage(&dest, &dest_arr);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("MyDest", name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name.get(), nullptr);

    HandleGuard<StringObjectHandle, StringObject_Release> name_str;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, name_str.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str.get(), nullptr);

    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str, dest), VANILLAPDF_ERROR_SUCCESS);

    // TryFind should return true and set the destination
    HandleGuard<DestinationHandle, Destination_Release> found_dest;
    boolean_type found = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTree_TryFind(tree, name_str, found_dest.out(), &found), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(found, VANILLAPDF_RV_TRUE);
    ASSERT_NE(found_dest.get(), nullptr);

    Destination_Release(dest);
    ArrayObject_Release(dest_arr);
}

// Test Remove (requires document context)
TEST_F(DestinationNameTreeWithDocument, Remove) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree.get(), nullptr);

    // Create and insert a destination using the page from document
    DestinationHandle* dest = nullptr;
    ArrayObjectHandle* dest_arr = nullptr;
    CreateFitDestinationWithPage(&dest, &dest_arr);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("ToRemove", name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name.get(), nullptr);

    HandleGuard<StringObjectHandle, StringObject_Release> name_str;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name, name_str.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str.get(), nullptr);

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

    Destination_Release(dest);
    ArrayObject_Release(dest_arr);
}

// Test iterator with multiple items (requires document context)
TEST_F(DestinationNameTreeWithDocument, Iterator_WithItems) {
    HandleGuard<DestinationNameTreeHandle, DestinationNameTree_Release> tree;
    ASSERT_EQ(DestinationNameTree_Create(tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(tree.get(), nullptr);

    // Create first destination
    DestinationHandle* dest1 = nullptr;
    ArrayObjectHandle* dest_arr1 = nullptr;
    CreateFitDestinationWithPage(&dest1, &dest_arr1);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name1;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("TestDest", name1.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name1.get(), nullptr);
    HandleGuard<StringObjectHandle, StringObject_Release> name_str1;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name1, name_str1.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str1.get(), nullptr);

    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str1, dest1), VANILLAPDF_ERROR_SUCCESS);

    // Create a second destination. A second Insert previously failed with
    // "The key Limits was already present in the dictionary" - see issue #227
    DestinationHandle* dest2 = nullptr;
    ArrayObjectHandle* dest_arr2 = nullptr;
    CreateFitDestinationWithPage(&dest2, &dest_arr2);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name2;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("OtherDest", name2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name2.get(), nullptr);
    HandleGuard<StringObjectHandle, StringObject_Release> name_str2;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name2, name_str2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_str2.get(), nullptr);

    ASSERT_EQ(DestinationNameTree_Insert(tree, name_str2, dest2), VANILLAPDF_ERROR_SUCCESS);

    // Get iterator
    HandleGuard<DestinationNameTreeIteratorHandle, DestinationNameTreeIterator_Release> iter;
    ASSERT_EQ(DestinationNameTree_GetIterator(tree, iter.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(iter.get(), nullptr);

    // Count items
    int count = 0;
    boolean_type valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DestinationNameTreeIterator_IsValid(iter, &valid), VANILLAPDF_ERROR_SUCCESS);
    while (valid == VANILLAPDF_RV_TRUE) {
        // Get key
        HandleGuard<StringObjectHandle, StringObject_Release> key;
        ASSERT_EQ(DestinationNameTreeIterator_GetKey(iter, key.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(key.get(), nullptr);

        // Get value
        HandleGuard<DestinationHandle, Destination_Release> value;
        ASSERT_EQ(DestinationNameTreeIterator_GetValue(iter, value.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(value.get(), nullptr);

        count++;
        ASSERT_EQ(DestinationNameTreeIterator_Next(iter), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DestinationNameTreeIterator_IsValid(iter, &valid), VANILLAPDF_ERROR_SUCCESS);
    }

    EXPECT_EQ(count, 2);

    Destination_Release(dest1);
    ArrayObject_Release(dest_arr1);
    Destination_Release(dest2);
    ArrayObject_Release(dest_arr2);
}
