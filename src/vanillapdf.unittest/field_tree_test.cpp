#include "unittest.h"
#include "handle_guard.h"

#include <cstring>

// Structural view of the interactive form field hierarchy - the top level,
// the child walk, the parent round trip and the mutators of FieldTreeHandle.
// The flat view is covered next to the form in interactive_forms_test.cpp.
namespace field_tree {

static void CreateMemoryDocument(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
}

static void RegisterIndirectObject(FileHandle* file, DictionaryObjectHandle* dict) {
    HandleGuard<ObjectHandle, Object_Release> dict_object;
    ASSERT_EQ(DictionaryObject_ToObject(dict, dict_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> entry;
    ASSERT_EQ(File_AllocateNewEntry(file, entry.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefUsedEntry_SetReference(entry, dict_object), VANILLAPDF_ERROR_SUCCESS);
}

static void CreateReferenceTo(
    DictionaryObjectHandle* dict,
    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release>& reference
) {
    HandleGuard<ObjectHandle, Object_Release> dict_object;
    ASSERT_EQ(DictionaryObject_ToObject(dict, dict_object.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(IndirectReferenceObject_Create(reference.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IndirectReferenceObject_SetReferencedObject(reference, dict_object), VANILLAPDF_ERROR_SUCCESS);
}

static void InsertReferenceArrayEntry(
    DictionaryObjectHandle* dict,
    const char* key_name,
    std::initializer_list<DictionaryObjectHandle*> targets
) {
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array;
    ASSERT_EQ(ArrayObject_Create(array.out()), VANILLAPDF_ERROR_SUCCESS);

    for (DictionaryObjectHandle* target : targets) {
        HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> reference;
        CreateReferenceTo(target, reference);

        HandleGuard<ObjectHandle, Object_Release> reference_object;
        ASSERT_EQ(IndirectReferenceObject_ToObject(reference, reference_object.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ArrayObject_Append(array, reference_object), VANILLAPDF_ERROR_SUCCESS);
    }

    HandleGuard<ObjectHandle, Object_Release> array_object;
    ASSERT_EQ(ArrayObject_ToObject(array, array_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> key;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, array_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

static void InsertParentEntry(DictionaryObjectHandle* child, DictionaryObjectHandle* parent) {
    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> reference;
    CreateReferenceTo(parent, reference);

    HandleGuard<ObjectHandle, Object_Release> reference_object;
    ASSERT_EQ(IndirectReferenceObject_ToObject(reference, reference_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Parent", key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(child, key, reference_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

static void RemoveEntry(DictionaryObjectHandle* dict, const char* key_name) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type removed = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DictionaryObject_Remove(dict, key, &removed), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(removed, VANILLAPDF_RV_TRUE);
}

static void InsertNameEntry(DictionaryObjectHandle* dict, const char* key_name, const char* value) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<NameObjectHandle, NameObject_Release> name_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString(value, name_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> value_object;
    ASSERT_EQ(NameObject_ToObject(name_value, value_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, value_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

static void InsertStringEntry(DictionaryObjectHandle* dict, const char* key_name, const char* value) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(value, literal_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> string_value;
    ASSERT_EQ(LiteralStringObject_ToStringObject(literal_value, string_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> value_object;
    ASSERT_EQ(StringObject_ToObject(string_value, value_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, value_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

static void InsertIntegerEntry(DictionaryObjectHandle* dict, const char* key_name, bigint_type value) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_CreateFromIntegerValue(value, integer_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> value_object;
    ASSERT_EQ(IntegerObject_ToObject(integer_value, value_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, value_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

static std::string BufferToString(BufferHandle* buffer) {
    string_type data = nullptr;
    size_type size = 0;
    EXPECT_EQ(Buffer_GetData(buffer, &data, &size), VANILLAPDF_ERROR_SUCCESS);
    return std::string(data, size);
}

static std::string StringObjectToString(StringObjectHandle* string_object) {
    HandleGuard<BufferHandle, Buffer_Release> value;
    EXPECT_EQ(StringObject_GetValue(string_object, value.out()), VANILLAPDF_ERROR_SUCCESS);
    return BufferToString(value);
}

// Looks a terminal field up by its fully qualified name given as a literal
static error_type FindFieldByName(FieldTreeHandle* tree, const char* qualified_name, FieldHandle** result) {
    return FieldTree_FindField(tree, qualified_name, static_cast<size_type>(strlen(qualified_name)), result);
}

static std::string GetQualifiedName(FieldHandle* field) {
    HandleGuard<BufferHandle, Buffer_Release> qualified_name;
    EXPECT_EQ(Field_GetQualifiedName(field, qualified_name.out()), VANILLAPDF_ERROR_SUCCESS);
    return BufferToString(qualified_name);
}

static std::string GetFieldQualifiedName(FieldTreeHandle* tree, size_type index) {
    HandleGuard<FieldHandle, Field_Release> field;
    EXPECT_EQ(FieldTree_GetField(tree, index, field.out()), VANILLAPDF_ERROR_SUCCESS);
    return GetQualifiedName(field);
}

static std::string GetRootChildQualifiedName(FieldTreeHandle* tree, size_type index) {
    HandleGuard<FieldHandle, Field_Release> child;
    EXPECT_EQ(FieldTree_GetRootChild(tree, index, child.out()), VANILLAPDF_ERROR_SUCCESS);
    return GetQualifiedName(child);
}

static std::string GetChildQualifiedName(FieldHandle* parent, size_type index) {
    HandleGuard<FieldHandle, Field_Release> child;
    EXPECT_EQ(Field_GetChild(parent, index, child.out()), VANILLAPDF_ERROR_SUCCESS);
    return GetQualifiedName(child);
}

static size_type GetFieldCount(FieldTreeHandle* tree) {
    size_type count = 0;
    EXPECT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    return count;
}

static size_type GetRootChildCount(FieldTreeHandle* tree) {
    size_type count = 0;
    EXPECT_EQ(FieldTree_GetRootChildCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    return count;
}

static size_type GetChildCount(FieldHandle* field) {
    size_type count = 0;
    EXPECT_EQ(Field_GetChildCount(field, &count), VANILLAPDF_ERROR_SUCCESS);
    return count;
}

static void CreateTextFieldDictionary(
    FileHandle* file,
    const char* partial_name,
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release>& dictionary,
    HandleGuard<FieldHandle, Field_Release>& field
) {
    ASSERT_EQ(DictionaryObject_Create(dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(dictionary, "T", partial_name);
    InsertNameEntry(dictionary, "FT", "Tx");
    RegisterIndirectObject(file, dictionary);
    ASSERT_EQ(Field_CreateFromDictionary(dictionary, field.out()), VANILLAPDF_ERROR_SUCCESS);
}

// A small hierarchy shared by most tests:
//
//   /Fields [ address, email ]
//   address (group, /FT /Tx) /Kids [ street, city ]
//   email   (terminal)
//
// The form dictionary carries the document defaults /DA and /Q.
struct SampleHierarchy {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> address;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> street;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> city;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> email;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
};

static void BuildSampleHierarchy(SampleHierarchy& sample) {
    CreateMemoryDocument(sample.io_stream, sample.file, sample.document);

    ASSERT_EQ(DictionaryObject_Create(sample.address.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(sample.address, "T", "address");
    InsertNameEntry(sample.address, "FT", "Tx");
    RegisterIndirectObject(sample.file, sample.address);

    ASSERT_EQ(DictionaryObject_Create(sample.street.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(sample.street, "T", "street");
    InsertStringEntry(sample.street, "V", "Main St");
    RegisterIndirectObject(sample.file, sample.street);
    InsertParentEntry(sample.street, sample.address);

    ASSERT_EQ(DictionaryObject_Create(sample.city.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(sample.city, "T", "city");
    RegisterIndirectObject(sample.file, sample.city);
    InsertParentEntry(sample.city, sample.address);

    InsertReferenceArrayEntry(sample.address, "Kids", { sample.street.get(), sample.city.get() });

    ASSERT_EQ(DictionaryObject_Create(sample.email.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(sample.email, "T", "email");
    InsertNameEntry(sample.email, "FT", "Tx");
    RegisterIndirectObject(sample.file, sample.email);

    ASSERT_EQ(DictionaryObject_Create(sample.form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(sample.form_dictionary, "DA", "/Helv 0 Tf 0 g");
    InsertIntegerEntry(sample.form_dictionary, "Q", 1);
    InsertReferenceArrayEntry(sample.form_dictionary, "Fields", { sample.address.get(), sample.email.get() });

    ASSERT_EQ(InteractiveForm_CreateFromDictionary(sample.form_dictionary, sample.form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetFieldTree(sample.form, sample.tree.out()), VANILLAPDF_ERROR_SUCCESS);
}

// --- Walking down ---

// The top level is the root /Fields entries - groups included - and a
// group's children are its /Kids fields, in array order
TEST(FieldTree, ChildrenWalkFromTheTopLevel) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    ASSERT_EQ(GetRootChildCount(sample.tree), 2u);
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 0), "address");
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 1), "email");

    HandleGuard<FieldHandle, Field_Release> out_of_range;
    EXPECT_EQ(FieldTree_GetRootChild(sample.tree, 2, out_of_range.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 0, address.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type terminal = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Field_IsTerminal(address, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_FALSE);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(address, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_Text);

    ASSERT_EQ(GetChildCount(address), 2u);
    EXPECT_EQ(GetChildQualifiedName(address, 0), "address.street");
    EXPECT_EQ(GetChildQualifiedName(address, 1), "address.city");

    HandleGuard<FieldHandle, Field_Release> street;
    ASSERT_EQ(Field_GetChild(address, 0, street.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetChildCount(street), 0u);
    EXPECT_EQ(Field_GetChild(street, 0, out_of_range.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // The flat view is the same tree, terminals only, in the same order
    ASSERT_EQ(GetFieldCount(sample.tree), 3u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 0), "address.street");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 1), "address.city");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 2), "email");
}

// A /Kids entry is a child field when it carries /T, /Kids or /FT - a
// nameless intermediate node is legal and its subtree is not lost
TEST(FieldTree, NamelessIntermediateNodeIsWalked) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> group;
    ASSERT_EQ(DictionaryObject_Create(group.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(group, "T", "group");
    RegisterIndirectObject(file, group);

    // No /T of its own - contributes no name segment
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> nameless;
    ASSERT_EQ(DictionaryObject_Create(nameless.out()), VANILLAPDF_ERROR_SUCCESS);
    RegisterIndirectObject(file, nameless);
    InsertParentEntry(nameless, group);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> leaf;
    ASSERT_EQ(DictionaryObject_Create(leaf.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(leaf, "T", "leaf");
    InsertNameEntry(leaf, "FT", "Tx");
    RegisterIndirectObject(file, leaf);
    InsertParentEntry(leaf, nameless);

    InsertReferenceArrayEntry(nameless, "Kids", { leaf.get() });
    InsertReferenceArrayEntry(group, "Kids", { nameless.get() });

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;
    ASSERT_EQ(DictionaryObject_Create(form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertReferenceArrayEntry(form_dictionary, "Fields", { group.get() });

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    ASSERT_EQ(InteractiveForm_CreateFromDictionary(form_dictionary, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetFieldTree(form, tree.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(GetFieldCount(tree), 1u);
    EXPECT_EQ(GetFieldQualifiedName(tree, 0), "group.leaf");

    HandleGuard<FieldHandle, Field_Release> found;
    EXPECT_EQ(FindFieldByName(tree, "group.leaf", found.out()), VANILLAPDF_ERROR_SUCCESS);
}

// --- Walking up ---

// A nested field reports its group, a top-level field has no /Parent and
// reports nothing - whether the handle came from the tree or was created
// straight from the dictionary
TEST(FieldTree, GetParentRoundTrips) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<FieldHandle, Field_Release> street;
    ASSERT_EQ(FindFieldByName(sample.tree, "address.street", street.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> street_parent;
    ASSERT_EQ(Field_GetParent(street, street_parent.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetQualifiedName(street_parent), "address");

    boolean_type terminal = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Field_IsTerminal(street_parent, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_FALSE);
    EXPECT_EQ(GetChildCount(street_parent), 2u);

    HandleGuard<FieldHandle, Field_Release> beyond_top;
    EXPECT_EQ(Field_GetParent(street_parent, beyond_top.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // The parent handed back is a valid parent for the mutators
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> zip_dictionary;
    HandleGuard<FieldHandle, Field_Release> zip;
    CreateTextFieldDictionary(sample.file, "zip", zip_dictionary, zip);
    ASSERT_EQ(FieldTree_AddChild(sample.tree, street_parent, zip), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 2), "address.zip");

    // A field is a view over its dictionary - a wrapper created straight
    // from the dictionary behaves exactly like one handed out by the tree
    HandleGuard<FieldHandle, Field_Release> detached_street;
    ASSERT_EQ(Field_CreateFromDictionary(sample.street, detached_street.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> detached_street_parent;
    ASSERT_EQ(Field_GetParent(detached_street, detached_street_parent.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetQualifiedName(detached_street_parent), "address");

    HandleGuard<FieldHandle, Field_Release> detached_email;
    ASSERT_EQ(Field_CreateFromDictionary(sample.email, detached_email.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> detached_email_parent;
    EXPECT_EQ(Field_GetParent(detached_email, detached_email_parent.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

// --- Values ---

// /V and /DV are exposed as raw objects, resolved through /Parent
TEST(FieldTree, ValueObjectsResolveThroughParent) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    InsertStringEntry(sample.address, "DV", "unknown");

    HandleGuard<FieldHandle, Field_Release> city;
    ASSERT_EQ(FindFieldByName(sample.tree, "address.city", city.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> missing_value;
    EXPECT_EQ(Field_GetValue(city, missing_value.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<ObjectHandle, Object_Release> inherited_default;
    ASSERT_EQ(Field_GetDefaultValue(city, inherited_default.out()), VANILLAPDF_ERROR_SUCCESS);

    ObjectType default_type = ObjectType_Undefined;
    ASSERT_EQ(Object_GetObjectType(inherited_default, &default_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(default_type, ObjectType_String);

    HandleGuard<FieldHandle, Field_Release> street;
    ASSERT_EQ(FindFieldByName(sample.tree, "address.street", street.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> own_value;
    ASSERT_EQ(Field_GetValue(street, own_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> own_value_string;
    ASSERT_EQ(StringObject_FromObject(own_value, own_value_string.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(StringObjectToString(own_value_string), "Main St");
}

// --- Appearance defaults ---

// /DA and /Q resolve field, then ancestors, then the form's document
// default - the field getters stop at the hierarchy, the form finishes
TEST(FieldTree, ResolveDefaultAppearanceAndQuaddingThroughTheForm) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<FieldHandle, Field_Release> city;
    ASSERT_EQ(FindFieldByName(sample.tree, "address.city", city.out()), VANILLAPDF_ERROR_SUCCESS);

    // Nothing on the field or its group - the form default applies
    HandleGuard<StringObjectHandle, StringObject_Release> field_appearance;
    EXPECT_EQ(Field_GetDefaultAppearance(city, field_appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<StringObjectHandle, StringObject_Release> form_appearance;
    ASSERT_EQ(InteractiveForm_ResolveDefaultAppearance(sample.form, city, form_appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(StringObjectToString(form_appearance), "/Helv 0 Tf 0 g");

    QuaddingType quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(InteractiveForm_ResolveQuadding(sample.form, city, &quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(quadding, QuaddingType_Centered);

    // The group's entries are inherited before the form is consulted
    InsertStringEntry(sample.address, "DA", "/TiRo 10 Tf 0 g");
    InsertIntegerEntry(sample.address, "Q", 2);

    HandleGuard<StringObjectHandle, StringObject_Release> group_appearance;
    ASSERT_EQ(InteractiveForm_ResolveDefaultAppearance(sample.form, city, group_appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(StringObjectToString(group_appearance), "/TiRo 10 Tf 0 g");

    ASSERT_EQ(InteractiveForm_ResolveQuadding(sample.form, city, &quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(quadding, QuaddingType_RightJustified);

    // The field's own entries win over everything
    InsertStringEntry(sample.city, "DA", "/Cour 8 Tf 0 g");
    InsertIntegerEntry(sample.city, "Q", 0);

    HandleGuard<StringObjectHandle, StringObject_Release> own_appearance;
    ASSERT_EQ(InteractiveForm_ResolveDefaultAppearance(sample.form, city, own_appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(StringObjectToString(own_appearance), "/Cour 8 Tf 0 g");

    ASSERT_EQ(InteractiveForm_ResolveQuadding(sample.form, city, &quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(quadding, QuaddingType_LeftJustified);
}

// --- AddChild ---

// Adding under a group sets /Parent and prefixes the name
TEST(FieldTree, AddChildUnderGroup) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 0, address.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> zip_dictionary;
    HandleGuard<FieldHandle, Field_Release> zip;
    CreateTextFieldDictionary(sample.file, "zip", zip_dictionary, zip);
    ASSERT_EQ(FieldTree_AddChild(sample.tree, address, zip), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(GetChildCount(address), 3u);
    EXPECT_EQ(GetChildQualifiedName(address, 2), "address.zip");

    ASSERT_EQ(GetFieldCount(sample.tree), 4u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 2), "address.zip");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 3), "email");

    // /Parent was written - the new child walks back up to the group
    HandleGuard<FieldHandle, Field_Release> found_zip;
    ASSERT_EQ(FindFieldByName(sample.tree, "address.zip", found_zip.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> zip_parent;
    ASSERT_EQ(Field_GetParent(found_zip, zip_parent.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetQualifiedName(zip_parent), "address");
}

// Adding a child to a terminal field turns it into a group - it leaves the
// flat view and its child takes its place
TEST(FieldTree, AddChildTurnsTerminalIntoGroup) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<FieldHandle, Field_Release> email;
    ASSERT_EQ(FindFieldByName(sample.tree, "email", email.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> work_dictionary;
    HandleGuard<FieldHandle, Field_Release> work;
    CreateTextFieldDictionary(sample.file, "work", work_dictionary, work);
    ASSERT_EQ(FieldTree_AddChild(sample.tree, email, work), VANILLAPDF_ERROR_SUCCESS);

    boolean_type terminal = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Field_IsTerminal(email, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(GetFieldCount(sample.tree), 3u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 2), "email.work");

    HandleGuard<FieldHandle, Field_Release> group_lookup;
    EXPECT_EQ(FindFieldByName(sample.tree, "email", group_lookup.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

// A top-level field carrying a stale /Parent has it removed on the way in
TEST(FieldTree, AddRootChildClearsParent) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> phone_dictionary;
    HandleGuard<FieldHandle, Field_Release> phone;
    CreateTextFieldDictionary(sample.file, "phone", phone_dictionary, phone);
    InsertParentEntry(phone_dictionary, sample.address);

    ASSERT_EQ(FieldTree_AddRootChild(sample.tree, phone), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(GetRootChildCount(sample.tree), 3u);
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 2), "phone");

    HandleGuard<FieldHandle, Field_Release> phone_parent;
    EXPECT_EQ(Field_GetParent(phone, phone_parent.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(GetQualifiedName(phone), "phone");
}

// Every refusal leaves the hierarchy untouched
TEST(FieldTree, AddChildRefusals) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 0, address.out()), VANILLAPDF_ERROR_SUCCESS);

    // Duplicate fully qualified names, at the top level and under a group
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> duplicate_email_dictionary;
    HandleGuard<FieldHandle, Field_Release> duplicate_email;
    CreateTextFieldDictionary(sample.file, "email", duplicate_email_dictionary, duplicate_email);
    EXPECT_EQ(FieldTree_AddRootChild(sample.tree, duplicate_email), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> duplicate_city_dictionary;
    HandleGuard<FieldHandle, Field_Release> duplicate_city;
    CreateTextFieldDictionary(sample.file, "city", duplicate_city_dictionary, duplicate_city);
    EXPECT_EQ(FieldTree_AddChild(sample.tree, address, duplicate_city), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // The same partial name under a different parent is a different field
    EXPECT_EQ(FieldTree_AddRootChild(sample.tree, duplicate_city), VANILLAPDF_ERROR_SUCCESS);

    // A field already in the hierarchy cannot be added again
    HandleGuard<FieldHandle, Field_Release> street;
    ASSERT_EQ(FindFieldByName(sample.tree, "address.street", street.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(FieldTree_AddRootChild(sample.tree, street), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_AddChild(sample.tree, address, street), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // A parent from another hierarchy is refused - nothing but the tree's
    // own index tells it apart from a field of this document
    SampleHierarchy other;
    BuildSampleHierarchy(other);

    HandleGuard<FieldHandle, Field_Release> other_address;
    ASSERT_EQ(FieldTree_GetRootChild(other.tree, 0, other_address.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> stray_dictionary;
    HandleGuard<FieldHandle, Field_Release> stray;
    CreateTextFieldDictionary(sample.file, "stray", stray_dictionary, stray);
    EXPECT_EQ(FieldTree_AddChild(sample.tree, other_address, stray), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // A terminal field carrying widget annotations cannot take child fields
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> radio_group;
    ASSERT_EQ(DictionaryObject_Create(radio_group.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(radio_group, "T", "gender");
    InsertNameEntry(radio_group, "FT", "Btn");
    RegisterIndirectObject(sample.file, radio_group);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> widget;
    ASSERT_EQ(DictionaryObject_Create(widget.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertNameEntry(widget, "Subtype", "Widget");
    RegisterIndirectObject(sample.file, widget);
    InsertReferenceArrayEntry(radio_group, "Kids", { widget.get() });

    HandleGuard<FieldHandle, Field_Release> radio_field;
    ASSERT_EQ(Field_CreateFromDictionary(radio_group, radio_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_AddRootChild(sample.tree, radio_field), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(FieldTree_AddChild(sample.tree, radio_field, stray), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // A field merged with its widget annotation cannot take child fields
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> merged;
    ASSERT_EQ(DictionaryObject_Create(merged.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(merged, "T", "merged");
    InsertNameEntry(merged, "FT", "Tx");
    InsertNameEntry(merged, "Subtype", "Widget");
    RegisterIndirectObject(sample.file, merged);

    HandleGuard<FieldHandle, Field_Release> merged_field;
    ASSERT_EQ(Field_CreateFromDictionary(merged, merged_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_AddRootChild(sample.tree, merged_field), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(FieldTree_AddChild(sample.tree, merged_field, stray), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Nothing but the accepted additions made it in
    ASSERT_EQ(GetRootChildCount(sample.tree), 5u);
    ASSERT_EQ(GetFieldCount(sample.tree), 6u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 3), "city");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 4), "gender");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 5), "merged");
}

// --- InsertChild ---

// The index is a position among the container's entries; the end appends
TEST(FieldTree, InsertChildAtPosition) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> first_dictionary;
    HandleGuard<FieldHandle, Field_Release> first;
    CreateTextFieldDictionary(sample.file, "first", first_dictionary, first);
    ASSERT_EQ(FieldTree_InsertRootChild(sample.tree, 0, first), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> middle_dictionary;
    HandleGuard<FieldHandle, Field_Release> middle;
    CreateTextFieldDictionary(sample.file, "middle", middle_dictionary, middle);
    ASSERT_EQ(FieldTree_InsertRootChild(sample.tree, 2, middle), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> last_dictionary;
    HandleGuard<FieldHandle, Field_Release> last;
    CreateTextFieldDictionary(sample.file, "last", last_dictionary, last);
    ASSERT_EQ(FieldTree_InsertRootChild(sample.tree, 4, last), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(GetRootChildCount(sample.tree), 5u);
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 0), "first");
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 1), "address");
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 2), "middle");
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 3), "email");
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 4), "last");

    // The flat view follows the same order
    ASSERT_EQ(GetFieldCount(sample.tree), 6u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 0), "first");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 1), "address.street");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 3), "middle");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 5), "last");

    // Past the end is out of range, and nothing changes
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> beyond_dictionary;
    HandleGuard<FieldHandle, Field_Release> beyond;
    CreateTextFieldDictionary(sample.file, "beyond", beyond_dictionary, beyond);
    EXPECT_EQ(FieldTree_InsertRootChild(sample.tree, 6, beyond), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(GetRootChildCount(sample.tree), 5u);

    // Under a group, the index counts the group's /Kids
    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 1, address.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> country_dictionary;
    HandleGuard<FieldHandle, Field_Release> country;
    CreateTextFieldDictionary(sample.file, "country", country_dictionary, country);
    ASSERT_EQ(FieldTree_InsertChild(sample.tree, address, 1, country), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(GetChildCount(address), 3u);
    EXPECT_EQ(GetChildQualifiedName(address, 0), "address.street");
    EXPECT_EQ(GetChildQualifiedName(address, 1), "address.country");
    EXPECT_EQ(GetChildQualifiedName(address, 2), "address.city");

    EXPECT_EQ(FieldTree_InsertChild(sample.tree, address, 4, beyond), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(GetChildCount(address), 3u);
}

// --- RemoveChild ---

// Removing a nested field takes it out of its group's /Kids and clears
// /Parent; removing a top-level field takes it out of /Fields
TEST(FieldTree, RemoveChildAtEitherLevel) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<FieldHandle, Field_Release> street;
    ASSERT_EQ(FindFieldByName(sample.tree, "address.street", street.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_RemoveChild(sample.tree, street), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 0, address.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(GetChildCount(address), 1u);
    EXPECT_EQ(GetChildQualifiedName(address, 0), "address.city");

    // /Parent is gone - the removed field neither walks up nor keeps the
    // group's name prefix
    HandleGuard<FieldHandle, Field_Release> street_parent;
    EXPECT_EQ(Field_GetParent(street, street_parent.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(GetQualifiedName(street), "street");

    ASSERT_EQ(GetFieldCount(sample.tree), 2u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 0), "address.city");
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 1), "email");

    HandleGuard<FieldHandle, Field_Release> email;
    ASSERT_EQ(FindFieldByName(sample.tree, "email", email.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_RemoveChild(sample.tree, email), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(GetRootChildCount(sample.tree), 1u);
    ASSERT_EQ(GetFieldCount(sample.tree), 1u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 0), "address.city");

    // Removing twice, or a field that never belonged, is refused
    EXPECT_EQ(FieldTree_RemoveChild(sample.tree, email), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> stranger_dictionary;
    HandleGuard<FieldHandle, Field_Release> stranger;
    CreateTextFieldDictionary(sample.file, "stranger", stranger_dictionary, stranger);
    EXPECT_EQ(FieldTree_RemoveChild(sample.tree, stranger), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// A group goes with its whole subtree; a group emptied by removals stays
TEST(FieldTree, RemoveGroupTakesSubtreeAndEmptiedGroupStays) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 0, address.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> city;
    ASSERT_EQ(Field_GetChild(address, 1, city.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_RemoveChild(sample.tree, city), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> street;
    ASSERT_EQ(Field_GetChild(address, 0, street.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_RemoveChild(sample.tree, street), VANILLAPDF_ERROR_SUCCESS);

    // The emptied group is still a top-level field, now enumerated as a
    // widget-less terminal until it receives a child again
    ASSERT_EQ(GetRootChildCount(sample.tree), 2u);
    EXPECT_EQ(GetRootChildQualifiedName(sample.tree, 0), "address");
    EXPECT_EQ(GetChildCount(address), 0u);
    ASSERT_EQ(GetFieldCount(sample.tree), 2u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 0), "address");

    // Rebuild the group and remove it as a whole
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> zip_dictionary;
    HandleGuard<FieldHandle, Field_Release> zip;
    CreateTextFieldDictionary(sample.file, "zip", zip_dictionary, zip);
    ASSERT_EQ(FieldTree_AddChild(sample.tree, address, zip), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(GetFieldQualifiedName(sample.tree, 0), "address.zip");

    ASSERT_EQ(FieldTree_RemoveChild(sample.tree, address), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(GetRootChildCount(sample.tree), 1u);
    ASSERT_EQ(GetFieldCount(sample.tree), 1u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 0), "email");

    HandleGuard<FieldHandle, Field_Release> gone;
    EXPECT_EQ(FindFieldByName(sample.tree, "address.zip", gone.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

// The walk is the authority for where a field lives, not its /Parent entry
// - a missing or mismatched one is the common way a file is already dirty,
// and navigating by it would remove from the wrong array or none at all
TEST(FieldTree, RemoveChildNavigatesByTheWalkNotByParent) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    // street loses its /Parent, city's /Parent points at the wrong node
    RemoveEntry(sample.street, "Parent");
    RemoveEntry(sample.city, "Parent");
    InsertParentEntry(sample.city, sample.email);
    ASSERT_EQ(FieldTree_Invalidate(sample.tree), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 0, address.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(GetChildCount(address), 2u);

    HandleGuard<FieldHandle, Field_Release> street;
    ASSERT_EQ(Field_GetChild(address, 0, street.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_RemoveChild(sample.tree, street), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(GetChildCount(address), 1u);
    EXPECT_EQ(GetRootChildCount(sample.tree), 2u);

    HandleGuard<FieldHandle, Field_Release> city;
    ASSERT_EQ(Field_GetChild(address, 0, city.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_RemoveChild(sample.tree, city), VANILLAPDF_ERROR_SUCCESS);

    // Both left the group's /Kids; the wrong /Parent target was not touched
    EXPECT_EQ(GetChildCount(address), 0u);
    EXPECT_EQ(GetRootChildCount(sample.tree), 2u);

    HandleGuard<FieldHandle, Field_Release> email;
    ASSERT_EQ(FieldTree_GetRootChild(sample.tree, 1, email.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetChildCount(email), 0u);

    HandleGuard<FieldHandle, Field_Release> city_parent;
    EXPECT_EQ(Field_GetParent(city, city_parent.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

// --- Invalidate ---

// The tree cannot observe edits made underneath it through the dictionary
// API; Invalidate is the contract for that path
TEST(FieldTree, InvalidateAfterRawEdit) {
    SampleHierarchy sample;
    BuildSampleHierarchy(sample);

    ASSERT_EQ(GetFieldCount(sample.tree), 3u);

    // Drop the /Fields entry for the email field behind the tree's back,
    // through the form dictionary itself
    HandleGuard<NameObjectHandle, NameObject_Release> fields_key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Fields", fields_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> fields_object;
    ASSERT_EQ(DictionaryObject_Find(sample.form_dictionary, fields_key, fields_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ArrayObjectHandle, ArrayObject_Release> fields_array;
    ASSERT_EQ(ArrayObject_FromObject(fields_object, fields_array.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Remove(fields_array, 1), VANILLAPDF_ERROR_SUCCESS);

    // Stale until told otherwise
    EXPECT_EQ(GetFieldCount(sample.tree), 3u);

    ASSERT_EQ(FieldTree_Invalidate(sample.tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(GetFieldCount(sample.tree), 2u);
    EXPECT_EQ(GetFieldQualifiedName(sample.tree, 1), "address.city");
}

// --- Attachment ---

// A hierarchy created for a document is usable right away and can be
// populated before a form adopts it; once attached, the caller's handle is
// the form's hierarchy
TEST(FieldTree, CreatedTreeIsUsableBeforeAttachment) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    ASSERT_EQ(FieldTree_CreateFromDocument(document, tree.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetFieldCount(tree), 0u);
    EXPECT_EQ(GetRootChildCount(tree), 0u);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    HandleGuard<FieldHandle, Field_Release> field;
    CreateTextFieldDictionary(file, "email", field_dictionary, field);
    ASSERT_EQ(FieldTree_AddRootChild(tree, field), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetFieldCount(tree), 1u);
    EXPECT_EQ(GetRootChildCount(tree), 1u);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    ASSERT_EQ(InteractiveForm_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetFieldTree(form, tree), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldTreeHandle, FieldTree_Release> form_tree;
    ASSERT_EQ(InteractiveForm_GetFieldTree(form, form_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(form_tree.get(), tree.get());
    EXPECT_EQ(GetFieldCount(form_tree), 1u);
}

// A hierarchy holds references into the document it was created for -
// attached to a form of another document, they would serialize as dangling
// object numbers, so the attachment is refused and the form is left as it was
TEST(FieldTree, AttachingTreeOfAnotherDocumentIsRefused) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> other_io_stream;
    HandleGuard<FileHandle, File_Release> other_file;
    HandleGuard<DocumentHandle, Document_Release> other_document;
    CreateMemoryDocument(other_io_stream, other_file, other_document);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    ASSERT_EQ(InteractiveForm_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldTreeHandle, FieldTree_Release> other_tree;
    ASSERT_EQ(FieldTree_CreateFromDocument(other_document, other_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(InteractiveForm_SetFieldTree(form, other_tree), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<FieldTreeHandle, FieldTree_Release> missing_tree;
    EXPECT_EQ(InteractiveForm_GetFieldTree(form, missing_tree.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // The document's own hierarchy is accepted as before
    HandleGuard<FieldTreeHandle, FieldTree_Release> own_tree;
    ASSERT_EQ(FieldTree_CreateFromDocument(document, own_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(InteractiveForm_SetFieldTree(form, own_tree), VANILLAPDF_ERROR_SUCCESS);
}

// --- Persistence ---

// A hierarchy authored through the tree survives a save and reopen cycle
// with its structure, names and /Parent links intact
TEST(FieldTree, AuthoredHierarchyPersistsAcrossSave) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    ASSERT_EQ(InteractiveForm_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_SetAcroForm(catalog, form), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    ASSERT_EQ(FieldTree_CreateFromDocument(document, tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetFieldTree(form, tree), VANILLAPDF_ERROR_SUCCESS);

    // A group is a field with a name and no type of its own
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> address_dictionary;
    ASSERT_EQ(DictionaryObject_Create(address_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(address_dictionary, "T", "address");
    RegisterIndirectObject(file, address_dictionary);

    HandleGuard<FieldHandle, Field_Release> address;
    ASSERT_EQ(Field_CreateFromDictionary(address_dictionary, address.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_AddRootChild(tree, address), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> street_dictionary;
    HandleGuard<FieldHandle, Field_Release> street;
    CreateTextFieldDictionary(file, "street", street_dictionary, street);
    ASSERT_EQ(FieldTree_AddChild(tree, address, street), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> email_dictionary;
    HandleGuard<FieldHandle, Field_Release> email;
    CreateTextFieldDictionary(file, "email", email_dictionary, email);
    ASSERT_EQ(FieldTree_AddRootChild(tree, email), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_stream;
    HandleGuard<FileHandle, File_Release> destination_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(destination_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(destination_stream, "temp_destination", destination_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(document, destination_file), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> reloaded_file;
    HandleGuard<DocumentHandle, Document_Release> reloaded_document;
    HandleGuard<CatalogHandle, Catalog_Release> reloaded_catalog;
    ASSERT_EQ(File_OpenStream(destination_stream, "temp_destination", reloaded_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Initialize(reloaded_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(reloaded_file, reloaded_document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(reloaded_document, reloaded_catalog.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> reloaded_form;
    ASSERT_EQ(Catalog_GetAcroForm(reloaded_catalog, reloaded_form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldTreeHandle, FieldTree_Release> reloaded_tree;
    ASSERT_EQ(InteractiveForm_GetFieldTree(reloaded_form, reloaded_tree.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(GetFieldCount(reloaded_tree), 2u);
    EXPECT_EQ(GetFieldQualifiedName(reloaded_tree, 0), "address.street");
    EXPECT_EQ(GetFieldQualifiedName(reloaded_tree, 1), "email");

    ASSERT_EQ(GetRootChildCount(reloaded_tree), 2u);
    EXPECT_EQ(GetRootChildQualifiedName(reloaded_tree, 0), "address");
    EXPECT_EQ(GetRootChildQualifiedName(reloaded_tree, 1), "email");

    HandleGuard<FieldHandle, Field_Release> reloaded_street;
    ASSERT_EQ(FindFieldByName(reloaded_tree, "address.street", reloaded_street.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> reloaded_street_parent;
    ASSERT_EQ(Field_GetParent(reloaded_street, reloaded_street_parent.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(GetQualifiedName(reloaded_street_parent), "address");
}

} // namespace field_tree
