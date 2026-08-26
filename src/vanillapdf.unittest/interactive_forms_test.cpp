#include "unittest.h"
#include "handle_guard.h"

#include <cstring>

namespace interactive_forms {

// Creates an in-memory document together with an attached interactive form.
// Creating and attaching are separate steps - InteractiveForm_CreateFromDocument
// registers the form as an indirect object, Catalog_SetAcroForm attaches it.
static void CreateMemoryDocumentWithForm(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document,
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release>& form
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(InteractiveForm_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_SetAcroForm(catalog, form), VANILLAPDF_ERROR_SUCCESS);
}

// A form created from the document is not attached until Catalog_SetAcroForm,
// so merely creating one must not modify the document
TEST(InteractiveForm, CreateFromDocumentDoesNotAttach) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    ASSERT_EQ(InteractiveForm_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(form.get(), nullptr);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);

    InteractiveFormHandle* found_form = NULL;
    EXPECT_EQ(Catalog_GetAcroForm(catalog, &found_form), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(found_form, nullptr);
}

// After Catalog_SetAcroForm the form is reachable like any other AcroForm
TEST(InteractiveForm, SetAcroFormAttachesToCatalog) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> found_form;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetAcroForm(catalog, found_form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_form.get(), nullptr);
}

// The catalog stores an indirect reference, so the attached form has to
// survive a save and reopen cycle
TEST(InteractiveForm, AttachedFormPersistsAcrossSave) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    ASSERT_EQ(InteractiveForm_SetNeedAppearances(form, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

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

    boolean_type need_appearances = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(InteractiveForm_GetNeedAppearances(reloaded_form, &need_appearances), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(need_appearances, VANILLAPDF_RV_TRUE);
}

TEST(InteractiveForm, CreateFromDictionary) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary;
    ASSERT_EQ(DictionaryObject_Create(dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    ASSERT_EQ(InteractiveForm_CreateFromDictionary(dictionary, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(form.get(), nullptr);
}

TEST(InteractiveForm, CreateRejectsNullParameters) {
    EXPECT_EQ(InteractiveForm_CreateFromDocument(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(InteractiveForm_CreateFromDictionary(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(InteractiveForm, NeedAppearancesMissingOnBlankForm) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    boolean_type need_appearances = VANILLAPDF_RV_TRUE;
    EXPECT_EQ(InteractiveForm_GetNeedAppearances(form, &need_appearances), VANILLAPDF_ERROR_OBJECT_MISSING);
}

TEST(InteractiveForm, SetAndGetNeedAppearances) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    boolean_type need_appearances = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(InteractiveForm_SetNeedAppearances(form, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetNeedAppearances(form, &need_appearances), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(need_appearances, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(InteractiveForm_SetNeedAppearances(form, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetNeedAppearances(form, &need_appearances), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(need_appearances, VANILLAPDF_RV_FALSE);
}

TEST(InteractiveForm, ToAndFromUnknown) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(InteractiveForm_ToUnknown(form, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> converted;
    ASSERT_EQ(InteractiveForm_FromUnknown(unknown, converted.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted.get(), nullptr);
}

// --- SignatureFlags tests ---

TEST(SignatureFlags, MissingOnBlankForm) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    SignatureFlagsHandle* flags = nullptr;
    EXPECT_EQ(InteractiveForm_GetSignatureFlags(form, &flags), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(flags, nullptr);
}

// A freshly created /SigFlags entry has every flag cleared
TEST(SignatureFlags, CreatedFlagsStartCleared) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> flags;
    ASSERT_EQ(SignatureFlags_Create(flags.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetSignatureFlags(form, flags), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(flags.get(), nullptr);

    boolean_type signatures_exist = VANILLAPDF_RV_TRUE;
    boolean_type append_only = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureFlags_GetSignaturesExist(flags, &signatures_exist), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureFlags_GetAppendOnly(flags, &append_only), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(signatures_exist, VANILLAPDF_RV_FALSE);
    EXPECT_EQ(append_only, VANILLAPDF_RV_FALSE);
}

TEST(SignatureFlags, SetAndGetSignaturesExist) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> flags;
    ASSERT_EQ(SignatureFlags_Create(flags.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetSignatureFlags(form, flags), VANILLAPDF_ERROR_SUCCESS);

    boolean_type value = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(SignatureFlags_SetSignaturesExist(flags, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureFlags_GetSignaturesExist(flags, &value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(value, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(SignatureFlags_SetSignaturesExist(flags, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureFlags_GetSignaturesExist(flags, &value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(value, VANILLAPDF_RV_FALSE);
}

// The two flags occupy distinct bits and must not disturb each other
TEST(SignatureFlags, FlagsAreIndependent) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> flags;
    ASSERT_EQ(SignatureFlags_Create(flags.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetSignatureFlags(form, flags), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SignatureFlags_SetAppendOnly(flags, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    boolean_type signatures_exist = VANILLAPDF_RV_TRUE;
    boolean_type append_only = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureFlags_GetSignaturesExist(flags, &signatures_exist), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureFlags_GetAppendOnly(flags, &append_only), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(signatures_exist, VANILLAPDF_RV_FALSE);
    EXPECT_EQ(append_only, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(SignatureFlags_SetSignaturesExist(flags, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureFlags_GetSignaturesExist(flags, &signatures_exist), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureFlags_GetAppendOnly(flags, &append_only), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(signatures_exist, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(append_only, VANILLAPDF_RV_TRUE);
}

// Changes made through the handle have to be visible on the form itself,
// confirming the handle wraps the live /SigFlags object rather than a copy
TEST(SignatureFlags, ChangesAreVisibleThroughTheForm) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> created;
    ASSERT_EQ(SignatureFlags_Create(created.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetSignatureFlags(form, created), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureFlags_SetSignaturesExist(created, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> found;
    ASSERT_EQ(InteractiveForm_GetSignatureFlags(form, found.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type signatures_exist = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureFlags_GetSignaturesExist(found, &signatures_exist), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(signatures_exist, VANILLAPDF_RV_TRUE);
}

TEST(SignatureFlags, RejectNullParameters) {
    boolean_type value = VANILLAPDF_RV_FALSE;

    EXPECT_EQ(SignatureFlags_GetSignaturesExist(nullptr, &value), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(SignatureFlags_GetAppendOnly(nullptr, &value), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(SignatureFlags_SetSignaturesExist(nullptr, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(SignatureFlags_SetAppendOnly(nullptr, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> flags;
    ASSERT_EQ(SignatureFlags_Create(flags.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetSignatureFlags(form, flags), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(SignatureFlags_GetSignaturesExist(flags, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureFlags, ToAndFromUnknown) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> flags;
    ASSERT_EQ(SignatureFlags_Create(flags.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetSignatureFlags(form, flags), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(SignatureFlags_ToUnknown(flags, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> converted;
    ASSERT_EQ(SignatureFlags_FromUnknown(unknown, converted.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted.get(), nullptr);
}

// --- Flat terminal field enumeration ---

// Creates an in-memory document without a form, needed to register field
// dictionaries as indirect objects
static void CreateMemoryDocument(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
}

// Registers the dictionary as an indirect object within the file, so that
// references to it resolve during hierarchy walks
static void RegisterIndirectObject(FileHandle* file, DictionaryObjectHandle* dict) {
    HandleGuard<ObjectHandle, Object_Release> dict_object;
    ASSERT_EQ(DictionaryObject_ToObject(dict, dict_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> entry;
    ASSERT_EQ(File_AllocateNewEntry(file, entry.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefUsedEntry_SetReference(entry, dict_object), VANILLAPDF_ERROR_SUCCESS);
}

// Creates a fresh indirect reference to an already registered dictionary
static void CreateReferenceTo(
    DictionaryObjectHandle* dict,
    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release>& reference
) {
    HandleGuard<ObjectHandle, Object_Release> dict_object;
    ASSERT_EQ(DictionaryObject_ToObject(dict, dict_object.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(IndirectReferenceObject_Create(reference.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IndirectReferenceObject_SetReferencedObject(reference, dict_object), VANILLAPDF_ERROR_SUCCESS);
}

// Inserts an array of indirect references under the given key - both /Fields
// and /Kids shall contain indirect references (Table 218, Table 220)
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

// Links a child dictionary to its parent through the /Parent entry, which
// shall be an indirect reference (Table 220)
static void InsertParentEntry(DictionaryObjectHandle* child, DictionaryObjectHandle* parent) {
    HandleGuard<IndirectReferenceObjectHandle, IndirectReferenceObject_Release> reference;
    CreateReferenceTo(parent, reference);

    HandleGuard<ObjectHandle, Object_Release> reference_object;
    ASSERT_EQ(IndirectReferenceObject_ToObject(reference, reference_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Parent", key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(child, key, reference_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Inserts a name entry into a dictionary
static void InsertNameEntry(DictionaryObjectHandle* dict, const char* key_name, const char* value) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<NameObjectHandle, NameObject_Release> name_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString(value, name_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> value_object;
    ASSERT_EQ(NameObject_ToObject(name_value, value_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, value_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Inserts a literal string entry into a dictionary
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

// Inserts an integer entry into a dictionary
static void InsertIntegerEntry(DictionaryObjectHandle* dict, const char* key_name, bigint_type value) {
    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer_value;
    ASSERT_EQ(NameObject_CreateFromDecodedString(key_name, key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_CreateFromIntegerValue(value, integer_value.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> value_object;
    ASSERT_EQ(IntegerObject_ToObject(integer_value, value_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict, key, value_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// Reads the buffer contents into a std::string for comparisons
static std::string BufferToString(BufferHandle* buffer) {
    string_type data = nullptr;
    size_type size = 0;
    EXPECT_EQ(Buffer_GetData(buffer, &data, &size), VANILLAPDF_ERROR_SUCCESS);
    return std::string(data, size);
}

// Looks a terminal field up by its fully qualified name given as a literal
static error_type FindFieldByName(FieldTreeHandle* tree, const char* qualified_name, FieldHandle** result) {
    return FieldTree_FindField(tree, qualified_name, static_cast<size_type>(strlen(qualified_name)), result);
}

// Reads the fully qualified name of the terminal field at the given index
static std::string GetFieldQualifiedName(FieldTreeHandle* tree, size_type index) {
    HandleGuard<FieldHandle, Field_Release> field;
    EXPECT_EQ(FieldTree_GetField(tree, index, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> qualified_name;
    EXPECT_EQ(Field_GetQualifiedName(field, qualified_name.out()), VANILLAPDF_ERROR_SUCCESS);

    return BufferToString(qualified_name);
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

// Wraps a form dictionary and obtains its field hierarchy
static void CreateFormWithTree(
    DictionaryObjectHandle* form_dictionary,
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release>& form,
    HandleGuard<FieldTreeHandle, FieldTree_Release>& tree
) {
    ASSERT_EQ(InteractiveForm_CreateFromDictionary(form_dictionary, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetFieldTree(form, tree.out()), VANILLAPDF_ERROR_SUCCESS);
}

// Gives a blank form an empty hierarchy, the way a document author starts
static void AttachEmptyTree(
    DocumentHandle* document,
    InteractiveFormHandle* form,
    HandleGuard<FieldTreeHandle, FieldTree_Release>& tree
) {
    ASSERT_EQ(FieldTree_CreateFromDocument(document, tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetFieldTree(form, tree), VANILLAPDF_ERROR_SUCCESS);
}

// Creates a registered terminal text field dictionary with the given name
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

// Document-wide defaults for /DA and /Q roundtrip through the form setters
TEST(InteractiveForm, SetAndGetDocumentDefaults) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<StringObjectHandle, StringObject_Release> appearance;
    CreateStringObject("/Helv 0 Tf 0 g", appearance);
    ASSERT_EQ(InteractiveForm_SetDefaultAppearance(form, appearance), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetQuadding(form, QuaddingType_Centered), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> read_appearance;
    ASSERT_EQ(InteractiveForm_GetDefaultAppearance(form, read_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> read_appearance_value;
    ASSERT_EQ(StringObject_GetValue(read_appearance, read_appearance_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(read_appearance_value), "/Helv 0 Tf 0 g");

    QuaddingType quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(InteractiveForm_GetQuadding(form, &quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(quadding, QuaddingType_Centered);
}

// A blank form has no /Fields entry - reading it never creates one, so
// there is no hierarchy to hand out until one is attached
TEST(InteractiveForm, FieldTreeMissingOnBlankForm) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    FieldTreeHandle* missing_tree = nullptr;
    EXPECT_EQ(InteractiveForm_GetFieldTree(form, &missing_tree), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(missing_tree, nullptr);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    AttachEmptyTree(document, form, tree);

    size_type count = 1;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(count, 0u);

    ASSERT_EQ(FieldTree_GetRootChildCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(count, 0u);

    HandleGuard<FieldHandle, Field_Release> field;
    EXPECT_EQ(FieldTree_GetField(tree, 0, field.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(FieldTree_GetRootChild(tree, 0, field.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(FindFieldByName(tree, "missing", field.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // The attached instance is the one the form hands out from now on
    HandleGuard<FieldTreeHandle, FieldTree_Release> found_tree;
    ASSERT_EQ(InteractiveForm_GetFieldTree(form, found_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(found_tree.get(), tree.get());
}

// The enumeration hides grouping nodes and yields the logical terminal
// fields in document order, with attributes resolved through the hierarchy
TEST(InteractiveForm, FlatEnumerationSkipsNonTerminals) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    // Non-terminal group carrying the field type for its children to inherit
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> group;
    ASSERT_EQ(DictionaryObject_Create(group.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(group, "T", "group");
    InsertNameEntry(group, "FT", "Tx");
    RegisterIndirectObject(file, group);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> first_child;
    ASSERT_EQ(DictionaryObject_Create(first_child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(first_child, "T", "first");
    RegisterIndirectObject(file, first_child);
    InsertParentEntry(first_child, group);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> second_child;
    ASSERT_EQ(DictionaryObject_Create(second_child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(second_child, "T", "second");
    RegisterIndirectObject(file, second_child);
    InsertParentEntry(second_child, group);

    InsertReferenceArrayEntry(group, "Kids", { first_child.get(), second_child.get() });

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> standalone;
    ASSERT_EQ(DictionaryObject_Create(standalone.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(standalone, "T", "email");
    InsertNameEntry(standalone, "FT", "Tx");
    RegisterIndirectObject(file, standalone);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;
    ASSERT_EQ(DictionaryObject_Create(form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertReferenceArrayEntry(form_dictionary, "Fields", { group.get(), standalone.get() });

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    CreateFormWithTree(form_dictionary, form, tree);

    size_type count = 0;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(count, 3u);

    EXPECT_EQ(GetFieldQualifiedName(tree, 0), "group.first");
    EXPECT_EQ(GetFieldQualifiedName(tree, 1), "group.second");
    EXPECT_EQ(GetFieldQualifiedName(tree, 2), "email");

    // Children carry no /FT of their own - the type resolves from the group
    HandleGuard<FieldHandle, Field_Release> first_field;
    ASSERT_EQ(FieldTree_GetField(tree, 0, first_field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType first_field_type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(first_field, &first_field_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(first_field_type, FieldType_Text);

    boolean_type terminal = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Field_IsTerminal(first_field, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_TRUE);
}

// A radio button group is one logical field with one value - its widget
// annotations do not multiply the enumeration
TEST(InteractiveForm, RadioGroupIsSingleField) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> radio_group;
    ASSERT_EQ(DictionaryObject_Create(radio_group.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(radio_group, "T", "gender");
    InsertNameEntry(radio_group, "FT", "Btn");
    InsertIntegerEntry(radio_group, "Ff", FieldFlags_Radio);
    RegisterIndirectObject(file, radio_group);

    // Widget annotations carry no /T partial name
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> first_widget;
    ASSERT_EQ(DictionaryObject_Create(first_widget.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertNameEntry(first_widget, "Subtype", "Widget");
    RegisterIndirectObject(file, first_widget);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> second_widget;
    ASSERT_EQ(DictionaryObject_Create(second_widget.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertNameEntry(second_widget, "Subtype", "Widget");
    RegisterIndirectObject(file, second_widget);

    InsertReferenceArrayEntry(radio_group, "Kids", { first_widget.get(), second_widget.get() });

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;
    ASSERT_EQ(DictionaryObject_Create(form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertReferenceArrayEntry(form_dictionary, "Fields", { radio_group.get() });

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    CreateFormWithTree(form_dictionary, form, tree);

    size_type count = 0;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(count, 1u);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(FieldTree_GetField(tree, 0, field.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType field_type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(field_type, FieldType_Button);

    boolean_type terminal = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Field_IsTerminal(field, &terminal), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(terminal, VANILLAPDF_RV_TRUE);

    // The widgets are not children of the field
    size_type child_count = 1;
    ASSERT_EQ(Field_GetChildCount(field, &child_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(child_count, 0u);
}

TEST(InteractiveForm, FindFieldByQualifiedName) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> group;
    ASSERT_EQ(DictionaryObject_Create(group.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(group, "T", "group");
    InsertNameEntry(group, "FT", "Tx");
    RegisterIndirectObject(file, group);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> child;
    ASSERT_EQ(DictionaryObject_Create(child.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(child, "T", "first");
    RegisterIndirectObject(file, child);
    InsertParentEntry(child, group);

    InsertReferenceArrayEntry(group, "Kids", { child.get() });

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;
    ASSERT_EQ(DictionaryObject_Create(form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertReferenceArrayEntry(form_dictionary, "Fields", { group.get() });

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    CreateFormWithTree(form_dictionary, form, tree);

    HandleGuard<FieldHandle, Field_Release> found;
    ASSERT_EQ(FindFieldByName(tree, "group.first", found.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> partial_name;
    ASSERT_EQ(Field_GetName(found, partial_name.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> partial_name_value;
    ASSERT_EQ(StringObject_GetValue(partial_name, partial_name_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(partial_name_value), "first");

    // The grouping node is not part of the enumeration
    HandleGuard<FieldHandle, Field_Release> group_lookup;
    EXPECT_EQ(FindFieldByName(tree, "group", group_lookup.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<FieldHandle, Field_Release> missing;
    EXPECT_EQ(FindFieldByName(tree, "group.missing", missing.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

// Appending a field through AddChild must be reflected by the enumeration
// built before the change
TEST(InteractiveForm, FieldCacheInvalidatedOnAddChild) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> standalone;
    ASSERT_EQ(DictionaryObject_Create(standalone.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(standalone, "T", "email");
    InsertNameEntry(standalone, "FT", "Tx");
    RegisterIndirectObject(file, standalone);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;
    ASSERT_EQ(DictionaryObject_Create(form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertReferenceArrayEntry(form_dictionary, "Fields", { standalone.get() });

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    CreateFormWithTree(form_dictionary, form, tree);

    size_type count = 0;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(count, 1u);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> appended;
    HandleGuard<FieldHandle, Field_Release> appended_field;
    CreateTextFieldDictionary(file, "phone", appended, appended_field);

    ASSERT_EQ(FieldTree_AddRootChild(tree, appended_field), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(count, 2u);

    // Appending preserves document order
    EXPECT_EQ(GetFieldQualifiedName(tree, 0), "email");
    EXPECT_EQ(GetFieldQualifiedName(tree, 1), "phone");
}

// An empty hierarchy attached to a blank form accepts fields
TEST(InteractiveForm, AttachedTreeAcceptsFields) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    AttachEmptyTree(document, form, tree);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    HandleGuard<FieldHandle, Field_Release> field;
    CreateTextFieldDictionary(file, "email", field_dictionary, field);
    ASSERT_EQ(FieldTree_AddRootChild(tree, field), VANILLAPDF_ERROR_SUCCESS);

    size_type count = 0;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(count, 1u);

    HandleGuard<FieldHandle, Field_Release> found;
    EXPECT_EQ(FindFieldByName(tree, "email", found.out()), VANILLAPDF_ERROR_SUCCESS);
}

// The container arrays hold indirect references, so a field backed by a
// direct dictionary is refused instead of serializing a dangling reference
TEST(InteractiveForm, AddRootChildRejectsDirectDictionary) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    AttachEmptyTree(document, form, tree);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    ASSERT_EQ(DictionaryObject_Create(field_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(field_dictionary, "T", "email");
    InsertNameEntry(field_dictionary, "FT", "Tx");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(field_dictionary, field.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(FieldTree_AddRootChild(tree, field), VANILLAPDF_ERROR_PARAMETER_VALUE);

    size_type count = 1;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(count, 0u);
}

TEST(InteractiveForm, FieldTreeRejectsNullParameters) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    AttachEmptyTree(document, form, tree);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    HandleGuard<FieldHandle, Field_Release> field;
    CreateTextFieldDictionary(file, "email", field_dictionary, field);

    EXPECT_EQ(InteractiveForm_GetFieldTree(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(InteractiveForm_SetFieldTree(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(InteractiveForm_SetFieldTree(form, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(InteractiveForm_ResolveDefaultAppearance(nullptr, field, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(InteractiveForm_ResolveDefaultAppearance(form, nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(InteractiveForm_ResolveQuadding(nullptr, field, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(InteractiveForm_ResolveQuadding(form, nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_CreateFromDocument(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_GetFieldCount(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_GetField(nullptr, 0, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_FindField(tree, nullptr, 0, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_FindField(tree, "email", 5, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_GetRootChildCount(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_GetRootChild(nullptr, 0, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_AddRootChild(nullptr, field), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_AddRootChild(tree, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_InsertRootChild(tree, 0, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_AddChild(nullptr, nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_AddChild(tree, field, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_AddChild(tree, nullptr, field), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_InsertChild(tree, field, 0, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_RemoveChild(tree, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(FieldTree_Invalidate(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetChildCount(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetChild(nullptr, 0, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetParent(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetQualifiedName(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetValue(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetDefaultValue(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Nothing above reached the hierarchy
    size_type count = 1;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(count, 0u);
}

// The attached hierarchy and the appended reference have to survive a save
// and reopen cycle
TEST(InteractiveForm, AddedFieldPersistsAcrossSave) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    AttachEmptyTree(document, form, tree);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    HandleGuard<FieldHandle, Field_Release> field;
    CreateTextFieldDictionary(file, "email", field_dictionary, field);
    ASSERT_EQ(FieldTree_AddRootChild(tree, field), VANILLAPDF_ERROR_SUCCESS);

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

    size_type count = 0;
    ASSERT_EQ(FieldTree_GetFieldCount(reloaded_tree, &count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(count, 1u);
    EXPECT_EQ(GetFieldQualifiedName(reloaded_tree, 0), "email");
}

// Malformed documents can link /Kids in a cycle; the enumeration has to
// recognize the revisit and terminate
TEST(InteractiveForm, CyclicKidsTerminates) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> first_group;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> second_group;
    ASSERT_EQ(DictionaryObject_Create(first_group.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Create(second_group.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(first_group, "T", "first");
    InsertStringEntry(second_group, "T", "second");
    RegisterIndirectObject(file, first_group);
    RegisterIndirectObject(file, second_group);

    InsertReferenceArrayEntry(first_group, "Kids", { second_group.get() });
    InsertReferenceArrayEntry(second_group, "Kids", { first_group.get() });

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;
    ASSERT_EQ(DictionaryObject_Create(form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertReferenceArrayEntry(form_dictionary, "Fields", { first_group.get() });

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    CreateFormWithTree(form_dictionary, form, tree);

    // Both nodes are non-terminal grouping nodes, so nothing is enumerated -
    // the point is that the call returns instead of looping forever
    size_type count = 1;
    ASSERT_EQ(FieldTree_GetFieldCount(tree, &count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(count, 0u);
}

// Fields resolve /DA and /Q through the /Parent chain only; the form owns
// the document default and performs the full lookup on request
TEST(InteractiveForm, DefaultAppearanceAndQuaddingFallback) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> plain_field;
    ASSERT_EQ(DictionaryObject_Create(plain_field.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(plain_field, "T", "notes");
    InsertNameEntry(plain_field, "FT", "Tx");
    RegisterIndirectObject(file, plain_field);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> styled_field;
    ASSERT_EQ(DictionaryObject_Create(styled_field.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(styled_field, "T", "title");
    InsertNameEntry(styled_field, "FT", "Tx");
    InsertStringEntry(styled_field, "DA", "/TiRo 12 Tf 0 g");
    InsertIntegerEntry(styled_field, "Q", 2);
    RegisterIndirectObject(file, styled_field);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_dictionary;
    ASSERT_EQ(DictionaryObject_Create(form_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertStringEntry(form_dictionary, "DA", "/Helv 0 Tf 0 g");
    InsertIntegerEntry(form_dictionary, "Q", 1);
    InsertReferenceArrayEntry(form_dictionary, "Fields", { plain_field.get(), styled_field.get() });

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    CreateFormWithTree(form_dictionary, form, tree);

    // The plain field carries no /DA or /Q - the caller falls back to the form
    HandleGuard<FieldHandle, Field_Release> plain;
    ASSERT_EQ(FieldTree_GetField(tree, 0, plain.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> plain_appearance;
    EXPECT_EQ(Field_GetDefaultAppearance(plain, plain_appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    QuaddingType plain_quadding = QuaddingType_LeftJustified;
    EXPECT_EQ(Field_GetQuadding(plain, &plain_quadding), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<StringObjectHandle, StringObject_Release> form_appearance;
    ASSERT_EQ(InteractiveForm_GetDefaultAppearance(form, form_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> form_appearance_value;
    ASSERT_EQ(StringObject_GetValue(form_appearance, form_appearance_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(form_appearance_value), "/Helv 0 Tf 0 g");

    QuaddingType form_quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(InteractiveForm_GetQuadding(form, &form_quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(form_quadding, QuaddingType_Centered);

    // The form performs that fallback itself on request
    HandleGuard<StringObjectHandle, StringObject_Release> plain_resolved_appearance;
    ASSERT_EQ(InteractiveForm_ResolveDefaultAppearance(form, plain, plain_resolved_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> plain_resolved_appearance_value;
    ASSERT_EQ(StringObject_GetValue(plain_resolved_appearance, plain_resolved_appearance_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(plain_resolved_appearance_value), "/Helv 0 Tf 0 g");

    QuaddingType plain_resolved_quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(InteractiveForm_ResolveQuadding(form, plain, &plain_resolved_quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(plain_resolved_quadding, QuaddingType_Centered);

    // The styled field carries its own /DA and /Q, which win over the form
    HandleGuard<FieldHandle, Field_Release> styled;
    ASSERT_EQ(FieldTree_GetField(tree, 1, styled.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> styled_appearance;
    ASSERT_EQ(Field_GetDefaultAppearance(styled, styled_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> styled_appearance_value;
    ASSERT_EQ(StringObject_GetValue(styled_appearance, styled_appearance_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(styled_appearance_value), "/TiRo 12 Tf 0 g");

    QuaddingType styled_quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(Field_GetQuadding(styled, &styled_quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(styled_quadding, QuaddingType_RightJustified);

    HandleGuard<StringObjectHandle, StringObject_Release> styled_resolved_appearance;
    ASSERT_EQ(InteractiveForm_ResolveDefaultAppearance(form, styled, styled_resolved_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> styled_resolved_appearance_value;
    ASSERT_EQ(StringObject_GetValue(styled_resolved_appearance, styled_resolved_appearance_value.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(BufferToString(styled_resolved_appearance_value), "/TiRo 12 Tf 0 g");

    QuaddingType styled_resolved_quadding = QuaddingType_LeftJustified;
    ASSERT_EQ(InteractiveForm_ResolveQuadding(form, styled, &styled_resolved_quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(styled_resolved_quadding, QuaddingType_RightJustified);
}

// With no /DA anywhere the lookup is honestly empty; /Q has a specification
// default (Table 222) and always resolves
TEST(InteractiveForm, ResolveWithoutDocumentDefaults) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    AttachEmptyTree(document, form, tree);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    HandleGuard<FieldHandle, Field_Release> field;
    CreateTextFieldDictionary(file, "notes", field_dictionary, field);
    ASSERT_EQ(FieldTree_AddRootChild(tree, field), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> appearance;
    EXPECT_EQ(InteractiveForm_ResolveDefaultAppearance(form, field, appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    QuaddingType quadding = QuaddingType_RightJustified;
    ASSERT_EQ(InteractiveForm_ResolveQuadding(form, field, &quadding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(quadding, QuaddingType_LeftJustified);
}

} // namespace interactive_forms
