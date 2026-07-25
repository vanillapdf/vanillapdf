#include "unittest.h"
#include "handle_guard.h"

namespace interactive_forms {

// Creates an in-memory document together with its interactive form.
// Document_CreateAcroForm registers the form as an indirect object and
// attaches it to the catalog, which a bare dictionary could not do.
static void CreateMemoryDocumentWithForm(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document,
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release>& form
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateAcroForm(document, form.out()), VANILLAPDF_ERROR_SUCCESS);
}

// A document without an AcroForm gets one, registered and attached
TEST(InteractiveForm, CreateAcroFormOnDocumentWithoutOne) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    ASSERT_NE(form.get(), nullptr);

    // Once created it is reachable through the catalog like any other AcroForm
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> found_form;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetAcroForm(catalog, found_form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_form.get(), nullptr);
}

// Repeated calls must return the existing form rather than replacing it
TEST(InteractiveForm, CreateAcroFormIsIdempotent) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    ASSERT_EQ(InteractiveForm_SetNeedAppearances(form, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> second_form;
    ASSERT_EQ(Document_CreateAcroForm(document, second_form.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type need_appearances = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(InteractiveForm_GetNeedAppearances(second_form, &need_appearances), VANILLAPDF_ERROR_SUCCESS);
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
    EXPECT_EQ(Document_CreateAcroForm(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
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

// CreateFields inserts an empty /Fields array, which GetFields then finds
TEST(InteractiveForm, CreateFieldsThenGetFields) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldCollectionHandle, FieldCollection_Release> created_fields;
    ASSERT_EQ(InteractiveForm_CreateFields(form, created_fields.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(created_fields.get(), nullptr);

    size_type size = 1;
    ASSERT_EQ(FieldCollection_GetSize(created_fields, &size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(size, 0u);

    HandleGuard<FieldCollectionHandle, FieldCollection_Release> found_fields;
    ASSERT_EQ(InteractiveForm_GetFields(form, found_fields.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_fields.get(), nullptr);
}

// Repeated calls must reuse the existing /Fields array
TEST(InteractiveForm, CreateFieldsIsIdempotent) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<FieldCollectionHandle, FieldCollection_Release> first;
    HandleGuard<FieldCollectionHandle, FieldCollection_Release> second;
    ASSERT_EQ(InteractiveForm_CreateFields(form, first.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_CreateFields(form, second.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type size = 1;
    ASSERT_EQ(FieldCollection_GetSize(second, &size), VANILLAPDF_ERROR_SUCCESS);
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
    ASSERT_EQ(InteractiveForm_CreateSignatureFlags(form, flags.out()), VANILLAPDF_ERROR_SUCCESS);
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
    ASSERT_EQ(InteractiveForm_CreateSignatureFlags(form, flags.out()), VANILLAPDF_ERROR_SUCCESS);

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
    ASSERT_EQ(InteractiveForm_CreateSignatureFlags(form, flags.out()), VANILLAPDF_ERROR_SUCCESS);

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
    ASSERT_EQ(InteractiveForm_CreateSignatureFlags(form, created.out()), VANILLAPDF_ERROR_SUCCESS);
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
    ASSERT_EQ(InteractiveForm_CreateSignatureFlags(form, flags.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(SignatureFlags_GetSignaturesExist(flags, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureFlags, ToAndFromUnknown) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    CreateMemoryDocumentWithForm(io_stream, file, document, form);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> flags;
    ASSERT_EQ(InteractiveForm_CreateSignatureFlags(form, flags.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(SignatureFlags_ToUnknown(flags, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    HandleGuard<SignatureFlagsHandle, SignatureFlags_Release> converted;
    ASSERT_EQ(SignatureFlags_FromUnknown(unknown, converted.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted.get(), nullptr);
}

} // namespace interactive_forms
