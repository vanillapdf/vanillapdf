#include "unittest.h"

namespace objects {

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

TEST(LiteralStringObject, EscapeSequences) {

    const char TEST_DATA[] = "\\n\\r\\t\\b\\f\\(\\)\\\\";

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

    const char EXPECTED[] = { '\n', '\r', '\t', '\b', '\f', '(', ')', '\\' };

    ASSERT_EQ(buffer_size, sizeof(EXPECTED));
    for (uint32_t i = 0; i < buffer_size; ++i) {
        EXPECT_EQ(buffer_data[i], EXPECTED[i]);
    }

    ASSERT_EQ(Buffer_Release(buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(literal_string_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(LiteralStringObject, BackslashEOL) {

    const char TEST_DATA[] = "ABC\\\nDEF";

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

    const char EXPECTED[] = "ABCDEF";

    ASSERT_EQ(buffer_size, strlen(EXPECTED));
    for (uint32_t i = 0; i < buffer_size; ++i) {
        EXPECT_EQ(buffer_data[i], EXPECTED[i]);
    }

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

TEST(DictionaryObject, InsertOverwrite) {

    const char AUTHOR_NAME[] = "Vanilla.PDF Labs s.r.o.";
    const char NEW_AUTHOR_NAME[] = "Another Author";

    DictionaryObjectHandle* dictionary_object = NULL;

    ObjectHandle* author_base_object = NULL;
    StringObjectHandle* author_string_object = NULL;
    LiteralStringObjectHandle* author_literal_string_object = NULL;

    ObjectHandle* new_author_base_object = NULL;
    StringObjectHandle* new_author_string_object = NULL;
    LiteralStringObjectHandle* new_author_literal_string_object = NULL;

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

    // Create a new object with a different value
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(NEW_AUTHOR_NAME, &new_author_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(new_author_literal_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_ToStringObject(new_author_literal_string_object, &new_author_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(new_author_string_object, nullptr);

    ASSERT_EQ(StringObject_ToObject(new_author_string_object, &new_author_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(new_author_base_object, nullptr);

    // Insert Second, overwrite false, expect failure
    ASSERT_NE(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, new_author_base_object, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    // Verify dictionary still contains the original value
    ASSERT_EQ(DictionaryObject_Find(dictionary_object, NameConstant_Author, &check_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_base_object, nullptr);
    EXPECT_EQ(check_base_object, author_base_object);

    ASSERT_EQ(StringObject_FromObject(check_base_object, &check_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_FromStringObject(check_string_object, &check_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_literal_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(check_literal_string_object, &check_string_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_buffer, nullptr);

    ASSERT_EQ(Buffer_GetData(check_string_buffer, &check_string_data, &check_string_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_data, nullptr);

    ASSERT_EQ(check_string_size, strlen(AUTHOR_NAME));
    for (uint32_t i = 0; i < check_string_size; ++i) {
        EXPECT_EQ(check_string_data[i], AUTHOR_NAME[i]);
    }

    ASSERT_EQ(Buffer_Release(check_string_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(check_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(check_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(check_base_object), VANILLAPDF_ERROR_SUCCESS);

    // Insert Second, overwrite true
    ASSERT_EQ(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, new_author_base_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Find the item in the dictionary and verify it was overwritten
    ASSERT_EQ(DictionaryObject_Find(dictionary_object, NameConstant_Author, &check_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_base_object, nullptr);
    EXPECT_EQ(check_base_object, new_author_base_object);

    ASSERT_EQ(StringObject_FromObject(check_base_object, &check_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_FromStringObject(check_string_object, &check_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_literal_string_object, nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(check_literal_string_object, &check_string_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_buffer, nullptr);

    ASSERT_EQ(Buffer_GetData(check_string_buffer, &check_string_data, &check_string_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(check_string_data, nullptr);

    ASSERT_EQ(check_string_size, strlen(NEW_AUTHOR_NAME));
    for (uint32_t i = 0; i < check_string_size; ++i) {
        EXPECT_EQ(check_string_data[i], NEW_AUTHOR_NAME[i]);
    }

    // Release the check objects
    ASSERT_EQ(Buffer_Release(check_string_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(check_literal_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(check_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(check_base_object), VANILLAPDF_ERROR_SUCCESS);

    // Release the original inserted objects
    ASSERT_EQ(Object_Release(author_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(author_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(author_literal_string_object), VANILLAPDF_ERROR_SUCCESS);

    // Release the new inserted objects
    ASSERT_EQ(Object_Release(new_author_base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(new_author_string_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(new_author_literal_string_object), VANILLAPDF_ERROR_SUCCESS);

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

    // Release the found object reference
    ASSERT_EQ(Object_Release(found_object_reference), VANILLAPDF_ERROR_SUCCESS);

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

TEST(NameObject, CreateRelease) {
    NameObjectHandle* name_ptr = nullptr;

    ASSERT_EQ(NameObject_Create(&name_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_ptr, nullptr);
    ASSERT_EQ(NameObject_Release(name_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(NameObject, NullCheck) {
    EXPECT_EQ(NameObject_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(NameObject_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

struct EncodedNameCase {
    std::string_view encoded;
    std::string_view expected_decoded;
};

class NameObjectParamTest : public ::testing::TestWithParam<EncodedNameCase> {};

TEST_P(NameObjectParamTest, CreateFromEncodedString) {

    const auto& param = GetParam();

    NameObjectHandle* name_ptr = nullptr;
    BufferHandle* decoded_buffer_ptr = nullptr;

    string_type decoded_buffer_data = nullptr;
    size_type decoded_buffer_size = 0;

    ASSERT_EQ(NameObject_CreateFromEncodedString(param.encoded.data(), &name_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_ptr, nullptr);

    ASSERT_EQ(NameObject_GetValue(name_ptr, &decoded_buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(decoded_buffer_ptr, nullptr);

    ASSERT_EQ(Buffer_GetData(decoded_buffer_ptr, &decoded_buffer_data, &decoded_buffer_size), VANILLAPDF_ERROR_SUCCESS);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(decoded_buffer_size, param.expected_decoded.size());

    for (uint32_t i = 0; i < decoded_buffer_size; ++i) {
        EXPECT_EQ(decoded_buffer_data[i], param.expected_decoded[i]);
    }

    ASSERT_EQ(Buffer_Release(decoded_buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(name_ptr), VANILLAPDF_ERROR_SUCCESS);
}

INSTANTIATE_TEST_SUITE_P(
    /* empty */,
    NameObjectParamTest,
    ::testing::Values(
        EncodedNameCase{ "", "" },
        EncodedNameCase{ "Hello", "Hello" },
        EncodedNameCase{ "#01#02#03#FA#FB#FC#FD#FE#FF", "\x01\x02\x03\xFA\xFB\xFC\xFD\xFE\xFF" },
        EncodedNameCase{ "#20", " " },
        EncodedNameCase{ "#41#42#43", "ABC" },
        EncodedNameCase{ "#00#FF", std::string_view("\x00\xFF", 2) },
        EncodedNameCase{ "#41#42#43", "ABC" },
        EncodedNameCase{ "Test#20Name", "Test Name" },
        EncodedNameCase{ "Name#23With#23Hashes", "Name#With#Hashes" }
    )
);

TEST(NameObject, ToPdfEncoding) {

    const char DECODED[] = "A#B C\nD";
    const char EXPECTED_PDF[] = "/A#23B#20C#0AD";

    BufferHandle* pdf_buffer = nullptr;
    NameObjectHandle* name_ptr = nullptr;
    ObjectHandle* base_object = nullptr;

    string_type pdf_data = nullptr;
    size_type pdf_size = 0;

    ASSERT_EQ(NameObject_CreateFromDecodedString(DECODED, &name_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_ptr, nullptr);

    ASSERT_EQ(NameObject_ToObject(name_ptr, &base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_object, nullptr);

    ASSERT_EQ(Object_ToPdf(base_object, &pdf_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pdf_buffer, nullptr);

    ASSERT_EQ(Buffer_GetData(pdf_buffer, &pdf_data, &pdf_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pdf_data, nullptr);

    ASSERT_EQ(pdf_size, strlen(EXPECTED_PDF));
    for (uint32_t i = 0; i < pdf_size; ++i) {
        EXPECT_EQ(pdf_data[i], EXPECTED_PDF[i]);
    }

    ASSERT_EQ(Buffer_Release(pdf_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(base_object), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(name_ptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(NameObject, InvalidEncodedInput) {

    NameObjectHandle* name_ptr = nullptr;

    EXPECT_NE(NameObject_CreateFromEncodedString("Invalid Name", &name_ptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(name_ptr, nullptr);

    EXPECT_NE(NameObject_CreateFromEncodedString("#0", &name_ptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(name_ptr, nullptr);
}

TEST(NameObject, Equals) {

    NameObjectHandle* first_ptr = nullptr;
    NameObjectHandle* second_ptr = nullptr;
    boolean_type are_equal = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(NameObject_CreateFromDecodedString("Name", &first_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Name", &second_ptr), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(NameObject_Equals(first_ptr, second_ptr, &are_equal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(are_equal, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(NameObject_Release(second_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Other", &second_ptr), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(NameObject_Equals(first_ptr, second_ptr, &are_equal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(are_equal, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(NameObject_Release(first_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(second_ptr), VANILLAPDF_ERROR_SUCCESS);
}

} /* objects */
