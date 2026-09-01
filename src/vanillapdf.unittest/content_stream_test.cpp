#include "unittest.h"
#include "handle_guard.h"

// Exercises the standardized content object / operation cast C API.
//
// Begin/end text operations are default-constructible and can now be created
// directly, so their conversions are tested in isolation. An inline image
// requires a dictionary and data, so it is obtained by parsing a small,
// well-formed inline image content stream.

// A begin text (BT) operation round-trips through its base content operation.
TEST(ContentOperationBeginText, Conversion) {
    HandleGuard<ContentOperationBeginTextHandle, ContentOperationBeginText_Release> begin_text;
    ASSERT_EQ(ContentOperationBeginText_Create(begin_text.out()), VANILLAPDF_ERROR_SUCCESS);

    // derived -> base
    HandleGuard<ContentOperationHandle, ContentOperation_Release> operation;
    ASSERT_EQ(ContentOperationBeginText_ToContentOperation(begin_text, operation.out()), VANILLAPDF_ERROR_SUCCESS);

    ContentOperationType operation_type = ContentOperationType_Undefined;
    ASSERT_EQ(ContentOperation_GetOperationType(operation, &operation_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(operation_type, ContentOperationType_BeginText);

    // base -> derived
    HandleGuard<ContentOperationBeginTextHandle, ContentOperationBeginText_Release> begin_text_again;
    ASSERT_EQ(ContentOperationBeginText_FromContentOperation(operation, begin_text_again.out()), VANILLAPDF_ERROR_SUCCESS);
}

// An end text (ET) operation round-trips through its base content operation.
TEST(ContentOperationEndText, Conversion) {
    HandleGuard<ContentOperationEndTextHandle, ContentOperationEndText_Release> end_text;
    ASSERT_EQ(ContentOperationEndText_Create(end_text.out()), VANILLAPDF_ERROR_SUCCESS);

    // derived -> base
    HandleGuard<ContentOperationHandle, ContentOperation_Release> operation;
    ASSERT_EQ(ContentOperationEndText_ToContentOperation(end_text, operation.out()), VANILLAPDF_ERROR_SUCCESS);

    ContentOperationType operation_type = ContentOperationType_Undefined;
    ASSERT_EQ(ContentOperation_GetOperationType(operation, &operation_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(operation_type, ContentOperationType_EndText);

    // base -> derived
    HandleGuard<ContentOperationEndTextHandle, ContentOperationEndText_Release> end_text_again;
    ASSERT_EQ(ContentOperationEndText_FromContentOperation(operation, end_text_again.out()), VANILLAPDF_ERROR_SUCCESS);
}

namespace {

// Parse a content stream containing a single inline image
// and return its raw data and sample data as strings
void ParseInlineImageData(const std::string& content, std::string& image_data, std::string& image_samples) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> file;
    ASSERT_EQ(File_CreateStream(io, "content_stream_test", file.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buffer;
    ASSERT_EQ(Buffer_CreateFromData(content.data(), content.size(), buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputStreamHandle, InputStream_Release> stream;
    ASSERT_EQ(InputStream_CreateFromBuffer(buffer, stream.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentParserHandle, ContentParser_Release> parser;
    ASSERT_EQ(ContentParser_Create(file, stream, parser.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> instructions;
    ASSERT_EQ(ContentParser_ReadInstructionCollection(parser, instructions.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type count = 0;
    ASSERT_EQ(ContentInstructionCollection_GetSize(instructions, &count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(count, 1u);

    HandleGuard<ContentInstructionHandle, ContentInstruction_Release> instruction;
    ASSERT_EQ(ContentInstructionCollection_At(instructions, 0, instruction.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentObjectHandle, ContentObject_Release> object;
    ASSERT_EQ(ContentObject_FromInstruction(instruction, object.out()), VANILLAPDF_ERROR_SUCCESS);

    ContentObjectType object_type = ContentObjectType_Undefined;
    ASSERT_EQ(ContentObject_GetObjectType(object, &object_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(object_type, ContentObjectType_InlineImage);

    HandleGuard<ContentObjectInlineImageHandle, ContentObjectInlineImage_Release> image;
    ASSERT_EQ(ContentObjectInlineImage_FromContentObject(object, image.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> image_data_buffer;
    ASSERT_EQ(ContentObjectInlineImage_GetData(image, image_data_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type image_data_size = 0;
    string_type image_data_bytes = nullptr;
    ASSERT_EQ(Buffer_GetData(image_data_buffer, &image_data_bytes, &image_data_size), VANILLAPDF_ERROR_SUCCESS);

    image_data.assign(image_data_bytes, image_data_size);

    HandleGuard<BufferHandle, Buffer_Release> image_samples_buffer;
    ASSERT_EQ(ContentObjectInlineImage_GetSamples(image, image_samples_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type image_samples_size = 0;
    string_type image_samples_bytes = nullptr;
    ASSERT_EQ(Buffer_GetData(image_samples_buffer, &image_samples_bytes, &image_samples_size), VANILLAPDF_ERROR_SUCCESS);

    image_samples.assign(image_samples_bytes, image_samples_size);
}

} // namespace

// Parse a well-formed inline image ("BI ... ID ... EI") and round-trip the
// resulting content object through the inline image conversions.
TEST(ContentObjectInlineImage, Conversion) {
    const char content[] = "BI /W 1 /H 1 ID abc EI ";
    const char expected_image_data[] = "abc";

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> file;
    ASSERT_EQ(File_CreateStream(io, "content_stream_test", file.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buffer;
    ASSERT_EQ(Buffer_CreateFromData(content, sizeof(content) - 1, buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputStreamHandle, InputStream_Release> stream;
    ASSERT_EQ(InputStream_CreateFromBuffer(buffer, stream.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentParserHandle, ContentParser_Release> parser;
    ASSERT_EQ(ContentParser_Create(file, stream, parser.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> instructions;
    ASSERT_EQ(ContentParser_ReadInstructionCollection(parser, instructions.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type count = 0;
    ASSERT_EQ(ContentInstructionCollection_GetSize(instructions, &count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(count, 1u);

    HandleGuard<ContentInstructionHandle, ContentInstruction_Release> instruction;
    ASSERT_EQ(ContentInstructionCollection_At(instructions, 0, instruction.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentObjectHandle, ContentObject_Release> object;
    ASSERT_EQ(ContentObject_FromInstruction(instruction, object.out()), VANILLAPDF_ERROR_SUCCESS);

    ContentObjectType object_type = ContentObjectType_Undefined;
    ASSERT_EQ(ContentObject_GetObjectType(object, &object_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(object_type, ContentObjectType_InlineImage);

    // base -> derived
    HandleGuard<ContentObjectInlineImageHandle, ContentObjectInlineImage_Release> image;
    ASSERT_EQ(ContentObjectInlineImage_FromContentObject(object, image.out()), VANILLAPDF_ERROR_SUCCESS);

    // The image data must contain only the payload between "ID" and "EI",
    // without the operators themselves or the surrounding delimiters.
    HandleGuard<BufferHandle, Buffer_Release> image_data;
    ASSERT_EQ(ContentObjectInlineImage_GetData(image, image_data.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type image_data_size = 0;
    string_type image_data_bytes = nullptr;
    ASSERT_EQ(Buffer_GetData(image_data, &image_data_bytes, &image_data_size), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(image_data_size, strlen(expected_image_data));
    ASSERT_EQ(std::string(image_data_bytes, image_data_size), expected_image_data);

    // derived -> base
    HandleGuard<ContentObjectHandle, ContentObject_Release> object_again;
    ASSERT_EQ(ContentObjectInlineImage_ToContentObject(image, object_again.out()), VANILLAPDF_ERROR_SUCCESS);
}

// The image data boundaries are delimited by white-space characters,
// which shall not be included in the data themselves.
TEST(ContentObjectInlineImage, DataDelimiters) {
    std::string image_data;
    std::string image_samples;

    // "EI" without surrounding white-space is a part of the image data
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI ID aEIb EI ", image_data, image_samples));
    ASSERT_EQ(image_data, "aEIb");

    // White-space inside the image data is preserved
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI ID a E b EI ", image_data, image_samples));
    ASSERT_EQ(image_data, "a E b");

    // "EI" followed by a regular character is a part of the image data
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI ID abc EIx EI ", image_data, image_samples));
    ASSERT_EQ(image_data, "abc EIx");

    // The image data may be empty
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI ID EI ", image_data, image_samples));
    ASSERT_EQ(image_data, "");

    // The end of the stream terminates the image data as well
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI ID abc EI", image_data, image_samples));
    ASSERT_EQ(image_data, "abc");

    // Only the single delimiter after "ID" is consumed,
    // additional white-space belongs to the image data
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI ID  abc EI ", image_data, image_samples));
    ASSERT_EQ(image_data, " abc");

    // A missing delimiter after "ID" is tolerated with a warning,
    // the image data begin immediately after the operator
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI ID(a) EI ", image_data, image_samples));
    ASSERT_EQ(image_data, "(a)");
}

// The image samples have all the filters from the image dictionary applied,
// recognizing both the abbreviated and the full names.
TEST(ContentObjectInlineImage, Samples) {
    std::string image_data;
    std::string image_samples;

    // Abbreviated filter name
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI /W 2 /H 1 /BPC 8 /CS /G /F /AHx ID 6162> EI ", image_data, image_samples));
    ASSERT_EQ(image_data, "6162>");
    ASSERT_EQ(image_samples, "ab");

    // Full filter name
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI /Filter /ASCIIHexDecode ID 6162> EI ", image_data, image_samples));
    ASSERT_EQ(image_samples, "ab");

    // Array of filter names
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI /F [/AHx] ID 6162> EI ", image_data, image_samples));
    ASSERT_EQ(image_samples, "ab");

    // Without any filter the samples equal the raw data
    ASSERT_NO_FATAL_FAILURE(ParseInlineImageData("BI /W 1 /H 1 ID abc EI ", image_data, image_samples));
    ASSERT_EQ(image_data, "abc");
    ASSERT_EQ(image_samples, "abc");
}

// A content stream is untrusted input, so a malformed operator has to come back
// as an error. These used to be asserted as well as thrown, which aborted a
// debug build on a document a caller could legitimately hand us - parsing
// "1 2 3 Tf" tripped assert(operands.size() == 2) before the ParseException on
// the following line ever ran.
//
// Only the operators the parser builds a dedicated operation for are covered.
// Everything else (cm, q, Q, RG, rg, Td, ...) becomes an OperationGeneric,
// which does not check its operands at all.
struct MalformedOperationCase {
    std::string_view name;
    std::string_view content;
};

class MalformedContentStreamTest : public ::testing::TestWithParam<MalformedOperationCase> {};

TEST_P(MalformedContentStreamTest, ReportsErrorInsteadOfAborting) {
    const auto& param = GetParam();

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> file;
    ASSERT_EQ(File_CreateStream(io, "malformed_content_stream_test", file.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buffer;
    ASSERT_EQ(Buffer_CreateFromData(param.content.data(), static_cast<size_type>(param.content.size()), buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputStreamHandle, InputStream_Release> stream;
    ASSERT_EQ(InputStream_CreateFromBuffer(buffer, stream.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentParserHandle, ContentParser_Release> parser;
    ASSERT_EQ(ContentParser_Create(file, stream, parser.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> instructions;
    EXPECT_NE(ContentParser_ReadInstructionCollection(parser, instructions.out()), VANILLAPDF_ERROR_SUCCESS);
}

INSTANTIATE_TEST_SUITE_P(
    ContentStream,
    MalformedContentStreamTest,
    ::testing::Values(
        MalformedOperationCase{ "TooManyTextFontOperands",  "1 2 3 Tf " },
        MalformedOperationCase{ "TooFewTextFontOperands",   "1 Tf " },
        MalformedOperationCase{ "TextFontOperandTypes",     "(a) (b) Tf " },
        MalformedOperationCase{ "BeginTextWithOperands",    "1 2 BT " },
        MalformedOperationCase{ "EndTextWithOperands",      "1 ET " },
        MalformedOperationCase{ "TextShowOperandType",      "1 Tj " },
        MalformedOperationCase{ "TextShowArrayOperandType", "1 TJ " }
    ),
    [](const ::testing::TestParamInfo<MalformedOperationCase>& info) {
        return std::string(info.param.name);
    }
);
