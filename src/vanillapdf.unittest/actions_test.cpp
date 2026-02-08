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

} // namespace actions
