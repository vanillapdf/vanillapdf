#include "unittest.h"
#include "handle_guard.h"

#include "vanillapdf/c_vanillapdf_api.h"

namespace byte_range_objects {

// Tests for ByteRange and ByteRangeCollection manipulation API

TEST(ByteRange, CreateEmptyAndSet) {
    HandleGuard<ByteRangeHandle, ByteRange_Release> range;
    ASSERT_EQ(ByteRange_Create(range.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(range.get(), nullptr);

    // Create offset and length
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
    ASSERT_EQ(IntegerObject_Create(offset.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset, 100), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
    ASSERT_EQ(IntegerObject_Create(length.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length, 200), VANILLAPDF_ERROR_SUCCESS);

    // Set values
    ASSERT_EQ(ByteRange_SetOffset(range, offset), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRange_SetLength(range, length), VANILLAPDF_ERROR_SUCCESS);

    // Verify values
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> retrieved_offset;
    ASSERT_EQ(ByteRange_GetOffset(range, retrieved_offset.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 100);

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> retrieved_length;
    ASSERT_EQ(ByteRange_GetLength(range, retrieved_length.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type length_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_length, &length_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(length_value, 200);
}

TEST(ByteRange, CreateFromData) {
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
    ASSERT_EQ(IntegerObject_Create(offset.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset, 500), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
    ASSERT_EQ(IntegerObject_Create(length.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length, 250), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ByteRangeHandle, ByteRange_Release> range;
    ASSERT_EQ(ByteRange_CreateFromData(offset, length, range.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(range.get(), nullptr);

    // Verify values
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> retrieved_offset;
    ASSERT_EQ(ByteRange_GetOffset(range, retrieved_offset.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 500);

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> retrieved_length;
    ASSERT_EQ(ByteRange_GetLength(range, retrieved_length.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type length_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_length, &length_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(length_value, 250);
}

TEST(ByteRangeCollection, CreateEmptyAndAppend) {
    HandleGuard<ByteRangeCollectionHandle, ByteRangeCollection_Release> collection;
    ASSERT_EQ(ByteRangeCollection_Create(collection.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(collection.get(), nullptr);

    // Verify initially empty
    size_type size = 999;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 0);

    // Create and append a range
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
    ASSERT_EQ(IntegerObject_Create(offset.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset, 0), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
    ASSERT_EQ(IntegerObject_Create(length.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length, 1000), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ByteRangeHandle, ByteRange_Release> range;
    ASSERT_EQ(ByteRange_CreateFromData(offset, length, range.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);

    // Verify size is now 1
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 1);
}

TEST(ByteRangeCollection, AppendMultipleAndRetrieve) {
    HandleGuard<ByteRangeCollectionHandle, ByteRangeCollection_Release> collection;
    ASSERT_EQ(ByteRangeCollection_Create(collection.out()), VANILLAPDF_ERROR_SUCCESS);

    // Append 3 ranges
    for (int i = 0; i < 3; i++) {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
        ASSERT_EQ(IntegerObject_Create(offset.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 1000), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
        ASSERT_EQ(IntegerObject_Create(length.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, (i + 1) * 100), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ByteRangeHandle, ByteRange_Release> range;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, range.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
    }

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 3);

    // Retrieve and verify each range - this tests the indexing bug fix!
    for (size_type i = 0; i < 3; i++) {
        HandleGuard<ByteRangeHandle, ByteRange_Release> range;
        ASSERT_EQ(ByteRangeCollection_GetValue(collection, i, range.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
        ASSERT_EQ(ByteRange_GetOffset(range, offset.out()), VANILLAPDF_ERROR_SUCCESS);
        bigint_type offset_value = 0;
        ASSERT_EQ(IntegerObject_GetIntegerValue(offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
        EXPECT_EQ(offset_value, i * 1000) << "ByteRange[" << i << "] offset mismatch";

        HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
        ASSERT_EQ(ByteRange_GetLength(range, length.out()), VANILLAPDF_ERROR_SUCCESS);
        bigint_type length_value = 0;
        ASSERT_EQ(IntegerObject_GetIntegerValue(length, &length_value), VANILLAPDF_ERROR_SUCCESS);
        EXPECT_EQ(length_value, (i + 1) * 100) << "ByteRange[" << i << "] length mismatch";
    }
}

TEST(ByteRangeCollection, InsertAtBeginning) {
    HandleGuard<ByteRangeCollectionHandle, ByteRangeCollection_Release> collection;
    ASSERT_EQ(ByteRangeCollection_Create(collection.out()), VANILLAPDF_ERROR_SUCCESS);

    // Append first range
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset1;
    ASSERT_EQ(IntegerObject_Create(offset1.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset1, 1000), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> length1;
    ASSERT_EQ(IntegerObject_Create(length1.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length1, 100), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<ByteRangeHandle, ByteRange_Release> range1;
    ASSERT_EQ(ByteRange_CreateFromData(offset1, length1, range1.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRangeCollection_Append(collection, range1), VANILLAPDF_ERROR_SUCCESS);

    // Insert at beginning
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset2;
    ASSERT_EQ(IntegerObject_Create(offset2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset2, 0), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> length2;
    ASSERT_EQ(IntegerObject_Create(length2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length2, 500), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<ByteRangeHandle, ByteRange_Release> range2;
    ASSERT_EQ(ByteRange_CreateFromData(offset2, length2, range2.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRangeCollection_Insert(collection, 0, range2), VANILLAPDF_ERROR_SUCCESS);

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 2);

    // Verify first element is the inserted one
    HandleGuard<ByteRangeHandle, ByteRange_Release> retrieved_range;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 0, retrieved_range.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> retrieved_offset;
    ASSERT_EQ(ByteRange_GetOffset(retrieved_range, retrieved_offset.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 0);
}

TEST(ByteRangeCollection, InsertInMiddle) {
    HandleGuard<ByteRangeCollectionHandle, ByteRangeCollection_Release> collection;
    ASSERT_EQ(ByteRangeCollection_Create(collection.out()), VANILLAPDF_ERROR_SUCCESS);

    // Append two ranges
    for (int i = 0; i < 2; i++) {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
        ASSERT_EQ(IntegerObject_Create(offset.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 200), VANILLAPDF_ERROR_SUCCESS);
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
        ASSERT_EQ(IntegerObject_Create(length.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, 100), VANILLAPDF_ERROR_SUCCESS);
        HandleGuard<ByteRangeHandle, ByteRange_Release> range;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, range.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
    }

    // Insert in middle
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset_mid;
    ASSERT_EQ(IntegerObject_Create(offset_mid.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(offset_mid, 100), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> length_mid;
    ASSERT_EQ(IntegerObject_Create(length_mid.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_SetIntegerValue(length_mid, 100), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<ByteRangeHandle, ByteRange_Release> range_mid;
    ASSERT_EQ(ByteRange_CreateFromData(offset_mid, length_mid, range_mid.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ByteRangeCollection_Insert(collection, 1, range_mid), VANILLAPDF_ERROR_SUCCESS);

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 3);

    // Verify middle element
    HandleGuard<ByteRangeHandle, ByteRange_Release> retrieved_range;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 1, retrieved_range.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> retrieved_offset;
    ASSERT_EQ(ByteRange_GetOffset(retrieved_range, retrieved_offset.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type offset_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(retrieved_offset, &offset_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(offset_value, 100);
}

TEST(ByteRangeCollection, RemoveFromCollection) {
    HandleGuard<ByteRangeCollectionHandle, ByteRangeCollection_Release> collection;
    ASSERT_EQ(ByteRangeCollection_Create(collection.out()), VANILLAPDF_ERROR_SUCCESS);

    // Append 3 ranges
    for (int i = 0; i < 3; i++) {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
        ASSERT_EQ(IntegerObject_Create(offset.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 100), VANILLAPDF_ERROR_SUCCESS);
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
        ASSERT_EQ(IntegerObject_Create(length.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, 50), VANILLAPDF_ERROR_SUCCESS);
        HandleGuard<ByteRangeHandle, ByteRange_Release> range;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, range.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
    }

    // Remove middle element
    ASSERT_EQ(ByteRangeCollection_Remove(collection, 1), VANILLAPDF_ERROR_SUCCESS);

    // Verify size
    size_type size = 0;
    ASSERT_EQ(ByteRangeCollection_GetSize(collection, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 2);

    // Verify first element unchanged (offset=0)
    HandleGuard<ByteRangeHandle, ByteRange_Release> range0;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 0, range0.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset0;
    ASSERT_EQ(ByteRange_GetOffset(range0, offset0.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type value0 = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(offset0, &value0), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(value0, 0);

    // Verify second element is now the third (offset=200)
    HandleGuard<ByteRangeHandle, ByteRange_Release> range1;
    ASSERT_EQ(ByteRangeCollection_GetValue(collection, 1, range1.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset1;
    ASSERT_EQ(ByteRange_GetOffset(range1, offset1.out()), VANILLAPDF_ERROR_SUCCESS);
    bigint_type value1 = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(offset1, &value1), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(value1, 200);
}

TEST(ByteRangeCollection, ClearCollection) {
    HandleGuard<ByteRangeCollectionHandle, ByteRangeCollection_Release> collection;
    ASSERT_EQ(ByteRangeCollection_Create(collection.out()), VANILLAPDF_ERROR_SUCCESS);

    // Append several ranges
    for (int i = 0; i < 5; i++) {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> offset;
        ASSERT_EQ(IntegerObject_Create(offset.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(offset, i * 100), VANILLAPDF_ERROR_SUCCESS);
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> length;
        ASSERT_EQ(IntegerObject_Create(length.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(length, 50), VANILLAPDF_ERROR_SUCCESS);
        HandleGuard<ByteRangeHandle, ByteRange_Release> range;
        ASSERT_EQ(ByteRange_CreateFromData(offset, length, range.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ByteRangeCollection_Append(collection, range), VANILLAPDF_ERROR_SUCCESS);
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
}

} // namespace byte_range_objects
