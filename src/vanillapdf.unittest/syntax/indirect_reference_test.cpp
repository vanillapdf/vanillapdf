#include "unittest.h"
#include "handle_guard.h"

namespace indirect_reference {

// Creates an empty document, whose file allocates the tested xref entries
class IndirectReferenceTest : public ::testing::Test {
protected:
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> font;
    HandleGuard<ObjectHandle, Object_Release> font_object;
    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> font_entry;
    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> font_reference;

    biguint_type font_object_number = 0;
    ushort_type font_generation_number = 0;

    void SetUp() override {
        ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_CreateStream(io_stream, "indirect_reference", file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);

        // Register the font as a new indirect object within the file
        ASSERT_EQ(DictionaryObject_Create(font.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DictionaryObject_ToObject(font, font_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_AllocateNewEntry(file, font_entry.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_SetReference(font_entry, font_object), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_GetObjectNumber(font_entry, &font_object_number), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_GetGenerationNumber(font_entry, &font_generation_number), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(IndirectReferenceObject_Create(font_reference.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IndirectReferenceObject_SetReferencedObject(font_reference, font_object), VANILLAPDF_ERROR_SUCCESS);
    }
};

// An indirect object inserted directly into a dictionary, instead of through a reference,
// is also owned by that dictionary - its own xref entry still determines the number
TEST_F(IndirectReferenceTest, IndirectObjectInsertedDirectly) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> container;
    ASSERT_EQ(DictionaryObject_Create(container.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Font", key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(container, key, font_object, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    biguint_type object_number = 0;
    ASSERT_EQ(IndirectReferenceObject_GetReferencedObjectNumber(font_reference, &object_number), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(object_number, font_object_number);

    ushort_type generation_number = 0;
    ASSERT_EQ(IndirectReferenceObject_GetReferencedGenerationNumber(font_reference, &generation_number), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(generation_number, font_generation_number);
}

// Assigning another object to the xref entry detaches the font,
// which then has neither an xref entry nor an owner
TEST_F(IndirectReferenceTest, DetachedReferencedObject) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> replacement;
    ASSERT_EQ(DictionaryObject_Create(replacement.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> replacement_object;
    ASSERT_EQ(DictionaryObject_ToObject(replacement, replacement_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefUsedEntry_SetReference(font_entry, replacement_object), VANILLAPDF_ERROR_SUCCESS);

    biguint_type object_number = 0;
    EXPECT_EQ(IndirectReferenceObject_GetReferencedObjectNumber(font_reference, &object_number), VANILLAPDF_ERROR_INVALID_OPERATION);

    ushort_type generation_number = 0;
    EXPECT_EQ(IndirectReferenceObject_GetReferencedGenerationNumber(font_reference, &generation_number), VANILLAPDF_ERROR_INVALID_OPERATION);
}

} /* indirect_reference */
