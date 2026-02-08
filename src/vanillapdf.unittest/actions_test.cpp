#include "unittest.h"
#include "handle_guard.h"

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
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict(CreateActionDict("GoTo"));

    HandleGuard<ActionHandle, Action_Release> action;
    ASSERT_EQ(Action_CreateFromDictionary(dict.get(), action.out()), VANILLAPDF_ERROR_SUCCESS);

    ActionType type = ActionType_Undefined;
    ASSERT_EQ(Action_GetActionType(action.get(), &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, ActionType_GoTo);
}

TEST(ActionType, URIAction) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict(CreateActionDict("URI"));

    HandleGuard<ActionHandle, Action_Release> action;
    ASSERT_EQ(Action_CreateFromDictionary(dict.get(), action.out()), VANILLAPDF_ERROR_SUCCESS);

    ActionType type = ActionType_Undefined;
    ASSERT_EQ(Action_GetActionType(action.get(), &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, ActionType_URI);
}

TEST(ActionType, NamedAction) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict(CreateActionDict("Named"));

    HandleGuard<ActionHandle, Action_Release> action;
    ASSERT_EQ(Action_CreateFromDictionary(dict.get(), action.out()), VANILLAPDF_ERROR_SUCCESS);

    ActionType type = ActionType_Undefined;
    ASSERT_EQ(Action_GetActionType(action.get(), &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, ActionType_Named);
}

TEST(ActionType, GoToRemoteAction) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict(CreateActionDict("GoToR"));

    HandleGuard<ActionHandle, Action_Release> action;
    ASSERT_EQ(Action_CreateFromDictionary(dict.get(), action.out()), VANILLAPDF_ERROR_SUCCESS);

    ActionType type = ActionType_Undefined;
    ASSERT_EQ(Action_GetActionType(action.get(), &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, ActionType_GoToRemote);
}

TEST(ActionType, LaunchAction) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict(CreateActionDict("Launch"));

    HandleGuard<ActionHandle, Action_Release> action;
    ASSERT_EQ(Action_CreateFromDictionary(dict.get(), action.out()), VANILLAPDF_ERROR_SUCCESS);

    ActionType type = ActionType_Undefined;
    ASSERT_EQ(Action_GetActionType(action.get(), &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, ActionType_Launch);
}

TEST(ActionType, JavaScriptAction) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict(CreateActionDict("JavaScript"));

    HandleGuard<ActionHandle, Action_Release> action;
    ASSERT_EQ(Action_CreateFromDictionary(dict.get(), action.out()), VANILLAPDF_ERROR_SUCCESS);

    ActionType type = ActionType_Undefined;
    ASSERT_EQ(Action_GetActionType(action.get(), &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, ActionType_JavaScript);
}

TEST(ActionType, InvalidActionType) {
    // Create dictionary with an invalid /S value - this should not crash
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict(CreateActionDict("InvalidType"));

    ActionHandle* action = nullptr;
    EXPECT_NE(Action_CreateFromDictionary(dict.get(), &action), VANILLAPDF_ERROR_SUCCESS);
}

TEST(ActionType, MissingSEntry) {
    // Create dictionary without /S entry
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);

    ActionHandle* action = nullptr;
    EXPECT_NE(Action_CreateFromDictionary(dict.get(), &action), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Action, CreateFromDictionaryNull) {
    ActionHandle* action = nullptr;
    EXPECT_EQ(Action_CreateFromDictionary(nullptr, &action), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Action_CreateFromDictionary(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
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
