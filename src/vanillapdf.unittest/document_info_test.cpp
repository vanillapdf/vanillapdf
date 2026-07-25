#include "unittest.h"
#include "handle_guard.h"

#include <string>

namespace document_info {

// Creates an empty in-memory document, which Document_CreateFile
// already populates with a document information dictionary
static void CreateMemoryDocument(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
}

// Reads the decoded contents of a string object into a std::string
static std::string GetDecodedString(StringObjectHandle* handle) {
    HandleGuard<BufferHandle, Buffer_Release> buffer;
    if (StringObject_GetValue(handle, buffer.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return std::string();
    }

    string_type data = nullptr;
    size_type size = 0;
    if (Buffer_GetData(buffer, &data, &size) != VANILLAPDF_ERROR_SUCCESS) {
        return std::string();
    }

    return std::string(data, size);
}

static void SetAndExpectString(
    DocumentInfoHandle* info,
    error_type (CALLING_CONVENTION *setter)(DocumentInfoHandle*, StringObjectHandle*),
    error_type (CALLING_CONVENTION *getter)(DocumentInfoHandle*, StringObjectHandle**),
    const char* value
) {
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> source;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(value, source.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(setter(info, reinterpret_cast<StringObjectHandle*>(source.get())), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> result;
    ASSERT_EQ(getter(info, result.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetDecodedString(result), std::string(value));
}

TEST(DocumentInfo, SetAndGetStringEntries) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> info;
    ASSERT_EQ(Document_CreateDocumentInfo(document, info.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(info.get(), nullptr);

    SetAndExpectString(info, DocumentInfo_SetTitle, DocumentInfo_GetTitle, "Quarterly Report");
    SetAndExpectString(info, DocumentInfo_SetAuthor, DocumentInfo_GetAuthor, "Jane Doe");
    SetAndExpectString(info, DocumentInfo_SetSubject, DocumentInfo_GetSubject, "Finance");
    SetAndExpectString(info, DocumentInfo_SetKeywords, DocumentInfo_GetKeywords, "report,finance,2026");
    SetAndExpectString(info, DocumentInfo_SetCreator, DocumentInfo_GetCreator, "Vanilla.PDF UI");
    SetAndExpectString(info, DocumentInfo_SetProducer, DocumentInfo_GetProducer, "Vanilla.PDF");
}

TEST(DocumentInfo, SetStringEntryOverwrite) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> info;
    ASSERT_EQ(Document_CreateDocumentInfo(document, info.out()), VANILLAPDF_ERROR_SUCCESS);

    SetAndExpectString(info, DocumentInfo_SetTitle, DocumentInfo_GetTitle, "First title");
    SetAndExpectString(info, DocumentInfo_SetTitle, DocumentInfo_GetTitle, "Second title");
}

TEST(DocumentInfo, SetAndGetDateEntries) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> info;
    ASSERT_EQ(Document_CreateDocumentInfo(document, info.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DateHandle, Date_Release> source_date;
    ASSERT_EQ(Date_CreateEmpty(source_date.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetYear(source_date, 2026), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetMonth(source_date, 7), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetDay(source_date, 25), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DocumentInfo_SetModificationDate(info, source_date), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DateHandle, Date_Release> result_date;
    ASSERT_EQ(DocumentInfo_GetModificationDate(info, result_date.out()), VANILLAPDF_ERROR_SUCCESS);

    integer_type year = 0;
    integer_type month = 0;
    integer_type day = 0;
    ASSERT_EQ(Date_GetYear(result_date, &year), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_GetMonth(result_date, &month), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_GetDay(result_date, &day), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(year, 2026);
    EXPECT_EQ(month, 7);
    EXPECT_EQ(day, 25);
}

TEST(DocumentInfo, SetAndGetTrapped) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> info;
    ASSERT_EQ(Document_CreateDocumentInfo(document, info.out()), VANILLAPDF_ERROR_SUCCESS);

    DocumentTrappedType trapped = DocumentTrappedType_Undefined;

    ASSERT_EQ(DocumentInfo_SetTrapped(info, DocumentTrappedType_True), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentInfo_GetTrapped(info, &trapped), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(trapped, DocumentTrappedType_True);

    ASSERT_EQ(DocumentInfo_SetTrapped(info, DocumentTrappedType_False), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentInfo_GetTrapped(info, &trapped), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(trapped, DocumentTrappedType_False);

    ASSERT_EQ(DocumentInfo_SetTrapped(info, DocumentTrappedType_Unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentInfo_GetTrapped(info, &trapped), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(trapped, DocumentTrappedType_Unknown);
}

// DocumentTrappedType_Undefined has no /Trapped representation and is rejected
// instead of being written out as a bogus name
TEST(DocumentInfo, SetTrappedUndefinedRejected) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> info;
    ASSERT_EQ(Document_CreateDocumentInfo(document, info.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(DocumentInfo_SetTrapped(info, DocumentTrappedType_Undefined), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// Repeated calls must return the same document information dictionary
// rather than allocating a second one and orphaning the original
TEST(DocumentInfo, CreateDocumentInfoIsIdempotent) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> first;
    ASSERT_EQ(Document_CreateDocumentInfo(document, first.out()), VANILLAPDF_ERROR_SUCCESS);
    SetAndExpectString(first, DocumentInfo_SetTitle, DocumentInfo_GetTitle, "Persisted title");

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> second;
    ASSERT_EQ(Document_CreateDocumentInfo(document, second.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> title;
    ASSERT_EQ(DocumentInfo_GetTitle(second, title.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetDecodedString(title), std::string("Persisted title"));
}

// Metadata set through the setters has to survive a save/reload cycle
TEST(DocumentInfo, SettersPersistAcrossSave) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> info;
    ASSERT_EQ(Document_CreateDocumentInfo(document, info.out()), VANILLAPDF_ERROR_SUCCESS);
    SetAndExpectString(info, DocumentInfo_SetTitle, DocumentInfo_GetTitle, "Saved title");
    SetAndExpectString(info, DocumentInfo_SetAuthor, DocumentInfo_GetAuthor, "Saved author");

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_stream;
    HandleGuard<FileHandle, File_Release> destination_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(destination_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(destination_stream, "temp_destination", destination_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(document, destination_file), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> reloaded_file;
    HandleGuard<DocumentHandle, Document_Release> reloaded_document;
    ASSERT_EQ(File_OpenStream(destination_stream, "temp_destination", reloaded_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Initialize(reloaded_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(reloaded_file, reloaded_document.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> reloaded_info;
    ASSERT_EQ(Document_GetDocumentInfo(reloaded_document, reloaded_info.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> title;
    HandleGuard<StringObjectHandle, StringObject_Release> author;
    ASSERT_EQ(DocumentInfo_GetTitle(reloaded_info, title.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentInfo_GetAuthor(reloaded_info, author.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(GetDecodedString(title), std::string("Saved title"));
    EXPECT_EQ(GetDecodedString(author), std::string("Saved author"));
}

TEST(DocumentInfo, SettersRejectNullParameters) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DocumentInfoHandle, DocumentInfo_Release> info;
    ASSERT_EQ(Document_CreateDocumentInfo(document, info.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(DocumentInfo_SetTitle(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(DocumentInfo_SetTitle(info, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(DocumentInfo_SetCreationDate(info, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Document_CreateDocumentInfo(nullptr, info.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Document_CreateDocumentInfo(document, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // namespace document_info
