#include "unittest.h"

#include <gtest/gtest.h>

TEST(Buffer, CreateRelease) {

    BufferHandle* buffer_ptr = nullptr;

    ASSERT_EQ(Buffer_Create(&buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr, nullptr);

    ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Buffer, CreateFromDataRelease) {

    const char BUFFER_DATA[] = "BUFFER_DATA";

    BufferHandle* buffer_ptr = nullptr;
    string_type check_data_ptr = nullptr;
    size_type check_data_len = 0;

    // Create buffer with data
    ASSERT_EQ(Buffer_CreateFromData(BUFFER_DATA, sizeof(BUFFER_DATA), &buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr, nullptr);

    // Read data information from buffer
    ASSERT_EQ(Buffer_GetData(buffer_ptr, &check_data_ptr, &check_data_len), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_data_ptr, nullptr);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(check_data_len, sizeof(BUFFER_DATA));
    EXPECT_STREQ(check_data_ptr, BUFFER_DATA);

    // Release the actual buffer allocation
    ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Buffer, NullCheck) {
    EXPECT_EQ(Buffer_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Buffer_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
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

TEST(LiteralStringObject, ParenthesesIncluded) {

    // In the previous algorithm implementation there was incorrect logic for handling unescaped parentheses.
    // Let's double down on this, that they should be part of the result.

    // NOTE:
    // Currently the wrapping parentheses are not part of the input, thus the result is the same in this case.

    const char TEST_DATA[] = "(test)";

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    BufferHandle* buffer_ptr = nullptr;
    LiteralStringObjectHandle* literal_string_ptr = nullptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(TEST_DATA, &literal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr, nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(literal_string_ptr, &buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr, nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(buffer_size, strlen(TEST_DATA));

    for (uint32_t i = 0; i < buffer_size; ++i) {
        EXPECT_EQ(buffer_data[i], TEST_DATA[i]);
    }

    ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(literal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(LiteralStringObject, UnbalancedParentheses) {

    const char TEST_DATA[] = "())";

    BufferHandle* buffer_ptr = nullptr;
    LiteralStringObjectHandle* literal_string_ptr = nullptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(TEST_DATA, &literal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr, nullptr);

    // Invalid content was provided and it should trigger and error
    EXPECT_NE(LiteralStringObject_GetValue(literal_string_ptr, &buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(buffer_ptr, nullptr);

    ASSERT_EQ(LiteralStringObject_Release(literal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(LiteralStringObject, OctalTwoDigitCompare) {

    // There was a bug in the application, which was expecting octal format to always include 3 digits.
    // To my understanding ddd means no 3 digits, no questions, however I might be wrong sometimes.

    // Quote from specs:
    // EXAMPLE 5 the literal
    // (\0053)
    // denotes a string containing two characters, \005 (Control - E) followed by the digit 3, whereas both
    // (\053)
    // and
    // (\53)
    // denote strings containing the single character \053, a plus sign(+)

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    BufferHandle* buffer_ptr = nullptr;
    LiteralStringObjectHandle* literal_string_ptr = nullptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("\\53", &literal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr, nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(literal_string_ptr, &buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr, nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);
    
    ASSERT_EQ(buffer_size, 1);
    EXPECT_EQ(buffer_data[0], 053);

    ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(literal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(HexadecimalStringObject, GetValue) {

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    BufferHandle* buffer_ptr = nullptr;
    HexadecimalStringObjectHandle* hexadecimal_string_ptr = nullptr;

    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0027", &hexadecimal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(hexadecimal_string_ptr, nullptr);

    ASSERT_EQ(HexadecimalStringObject_GetValue(hexadecimal_string_ptr, &buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr, nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);

    ASSERT_EQ(buffer_size, 2);
    EXPECT_EQ(buffer_data[0], 0x00);
    EXPECT_EQ(buffer_data[1], 0x27);

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
    EXPECT_EQ(buffer_data[0], 0x00);
    EXPECT_EQ(buffer_data[1], 0x27);

    ASSERT_EQ(Buffer_Release(buffer_check_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HexadecimalStringObject_Release(hexadecimal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(BaseFontRange, CreateRelease) {

    BaseFontRangeHandle* font_range_ptr = nullptr;

    ASSERT_EQ(BaseFontRange_Create(&font_range_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(font_range_ptr, nullptr);

    ASSERT_EQ(BaseFontRange_Release(font_range_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(BaseFontRange, NullCheck) {
    EXPECT_EQ(BaseFontRange_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(BaseFontRange_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
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
        EXPECT_EQ(response_data_str[i], response_data_content[i]);
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
    EXPECT_EQ(Rectangle_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Rectangle_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Rectangle, GetSet) {

    const bigint_type CHECK_VALUE = 123456;

    bigint_type int_value = 0;
    RectangleHandle* rectangle_ptr = nullptr;

    ASSERT_EQ(Rectangle_Create(&rectangle_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(rectangle_ptr, nullptr);

    ASSERT_EQ(Rectangle_SetLowerLeftX(rectangle_ptr, CHECK_VALUE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_GetLowerLeftX(rectangle_ptr, &int_value), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(int_value, CHECK_VALUE);

    ASSERT_EQ(Rectangle_Release(rectangle_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(File, LoadEmptyError) {

    FileHandle* file = NULL;
    InputOutputStreamHandle* input_output_stream = NULL;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(&input_output_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(input_output_stream, nullptr);

    ASSERT_EQ(File_OpenStream(input_output_stream, "UNUSED", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(file, nullptr);

    // Initialization should fail, however gracefully
    // This could cause the application to hang, so let's cover it in the test
    EXPECT_NE(File_Initialize(file), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(InputOutputStream_Release(input_output_stream), VANILLAPDF_ERROR_SUCCESS);
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

    EXPECT_EQ(InputOutputStream_Flush(input_output_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(input_output_stream), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DictionaryObject, InsertOverwrite) {

    const char AUTHOR_NAME[] = "Vanilla.PDF Labs s.r.o.";

    DictionaryObjectHandle* dictionary_object = NULL;

    ObjectHandle* author_base_object = NULL;
    StringObjectHandle* author_string_object = NULL;
    LiteralStringObjectHandle* author_literal_string_object = NULL;

    ObjectHandle* check_base_object = NULL;
    StringObjectHandle* check_string_object = NULL;
    LiteralStringObjectHandle* check_literal_string_object = NULL;

    BufferHandle* check_string_buffer = NULL;
    string_type check_string_data = NULL;
    size_type check_string_size = 0;

    ASSERT_EQ(DictionaryObject_Create(&dictionary_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object, nullptr);

    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(AUTHOR_NAME, &author_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_literal_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_ToStringObject(author_literal_string_object, &author_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_string_object, nullptr);

    ASSERT_EQ(StringObject_ToObject(author_string_object, &author_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_base_object, nullptr);

    // Insert First, overwrite true
    ASSERT_EQ(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, author_base_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Insert Second, overwrite false, expect failure
    ASSERT_NE(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, author_base_object, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    // Insert Second, overwrite true
    ASSERT_EQ(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, author_base_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Find the item in the dictionary
    ASSERT_EQ(DictionaryObject_Find(dictionary_object, NameConstant_Author, &check_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_base_object, nullptr);

    ASSERT_EQ(StringObject_FromObject(check_base_object, &check_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_FromStringObject(check_string_object, &check_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_literal_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(check_literal_string_object, &check_string_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_buffer, nullptr);

    ASSERT_EQ(Buffer_GetData(check_string_buffer, &check_string_data, &check_string_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_data, nullptr);

    // Verify the item was stored in the dictionary
    ASSERT_EQ(check_string_size, strlen(AUTHOR_NAME));

    for (uint32_t i = 0; i < check_string_size; ++i) {
        EXPECT_EQ(check_string_data[i], AUTHOR_NAME[i]);
    }

    // Release the check objects
    ASSERT_EQ(LiteralStringObject_Release(check_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(check_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(check_base_object), VANILLAPDF_ERROR_SUCCESS);

    // Release the original inserted objects
    ASSERT_EQ(Object_Release(author_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(author_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(author_literal_string_object), VANILLAPDF_ERROR_SUCCESS);

    // Release the container dictionary
    ASSERT_EQ(DictionaryObject_Release(dictionary_object), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DictionaryObject, TryFind) {

    const char AUTHOR_NAME[] = "Vanilla.PDF Labs s.r.o.";

    DictionaryObjectHandle* dictionary_object = nullptr;

    ObjectHandle* author_base_object = NULL;
    StringObjectHandle* author_string_object = NULL;
    LiteralStringObjectHandle* author_literal_string_object = NULL;

    boolean_type object_found = false;
    ObjectHandle* found_object_reference = nullptr;

    ASSERT_EQ(DictionaryObject_Create(&dictionary_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object, nullptr);

    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(AUTHOR_NAME, &author_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_literal_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_ToStringObject(author_literal_string_object, &author_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_string_object, nullptr);

    ASSERT_EQ(StringObject_ToObject(author_string_object, &author_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_base_object, nullptr);

    // Insert one element into the dictionary
    ASSERT_EQ(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, author_base_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // TryFind should return success with results being filled within output variables
    ASSERT_EQ(DictionaryObject_TryFind(dictionary_object, NameConstant_Author, &object_found, &found_object_reference), VANILLAPDF_ERROR_SUCCESS);

    // Entries are present in the dictionary
    EXPECT_EQ(object_found, true);
    EXPECT_EQ(found_object_reference, author_base_object);

    // Release the original inserted objects
    ASSERT_EQ(Object_Release(author_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(author_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(author_literal_string_object), VANILLAPDF_ERROR_SUCCESS);

    // Release the container dictionary
    ASSERT_EQ(DictionaryObject_Release(dictionary_object), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DictionaryObject, TryFindMissing) {

    DictionaryObjectHandle* dictionary_object = nullptr;

    boolean_type object_found = false;
    ObjectHandle* found_object_reference = nullptr;

    ASSERT_EQ(DictionaryObject_Create(&dictionary_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object, nullptr);

    // TryFind should return success, however the results should be empty
    ASSERT_EQ(DictionaryObject_TryFind(dictionary_object, NameConstant_Author, &object_found, &found_object_reference), VANILLAPDF_ERROR_SUCCESS);

    // Entries are not present in the dictionary
    EXPECT_EQ(object_found, false);
    EXPECT_EQ(found_object_reference, nullptr);

    // Release the container dictionary
    ASSERT_EQ(DictionaryObject_Release(dictionary_object), VANILLAPDF_ERROR_SUCCESS);
}

TEST(StreamObject, OnChangeEvent) {

    StreamObjectHandle* stream_object = NULL;
    DictionaryObjectHandle* dictionary_object = NULL;

    ASSERT_EQ(StreamObject_Create(&stream_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_object, nullptr);

    ASSERT_EQ(DictionaryObject_Create(&dictionary_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object, nullptr);

    ASSERT_EQ(StreamObject_SetHeader(stream_object, dictionary_object), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    ASSERT_EQ(DictionaryObject_Release(dictionary_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_Release(stream_object), VANILLAPDF_ERROR_SUCCESS);
}

TEST(RealObject, SerializationPrecision) {
    RealObjectHandle* real_object;
    ObjectHandle* base_object;
    BufferHandle* object_pdf_buffer;

    string_type object_pdf_data = nullptr;
    size_type object_pdf_size = 0;

    const real_type REAL_VALUE_DATA = 752.43f;
    const integer_type REAL_VALUE_DATA_PRECISION = 2;
    const char REAL_VALUE_STRING_CHECK[] = "752.43";

    ASSERT_EQ(RealObject_CreateFromData(REAL_VALUE_DATA, REAL_VALUE_DATA_PRECISION, &real_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(real_object, nullptr);

    ASSERT_EQ(RealObject_SetValue(real_object, REAL_VALUE_DATA), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_ToObject(real_object, &base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_object, nullptr);

    ASSERT_EQ(Object_ToPdf(base_object, &object_pdf_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(object_pdf_buffer, nullptr);

    ASSERT_EQ(Buffer_GetData(object_pdf_buffer, &object_pdf_data, &object_pdf_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(object_pdf_data, nullptr);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(object_pdf_size, strlen(REAL_VALUE_STRING_CHECK));

    for (uint32_t i = 0; i < object_pdf_size; ++i) {
        EXPECT_EQ(object_pdf_data[i], REAL_VALUE_STRING_CHECK[i]);
    }

    ASSERT_EQ(Buffer_Release(object_pdf_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(real_object), VANILLAPDF_ERROR_SUCCESS);
}

void SetCheckLoggingSeverity(LoggingSeverity desired_severity) {
    LoggingSeverity logging_severity = LoggingSeverity_Undefined;

    ASSERT_EQ(Logging_SetSeverity(desired_severity), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Logging_GetSeverity(&logging_severity), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(logging_severity, desired_severity);
}

TEST(Logging, Severity) {
    SetCheckLoggingSeverity(LoggingSeverity_Trace);
    SetCheckLoggingSeverity(LoggingSeverity_Debug);
    SetCheckLoggingSeverity(LoggingSeverity_Info);
    SetCheckLoggingSeverity(LoggingSeverity_Warning);
    SetCheckLoggingSeverity(LoggingSeverity_Error);
    SetCheckLoggingSeverity(LoggingSeverity_Critical);
    SetCheckLoggingSeverity(LoggingSeverity_Off);
}

namespace xref {

    TEST(Xref, CreateRelease) {
        XrefHandle* xref_ptr = nullptr;

        ASSERT_EQ(Xref_Create(&xref_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_ptr, nullptr);
        ASSERT_EQ(Xref_Release(xref_ptr), VANILLAPDF_ERROR_SUCCESS);
    }

    TEST(Xref, NullCheck) {
        EXPECT_EQ(Xref_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
        EXPECT_EQ(Xref_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    }

    TEST(Xref, AddRemoveEntry) {
        XrefHandle* xref_ptr = nullptr;

        XrefEntryHandle* xref_entry_ptr = nullptr;
        XrefFreeEntryHandle* xref_free_entry_ptr = nullptr;

        boolean_type remove_result = VANILLAPDF_RV_FALSE;
        boolean_type contains_result = VANILLAPDF_RV_FALSE;

        ASSERT_EQ(Xref_Create(&xref_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_ptr, nullptr);

        ASSERT_EQ(XrefFreeEntry_Create(1, 0, 65535, &xref_free_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_free_entry_ptr, nullptr);

        ASSERT_EQ(XrefFreeEntry_ToEntry(xref_free_entry_ptr, &xref_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_entry_ptr, nullptr);

        ASSERT_EQ(Xref_Insert(xref_ptr, xref_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Xref_Contains(xref_ptr, 1, &contains_result), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Xref_Remove(xref_ptr, 1, &remove_result), VANILLAPDF_ERROR_SUCCESS);

        EXPECT_EQ(contains_result, VANILLAPDF_RV_TRUE);
        EXPECT_EQ(remove_result, VANILLAPDF_RV_TRUE);

        ASSERT_EQ(XrefEntry_Release(xref_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefFreeEntry_Release(xref_free_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Xref_Release(xref_ptr), VANILLAPDF_ERROR_SUCCESS);
    }

    TEST(XrefFreeEntry, CreateRelease) {
        XrefFreeEntryHandle* xref_free_entry_ptr = nullptr;

        ASSERT_EQ(XrefFreeEntry_Create(0, 0, 0, &xref_free_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_free_entry_ptr, nullptr);
        ASSERT_EQ(XrefFreeEntry_Release(xref_free_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
    }

    TEST(XrefUsedEntry, CreateRelease) {
        XrefUsedEntryHandle* xref_used_entry_ptr = nullptr;

        ASSERT_EQ(XrefUsedEntry_Create(0, 0, 0, &xref_used_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_used_entry_ptr, nullptr);
        ASSERT_EQ(XrefUsedEntry_Release(xref_used_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
    }

    TEST(XrefCompressedEntry, CreateRelease) {
        XrefCompressedEntryHandle* xref_compressed_entry_ptr = nullptr;

        ASSERT_EQ(XrefCompressedEntry_Create(0, 0, 0, 0, &xref_compressed_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(xref_compressed_entry_ptr, nullptr);
        ASSERT_EQ(XrefCompressedEntry_Release(xref_compressed_entry_ptr), VANILLAPDF_ERROR_SUCCESS);
    }

} /* xref */

int main(int argc, char *argv[]) {

    TestEnvironment* test_environment = new TestEnvironment();

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(test_environment);
    return RUN_ALL_TESTS();
}
