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

// Inserts a /Kids array of indirect references to the given children
static void InsertKidsEntry(
    FileHandle* file,
    DictionaryObjectHandle* parent,
    std::initializer_list<DictionaryObjectHandle*> kids
) {
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> kids_array;
    ASSERT_EQ(ArrayObject_Create(kids_array.out()), VANILLAPDF_ERROR_SUCCESS);

    for (DictionaryObjectHandle* kid : kids) {
        HandleGuard<ObjectHandle, Object_Release> kid_object;
        ASSERT_EQ(DictionaryObject_ToObject(kid, kid_object.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> kid_entry;
        ASSERT_EQ(File_AllocateNewEntry(file, kid_entry.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(XrefUsedEntry_SetReference(kid_entry, kid_object), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> kid_reference;
        ASSERT_EQ(IndirectReferenceObject_Create(kid_reference.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IndirectReferenceObject_SetReferencedObject(kid_reference, kid_object), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ObjectHandle, Object_Release> kid_reference_object;
        ASSERT_EQ(IndirectReferenceObject_ToObject(kid_reference, kid_reference_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ArrayObject_Append(kids_array, kid_reference_object), VANILLAPDF_ERROR_SUCCESS);
    }

    HandleGuard<ObjectHandle, Object_Release> kids_array_object;
    ASSERT_EQ(ArrayObject_ToObject(kids_array, kids_array_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Kids", key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(parent, key, kids_array_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Reads the buffer contents into a std::string for comparisons
static std::string BufferToString(BufferHandle* buffer) {
    string_type data = nullptr;
    size_type size = 0;
    EXPECT_EQ(Buffer_GetData(buffer, &data, &size), VANILLAPDF_ERROR_SUCCESS);
    return std::string(data, size);
}

// --- Qualified name tests (12.7.3.2) ---

TEST(Field, QualifiedNameOwnOnly) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertStringEntry(dict, "T", "solo");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> qualified_name;
    ASSERT_EQ(Field_GetQualifiedName(field, qualified_name.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(qualified_name), "solo");
}

TEST(Field, QualifiedNameNested) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    ASSERT_EQ(DictionaryObject_Create(parent.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(parent, "T", "group");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    CreateFieldDict(child, "Tx");
    InsertStringEntry(child, "T", "first");
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> qualified_name;
    ASSERT_EQ(Field_GetQualifiedName(field, qualified_name.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(qualified_name), "group.first");
}

// Hierarchy levels without a /T partial name do not contribute a segment
TEST(Field, QualifiedNameSkipsUnnamedLevels) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> grandparent;
    ASSERT_EQ(DictionaryObject_Create(grandparent.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(grandparent, "T", "root");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    ASSERT_EQ(DictionaryObject_Create(parent.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertParentEntry(file, parent, grandparent);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    CreateFieldDict(child, "Tx");
    InsertStringEntry(child, "T", "leaf");
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> qualified_name;
    ASSERT_EQ(Field_GetQualifiedName(field, qualified_name.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(qualified_name), "root.leaf");
}

// Partial names are text strings - a UTF-16BE segment joins the qualified
// name as UTF-8 instead of raw bytes
TEST(Field, QualifiedNameNormalizesUtf16Segments) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    // UTF-16BE with BOM: U+0141 (LATIN CAPITAL LETTER L WITH STROKE)
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    ASSERT_EQ(DictionaryObject_Create(parent.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(parent, "T", "\xFE\xFF\x01\x41");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    CreateFieldDict(child, "Tx");
    InsertStringEntry(child, "T", "leaf");
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> qualified_name;
    ASSERT_EQ(Field_GetQualifiedName(field, qualified_name.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(qualified_name), "\xC5\x81.leaf");
}

// --- Attribute inheritance tests (Table 220) ---

TEST(Field, InheritedFieldFlags) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    CreateFieldDict(parent, "Tx");
    InsertIntegerEntry(parent, "Ff", FieldFlags_ReadOnly);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldFlags flags = FieldFlags_None;
    ASSERT_EQ(Field_GetFieldFlags(field, &flags), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(flags, FieldFlags_ReadOnly);
}

TEST(TextField, InheritedValue) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parent;
    CreateFieldDict(parent, "Tx");
    InsertStringEntry(parent, "V", "inherited text");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertParentEntry(file, child, parent);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(child, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<TextFieldHandle, TextField_Release> text_field;
    ASSERT_EQ(TextField_FromField(field, text_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> value;
    ASSERT_EQ(TextField_GetValue(text_field, value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> value_buffer;
    ASSERT_EQ(StringObject_GetValue(value, value_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(value_buffer), "inherited text");
}

// --- Terminal field classification tests (12.7.3.2) ---

TEST(Field, TerminalWithoutKids) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type terminal = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Field_IsTerminal(field, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_TRUE);
}

// Widget annotation kids carry no /T partial name, so the field stays terminal
TEST(Field, TerminalWithWidgetKids) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Btn");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> widget;
    ASSERT_EQ(DictionaryObject_Create(widget.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertNameEntry(widget, "Subtype", "Widget");

    InsertKidsEntry(file, dict, { widget.get() });

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type terminal = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Field_IsTerminal(field, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_TRUE);
}

// A kid carrying a /T partial name is a child field, making the node a
// non-terminal grouping node
TEST(Field, NonTerminalWithFieldKids) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(child, "T", "first");

    InsertKidsEntry(file, dict, { child.get() });

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type terminal = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Field_IsTerminal(field, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_FALSE);
}

// --- Quadding tests (Table 222) ---

TEST(Field, QuaddingKnownValues) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertIntegerEntry(dict, "Q", 1);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    QuaddingType quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(Field_GetQuadding(field, &quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(quadding, QuaddingType_Centered);
}

// A value outside the enumerated codes is a malformed document
TEST(Field, QuaddingUnknownValueFails) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertIntegerEntry(dict, "Q", 7);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    QuaddingType quadding = QuaddingType_LeftJustified;
    EXPECT_EQ(Field_GetQuadding(field, &quadding), VANILLAPDF_ERROR_PARSE_EXCEPTION);
}

// Creates a string object from a literal value
static void CreateStringObject(
    const char* value,
    HandleGuard<StringObjectHandle, StringObject_Release>& result
) {
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(value, literal.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_ToStringObject(literal, result.out()), VANILLAPDF_ERROR_SUCCESS);
}

// --- Setter tests ---

// Because the PERIOD is used as a separator for fully qualified names,
// a partial name shall not contain a PERIOD (12.7.3.2)
TEST(Field, SetNameRejectsPeriod) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> invalid_name;
    CreateStringObject("group.first", invalid_name);
    EXPECT_EQ(Field_SetName(field, invalid_name), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Field, SetAndGetName) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertStringEntry(dict, "T", "old_name");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> new_name;
    CreateStringObject("renamed", new_name);
    ASSERT_EQ(Field_SetName(field, new_name), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> read_name;
    ASSERT_EQ(Field_GetName(field, read_name.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> read_name_value;
    ASSERT_EQ(StringObject_GetValue(read_name, read_name_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(read_name_value), "renamed");
}

TEST(Field, SetAndGetAlternateName) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> alternate_name;
    CreateStringObject("Enter your first name", alternate_name);
    ASSERT_EQ(Field_SetAlternateName(field, alternate_name), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> read_name;
    ASSERT_EQ(Field_GetAlternateName(field, read_name.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> read_name_value;
    ASSERT_EQ(StringObject_GetValue(read_name, read_name_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(read_name_value), "Enter your first name");
}

TEST(Field, SetAndGetDefaultAppearance) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> appearance;
    CreateStringObject("/Helv 12 Tf 0 g", appearance);
    ASSERT_EQ(Field_SetDefaultAppearance(field, appearance), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> read_appearance;
    ASSERT_EQ(Field_GetDefaultAppearance(field, read_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> read_appearance_value;
    ASSERT_EQ(StringObject_GetValue(read_appearance, read_appearance_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(read_appearance_value), "/Helv 12 Tf 0 g");
}

TEST(Field, SetQuaddingOverridesValue) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    CreateFieldDict(dict, "Tx");
    InsertIntegerEntry(dict, "Q", 0);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_SetQuadding(field, QuaddingType_RightJustified), VANILLAPDF_ERROR_SUCCESS);

    QuaddingType quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(Field_GetQuadding(field, &quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(quadding, QuaddingType_RightJustified);
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
