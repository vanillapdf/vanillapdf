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

// Parse a well-formed inline image ("BI ... ID ... EI") and round-trip the
// resulting content object through the inline image conversions.
TEST(ContentObjectInlineImage, Conversion) {
    const char content[] = "BI /W 1 /H 1 ID abc EI ";

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

    // derived -> base
    HandleGuard<ContentObjectHandle, ContentObject_Release> object_again;
    ASSERT_EQ(ContentObjectInlineImage_ToContentObject(image, object_again.out()), VANILLAPDF_ERROR_SUCCESS);
}
