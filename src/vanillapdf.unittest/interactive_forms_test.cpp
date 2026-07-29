#include "unittest.h"
#include "handle_guard.h"

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

// A blank form has no /Fields entry at all
TEST(InteractiveForm, GetFieldsMissing) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    FieldCollectionHandle* fields = nullptr;
    EXPECT_EQ(InteractiveForm_GetFields(form, &fields), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(fields, nullptr);
}

// SetFields inserts the /Fields array, which GetFields then finds
TEST(InteractiveForm, SetFieldsThenGetFields) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldCollectionHandle, FieldCollection_Release> created_fields;
    ASSERT_EQ(FieldCollection_Create(created_fields.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetFields(form, created_fields), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(created_fields.get(), nullptr);

    size_type size = 1;
    ASSERT_EQ(FieldCollection_GetSize(created_fields, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 0u);

    HandleGuard<FieldCollectionHandle, FieldCollection_Release> found_fields;
    ASSERT_EQ(InteractiveForm_GetFields(form, found_fields.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_fields.get(), nullptr);
}

// Setting fields twice must replace the previous array rather than throw
TEST(InteractiveForm, SetFieldsOverwrite) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldCollectionHandle, FieldCollection_Release> first;
    HandleGuard<FieldCollectionHandle, FieldCollection_Release> second;
    ASSERT_EQ(FieldCollection_Create(first.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldCollection_Create(second.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(InteractiveForm_SetFields(form, first), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetFields(form, second), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldCollectionHandle, FieldCollection_Release> found;
    ASSERT_EQ(InteractiveForm_GetFields(form, found.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type size = 1;
    ASSERT_EQ(FieldCollection_GetSize(found, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 0u);
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

} // namespace interactive_forms
