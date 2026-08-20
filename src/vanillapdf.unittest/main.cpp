#include "unittest.h"
#include "handle_guard.h"

TEST(BufferArray, Conversion) {
    HandleGuard<BufferArrayHandle, BufferArray_Release> buffer_array_handle;
    HandleGuard<IUnknownHandle, IUnknown_Release> unknown_buffer_array_handle;

    ASSERT_EQ(BufferArray_Create(buffer_array_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_array_handle.get(), nullptr);

    ASSERT_EQ(BufferArray_ToUnknown(buffer_array_handle, unknown_buffer_array_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown_buffer_array_handle.get(), nullptr);
}

TEST(BaseFontRange, CreateRelease) {

    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> font_range_ptr;

    ASSERT_EQ(BaseFontRange_Create(font_range_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(font_range_ptr.get(), nullptr);
}

TEST(BaseFontRange, NullCheck) {
    EXPECT_EQ(BaseFontRange_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(BaseFontRange_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

void CheckBaseFontRangeValue(BaseFontRangeHandle* font_range, const std::vector<char>& request_data_content, const std::vector<char>& response_data_content) {

    HandleGuard<BufferHandle, Buffer_Release> request_data;
    HandleGuard<BufferHandle, Buffer_Release> response_data;

    ASSERT_EQ(Buffer_Create(request_data.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_SetData(request_data, request_data_content.data(), request_data_content.size()), VANILLAPDF_ERROR_SUCCESS);

    string_type response_data_str = nullptr;
    size_type response_data_len = 0;

    ASSERT_EQ(BaseFontRange_GetMappedValue(font_range, request_data, response_data.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_GetData(response_data, &response_data_str, &response_data_len), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(response_data_len, response_data_content.size());

    for (uint32_t i = 0; i < response_data_content.size(); ++i) {
        EXPECT_EQ(response_data_str[i], response_data_content[i]);
    }
}

void SetupBaseFontRange(HandleGuard<BaseFontRangeHandle, BaseFontRange_Release>& range,
                        const char* low, const char* high, const char* dest) {
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> range_low_ptr;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> range_high_ptr;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> range_destination;

    ASSERT_EQ(BaseFontRange_Create(range.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString(low, range_low_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString(high, range_high_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString(dest, range_destination.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> range_destination_str;
    HandleGuard<ObjectHandle, Object_Release> range_destination_obj;

    ASSERT_EQ(HexadecimalStringObject_ToStringObject(range_destination, range_destination_str.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_ToObject(range_destination_str, range_destination_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(BaseFontRange_SetRangeLow(range, range_low_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(BaseFontRange_SetRangeHigh(range, range_high_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(BaseFontRange_SetDestination(range, range_destination_obj), VANILLAPDF_ERROR_SUCCESS);
}

TEST(BaseFontRange, IncrementMapping) {

    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> font_range_ptr;
    SetupBaseFontRange(font_range_ptr, "0001", "0002", "00FF");

    // 0001 -> 00FF
    // 0002 -> 0100
    CheckBaseFontRangeValue(font_range_ptr, std::vector<char> { '\x00', '\x01' }, std::vector<char> { '\x00', '\xFF' });
    CheckBaseFontRangeValue(font_range_ptr, std::vector<char> { '\x00', '\x02' }, std::vector<char> { '\x01', '\x00' });
}

TEST(BaseFontRange, ContainsInRange) {
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    SetupBaseFontRange(range, "0020", "007E", "0020");

    // Key in range
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0041", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type result = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(BaseFontRange_Contains(range, key_buf, &result), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(result, VANILLAPDF_RV_TRUE);
}

TEST(BaseFontRange, ContainsOutOfRange) {
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    SetupBaseFontRange(range, "0020", "007E", "0020");

    // Key out of range
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("00FF", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type result = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(BaseFontRange_Contains(range, key_buf, &result), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(result, VANILLAPDF_RV_FALSE);
}

TEST(BaseFontRange, ContainsBoundaryValues) {
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    SetupBaseFontRange(range, "0020", "007E", "0020");

    // Low boundary (inclusive)
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> low_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0020", low_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> low_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(low_hex, low_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type low_result = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(BaseFontRange_Contains(range, low_buf, &low_result), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(low_result, VANILLAPDF_RV_TRUE);

    // High boundary (inclusive)
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> high_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("007E", high_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> high_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(high_hex, high_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type high_result = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(BaseFontRange_Contains(range, high_buf, &high_result), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(high_result, VANILLAPDF_RV_TRUE);
}

TEST(BaseFontRange, ContainsDifferentByteWidth) {
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    SetupBaseFontRange(range, "0020", "007E", "0020");

    // 4-byte key against 2-byte range should not match
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("00000041", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type result = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(BaseFontRange_Contains(range, key_buf, &result), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(result, VANILLAPDF_RV_FALSE);
}

TEST(BaseFontRange, GetMappedValueOutOfRange) {
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    SetupBaseFontRange(range, "0020", "007E", "0020");

    // Key out of range should fail
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("00FF", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> result;
    EXPECT_NE(BaseFontRange_GetMappedValue(range, key_buf, result.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(BaseFontRange, GetMappedValue4Byte) {
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    SetupBaseFontRange(range, "00004E00", "00009FFF", "00004E00");

    // Identity mapping: 00006F22 -> 00006F22
    CheckBaseFontRangeValue(range, std::vector<char> { '\x00', '\x00', '\x6F', '\x22' }, std::vector<char> { '\x00', '\x00', '\x6F', '\x22' });
}

TEST(Rectangle, CreateRelease) {
    HandleGuard<RectangleHandle, Rectangle_Release> rectangle_ptr;

    ASSERT_EQ(Rectangle_Create(rectangle_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(rectangle_ptr.get(), nullptr);
}

TEST(Rectangle, NullCheck) {
    EXPECT_EQ(Rectangle_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Rectangle, GetSet) {

    const real_type CHECK_VALUE = 123456.789;

    real_type real_value = 0;
    HandleGuard<RectangleHandle, Rectangle_Release> rectangle_ptr;

    ASSERT_EQ(Rectangle_Create(rectangle_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(rectangle_ptr.get(), nullptr);

    ASSERT_EQ(Rectangle_SetLowerLeftXReal(rectangle_ptr, CHECK_VALUE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_GetLowerLeftXReal(rectangle_ptr, &real_value), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(real_value, CHECK_VALUE);
}

TEST(Rectangle, GetSetAllCoordinates) {

    real_type real_value = 0;
    HandleGuard<RectangleHandle, Rectangle_Release> rectangle_ptr;

    ASSERT_EQ(Rectangle_Create(rectangle_ptr.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Rectangle_SetLowerLeftXReal(rectangle_ptr, 10.5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftYReal(rectangle_ptr, 20.5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightXReal(rectangle_ptr, 30.5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightYReal(rectangle_ptr, 40.5), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Rectangle_GetLowerLeftXReal(rectangle_ptr, &real_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(real_value, 10.5);

    ASSERT_EQ(Rectangle_GetLowerLeftYReal(rectangle_ptr, &real_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(real_value, 20.5);

    ASSERT_EQ(Rectangle_GetUpperRightXReal(rectangle_ptr, &real_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(real_value, 30.5);

    ASSERT_EQ(Rectangle_GetUpperRightYReal(rectangle_ptr, &real_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(real_value, 40.5);
}

TEST(Rectangle, RealAccessorNullChecks) {

    real_type real_value = 0;
    HandleGuard<RectangleHandle, Rectangle_Release> rectangle_ptr;

    ASSERT_EQ(Rectangle_Create(rectangle_ptr.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(Rectangle_GetLowerLeftXReal(nullptr, &real_value), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_GetLowerLeftYReal(nullptr, &real_value), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_GetUpperRightXReal(nullptr, &real_value), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_GetUpperRightYReal(nullptr, &real_value), VANILLAPDF_ERROR_PARAMETER_VALUE);

    EXPECT_EQ(Rectangle_GetLowerLeftXReal(rectangle_ptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_GetLowerLeftYReal(rectangle_ptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_GetUpperRightXReal(rectangle_ptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_GetUpperRightYReal(rectangle_ptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    EXPECT_EQ(Rectangle_SetLowerLeftXReal(nullptr, 0), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_SetLowerLeftYReal(nullptr, 0), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_SetUpperRightXReal(nullptr, 0), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_SetUpperRightYReal(nullptr, 0), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(File, LoadEmptyError) {

    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> input_output_stream;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(input_output_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(input_output_stream.get(), nullptr);

    ASSERT_EQ(File_OpenStream(input_output_stream, "UNUSED", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(file.get(), nullptr);

    // Initialization should fail, however gracefully
    // This could cause the application to hang, so let's cover it in the test
    EXPECT_NE(File_Initialize(file), VANILLAPDF_ERROR_SUCCESS);
}

TEST(File, XrefAllocation) {

    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> input_output_stream;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(input_output_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(input_output_stream, "UNUSED", file.out()), VANILLAPDF_ERROR_SUCCESS);

    for (int i = 0; i < 1000; ++i) {
        HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> used_entry_handle;
        ASSERT_EQ(File_AllocateNewEntry(file, used_entry_handle.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(used_entry_handle.get(), nullptr);
    }
}

TEST(InputOutputStream, Flush) {

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> input_output_stream;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(input_output_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(input_output_stream.get(), nullptr);

    EXPECT_EQ(InputOutputStream_Flush(input_output_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(MemoryBufferOutputStream, CreateRelease) {
    HandleGuard<MemoryBufferOutputStreamHandle, MemoryBufferOutputStream_Release> stream_ptr;

    ASSERT_EQ(MemoryBufferOutputStream_Create(stream_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_ptr.get(), nullptr);
}

TEST(MemoryBufferOutputStream, NullCheck) {
    EXPECT_EQ(MemoryBufferOutputStream_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(MemoryBufferOutputStream_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(MemoryBufferOutputStream, WriteString) {
    HandleGuard<MemoryBufferOutputStreamHandle, MemoryBufferOutputStream_Release> stream_ptr;

    ASSERT_EQ(MemoryBufferOutputStream_Create(stream_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_ptr.get(), nullptr);

    ASSERT_EQ(MemoryBufferOutputStream_WriteString(stream_ptr, "M@x!mum h@x0r!ng"), VANILLAPDF_ERROR_SUCCESS);
}

TEST(MemoryBufferOutputStream, WriteBuffer) {

    const char TEST_DATA[] = "TEST_DATA\x01\x02\xFF";

    offset_type stream_position = 0;
    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<MemoryBufferOutputStreamHandle, MemoryBufferOutputStream_Release> stream_ptr;

    ASSERT_EQ(MemoryBufferOutputStream_Create(stream_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_CreateFromData(TEST_DATA, strlen(TEST_DATA), buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    EXPECT_EQ(MemoryBufferOutputStream_WriteBuffer(stream_ptr, buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(MemoryBufferOutputStream_GetOutputPosition(stream_ptr, &stream_position), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(stream_position, strlen(TEST_DATA));
}

TEST(MemoryBufferOutputStream, WriteBufferLength) {

    const char TEST_DATA[] = "TEST_DATA\x01\x02\xFF";

    offset_type stream_position = 0;
    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<MemoryBufferOutputStreamHandle, MemoryBufferOutputStream_Release> stream_ptr;

    ASSERT_EQ(MemoryBufferOutputStream_Create(stream_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_CreateFromData(TEST_DATA, strlen(TEST_DATA), buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    EXPECT_EQ(MemoryBufferOutputStream_WriteBufferRange(stream_ptr, buffer_ptr, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(MemoryBufferOutputStream_GetOutputPosition(stream_ptr, &stream_position), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(stream_position, 0);

    EXPECT_EQ(MemoryBufferOutputStream_WriteBufferRange(stream_ptr, buffer_ptr, 2), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(MemoryBufferOutputStream_GetOutputPosition(stream_ptr, &stream_position), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(stream_position, 2);

    EXPECT_EQ(MemoryBufferOutputStream_WriteBufferRange(stream_ptr, buffer_ptr, 1000), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(MemoryBufferOutputStream_GetOutputPosition(stream_ptr, &stream_position), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(stream_position, strlen(TEST_DATA) + 2);
}

TEST(MemoryBufferOutputStream, SetOutputPosition) {
    HandleGuard<MemoryBufferOutputStreamHandle, MemoryBufferOutputStream_Release> stream_ptr;

    ASSERT_EQ(MemoryBufferOutputStream_Create(stream_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_ptr.get(), nullptr);

    EXPECT_EQ(MemoryBufferOutputStream_SetOutputPosition(stream_ptr, 0), VANILLAPDF_ERROR_SUCCESS);
}

TEST(MemoryBufferOutputStream, GetOutputPosition) {

    const char TEST_DATA[] = "123ABC";

    offset_type stream_position = 0;
    HandleGuard<MemoryBufferOutputStreamHandle, MemoryBufferOutputStream_Release> stream_ptr;

    ASSERT_EQ(MemoryBufferOutputStream_Create(stream_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_ptr.get(), nullptr);

    ASSERT_EQ(MemoryBufferOutputStream_GetOutputPosition(stream_ptr, &stream_position), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(stream_position, 0);

    ASSERT_EQ(MemoryBufferOutputStream_WriteString(stream_ptr, TEST_DATA), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(MemoryBufferOutputStream_GetOutputPosition(stream_ptr, &stream_position), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(stream_position, strlen(TEST_DATA));
}

TEST(MemoryBufferOutputStream, Flush) {
    HandleGuard<MemoryBufferOutputStreamHandle, MemoryBufferOutputStream_Release> stream_ptr;

    ASSERT_EQ(MemoryBufferOutputStream_Create(stream_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_ptr.get(), nullptr);

    EXPECT_EQ(MemoryBufferOutputStream_Flush(stream_ptr), VANILLAPDF_ERROR_SUCCESS);
}

int main(int argc, char *argv[]) {

    TestEnvironment* test_environment = new TestEnvironment();

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(test_environment);
    return RUN_ALL_TESTS();
}
