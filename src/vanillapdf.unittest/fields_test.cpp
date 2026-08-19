#include "unittest.h"
#include "handle_guard.h"

// Helper to create a dictionary with /FT entry for a given field type
static void CreateFieldDict(
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release>& dict,
    const char* field_type_name
) {
    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> ft_key;
    HandleGuard<NameObjectHandle, NameObject_Release> ft_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", ft_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString(field_type_name, ft_value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(ft_value.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Helper to insert a literal string entry into a dictionary
static void InsertStringEntry(
    DictionaryObjectHandle* dict,
    const char* key_name,
    const char* value
) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> str;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(value, str.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, reinterpret_cast<ObjectHandle*>(str.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Helper to insert a name entry into a dictionary
static void InsertNameEntry(
    DictionaryObjectHandle* dict,
    const char* key_name,
    const char* value
) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<NameObjectHandle, NameObject_Release> val;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString(value, val.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, reinterpret_cast<ObjectHandle*>(val.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Helper to insert an integer entry into a dictionary
static void InsertIntegerEntry(
    DictionaryObjectHandle* dict,
    const char* key_name,
    bigint_type value
) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_CreateFromIntegerValue(value, integer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, reinterpret_cast<ObjectHandle*>(integer.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Creates an in-memory document, needed to register parent fields as
// indirect objects
static void CreateMemoryDocument(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
}

// Helper to link a child dictionary to its parent through the /Parent entry.
// /Parent shall be an indirect reference (Table 220), so the parent is first
// registered as an indirect object within the file.
static void InsertParentEntry(
    FileHandle* file,
    DictionaryObjectHandle* child,
    DictionaryObjectHandle* parent
) {
    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> parent_entry;
    ASSERT_EQ(File_AllocateNewEntry(file, parent_entry.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefUsedEntry_SetReference(parent_entry, reinterpret_cast<ObjectHandle*>(parent)), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> parent_reference;
    ASSERT_EQ(IndirectReferenceObject_Create(parent_reference.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IndirectReferenceObject_SetReferencedObject(parent_reference, reinterpret_cast<ObjectHandle*>(parent)), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Parent", key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(child, key, reinterpret_cast<ObjectHandle*>(parent_reference.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// --- Field base class tests ---

TEST(Field, GetName) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertStringEntry(dict, "T", "first_name");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> name;
    ASSERT_EQ(Field_GetName(field, name.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(StringObject_GetValue(name, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    size_type len = 0;
    ASSERT_EQ(Buffer_GetData(buf, &data, &len), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(std::string(data, len), "first_name");
}

TEST(Field, GetNameMissing) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> name;
    ASSERT_EQ(Field_GetName(field, name.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

TEST(Field, GetAlternateName) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertStringEntry(dict, "TU", "Enter your first name");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> alt_name;
    ASSERT_EQ(Field_GetAlternateName(field, alt_name.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(StringObject_GetValue(alt_name, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    size_type len = 0;
    ASSERT_EQ(Buffer_GetData(buf, &data, &len), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(std::string(data, len), "Enter your first name");
}

TEST(Field, GetAndSetFieldFlags) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    // Initially missing
    FieldFlags flags = FieldFlags_None;
    ASSERT_EQ(Field_GetFieldFlags(field, &flags), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Set flags
    ASSERT_EQ(Field_SetFieldFlags(field, static_cast<FieldFlags>(FieldFlags_ReadOnly | FieldFlags_Required)), VANILLAPDF_ERROR_SUCCESS);

    // Read back
    ASSERT_EQ(Field_GetFieldFlags(field, &flags), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_TRUE(flags & FieldFlags_ReadOnly);
    EXPECT_TRUE(flags & FieldFlags_Required);
    EXPECT_FALSE(flags & FieldFlags_Multiline);
}

TEST(Field, SetFieldFlagsOverwrite) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertIntegerEntry(dict, "Ff", FieldFlags_ReadOnly);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldFlags flags = FieldFlags_None;
    ASSERT_EQ(Field_GetFieldFlags(field, &flags), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(flags, FieldFlags_ReadOnly);

    // Overwrite
    ASSERT_EQ(Field_SetFieldFlags(field, FieldFlags_Multiline), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Field_GetFieldFlags(field, &flags), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(flags, FieldFlags_Multiline);
}

// --- Field type inheritance tests (Table 220 - /FT is inheritable) ---

TEST(Field, TypeInheritedFromParent) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    CreateFieldDict(parent, "Tx");

    // A terminal field merged into its widget annotation carries no /FT of its own
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(field, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_Text);
}

TEST(Field, TypeInheritedFromGrandparent) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> grandparent;
    CreateFieldDict(grandparent, "Btn");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    ASSERT_EQ(DictionaryObject_Create(parent.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertParentEntry(file, parent, grandparent);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(field, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_Button);
}

TEST(Field, OwnTypeTakesPrecedenceOverParent) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    CreateFieldDict(parent, "Tx");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    CreateFieldDict(child, "Ch");
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(field, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_Choice);
}

TEST(Field, TypeMissingThroughoutChainIsNonTerminal) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    ASSERT_EQ(DictionaryObject_Create(parent.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(field, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_NonTerminal);
}

TEST(Field, CyclicParentChainTerminates) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> first;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> second;
    ASSERT_EQ(DictionaryObject_Create(first.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Create(second.out()), VANILLAPDF_ERROR_SUCCESS);

    // Neither dictionary declares /FT, so the resolution has to recognize the
    // cycle and give up instead of looping forever
    InsertParentEntry(file, first, second);
    InsertParentEntry(file, second, first);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(first, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(field, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_NonTerminal);
}

TEST(Field, DirectParentEndsResolution) {

    // Table 220 requires /Parent to be an indirect reference. A direct
    // dictionary in its place is an embedded copy rather than a link to the
    // actual parent node, so the resolution refuses to follow it.
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    CreateFieldDict(parent, "Tx");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> parent_key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Parent", parent_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(child, parent_key, reinterpret_cast<ObjectHandle*>(parent.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(field, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_NonTerminal);
}

// --- TextField tests ---

TEST(TextField, GetAndSetValue) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<TextFieldHandle, TextField_Release> text_field;
    ASSERT_EQ(TextField_FromField(field, text_field.out()), VANILLAPDF_ERROR_SUCCESS);

    // Initially missing
    HandleGuard<StringObjectHandle, StringObject_Release> value;
    ASSERT_EQ(TextField_GetValue(text_field, value.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Set value
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> new_literal;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("John Doe", new_literal.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<StringObjectHandle, StringObject_Release> new_value;
    ASSERT_EQ(LiteralStringObject_ToStringObject(new_literal, new_value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextField_SetValue(text_field, new_value), VANILLAPDF_ERROR_SUCCESS);

    // Read back
    HandleGuard<StringObjectHandle, StringObject_Release> read_value;
    ASSERT_EQ(TextField_GetValue(text_field, read_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(StringObject_GetValue(read_value, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    size_type len = 0;
    ASSERT_EQ(Buffer_GetData(buf, &data, &len), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(std::string(data, len), "John Doe");
}

TEST(TextField, GetDefaultValue) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertStringEntry(dict, "DV", "default text");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<TextFieldHandle, TextField_Release> text_field;
    ASSERT_EQ(TextField_FromField(field, text_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> dv;
    ASSERT_EQ(TextField_GetDefaultValue(text_field, dv.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(StringObject_GetValue(dv, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    size_type len = 0;
    ASSERT_EQ(Buffer_GetData(buf, &data, &len), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(std::string(data, len), "default text");
}

TEST(TextField, GetMaxLength) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertIntegerEntry(dict, "MaxLen", 50);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<TextFieldHandle, TextField_Release> text_field;
    ASSERT_EQ(TextField_FromField(field, text_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> max_len;
    ASSERT_EQ(TextField_GetMaxLength(text_field, max_len.out()), VANILLAPDF_ERROR_SUCCESS);

    bigint_type max_len_value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(max_len, &max_len_value), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(max_len_value, 50);
}

TEST(TextField, SetValueOverwrite) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertStringEntry(dict, "V", "old value");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<TextFieldHandle, TextField_Release> text_field;
    ASSERT_EQ(TextField_FromField(field, text_field.out()), VANILLAPDF_ERROR_SUCCESS);

    // Overwrite existing value
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> new_literal;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("new value", new_literal.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<StringObjectHandle, StringObject_Release> new_value;
    ASSERT_EQ(LiteralStringObject_ToStringObject(new_literal, new_value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextField_SetValue(text_field, new_value), VANILLAPDF_ERROR_SUCCESS);

    // Read back
    HandleGuard<StringObjectHandle, StringObject_Release> read_value;
    ASSERT_EQ(TextField_GetValue(text_field, read_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(StringObject_GetValue(read_value, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    size_type len = 0;
    ASSERT_EQ(Buffer_GetData(buf, &data, &len), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(std::string(data, len), "new value");
}

// --- ButtonField tests ---

TEST(ButtonField, GetAndSetValue) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Btn");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ButtonFieldHandle, ButtonField_Release> button_field;
    ASSERT_EQ(ButtonField_FromField(field, button_field.out()), VANILLAPDF_ERROR_SUCCESS);

    // Initially missing
    HandleGuard<NameObjectHandle, NameObject_Release> value;
    ASSERT_EQ(ButtonField_GetValue(button_field, value.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Set value (check the checkbox)
    HandleGuard<NameObjectHandle, NameObject_Release> yes_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Yes", yes_value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ButtonField_SetValue(button_field, yes_value), VANILLAPDF_ERROR_SUCCESS);

    // Read back
    HandleGuard<NameObjectHandle, NameObject_Release> read_value;
    ASSERT_EQ(ButtonField_GetValue(button_field, read_value.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(ButtonField, CheckboxToggle) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Btn");
    InsertNameEntry(dict, "V", "Off");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ButtonFieldHandle, ButtonField_Release> button_field;
    ASSERT_EQ(ButtonField_FromField(field, button_field.out()), VANILLAPDF_ERROR_SUCCESS);

    // Toggle to Yes
    HandleGuard<NameObjectHandle, NameObject_Release> yes_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Yes", yes_value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ButtonField_SetValue(button_field, yes_value), VANILLAPDF_ERROR_SUCCESS);

    // Verify
    HandleGuard<NameObjectHandle, NameObject_Release> read_value;
    ASSERT_EQ(ButtonField_GetValue(button_field, read_value.out()), VANILLAPDF_ERROR_SUCCESS);
}

// --- ChoiceField tests ---

TEST(ChoiceField, GetAndSetValue) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Ch");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ChoiceFieldHandle, ChoiceField_Release> choice_field;
    ASSERT_EQ(ChoiceField_FromField(field, choice_field.out()), VANILLAPDF_ERROR_SUCCESS);

    // Initially missing
    HandleGuard<StringObjectHandle, StringObject_Release> value;
    ASSERT_EQ(ChoiceField_GetValue(choice_field, value.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Set value
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> new_literal;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("Option B", new_literal.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<StringObjectHandle, StringObject_Release> new_value;
    ASSERT_EQ(LiteralStringObject_ToStringObject(new_literal, new_value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ChoiceField_SetValue(choice_field, new_value), VANILLAPDF_ERROR_SUCCESS);

    // Read back
    HandleGuard<StringObjectHandle, StringObject_Release> read_value;
    ASSERT_EQ(ChoiceField_GetValue(choice_field, read_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(StringObject_GetValue(read_value, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    size_type len = 0;
    ASSERT_EQ(Buffer_GetData(buf, &data, &len), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(std::string(data, len), "Option B");
}

TEST(ChoiceField, GetOptionCountMissing) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Ch");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ChoiceFieldHandle, ChoiceField_Release> choice_field;
    ASSERT_EQ(ChoiceField_FromField(field, choice_field.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type count = 0;
    ASSERT_EQ(ChoiceField_GetOptionCount(choice_field, &count), VANILLAPDF_ERROR_OBJECT_MISSING);
}

// --- FieldFlags bit position tests ---

// The flag values are part of the ABI, so their bit positions are pinned here.
// InteractiveForm behaviour itself is covered in interactive_forms_test.cpp.
TEST(Field, FieldFlagsBitPositions) {
    ASSERT_EQ(FieldFlags_ReadOnly, (1 << 0));
    ASSERT_EQ(FieldFlags_Required, (1 << 1));
    ASSERT_EQ(FieldFlags_NoExport, (1 << 2));
    ASSERT_EQ(FieldFlags_Radio, (1 << 15));
    ASSERT_EQ(FieldFlags_PushButton, (1 << 16));
    ASSERT_EQ(FieldFlags_Multiline, (1 << 12));
    ASSERT_EQ(FieldFlags_Password, (1 << 13));
    ASSERT_EQ(FieldFlags_Combo, (1 << 17));
    ASSERT_EQ(FieldFlags_Edit, (1 << 18));
    ASSERT_EQ(FieldFlags_Sort, (1 << 19));
}
