#include "unittest.h"
#include "handle_guard.h"

#include <string>

namespace xref {

TEST(Xref, CreateRelease) {
    HandleGuard<XrefHandle, Xref_Release> xref_ptr;

    ASSERT_EQ(Xref_Create(xref_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xref_ptr.get(), nullptr);
}

TEST(Xref, NullCheck) {
    EXPECT_EQ(Xref_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Xref_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Xref, AddRemoveEntry) {
    HandleGuard<XrefHandle, Xref_Release> xref_ptr;

    HandleGuard<XrefEntryHandle, XrefEntry_Release> xref_entry_ptr;
    HandleGuard<XrefFreeEntryHandle, XrefFreeEntry_Release> xref_free_entry_ptr;

    boolean_type remove_result = VANILLAPDF_RV_FALSE;
    boolean_type contains_result = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(Xref_Create(xref_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xref_ptr.get(), nullptr);

    ASSERT_EQ(XrefFreeEntry_Create(1, 0, 65535, xref_free_entry_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xref_free_entry_ptr.get(), nullptr);

    ASSERT_EQ(XrefFreeEntry_ToEntry(xref_free_entry_ptr, xref_entry_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xref_entry_ptr.get(), nullptr);

    ASSERT_EQ(Xref_Insert(xref_ptr, xref_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Xref_Contains(xref_ptr, 1, &contains_result), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Xref_Remove(xref_ptr, 1, &remove_result), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(contains_result, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(remove_result, VANILLAPDF_RV_TRUE);
}

TEST(XrefFreeEntry, CreateRelease) {
    HandleGuard<XrefFreeEntryHandle, XrefFreeEntry_Release> xref_free_entry_ptr;

    ASSERT_EQ(XrefFreeEntry_Create(0, 0, 0, xref_free_entry_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xref_free_entry_ptr.get(), nullptr);
}

TEST(XrefUsedEntry, CreateRelease) {
    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> xref_used_entry_ptr;

    ASSERT_EQ(XrefUsedEntry_Create(0, 0, 0, xref_used_entry_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xref_used_entry_ptr.get(), nullptr);
}

TEST(XrefCompressedEntry, CreateRelease) {
    HandleGuard<XrefCompressedEntryHandle, XrefCompressedEntry_Release> xref_compressed_entry_ptr;

    ASSERT_EQ(XrefCompressedEntry_Create(0, 0, 0, 0, xref_compressed_entry_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xref_compressed_entry_ptr.get(), nullptr);
}

typedef error_type (*EntryFactory)(XrefEntryHandle** result);

static error_type CreateFreeEntry(XrefEntryHandle** result) {
    HandleGuard<XrefFreeEntryHandle, XrefFreeEntry_Release> entry;

    error_type error = XrefFreeEntry_Create(0, 65535, 0, entry.out());
    if (error != VANILLAPDF_ERROR_SUCCESS) {
        return error;
    }

    return XrefFreeEntry_ToEntry(entry, result);
}

static error_type CreateUsedEntry(XrefEntryHandle** result) {
    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> entry;

    error_type error = XrefUsedEntry_Create(1, 0, 0, entry.out());
    if (error != VANILLAPDF_ERROR_SUCCESS) {
        return error;
    }

    return XrefUsedEntry_ToEntry(entry, result);
}

static error_type CreateCompressedEntry(XrefEntryHandle** result) {
    HandleGuard<XrefCompressedEntryHandle, XrefCompressedEntry_Release> entry;

    error_type error = XrefCompressedEntry_Create(2, 0, 1, 0, entry.out());
    if (error != VANILLAPDF_ERROR_SUCCESS) {
        return error;
    }

    return XrefCompressedEntry_ToEntry(entry, result);
}

struct InUseTestCase {
    InUseTestCase(const char* name, EntryFactory factory, XrefEntryType type, boolean_type expected_in_use)
        : name(name), factory(factory), type(type), expected_in_use(expected_in_use) {
    }

    const char* name;
    EntryFactory factory;
    XrefEntryType type;
    boolean_type expected_in_use;
};

class XrefEntryInUse : public ::testing::TestWithParam<InUseTestCase> {};

// XrefEntry_InUse is documented as a check whether the entry is used or compressed.
// It used to report the internal lazy-load flag instead, which stays false until the
// referenced object has been parsed. None of the entries below hold a reference,
// therefore the value has to be derived purely from the entry type.
TEST_P(XrefEntryInUse, ReflectsEntryTypeWithoutReference) {
    const InUseTestCase& test_case = GetParam();

    HandleGuard<XrefEntryHandle, XrefEntry_Release> entry;
    ASSERT_EQ(test_case.factory(entry.out()), VANILLAPDF_ERROR_SUCCESS);

    XrefEntryType type = XrefEntryType_Null;
    ASSERT_EQ(XrefEntry_GetType(entry, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, test_case.type);

    boolean_type in_use = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(XrefEntry_InUse(entry, &in_use), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(in_use, test_case.expected_in_use);
}

INSTANTIATE_TEST_SUITE_P(EntryTypes, XrefEntryInUse,
    ::testing::Values(
        InUseTestCase("Free", CreateFreeEntry, XrefEntryType_Free, VANILLAPDF_RV_FALSE),
        InUseTestCase("Used", CreateUsedEntry, XrefEntryType_Used, VANILLAPDF_RV_TRUE),
        InUseTestCase("Compressed", CreateCompressedEntry, XrefEntryType_Compressed, VANILLAPDF_RV_TRUE)
    ),
    [](const ::testing::TestParamInfo<InUseTestCase>& info) {
        return std::string(info.param.name);
    }
);

// Attaching the object to the entry must not change the reported value
TEST(XrefUsedEntry, InUseIsStableAfterMaterialization) {
    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> used_entry;
    ASSERT_EQ(XrefUsedEntry_Create(1, 0, 0, used_entry.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XrefEntryHandle, XrefEntry_Release> entry;
    ASSERT_EQ(XrefUsedEntry_ToEntry(used_entry, entry.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type before = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(XrefEntry_InUse(entry, &before), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(before, VANILLAPDF_RV_TRUE);

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer;
    ASSERT_EQ(IntegerObject_Create(integer.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> object;
    ASSERT_EQ(IntegerObject_ToObject(integer, object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefUsedEntry_SetReference(used_entry, object), VANILLAPDF_ERROR_SUCCESS);

    boolean_type after = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(XrefEntry_InUse(entry, &after), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(after, VANILLAPDF_RV_TRUE);
}

} /* xref */
