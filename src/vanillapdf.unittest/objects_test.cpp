#include "unittest.h"
#include "handle_guard.h"

namespace objects {

TEST(LiteralStringObject, ParenthesesIncluded) {

    // In the previous algorithm implementation there was incorrect logic for handling unescaped parentheses.
    // Let's double down on this, that they should be part of the result.

    // NOTE:
    // Currently the wrapping parentheses are not part of the input, thus the result is the same in this case.

    const char TEST_DATA[] = "(test)";

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal_string_ptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(TEST_DATA, literal_string_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(literal_string_ptr, buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(buffer_size, strlen(TEST_DATA));

    for (uint32_t i = 0; i < buffer_size; ++i) {
        EXPECT_EQ(buffer_data[i], TEST_DATA[i]);
    }
}

TEST(LiteralStringObject, UnbalancedParentheses) {

    const char TEST_DATA[] = "())";

    BufferHandle* buffer_ptr = nullptr;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal_string_ptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(TEST_DATA, literal_string_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr.get(), nullptr);

    // Invalid content was provided and it should trigger and error
    EXPECT_NE(LiteralStringObject_GetValue(literal_string_ptr, &buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(buffer_ptr, nullptr);
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

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal_string_ptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("\\53", literal_string_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(literal_string_ptr, buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);

    ASSERT_EQ(buffer_size, 1);
    EXPECT_EQ(buffer_data[0], 053);
}

TEST(LiteralStringObject, EscapeSequences) {

    const char TEST_DATA[] = "\\n\\r\\t\\b\\f\\(\\)\\\\";

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal_string_ptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(TEST_DATA, literal_string_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(literal_string_ptr, buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);

    const char EXPECTED[] = { '\n', '\r', '\t', '\b', '\f', '(', ')', '\\' };

    ASSERT_EQ(buffer_size, sizeof(EXPECTED));
    for (uint32_t i = 0; i < buffer_size; ++i) {
        EXPECT_EQ(buffer_data[i], EXPECTED[i]);
    }
}

TEST(LiteralStringObject, BackslashEOL) {

    const char TEST_DATA[] = "ABC\\\nDEF";

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal_string_ptr;

    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(TEST_DATA, literal_string_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(literal_string_ptr.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_GetValue(literal_string_ptr, buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);

    const char EXPECTED[] = "ABCDEF";

    ASSERT_EQ(buffer_size, strlen(EXPECTED));
    for (uint32_t i = 0; i < buffer_size; ++i) {
        EXPECT_EQ(buffer_data[i], EXPECTED[i]);
    }
}

TEST(HexadecimalStringObject, GetValue) {

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> hexadecimal_string_ptr;

    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0027", hexadecimal_string_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(hexadecimal_string_ptr.get(), nullptr);

    ASSERT_EQ(HexadecimalStringObject_GetValue(hexadecimal_string_ptr, buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(buffer_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_data, nullptr);

    ASSERT_EQ(buffer_size, 2);
    EXPECT_EQ(buffer_data[0], 0x00);
    EXPECT_EQ(buffer_data[1], 0x27);
}

TEST(HexadecimalStringObject, SetValue) {

    const char NEW_VALUE[] = { 0x00, 0x27 };

    string_type buffer_data = nullptr;
    size_type buffer_size = 0;

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    HandleGuard<BufferHandle, Buffer_Release> buffer_check_ptr;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> hexadecimal_string_ptr;

    ASSERT_EQ(Buffer_Create(buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_SetData(buffer_ptr, NEW_VALUE, sizeof(NEW_VALUE)), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(HexadecimalStringObject_Create(hexadecimal_string_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HexadecimalStringObject_SetValue(hexadecimal_string_ptr, buffer_ptr), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HexadecimalStringObject_GetValue(hexadecimal_string_ptr, buffer_check_ptr.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_GetData(buffer_check_ptr, &buffer_data, &buffer_size), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(buffer_size, static_cast<decltype(buffer_size)>(2));
    EXPECT_EQ(buffer_data[0], 0x00);
    EXPECT_EQ(buffer_data[1], 0x27);
}

TEST(DictionaryObject, InsertOverwrite) {

    const char AUTHOR_NAME[] = "Vanilla.PDF Labs s.r.o.";
    const char NEW_AUTHOR_NAME[] = "Another Author";

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary_object;

    HandleGuard<ObjectHandle, Object_Release> author_base_object;
    HandleGuard<StringObjectHandle, StringObject_Release> author_string_object;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> author_literal_string_object;

    HandleGuard<ObjectHandle, Object_Release> new_author_base_object;
    HandleGuard<StringObjectHandle, StringObject_Release> new_author_string_object;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> new_author_literal_string_object;

    string_type check_string_data = NULL;
    size_type check_string_size = 0;

    ASSERT_EQ(DictionaryObject_Create(dictionary_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(AUTHOR_NAME, author_literal_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_literal_string_object.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_ToStringObject(author_literal_string_object, author_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_string_object.get(), nullptr);

    ASSERT_EQ(StringObject_ToObject(author_string_object, author_base_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_base_object.get(), nullptr);

    // Insert First, overwrite true
    ASSERT_EQ(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, author_base_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Create a new object with a different value
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(NEW_AUTHOR_NAME, new_author_literal_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(new_author_literal_string_object.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_ToStringObject(new_author_literal_string_object, new_author_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(new_author_string_object.get(), nullptr);

    ASSERT_EQ(StringObject_ToObject(new_author_string_object, new_author_base_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(new_author_base_object.get(), nullptr);

    // Insert Second, overwrite false, expect failure
    ASSERT_NE(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, new_author_base_object, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    // Verify dictionary still contains the original value
    {
        HandleGuard<ObjectHandle, Object_Release> check_base_object;
        HandleGuard<StringObjectHandle, StringObject_Release> check_string_object;
        HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> check_literal_string_object;
        HandleGuard<BufferHandle, Buffer_Release> check_string_buffer;

        ASSERT_EQ(DictionaryObject_Find(dictionary_object, NameConstant_Author, check_base_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_base_object.get(), nullptr);
        EXPECT_EQ(check_base_object.get(), author_base_object.get());

        ASSERT_EQ(StringObject_FromObject(check_base_object, check_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_string_object.get(), nullptr);

        ASSERT_EQ(LiteralStringObject_FromStringObject(check_string_object, check_literal_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_literal_string_object.get(), nullptr);

        ASSERT_EQ(LiteralStringObject_GetValue(check_literal_string_object, check_string_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_string_buffer.get(), nullptr);

        ASSERT_EQ(Buffer_GetData(check_string_buffer, &check_string_data, &check_string_size), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_string_data, nullptr);

        ASSERT_EQ(check_string_size, strlen(AUTHOR_NAME));
        for (uint32_t i = 0; i < check_string_size; ++i) {
            EXPECT_EQ(check_string_data[i], AUTHOR_NAME[i]);
        }
    }

    // Insert Second, overwrite true
    ASSERT_EQ(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, new_author_base_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Find the item in the dictionary and verify it was overwritten
    {
        HandleGuard<ObjectHandle, Object_Release> check_base_object;
        HandleGuard<StringObjectHandle, StringObject_Release> check_string_object;
        HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> check_literal_string_object;
        HandleGuard<BufferHandle, Buffer_Release> check_string_buffer;

        ASSERT_EQ(DictionaryObject_Find(dictionary_object, NameConstant_Author, check_base_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_base_object.get(), nullptr);
        EXPECT_EQ(check_base_object.get(), new_author_base_object.get());

        ASSERT_EQ(StringObject_FromObject(check_base_object, check_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_string_object.get(), nullptr);

        ASSERT_EQ(LiteralStringObject_FromStringObject(check_string_object, check_literal_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_literal_string_object.get(), nullptr);

        ASSERT_EQ(LiteralStringObject_GetValue(check_literal_string_object, check_string_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_string_buffer.get(), nullptr);

        ASSERT_EQ(Buffer_GetData(check_string_buffer, &check_string_data, &check_string_size), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(check_string_data, nullptr);

        ASSERT_EQ(check_string_size, strlen(NEW_AUTHOR_NAME));
        for (uint32_t i = 0; i < check_string_size; ++i) {
            EXPECT_EQ(check_string_data[i], NEW_AUTHOR_NAME[i]);
        }
    }
}

TEST(DictionaryObject, TryFind) {

    const char AUTHOR_NAME[] = "Vanilla.PDF Labs s.r.o.";

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary_object;

    HandleGuard<ObjectHandle, Object_Release> author_base_object;
    HandleGuard<StringObjectHandle, StringObject_Release> author_string_object;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> author_literal_string_object;

    boolean_type object_found = false;
    HandleGuard<ObjectHandle, Object_Release> found_object_reference;

    ASSERT_EQ(DictionaryObject_Create(dictionary_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(AUTHOR_NAME, author_literal_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_literal_string_object.get(), nullptr);

    ASSERT_EQ(LiteralStringObject_ToStringObject(author_literal_string_object, author_string_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_string_object.get(), nullptr);

    ASSERT_EQ(StringObject_ToObject(author_string_object, author_base_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(author_base_object.get(), nullptr);

    // Insert one element into the dictionary
    ASSERT_EQ(DictionaryObject_InsertConst(dictionary_object, NameConstant_Author, author_base_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // TryFind should return success with results being filled within output variables
    ASSERT_EQ(DictionaryObject_TryFind(dictionary_object, NameConstant_Author, &object_found, found_object_reference.out()), VANILLAPDF_ERROR_SUCCESS);

    // Entries are present in the dictionary
    EXPECT_EQ(object_found, true);
    EXPECT_EQ(found_object_reference.get(), author_base_object.get());
}

TEST(DictionaryObject, TryFindMissing) {

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary_object;

    boolean_type object_found = false;
    ObjectHandle* found_object_reference = nullptr;

    ASSERT_EQ(DictionaryObject_Create(dictionary_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object.get(), nullptr);

    // TryFind should return success, however the results should be empty
    ASSERT_EQ(DictionaryObject_TryFind(dictionary_object, NameConstant_Author, &object_found, &found_object_reference), VANILLAPDF_ERROR_SUCCESS);

    // Entries are not present in the dictionary
    EXPECT_EQ(object_found, false);
    EXPECT_EQ(found_object_reference, nullptr);
}

TEST(StreamObject, OnChangeEvent) {

    HandleGuard<StreamObjectHandle, StreamObject_Release> stream_object;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary_object;

    ASSERT_EQ(StreamObject_Create(stream_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(stream_object.get(), nullptr);

    ASSERT_EQ(DictionaryObject_Create(dictionary_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dictionary_object.get(), nullptr);

    ASSERT_EQ(StreamObject_SetHeader(stream_object, dictionary_object), VANILLAPDF_ERROR_SUCCESS);
}

TEST(RealObject, SerializationPrecision) {
    HandleGuard<RealObjectHandle, RealObject_Release> real_object;
    HandleGuard<ObjectHandle, Object_Release> base_object;
    HandleGuard<BufferHandle, Buffer_Release> object_pdf_buffer;

    string_type object_pdf_data = nullptr;
    size_type object_pdf_size = 0;

    const real_type REAL_VALUE_DATA = 752.43f;
    const integer_type REAL_VALUE_DATA_PRECISION = 2;
    const char REAL_VALUE_STRING_CHECK[] = "752.43";

    ASSERT_EQ(RealObject_CreateFromData(REAL_VALUE_DATA, REAL_VALUE_DATA_PRECISION, real_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(real_object.get(), nullptr);

    ASSERT_EQ(RealObject_SetValue(real_object, REAL_VALUE_DATA), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_ToObject(real_object, base_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_object.get(), nullptr);

    ASSERT_EQ(Object_ToPdf(base_object, object_pdf_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(object_pdf_buffer.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(object_pdf_buffer, &object_pdf_data, &object_pdf_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(object_pdf_data, nullptr);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(object_pdf_size, strlen(REAL_VALUE_STRING_CHECK));

    for (uint32_t i = 0; i < object_pdf_size; ++i) {
        EXPECT_EQ(object_pdf_data[i], REAL_VALUE_STRING_CHECK[i]);
    }
}

TEST(NameObject, CreateRelease) {
    HandleGuard<NameObjectHandle, NameObject_Release> name_ptr;

    ASSERT_EQ(NameObject_Create(name_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_ptr.get(), nullptr);
}

TEST(NameObject, NullCheck) {
    EXPECT_EQ(NameObject_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(NameObject_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

struct EncodedNameCase {
    std::string_view name;  // Test case name for display
    std::string_view encoded;
    std::string_view expected_decoded;
};

class NameObjectParamTest : public ::testing::TestWithParam<EncodedNameCase> {};

TEST_P(NameObjectParamTest, CreateFromEncodedString) {

    const auto& param = GetParam();

    HandleGuard<NameObjectHandle, NameObject_Release> name_ptr;
    HandleGuard<BufferHandle, Buffer_Release> decoded_buffer_ptr;

    string_type decoded_buffer_data = nullptr;
    size_type decoded_buffer_size = 0;

    ASSERT_EQ(NameObject_CreateFromEncodedString(param.encoded.data(), name_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_ptr.get(), nullptr);

    ASSERT_EQ(NameObject_GetValue(name_ptr, decoded_buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(decoded_buffer_ptr.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(decoded_buffer_ptr, &decoded_buffer_data, &decoded_buffer_size), VANILLAPDF_ERROR_SUCCESS);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(decoded_buffer_size, param.expected_decoded.size());

    for (uint32_t i = 0; i < decoded_buffer_size; ++i) {
        EXPECT_EQ(decoded_buffer_data[i], param.expected_decoded[i]);
    }
}

INSTANTIATE_TEST_SUITE_P(
    EncodedNames,
    NameObjectParamTest,
    ::testing::Values(
        EncodedNameCase{ "Empty", "", "" },
        EncodedNameCase{ "Simple", "Hello", "Hello" },
        EncodedNameCase{ "HexSequence", "#01#02#03#FA#FB#FC#FD#FE#FF", "\x01\x02\x03\xFA\xFB\xFC\xFD\xFE\xFF" },
        EncodedNameCase{ "Space", "#20", " " },
        EncodedNameCase{ "ABC", "#41#42#43", "ABC" },
        EncodedNameCase{ "NullAndFF", "#00#FF", std::string_view("\x00\xFF", 2) },
        EncodedNameCase{ "MixedEncoding", "Test#20Name", "Test Name" },
        EncodedNameCase{ "EscapedHashes", "Name#23With#23Hashes", "Name#With#Hashes" }
    ),
    [](const ::testing::TestParamInfo<EncodedNameCase>& info) {
        return std::string(info.param.name);
    }
);

TEST(NameObject, ToPdfEncoding) {

    const char DECODED[] = "A#B C\nD";
    const char EXPECTED_PDF[] = "/A#23B#20C#0AD";

    HandleGuard<BufferHandle, Buffer_Release> pdf_buffer;
    HandleGuard<NameObjectHandle, NameObject_Release> name_ptr;
    HandleGuard<ObjectHandle, Object_Release> base_object;

    string_type pdf_data = nullptr;
    size_type pdf_size = 0;

    ASSERT_EQ(NameObject_CreateFromDecodedString(DECODED, name_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(name_ptr.get(), nullptr);

    ASSERT_EQ(NameObject_ToObject(name_ptr, base_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_object.get(), nullptr);

    ASSERT_EQ(Object_ToPdf(base_object, pdf_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pdf_buffer.get(), nullptr);

    ASSERT_EQ(Buffer_GetData(pdf_buffer, &pdf_data, &pdf_size), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pdf_data, nullptr);

    ASSERT_EQ(pdf_size, strlen(EXPECTED_PDF));
    for (uint32_t i = 0; i < pdf_size; ++i) {
        EXPECT_EQ(pdf_data[i], EXPECTED_PDF[i]);
    }
}

TEST(NameObject, InvalidEncodedInput) {

    NameObjectHandle* name_ptr = nullptr;

    EXPECT_NE(NameObject_CreateFromEncodedString("Invalid Name", &name_ptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(name_ptr, nullptr);

    EXPECT_NE(NameObject_CreateFromEncodedString("#0", &name_ptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(name_ptr, nullptr);
}

TEST(NameObject, Equals) {

    HandleGuard<NameObjectHandle, NameObject_Release> first_ptr;
    HandleGuard<NameObjectHandle, NameObject_Release> second_ptr;
    boolean_type are_equal = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(NameObject_CreateFromDecodedString("Name", first_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Name", second_ptr.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(NameObject_Equals(first_ptr, second_ptr, &are_equal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(are_equal, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(NameObject_CreateFromDecodedString("Other", second_ptr.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(NameObject_Equals(first_ptr, second_ptr, &are_equal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(are_equal, VANILLAPDF_RV_FALSE);
}

// ---- Issue #301: Hash stale after child mutation ----

TEST(DictionaryObject, HashUpdatesAfterChildMutation) {

    // Regression test for issue #301:
    // After a child integer value is mutated in-place the dictionary hash must change.

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> int_obj;
    HandleGuard<ObjectHandle, Object_Release> int_base_obj;
    HandleGuard<ObjectHandle, Object_Release> dict_base_obj;

    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_CreateFromIntegerValue(42, int_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(int_obj, int_base_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_InsertConst(dict, NameConstant_Type, int_base_obj, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_ToObject(dict, dict_base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type hash_before = 0;
    ASSERT_EQ(Object_Hash(dict_base_obj, &hash_before), VANILLAPDF_ERROR_SUCCESS);

    // Mutate the child value without touching the dictionary structure
    ASSERT_EQ(IntegerObject_SetIntegerValue(int_obj, 99), VANILLAPDF_ERROR_SUCCESS);

    size_type hash_after = 0;
    ASSERT_EQ(Object_Hash(dict_base_obj, &hash_after), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_NE(hash_before, hash_after);
}

TEST(ArrayObject, HashUpdatesAfterChildMutation) {

    // Regression test for issue #301:
    // After a child integer value is mutated in-place the array hash must change.

    HandleGuard<ArrayObjectHandle, ArrayObject_Release> arr;
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> int_obj;
    HandleGuard<ObjectHandle, Object_Release> int_base_obj;
    HandleGuard<ObjectHandle, Object_Release> arr_base_obj;

    ASSERT_EQ(ArrayObject_Create(arr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_CreateFromIntegerValue(42, int_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_ToObject(int_obj, int_base_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(arr, int_base_obj), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(ArrayObject_ToObject(arr, arr_base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type hash_before = 0;
    ASSERT_EQ(Object_Hash(arr_base_obj, &hash_before), VANILLAPDF_ERROR_SUCCESS);

    // Mutate the child value without touching the array structure
    ASSERT_EQ(IntegerObject_SetIntegerValue(int_obj, 99), VANILLAPDF_ERROR_SUCCESS);

    size_type hash_after = 0;
    ASSERT_EQ(Object_Hash(arr_base_obj, &hash_after), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_NE(hash_before, hash_after);
}

TEST(IndirectReferenceObject, HashUpdatesAfterNumberChange) {

    // Regression test for issue #301:
    // After the referenced object number changes the hash must change.

    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> ref_obj;
    HandleGuard<ObjectHandle, Object_Release> ref_base_obj;

    ASSERT_EQ(IndirectReferenceObject_Create(ref_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IndirectReferenceObject_SetObjectNumber(ref_obj, 10), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IndirectReferenceObject_SetGenerationNumber(ref_obj, 0), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(IndirectReferenceObject_ToObject(ref_obj, ref_base_obj.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type hash_before = 0;
    ASSERT_EQ(Object_Hash(ref_base_obj, &hash_before), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(IndirectReferenceObject_SetObjectNumber(ref_obj, 20), VANILLAPDF_ERROR_SUCCESS);

    size_type hash_after = 0;
    ASSERT_EQ(Object_Hash(ref_base_obj, &hash_after), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_NE(hash_before, hash_after);
}

} /* objects */
