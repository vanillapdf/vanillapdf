#include "unittest.h"

#include "vanillapdf/c_vanillapdf_api.h"

namespace digital_signature_objects {

// Tests for ByteRange and ByteRangeCollection manipulation API

TEST(ByteRange, CreateEmptyAndSet) {
    ByteRangeHandle* range = nullptr;
    ASSERT_EQ(ByteRange_Create(&range), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(range, nullptr);

    // Create offset and length
    IntegerObjectHandle* offset = nullptr;
    ASSERT_EQ(IntegerObject_Create(&offset), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset, 100), VANILLAPDF_ERROR_SUCCESS);

    IntegerObjectHandle* length = nullptr;
    ASSERT_EQ(IntegerObject_Create(&length), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length, 200), VANILLAPDF_ERROR_SUCCESS);

    // Set values
    ASSERT_EQ(ByteRange_SetOffset(range, offset), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRange_SetLength(range, length), VANILLAPDF_ERROR_SUCCESS);

    // Verify values
    IntegerObjectHandle* retrieved_offset = nullptr;
    ASSERT_EQ(ByteRange_GetOffset(range, &retrieved_offset), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 100);

    IntegerObjectHandle* retrieved_length = nullptr;
    ASSERT_EQ(ByteRange_GetLength(range, &retrieved_length), VANILLAPDF_ERROR_SUCCESS);
    bigint_type length_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_length, &length_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(length_value, 200);

    // Cleanup
    IntegerObject_Release(retrieved_offset);
    IntegerObject_Release(retrieved_length);
    IntegerObject_Release(offset);
    IntegerObject_Release(length);
    ByteRange_Release(range);
}

TEST(ByteRange, CreateFromData) {
    IntegerObjectHandle* offset = nullptr;
    ASSERT_EQ(IntegerObject_Create(&offset), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset, 500), VANILLAPDF_ERROR_SUCCESS);

    IntegerObjectHandle* length = nullptr;
    ASSERT_EQ(IntegerObject_Create(&length), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length, 250), VANILLAPDF_ERROR_SUCCESS);

    ByteRangeHandle* range = nullptr;
    ASSERT_EQ(ByteRange_CreateFromData(offset, length, &range), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(range, nullptr);

    // Verify values
    IntegerObjectHandle* retrieved_offset = nullptr;
    ASSERT_EQ(ByteRange_GetOffset(range, &retrieved_offset), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 500);

    IntegerObjectHandle* retrieved_length = nullptr;
    ASSERT_EQ(ByteRange_GetLength(range, &retrieved_length), VANILLAPDF_ERROR_SUCCESS);
    bigint_type length_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_length, &length_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(length_value, 250);

    // Cleanup
    IntegerObject_Release(retrieved_offset);
    IntegerObject_Release(retrieved_length);
    ByteRange_Release(range);
}

TEST(ByteRangeCollection, CreateEmptyAndAppend) {
    ByteRangeCollectionHandle* collection = nullptr;
    ASSERT_EQ(ByteRangeCollection_Create(&collection), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(collection, nullptr);

    // Verify initially empty
    size_type size = 999;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 0);

    // Create and append a range
    IntegerObjectHandle* offset = nullptr;
    ASSERT_EQ(IntegerObject_Create(&offset), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset, 0), VANILLAPDF_ERROR_SUCCESS);

    IntegerObjectHandle* length = nullptr;
    ASSERT_EQ(IntegerObject_Create(&length), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length, 1000), VANILLAPDF_ERROR_SUCCESS);

    ByteRangeHandle* range = nullptr;
    ASSERT_EQ(ByteRange_CreateFromData(offset, length, &range), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);

    // Verify size is now 1
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 1);

    // Cleanup
    ByteRange_Release(range);
    ByteRangeCollection_Release(collection);
}

TEST(ByteRangeCollection, AppendMultipleAndRetrieve) {
    ByteRangeCollectionHandle* collection = nullptr;
    ASSERT_EQ(ByteRangeCollection_Create(&collection), VANILLAPDF_ERROR_SUCCESS);

    // Append 3 ranges
    for (int i = 0; i < 3; i++) {
        IntegerObjectHandle* offset = nullptr;
        ASSERT_EQ(IntegerObject_Create(&offset), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 1000), VANILLAPDF_ERROR_SUCCESS);

        IntegerObjectHandle* length = nullptr;
        ASSERT_EQ(IntegerObject_Create(&length), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, (i + 1) * 100), VANILLAPDF_ERROR_SUCCESS);

        ByteRangeHandle* range = nullptr;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, &range), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
        ByteRange_Release(range);
    }

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 3);

    // Retrieve and verify each range - this tests the indexing bug fix!
    for (size_type i = 0; i < 3; i++) {
        ByteRangeHandle* range = nullptr;
        ASSERT_EQ(ByteRangeCollection_GetValue(collection, i, &range), VANILLAPDF_ERROR_SUCCESS);

        IntegerObjectHandle* offset = nullptr;
        ASSERT_EQ(ByteRange_GetOffset(range, &offset), VANILLAPDF_ERROR_SUCCESS);
        bigint_type offset_value = 0;
        ASSERT_EQ(IntegerObject_GetIntegerValue(offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
        EXPECT_EQ(offset_value, i * 1000) << "ByteRange[" << i << "] offset mismatch";

        IntegerObjectHandle* length = nullptr;
        ASSERT_EQ(ByteRange_GetLength(range, &length), VANILLAPDF_ERROR_SUCCESS);
        bigint_type length_value = 0;
        ASSERT_EQ(IntegerObject_GetIntegerValue(length, &length_value), VANILLAPDF_ERROR_SUCCESS);
        EXPECT_EQ(length_value, (i + 1) * 100) << "ByteRange[" << i << "] length mismatch";

        IntegerObject_Release(offset);
        IntegerObject_Release(length);
        ByteRange_Release(range);
    }

    // Cleanup
    ByteRangeCollection_Release(collection);
}

TEST(ByteRangeCollection, InsertAtBeginning) {
    ByteRangeCollectionHandle* collection = nullptr;
    ASSERT_EQ(ByteRangeCollection_Create(&collection), VANILLAPDF_ERROR_SUCCESS);

    // Append first range
    IntegerObjectHandle* offset1 = nullptr;
    ASSERT_EQ(IntegerObject_Create(&offset1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset1, 1000), VANILLAPDF_ERROR_SUCCESS);
    IntegerObjectHandle* length1 = nullptr;
    ASSERT_EQ(IntegerObject_Create(&length1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length1, 100), VANILLAPDF_ERROR_SUCCESS);
    ByteRangeHandle* range1 = nullptr;
    ASSERT_EQ(ByteRange_CreateFromData(offset1, length1, &range1), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRangeCollection_Append(collection, range1), VANILLAPDF_ERROR_SUCCESS);
    ByteRange_Release(range1);

    // Insert at beginning
    IntegerObjectHandle* offset2 = nullptr;
    ASSERT_EQ(IntegerObject_Create(&offset2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset2, 0), VANILLAPDF_ERROR_SUCCESS);
    IntegerObjectHandle* length2 = nullptr;
    ASSERT_EQ(IntegerObject_Create(&length2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length2, 500), VANILLAPDF_ERROR_SUCCESS);
    ByteRangeHandle* range2 = nullptr;
    ASSERT_EQ(ByteRange_CreateFromData(offset2, length2, &range2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRangeCollection_Insert(collection, 0, range2), VANILLAPDF_ERROR_SUCCESS);
    ByteRange_Release(range2);

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 2);

    // Verify first element is the inserted one
    ByteRangeHandle* retrieved_range = nullptr;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 0, &retrieved_range), VANILLAPDF_ERROR_SUCCESS);
    IntegerObjectHandle* retrieved_offset = nullptr;
    ASSERT_EQ(ByteRange_GetOffset(retrieved_range, &retrieved_offset), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 0);

    // Cleanup
    IntegerObject_Release(retrieved_offset);
    ByteRange_Release(retrieved_range);
    ByteRangeCollection_Release(collection);
}

TEST(ByteRangeCollection, InsertInMiddle) {
    ByteRangeCollectionHandle* collection = nullptr;
    ASSERT_EQ(ByteRangeCollection_Create(&collection), VANILLAPDF_ERROR_SUCCESS);

    // Append two ranges
    for (int i = 0; i < 2; i++) {
        IntegerObjectHandle* offset = nullptr;
        ASSERT_EQ(IntegerObject_Create(&offset), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 200), VANILLAPDF_ERROR_SUCCESS);
        IntegerObjectHandle* length = nullptr;
        ASSERT_EQ(IntegerObject_Create(&length), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, 100), VANILLAPDF_ERROR_SUCCESS);
        ByteRangeHandle* range = nullptr;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, &range), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
        ByteRange_Release(range);
    }

    // Insert in middle
    IntegerObjectHandle* offset_mid = nullptr;
    ASSERT_EQ(IntegerObject_Create(&offset_mid), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset_mid, 100), VANILLAPDF_ERROR_SUCCESS);
    IntegerObjectHandle* length_mid = nullptr;
    ASSERT_EQ(IntegerObject_Create(&length_mid), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length_mid, 100), VANILLAPDF_ERROR_SUCCESS);
    ByteRangeHandle* range_mid = nullptr;
    ASSERT_EQ(ByteRange_CreateFromData(offset_mid, length_mid, &range_mid), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRangeCollection_Insert(collection, 1, range_mid), VANILLAPDF_ERROR_SUCCESS);
    ByteRange_Release(range_mid);

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 3);

    // Verify middle element
    ByteRangeHandle* retrieved_range = nullptr;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 1, &retrieved_range), VANILLAPDF_ERROR_SUCCESS);
    IntegerObjectHandle* retrieved_offset = nullptr;
    ASSERT_EQ(ByteRange_GetOffset(retrieved_range, &retrieved_offset), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 100);

    // Cleanup
    IntegerObject_Release(retrieved_offset);
    ByteRange_Release(retrieved_range);
    ByteRangeCollection_Release(collection);
}

TEST(ByteRangeCollection, RemoveFromCollection) {
    ByteRangeCollectionHandle* collection = nullptr;
    ASSERT_EQ(ByteRangeCollection_Create(&collection), VANILLAPDF_ERROR_SUCCESS);

    // Append 3 ranges
    for (int i = 0; i < 3; i++) {
        IntegerObjectHandle* offset = nullptr;
        ASSERT_EQ(IntegerObject_Create(&offset), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 100), VANILLAPDF_ERROR_SUCCESS);
        IntegerObjectHandle* length = nullptr;
        ASSERT_EQ(IntegerObject_Create(&length), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, 50), VANILLAPDF_ERROR_SUCCESS);
        ByteRangeHandle* range = nullptr;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, &range), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
        ByteRange_Release(range);
    }

    // Remove middle element
    ASSERT_EQ(ByteRangeCollection_Remove(collection, 1), VANILLAPDF_ERROR_SUCCESS);

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 2);

    // Verify first element unchanged (offset=0)
    ByteRangeHandle* range0 = nullptr;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 0, &range0), VANILLAPDF_ERROR_SUCCESS);
    IntegerObjectHandle* offset0 = nullptr;
    ASSERT_EQ(ByteRange_GetOffset(range0, &offset0), VANILLAPDF_ERROR_SUCCESS);
    bigint_type value0 = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(offset0, &value0), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(value0, 0);
    IntegerObject_Release(offset0);
    ByteRange_Release(range0);

    // Verify second element is now the third (offset=200)
    ByteRangeHandle* range1 = nullptr;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 1, &range1), VANILLAPDF_ERROR_SUCCESS);
    IntegerObjectHandle* offset1 = nullptr;
    ASSERT_EQ(ByteRange_GetOffset(range1, &offset1), VANILLAPDF_ERROR_SUCCESS);
    bigint_type value1 = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(offset1, &value1), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(value1, 200);
    IntegerObject_Release(offset1);
    ByteRange_Release(range1);

    // Cleanup
    ByteRangeCollection_Release(collection);
}

TEST(ByteRangeCollection, ClearCollection) {
    ByteRangeCollectionHandle* collection = nullptr;
    ASSERT_EQ(ByteRangeCollection_Create(&collection), VANILLAPDF_ERROR_SUCCESS);

    // Append several ranges
    for (int i = 0; i < 5; i++) {
        IntegerObjectHandle* offset = nullptr;
        ASSERT_EQ(IntegerObject_Create(&offset), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 100), VANILLAPDF_ERROR_SUCCESS);
        IntegerObjectHandle* length = nullptr;
        ASSERT_EQ(IntegerObject_Create(&length), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, 50), VANILLAPDF_ERROR_SUCCESS);
        ByteRangeHandle* range = nullptr;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, &range), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
        ByteRange_Release(range);
    }

    // Verify size before clear
    size_type size_before = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size_before), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size_before, 5);

    // Clear
    ASSERT_EQ(ByteRangeCollection_Clear(collection), VANILLAPDF_ERROR_SUCCESS);

    // Verify size after clear
    size_type size_after = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size_after), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size_after, 0);

    // Cleanup
    ByteRangeCollection_Release(collection);
}

} // namespace digital_signature_objects
