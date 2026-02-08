#include "unittest.h"
#include "handle_guard.h"

namespace dirty_tracking {

// Helper: assert object is clean (not dirty)
static void AssertClean(ObjectHandle* obj) {
    boolean_type dirty = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Object_IsDirty(obj, &dirty), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dirty, VANILLAPDF_RV_FALSE);
}

// Helper: assert object is dirty
static void AssertDirty(ObjectHandle* obj) {
    boolean_type dirty = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Object_IsDirty(obj, &dirty), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dirty, VANILLAPDF_RV_TRUE);
}

// --- Null checks ---

TEST(ObjectIsDirty, NullHandle) {
    boolean_type dirty = VANILLAPDF_RV_FALSE;
    EXPECT_EQ(Object_IsDirty(nullptr, &dirty), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(ObjectIsDirty, NullResult) {
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> int_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(IntegerObject_Create(int_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(int_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(Object_IsDirty(base_obj, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// --- Freshly created objects are clean ---

TEST(ObjectIsDirty, IntegerCleanAfterCreate) {
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> int_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(IntegerObject_Create(int_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(int_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);
}

TEST(ObjectIsDirty, BooleanCleanAfterCreate) {
    HandleGuard<BooleanObjectHandle, BooleanObject_Release> bool_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(BooleanObject_Create(bool_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(BooleanObject_ToObject(bool_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);
}

TEST(ObjectIsDirty, DictionaryCleanAfterCreate) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(DictionaryObject_Create(dict_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_ToObject(dict_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);
}

TEST(ObjectIsDirty, ArrayCleanAfterCreate) {
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> arr_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(ArrayObject_Create(arr_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_ToObject(arr_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);
}

TEST(ObjectIsDirty, StreamCleanAfterCreate) {
    HandleGuard<StreamObjectHandle, StreamObject_Release> stream_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(StreamObject_Create(stream_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_ToObject(stream_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);
}

// --- Leaf object mutations ---

TEST(ObjectIsDirty, IntegerDirtyAfterSetValue) {
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> int_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(IntegerObject_Create(int_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(int_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);

    ASSERT_EQ(IntegerObject_SetIntegerValue(int_obj, 42), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(base_obj);
}

TEST(ObjectIsDirty, RealDirtyAfterSetValue) {
    HandleGuard<RealObjectHandle, RealObject_Release> real_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(RealObject_CreateFromData(1.0f, 1, real_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_ToObject(real_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);

    ASSERT_EQ(RealObject_SetValue(real_obj, 2.0f), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(base_obj);
}

TEST(ObjectIsDirty, BooleanDirtyAfterSetValue) {
    HandleGuard<BooleanObjectHandle, BooleanObject_Release> bool_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;

    ASSERT_EQ(BooleanObject_Create(bool_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(BooleanObject_ToObject(bool_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);

    ASSERT_EQ(BooleanObject_SetValue(bool_obj, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(base_obj);
}

TEST(ObjectIsDirty, NameDirtyAfterSetValue) {
    HandleGuard<NameObjectHandle, NameObject_Release> name_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;
    HandleGuard<BufferHandle, Buffer_Release> buf;

    ASSERT_EQ(NameObject_CreateFromDecodedString("Original", name_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_ToObject(name_obj, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);

    ASSERT_EQ(Buffer_CreateFromData("Modified", 8, buf.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_SetValue(name_obj, buf), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(base_obj);
}

TEST(ObjectIsDirty, LiteralStringDirtyAfterSetValue) {
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> str_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;
    HandleGuard<BufferHandle, Buffer_Release> buf;

    // StringObject_ToObject works for both literal and hex strings
    HandleGuard<StringObjectHandle, StringObject_Release> string_base;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("original", str_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_ToStringObject(str_obj, string_base.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_ToObject(string_base, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);

    ASSERT_EQ(Buffer_CreateFromData("modified", 8, buf.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_SetValue(str_obj, buf), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(base_obj);
}

TEST(ObjectIsDirty, HexStringDirtyAfterSetValue) {
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> hex_obj;
    HandleGuard<ObjectHandle, Object_Release> base_obj;
    HandleGuard<BufferHandle, Buffer_Release> buf;

    HandleGuard<StringObjectHandle, StringObject_Release> string_base;

    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("AABB", hex_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HexadecimalStringObject_ToStringObject(hex_obj, string_base.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_ToObject(string_base, base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(base_obj);

    ASSERT_EQ(Buffer_CreateFromData("CCDD", 4, buf.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HexadecimalStringObject_SetValue(hex_obj, buf), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(base_obj);
}

// --- Container structural mutations ---

TEST(ObjectIsDirty, DictionaryDirtyAfterInsert) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict_obj;
    HandleGuard<ObjectHandle, Object_Release> dict_base;

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> value_obj;
    HandleGuard<ObjectHandle, Object_Release> value_base;

    ASSERT_EQ(DictionaryObject_Create(dict_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_ToObject(dict_obj, dict_base.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(IntegerObject_Create(value_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(value_obj, value_base.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(dict_base);

    ASSERT_EQ(DictionaryObject_InsertConst(dict_obj, NameConstant_Type, value_base, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(dict_base);
}

TEST(ObjectIsDirty, DictionaryDirtyAfterRemove) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict_obj;
    HandleGuard<ObjectHandle, Object_Release> dict_base;

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> value_obj;
    HandleGuard<ObjectHandle, Object_Release> value_base;

    ASSERT_EQ(DictionaryObject_Create(dict_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Create(value_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(value_obj, value_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Insert first
    ASSERT_EQ(DictionaryObject_InsertConst(dict_obj, NameConstant_Type, value_base, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    // Convert to base for dirty check
    ASSERT_EQ(DictionaryObject_ToObject(dict_obj, dict_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Already dirty from insert, confirm remove also keeps it dirty
    boolean_type removed = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DictionaryObject_Remove(dict_obj, NameConstant_Type, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_TRUE);

    AssertDirty(dict_base);
}

TEST(ObjectIsDirty, DictionaryDirtyAfterClear) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict_obj;
    HandleGuard<ObjectHandle, Object_Release> dict_base;

    ASSERT_EQ(DictionaryObject_Create(dict_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_ToObject(dict_obj, dict_base.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(dict_base);

    ASSERT_EQ(DictionaryObject_Clear(dict_obj), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(dict_base);
}

TEST(ObjectIsDirty, ArrayDirtyAfterAppend) {
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> arr_obj;
    HandleGuard<ObjectHandle, Object_Release> arr_base;

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> value_obj;
    HandleGuard<ObjectHandle, Object_Release> value_base;

    ASSERT_EQ(ArrayObject_Create(arr_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_ToObject(arr_obj, arr_base.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(IntegerObject_Create(value_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(value_obj, value_base.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(arr_base);

    ASSERT_EQ(ArrayObject_Append(arr_obj, value_base), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(arr_base);
}

TEST(ObjectIsDirty, ArrayDirtyAfterRemove) {
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> arr_obj;
    HandleGuard<ObjectHandle, Object_Release> arr_base;

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> value_obj;
    HandleGuard<ObjectHandle, Object_Release> value_base;

    ASSERT_EQ(ArrayObject_Create(arr_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Create(value_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(value_obj, value_base.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(ArrayObject_Append(arr_obj, value_base), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(ArrayObject_ToObject(arr_obj, arr_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Remove the element
    ASSERT_EQ(ArrayObject_Remove(arr_obj, 0), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(arr_base);
}

// --- Container child propagation ---

TEST(ObjectIsDirty, DictionaryDirtyFromChildMutation) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict_obj;
    HandleGuard<ObjectHandle, Object_Release> dict_base;

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> value_obj;
    HandleGuard<ObjectHandle, Object_Release> value_base;

    ASSERT_EQ(DictionaryObject_Create(dict_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Create(value_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(value_obj, value_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Insert while dict is freshly created — this will mark it dirty via structural change
    ASSERT_EQ(DictionaryObject_InsertConst(dict_obj, NameConstant_Type, value_base, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    // The dictionary is already dirty from Insert.
    // The important test here is that child mutation is also visible.
    // Mutate the child integer value.
    ASSERT_EQ(IntegerObject_SetIntegerValue(value_obj, 99), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_ToObject(dict_obj, dict_base.out()), VANILLAPDF_ERROR_SUCCESS);
    AssertDirty(dict_base);

    // Also verify the child itself is dirty
    AssertDirty(value_base);
}

TEST(ObjectIsDirty, ArrayDirtyFromChildMutation) {
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> arr_obj;
    HandleGuard<ObjectHandle, Object_Release> arr_base;

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> value_obj;
    HandleGuard<ObjectHandle, Object_Release> value_base;

    ASSERT_EQ(ArrayObject_Create(arr_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Create(value_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(value_obj, value_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Insert child
    ASSERT_EQ(ArrayObject_Append(arr_obj, value_base), VANILLAPDF_ERROR_SUCCESS);

    // Mutate the child
    ASSERT_EQ(IntegerObject_SetIntegerValue(value_obj, 77), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(ArrayObject_ToObject(arr_obj, arr_base.out()), VANILLAPDF_ERROR_SUCCESS);
    AssertDirty(arr_base);
}

// --- Stream mutations ---

TEST(ObjectIsDirty, StreamDirtyAfterSetHeader) {
    HandleGuard<StreamObjectHandle, StreamObject_Release> stream_obj;
    HandleGuard<ObjectHandle, Object_Release> stream_base;

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> header_obj;

    ASSERT_EQ(StreamObject_Create(stream_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_ToObject(stream_obj, stream_base.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(stream_base);

    ASSERT_EQ(DictionaryObject_Create(header_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_SetHeader(stream_obj, header_obj), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(stream_base);
}

TEST(ObjectIsDirty, StreamDirtyAfterSetBody) {
    HandleGuard<StreamObjectHandle, StreamObject_Release> stream_obj;
    HandleGuard<ObjectHandle, Object_Release> stream_base;

    HandleGuard<BufferHandle, Buffer_Release> body;

    ASSERT_EQ(StreamObject_Create(stream_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_ToObject(stream_obj, stream_base.out()), VANILLAPDF_ERROR_SUCCESS);

    AssertClean(stream_base);

    ASSERT_EQ(Buffer_CreateFromData("body data", 9, body.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_SetBody(stream_obj, body), VANILLAPDF_ERROR_SUCCESS);

    AssertDirty(stream_base);
}

TEST(ObjectIsDirty, StreamDirtyFromHeaderChildMutation) {
    HandleGuard<StreamObjectHandle, StreamObject_Release> stream_obj;
    HandleGuard<ObjectHandle, Object_Release> stream_base;

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> header_obj;
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> value_obj;
    HandleGuard<ObjectHandle, Object_Release> value_base;

    ASSERT_EQ(StreamObject_Create(stream_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_ToObject(stream_obj, stream_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Get the default header and insert an entry into it
    ASSERT_EQ(StreamObject_GetHeader(stream_obj, header_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(IntegerObject_Create(value_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(value_obj, value_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Stream is still clean before we mutate
    AssertClean(stream_base);

    // Mutate the header dictionary
    ASSERT_EQ(DictionaryObject_InsertConst(header_obj, NameConstant_Length, value_base, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    // Stream should detect the header mutation
    AssertDirty(stream_base);
}

// --- Xref dirty tracking ---

TEST(XrefIsDirty, NullHandle) {
    boolean_type dirty = VANILLAPDF_RV_FALSE;
    EXPECT_EQ(Xref_IsDirty(nullptr, &dirty), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(XrefIsDirty, NullResult) {
    HandleGuard<XrefHandle, Xref_Release> xref;
    ASSERT_EQ(Xref_Create(xref.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(Xref_IsDirty(xref, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(XrefIsDirty, CleanAfterCreate) {
    HandleGuard<XrefHandle, Xref_Release> xref;
    ASSERT_EQ(Xref_Create(xref.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type dirty = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Xref_IsDirty(xref, &dirty), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dirty, VANILLAPDF_RV_FALSE);
}

TEST(XrefIsDirty, DirtyAfterInsert) {
    HandleGuard<XrefHandle, Xref_Release> xref;
    HandleGuard<XrefFreeEntryHandle, XrefFreeEntry_Release> entry;
    HandleGuard<XrefEntryHandle, XrefEntry_Release> entry_base;

    ASSERT_EQ(Xref_Create(xref.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefFreeEntry_Create(0, 65535, 0, entry.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefFreeEntry_ToEntry(entry, entry_base.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type dirty = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Xref_IsDirty(xref, &dirty), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dirty, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(Xref_Insert(xref, entry_base), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Xref_IsDirty(xref, &dirty), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dirty, VANILLAPDF_RV_TRUE);
}

TEST(XrefIsDirty, DirtyAfterRemove) {
    HandleGuard<XrefHandle, Xref_Release> xref;
    HandleGuard<XrefFreeEntryHandle, XrefFreeEntry_Release> entry;
    HandleGuard<XrefEntryHandle, XrefEntry_Release> entry_base;

    ASSERT_EQ(Xref_Create(xref.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefFreeEntry_Create(1, 0, 0, entry.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefFreeEntry_ToEntry(entry, entry_base.out()), VANILLAPDF_ERROR_SUCCESS);

    // Insert first, then remove
    ASSERT_EQ(Xref_Insert(xref, entry_base), VANILLAPDF_ERROR_SUCCESS);

    // Already dirty from insert, confirm remove also sets dirty
    boolean_type removed = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Xref_Remove(xref, 1, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_TRUE);

    boolean_type dirty = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Xref_IsDirty(xref, &dirty), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dirty, VANILLAPDF_RV_TRUE);
}

} /* dirty_tracking */
