#include "unittest.h"
#include "handle_guard.h"

namespace object_number {

static void ExpectIndirect(ObjectHandle* object, biguint_type object_number, ushort_type generation_number) {
    boolean_type is_indirect = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Object_IsIndirect(object, &is_indirect), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_indirect, VANILLAPDF_RV_TRUE);

    biguint_type result_object_number = 0;
    ASSERT_EQ(Object_GetObjectNumber(object, &result_object_number), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(result_object_number, object_number);

    ushort_type result_generation_number = 0;
    ASSERT_EQ(Object_GetGenerationNumber(object, &result_generation_number), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(result_generation_number, generation_number);
}

static void ExpectDirect(ObjectHandle* object) {
    boolean_type is_indirect = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Object_IsIndirect(object, &is_indirect), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_indirect, VANILLAPDF_RV_FALSE);

    biguint_type object_number = 0;
    EXPECT_EQ(Object_GetObjectNumber(object, &object_number), VANILLAPDF_ERROR_INVALID_OPERATION);

    ushort_type generation_number = 0;
    EXPECT_EQ(Object_GetGenerationNumber(object, &generation_number), VANILLAPDF_ERROR_INVALID_OPERATION);
}

static void FindEntry(DictionaryObjectHandle* dictionary, string_type key, ObjectHandle** result) {
    HandleGuard<NameObjectHandle, NameObject_Release> key_name;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key, key_name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Find(dictionary, key_name, result), VANILLAPDF_ERROR_SUCCESS);
}

static void InsertEntry(DictionaryObjectHandle* dictionary, string_type key, ObjectHandle* value) {
    HandleGuard<NameObjectHandle, NameObject_Release> key_name;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key, key_name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dictionary, key_name, value, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

static void FindReference(DictionaryObjectHandle* dictionary, string_type key, IndirectReferenceObjectHandle** result) {
    HandleGuard<ObjectHandle, Object_Release> entry;
    ASSERT_NO_FATAL_FAILURE(FindEntry(dictionary, key, entry.out()));
    ASSERT_EQ(IndirectReferenceObject_FromObject(entry, result), VANILLAPDF_ERROR_SUCCESS);
}

static void GetFirstPageDictionary(DocumentHandle* document, DictionaryObjectHandle** result) {
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> page_tree;
    HandleGuard<PageObjectHandle, PageObject_Release> page;

    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, page_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_GetPage(page_tree, 1, page.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetBaseObject(page, result), VANILLAPDF_ERROR_SUCCESS);
}

// Name of the page dictionary entry, which holds the tested font dictionary.
// It mirrors a /Font resource dictionary with one indirect font /F1 and one inline font /F2.
static const char FONTS_KEY[] = "ObjectNumberTestFonts";

// Builds a document through the API, whose first page carries the tested font dictionary
class ObjectNumberTest : public ::testing::Test {
protected:
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> fonts;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> font;
    HandleGuard<ObjectHandle, Object_Release> font_object;

    biguint_type font_object_number = 0;
    ushort_type font_generation_number = 0;

    void SetUp() override {
        ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_CreateStream(io_stream, "object_number_source", file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(DictionaryObject_Create(fonts.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DictionaryObject_Create(font.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DictionaryObject_ToObject(font, font_object.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> inline_font;
        HandleGuard<ObjectHandle, Object_Release> inline_font_object;
        ASSERT_EQ(DictionaryObject_Create(inline_font.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DictionaryObject_ToObject(inline_font, inline_font_object.out()), VANILLAPDF_ERROR_SUCCESS);

        // Register the font as a new indirect object within the file
        HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> font_entry;
        ASSERT_EQ(File_AllocateNewEntry(file, font_entry.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_SetReference(font_entry, font_object), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_GetObjectNumber(font_entry, &font_object_number), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_GetGenerationNumber(font_entry, &font_generation_number), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> font_reference;
        HandleGuard<ObjectHandle, Object_Release> font_reference_object;
        ASSERT_EQ(IndirectReferenceObject_Create(font_reference.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IndirectReferenceObject_SetReferencedObject(font_reference, font_object), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IndirectReferenceObject_ToObject(font_reference, font_reference_object.out()), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_NO_FATAL_FAILURE(InsertEntry(fonts, "F1", font_reference_object));
        ASSERT_NO_FATAL_FAILURE(InsertEntry(fonts, "F2", inline_font_object));

        HandleGuard<ObjectHandle, Object_Release> fonts_object;
        ASSERT_EQ(DictionaryObject_ToObject(fonts, fonts_object.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> page_dictionary;
        ASSERT_NO_FATAL_FAILURE(GetFirstPageDictionary(document, page_dictionary.out()));
        ASSERT_NO_FATAL_FAILURE(InsertEntry(page_dictionary, FONTS_KEY, fonts_object));
    }

    // Saves the document, opens the result and returns the font dictionary of its first page
    void SaveAndReopen(
        HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& destination_stream,
        HandleGuard<FileHandle, File_Release>& destination_file,
        HandleGuard<DocumentHandle, Document_Release>& destination_document,
        DictionaryObjectHandle** destination_fonts
    ) {
        HandleGuard<FileHandle, File_Release> save_file;

        ASSERT_EQ(InputOutputStream_CreateFromMemory(destination_stream.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_CreateStream(destination_stream, "object_number_destination", save_file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_SaveFile(document, save_file), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(File_OpenStream(destination_stream, "object_number_destination", destination_file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_Initialize(destination_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_OpenFile(destination_file, destination_document.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> page_dictionary;
        ASSERT_NO_FATAL_FAILURE(GetFirstPageDictionary(destination_document, page_dictionary.out()));

        HandleGuard<ObjectHandle, Object_Release> fonts_object;
        ASSERT_NO_FATAL_FAILURE(FindEntry(page_dictionary, FONTS_KEY, fonts_object.out()));
        ASSERT_EQ(DictionaryObject_FromObject(fonts_object, destination_fonts), VANILLAPDF_ERROR_SUCCESS);
    }
};

TEST_F(ObjectNumberTest, CreatedIndirectObject) {
    ExpectIndirect(font_object, font_object_number, font_generation_number);
}

TEST_F(ObjectNumberTest, CreatedResolvedReference) {
    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> font_reference;
    ASSERT_NO_FATAL_FAILURE(FindReference(fonts, "F1", font_reference.out()));

    HandleGuard<ObjectHandle, Object_Release> referenced_font;
    ASSERT_EQ(IndirectReferenceObject_GetReferencedObject(font_reference, referenced_font.out()), VANILLAPDF_ERROR_SUCCESS);

    ExpectIndirect(referenced_font, font_object_number, font_generation_number);
}

TEST_F(ObjectNumberTest, CreatedInlineDictionary) {
    HandleGuard<ObjectHandle, Object_Release> inline_font_object;
    ASSERT_NO_FATAL_FAILURE(FindEntry(fonts, "F2", inline_font_object.out()));

    ExpectDirect(inline_font_object);
}

// The writer may renumber objects, so the parsed font is checked
// against the numbers written in the reference, which points to it
TEST_F(ObjectNumberTest, ParsedResolvedReference) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_stream;
    HandleGuard<FileHandle, File_Release> destination_file;
    HandleGuard<DocumentHandle, Document_Release> destination_document;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> destination_fonts;
    ASSERT_NO_FATAL_FAILURE(SaveAndReopen(destination_stream, destination_file, destination_document, destination_fonts.out()));

    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> font_reference;
    ASSERT_NO_FATAL_FAILURE(FindReference(destination_fonts, "F1", font_reference.out()));

    biguint_type referenced_object_number = 0;
    ushort_type referenced_generation_number = 0;
    ASSERT_EQ(IndirectReferenceObject_GetReferencedObjectNumber(font_reference, &referenced_object_number), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IndirectReferenceObject_GetReferencedGenerationNumber(font_reference, &referenced_generation_number), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> referenced_font;
    ASSERT_EQ(IndirectReferenceObject_GetReferencedObject(font_reference, referenced_font.out()), VANILLAPDF_ERROR_SUCCESS);

    ExpectIndirect(referenced_font, referenced_object_number, referenced_generation_number);

    // The same object loaded directly from the file reports the same numbers
    HandleGuard<ObjectHandle, Object_Release> file_font_object;
    ASSERT_EQ(File_GetIndirectObject(destination_file, referenced_object_number, referenced_generation_number, file_font_object.out()), VANILLAPDF_ERROR_SUCCESS);

    ExpectIndirect(file_font_object, referenced_object_number, referenced_generation_number);
}

TEST_F(ObjectNumberTest, ParsedInlineDictionary) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_stream;
    HandleGuard<FileHandle, File_Release> destination_file;
    HandleGuard<DocumentHandle, Document_Release> destination_document;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> destination_fonts;
    ASSERT_NO_FATAL_FAILURE(SaveAndReopen(destination_stream, destination_file, destination_document, destination_fonts.out()));

    HandleGuard<ObjectHandle, Object_Release> inline_font_object;
    ASSERT_NO_FATAL_FAILURE(FindEntry(destination_fonts, "F2", inline_font_object.out()));

    ExpectDirect(inline_font_object);
}

TEST(ObjectNumber, DetachedObject) {
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer_object;
    ASSERT_EQ(IntegerObject_Create(integer_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> object;
    ASSERT_EQ(IntegerObject_ToObject(integer_object, object.out()), VANILLAPDF_ERROR_SUCCESS);

    ExpectDirect(object);
}

TEST(ObjectNumber, NullCheck) {
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer_object;
    ASSERT_EQ(IntegerObject_Create(integer_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> object;
    ASSERT_EQ(IntegerObject_ToObject(integer_object, object.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_indirect = VANILLAPDF_RV_FALSE;
    biguint_type object_number = 0;
    ushort_type generation_number = 0;

    EXPECT_EQ(Object_IsIndirect(nullptr, &is_indirect), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Object_IsIndirect(object, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Object_GetObjectNumber(nullptr, &object_number), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Object_GetObjectNumber(object, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Object_GetGenerationNumber(nullptr, &generation_number), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Object_GetGenerationNumber(object, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} /* object_number */
