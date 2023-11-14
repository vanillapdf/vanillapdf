#include "unittest.h"

#include <gtest/gtest.h>

TEST(Buffer, CreateRelease) {

	BufferHandle* buffer_ptr = nullptr;
	auto create_result = Buffer_Create(&buffer_ptr);

	ASSERT_EQ(create_result, VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(buffer_ptr, nullptr);

	auto release_result = Buffer_Release(buffer_ptr);

	ASSERT_EQ(release_result, VANILLAPDF_ERROR_SUCCESS);
}

TEST(Buffer, NullCheck) {
	ASSERT_EQ(Buffer_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
	ASSERT_EQ(Buffer_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Buffer, Conversion) {
	BufferHandle* buffer_handle = nullptr;
	IUnknownHandle* unknown_buffer_handle = nullptr;

	ASSERT_EQ(Buffer_Create(&buffer_handle), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(buffer_handle, nullptr);

	ASSERT_EQ(Buffer_ToUnknown(buffer_handle, &unknown_buffer_handle), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(unknown_buffer_handle, nullptr);

	ASSERT_EQ(IUnknown_Release(unknown_buffer_handle), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(Buffer_Release(buffer_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(BufferArray, Conversion) {
	BufferArrayHandle* buffer_array_handle = nullptr;
	IUnknownHandle* unknown_buffer_array_handle = nullptr;

	ASSERT_EQ(BufferArray_Create(&buffer_array_handle), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(buffer_array_handle, nullptr);

	ASSERT_EQ(BufferArray_ToUnknown(buffer_array_handle, &unknown_buffer_array_handle), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(unknown_buffer_array_handle, nullptr);

	ASSERT_EQ(IUnknown_Release(unknown_buffer_array_handle), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(BufferArray_Release(buffer_array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(HexadecimalStringObject, GetValue) {

	string_type buffer_data = nullptr;
	size_type buffer_size = 0;

	BufferHandle* buffer_ptr = nullptr;
	HexadecimalStringObjectHandle* hexadecimal_string_ptr = nullptr;

	ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0027", &hexadecimal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_GetValue(hexadecimal_string_ptr, &buffer_ptr), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(buffer_size, static_cast<decltype(buffer_size)>(2));
	ASSERT_EQ(buffer_data[0], 0x00);
	ASSERT_EQ(buffer_data[1], 0x27);

	ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_Release(hexadecimal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(HexadecimalStringObject, SetValue) {

	const char NEW_VALUE[] = { 0x00, 0x27 };

	string_type buffer_data = nullptr;
	size_type buffer_size = 0;

	BufferHandle* buffer_ptr = nullptr;
	BufferHandle* buffer_check_ptr = nullptr;
	HexadecimalStringObjectHandle* hexadecimal_string_ptr = nullptr;

	ASSERT_EQ(Buffer_Create(&buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(Buffer_SetData(buffer_ptr, NEW_VALUE, sizeof(NEW_VALUE)), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(HexadecimalStringObject_Create(&hexadecimal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_SetValue(hexadecimal_string_ptr, buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_GetValue(hexadecimal_string_ptr, &buffer_check_ptr), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(Buffer_GetData(buffer_check_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(buffer_size, static_cast<decltype(buffer_size)>(2));
	ASSERT_EQ(buffer_data[0], 0x00);
	ASSERT_EQ(buffer_data[1], 0x27);

	ASSERT_EQ(Buffer_Release(buffer_check_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_Release(hexadecimal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(BaseFontRange, CreateRelease) {

	BaseFontRangeHandle* font_range_ptr = nullptr;
	auto create_result = BaseFontRange_Create(&font_range_ptr);

	ASSERT_EQ(create_result, VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(font_range_ptr, nullptr);

	auto release_result = BaseFontRange_Release(font_range_ptr);

	ASSERT_EQ(release_result, VANILLAPDF_ERROR_SUCCESS);
}

TEST(BaseFontRange, NullCheck) {
	ASSERT_EQ(BaseFontRange_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
	ASSERT_EQ(BaseFontRange_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

void CheckBaseFontRangeValue(BaseFontRangeHandle* font_range, const std::vector<char>& request_data_content, const std::vector<char>& response_data_content) {

	BufferHandle* request_data = nullptr;
	BufferHandle* response_data = nullptr;

	ASSERT_EQ(Buffer_Create(&request_data), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(Buffer_SetData(request_data, request_data_content.data(), request_data_content.size()), VANILLAPDF_ERROR_SUCCESS);

	string_type response_data_str = nullptr;
	size_type response_data_len = 0;

	ASSERT_EQ(BaseFontRange_GetMappedValue(font_range, request_data, &response_data), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(Buffer_GetData(response_data, &response_data_str, &response_data_len), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(response_data_len, response_data_content.size());

	for (uint32_t i = 0; i < response_data_content.size(); ++i) {
		ASSERT_EQ(response_data_str[i], response_data_content[i]);
	}

	ASSERT_EQ(Buffer_Release(request_data), VANILLAPDF_ERROR_SUCCESS);
}

TEST(BaseFontRange, IncrementMapping) {

	BaseFontRangeHandle* font_range_ptr = nullptr;
	ASSERT_EQ(BaseFontRange_Create(&font_range_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(font_range_ptr, nullptr);

	HexadecimalStringObjectHandle* range_low_ptr = nullptr;
	HexadecimalStringObjectHandle* range_high_ptr = nullptr;
	HexadecimalStringObjectHandle* range_destination = nullptr;

	ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0001", &range_low_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0002", &range_high_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("00FF", &range_destination), VANILLAPDF_ERROR_SUCCESS);

	StringObjectHandle* range_destination_str = nullptr;
	ObjectHandle* range_destination_obj = nullptr;

	ASSERT_EQ(HexadecimalStringObject_ToStringObject(range_destination, &range_destination_str), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(StringObject_ToObject(range_destination_str, &range_destination_obj), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(BaseFontRange_SetRangeLow(font_range_ptr, range_low_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(BaseFontRange_SetRangeHigh(font_range_ptr, range_high_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(BaseFontRange_SetDestination(font_range_ptr, range_destination_obj), VANILLAPDF_ERROR_SUCCESS);

	// 0001 -> 00FF
	// 0002 -> 0100
	CheckBaseFontRangeValue(font_range_ptr, std::vector<char> { '\x00', '\x01' }, std::vector<char> { '\x00', '\xFF' });
	CheckBaseFontRangeValue(font_range_ptr, std::vector<char> { '\x00', '\x02' }, std::vector<char> { '\x01', '\x00' });

	ASSERT_EQ(Object_Release(range_destination_obj), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(StringObject_Release(range_destination_str), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(HexadecimalStringObject_Release(range_destination), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_Release(range_high_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(HexadecimalStringObject_Release(range_low_ptr), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(BaseFontRange_Release(font_range_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Rectangle, CreateRelease) {
	RectangleHandle* rectangle_ptr = nullptr;

	ASSERT_EQ(Rectangle_Create(&rectangle_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(rectangle_ptr, nullptr);
	ASSERT_EQ(Rectangle_Release(rectangle_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Rectangle, NullCheck) {
	ASSERT_EQ(Rectangle_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
	ASSERT_EQ(Rectangle_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Rectangle, GetSet) {

	const bigint_type CHECK_VALUE = 123456;

	bigint_type int_value = 0;
	RectangleHandle* rectangle_ptr = nullptr;

	ASSERT_EQ(Rectangle_Create(&rectangle_ptr), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(rectangle_ptr, nullptr);

	ASSERT_EQ(Rectangle_SetLowerLeftX(rectangle_ptr, CHECK_VALUE), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(Rectangle_GetLowerLeftX(rectangle_ptr, &int_value), VANILLAPDF_ERROR_SUCCESS);

	ASSERT_EQ(int_value, CHECK_VALUE);

	ASSERT_EQ(Rectangle_Release(rectangle_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(File, XrefAllocation) {

	FileHandle* file = NULL;
	XrefUsedEntryHandle* used_entry_handle = NULL;
	InputOutputStreamHandle* input_output_stream = NULL;

	ASSERT_EQ(InputOutputStream_CreateFromMemory(&input_output_stream), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(File_CreateStream(input_output_stream, "UNUSED", &file), VANILLAPDF_ERROR_SUCCESS);

	for (int i = 0; i < 1000; ++i) {
		ASSERT_EQ(File_AllocateNewEntry(file, &used_entry_handle), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(used_entry_handle, nullptr);
		ASSERT_EQ(XrefUsedEntry_Release(used_entry_handle), VANILLAPDF_ERROR_SUCCESS);
	}

	ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(InputOutputStream_Release(input_output_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(InputOutputStream, Flush) {

	InputOutputStreamHandle* input_output_stream = NULL;

	ASSERT_EQ(InputOutputStream_CreateFromMemory(&input_output_stream), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_NE(input_output_stream, nullptr);

	ASSERT_EQ(InputOutputStream_Flush(input_output_stream), VANILLAPDF_ERROR_SUCCESS);
	ASSERT_EQ(InputOutputStream_Release(input_output_stream), VANILLAPDF_ERROR_SUCCESS);
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
