#include "unittest.h"
#include "handle_guard.h"
#include "test_data.h"

#include "vanillapdf/c_vanillapdf_api.h"

#include <string>

namespace signature_field {

// SignatureField_CreateFromDocument registers a field at the root of the
// hierarchy; Field_AddWidget links a separately created widget through
// /Kids and /Parent, the page's /Annots being the page's own business.

static void CreateDocument(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(stream, "source.pdf", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
}

static void CreateName(const char* text, HandleGuard<StringObjectHandle, StringObject_Release>& name) {
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(text, literal.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_ToStringObject(literal, name.out()), VANILLAPDF_ERROR_SUCCESS);
}

static error_type CreateField(DocumentHandle* document, const char* text, HandleGuard<SignatureFieldHandle, SignatureField_Release>& field) {
    HandleGuard<StringObjectHandle, StringObject_Release> name;
    CreateName(text, name);
    return SignatureField_CreateFromDocument(document, name, field.out());
}

static void CreateWidget(DocumentHandle* document, real_type width, HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release>& widget) {
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<RectangleHandle, Rectangle_Release> rect;
    ASSERT_EQ(Rectangle_Create(rect.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightXReal(rect, width), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetRect(annotation, rect), VANILLAPDF_ERROR_SUCCESS);
}

static void GetWidgetWidth(WidgetAnnotationHandle* widget, real_type* width) {
    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<RectangleHandle, Rectangle_Release> rect;
    ASSERT_EQ(Annotation_GetRect(annotation, rect.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_GetUpperRightXReal(rect, width), VANILLAPDF_ERROR_SUCCESS);
}

static void WidgetHasEntry(WidgetAnnotationHandle* widget, const char* key, boolean_type* result) {
    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary;
    ASSERT_EQ(Annotation_GetBaseObject(annotation, dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> name;
    ASSERT_EQ(NameObject_CreateFromEncodedString(key, name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Contains(dictionary, name, result), VANILLAPDF_ERROR_SUCCESS);
}

static void GetFieldTree(DocumentHandle* document, HandleGuard<FieldTreeHandle, FieldTree_Release>& field_tree) {
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    ASSERT_EQ(Catalog_GetAcroForm(catalog, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetFieldTree(form, field_tree.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(SignatureField, CreateFromDocumentAttachesToHierarchy) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> field;
    ASSERT_EQ(CreateField(document, "Signature1", field), VANILLAPDF_ERROR_SUCCESS);

    // The form and the hierarchy were created on demand
    HandleGuard<FieldTreeHandle, FieldTree_Release> field_tree;
    GetFieldTree(document, field_tree);

    const std::string name = "Signature1";
    HandleGuard<FieldHandle, Field_Release> found;
    ASSERT_EQ(FieldTree_FindField(field_tree, name.c_str(), static_cast<size_type>(name.size()), found.out()), VANILLAPDF_ERROR_SUCCESS);

    FieldType type = FieldType_Undefined;
    ASSERT_EQ(Field_GetType(found, &type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(type, FieldType_Signature);

    size_type widget_count = 1;
    ASSERT_EQ(Field_GetWidgetCount(found, &widget_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(widget_count, 0u);

    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> signature;
    EXPECT_EQ(SignatureField_GetValue(field, signature.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

TEST(SignatureField, DuplicateNameIsRejected) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> first;
    HandleGuard<SignatureFieldHandle, SignatureField_Release> second;
    ASSERT_EQ(CreateField(document, "Signature1", first), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(CreateField(document, "Signature1", second), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<FieldTreeHandle, FieldTree_Release> field_tree;
    GetFieldTree(document, field_tree);

    size_type field_count = 0;
    ASSERT_EQ(FieldTree_GetFieldCount(field_tree, &field_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(field_count, 1u);
}

// A refused name leaves nothing behind - not even the form
TEST(SignatureField, NameWithPeriodIsRejected) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> field;
    EXPECT_EQ(CreateField(document, "Signature.1", field), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    EXPECT_EQ(Catalog_GetAcroForm(catalog, form.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
}

TEST(SignatureField, AddAndRemoveWidget) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    ASSERT_EQ(CreateField(document, "Signature1", signature_field), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(SignatureField_ToField(signature_field, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    CreateWidget(document, 200, widget);
    ASSERT_EQ(Field_AddWidget(field, widget), VANILLAPDF_ERROR_SUCCESS);

    size_type widget_count = 0;
    ASSERT_EQ(Field_GetWidgetCount(field, &widget_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(widget_count, 1u);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> found;
    ASSERT_EQ(Field_GetWidget(field, 0, found.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type width = 0;
    GetWidgetWidth(found, &width);
    EXPECT_EQ(width, 200);

    boolean_type has_parent = VANILLAPDF_RV_FALSE;
    WidgetHasEntry(widget, "Parent", &has_parent);
    EXPECT_EQ(has_parent, VANILLAPDF_RV_TRUE);

    // A widget belongs to one field
    EXPECT_EQ(Field_AddWidget(field, widget), VANILLAPDF_ERROR_PARAMETER_VALUE);

    boolean_type removed = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(Field_RemoveWidget(field, widget, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(Field_GetWidgetCount(field, &widget_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(widget_count, 0u);
    EXPECT_EQ(Field_GetWidget(field, 0, found.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    WidgetHasEntry(widget, "Parent", &has_parent);
    EXPECT_EQ(has_parent, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(Field_RemoveWidget(field, widget, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_FALSE);
}

// A field merged with its widget annotation is its own single widget
TEST(SignatureField, MergedFieldIsItsOwnWidget) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> merged;
    CreateWidget(document, 100, merged);

    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(merged, annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary;
    ASSERT_EQ(Annotation_GetBaseObject(annotation, dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> key;
    HandleGuard<NameObjectHandle, NameObject_Release> value;
    HandleGuard<ObjectHandle, Object_Release> value_object;
    ASSERT_EQ(NameObject_CreateFromEncodedString("FT", key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromEncodedString("Sig", value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_ToObject(value, value_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dictionary, key, value_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(dictionary, field.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type widget_count = 0;
    ASSERT_EQ(Field_GetWidgetCount(field, &widget_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(widget_count, 1u);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> found;
    ASSERT_EQ(Field_GetWidget(field, 0, found.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type width = 0;
    GetWidgetWidth(found, &width);
    EXPECT_EQ(width, 100);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> another;
    CreateWidget(document, 300, another);
    EXPECT_EQ(Field_AddWidget(field, another), VANILLAPDF_ERROR_PARAMETER_VALUE);

    boolean_type removed = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(Field_RemoveWidget(field, merged, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_FALSE);
}

TEST(SignatureField, SignsThroughSignField) {
    const std::string name = "Signature1";

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    ASSERT_EQ(CreateField(document, name.c_str(), signature_field), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(SignatureField_ToField(signature_field, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    CreateWidget(document, 200, widget);

    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> appearance;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetAppearance(annotation, AppearanceType_Normal, appearance), VANILLAPDF_ERROR_SUCCESS);

    // The page's half of the widget
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> page_tree;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, page_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_GetPage(page_tree, 1, page.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PageAnnotationsHandle, PageAnnotations_Release> page_annotations;
    ASSERT_EQ(PageAnnotations_Create(page_annotations.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageAnnotations_Append(page_annotations, annotation), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetAnnotations(page, page_annotations), VANILLAPDF_ERROR_SUCCESS);

    // The field's half
    ASSERT_EQ(Field_AddWidget(field, widget), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE), SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    ASSERT_EQ(DocumentSignatureSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    HandleGuard<FileHandle, File_Release> signed_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(signed_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(signed_stream, "signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SignField(document, signed_file, name.c_str(), static_cast<size_type>(name.size()), settings), VANILLAPDF_ERROR_SUCCESS);

    // Reopen: the field is signed and still carries the widget with its appearance
    HandleGuard<FileHandle, File_Release> reopened_file;
    HandleGuard<DocumentHandle, Document_Release> reopened;
    ASSERT_EQ(File_OpenStream(signed_stream, "signed.pdf", reopened_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(reopened_file, reopened.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldTreeHandle, FieldTree_Release> field_tree;
    GetFieldTree(reopened, field_tree);

    HandleGuard<FieldHandle, Field_Release> found;
    ASSERT_EQ(FieldTree_FindField(field_tree, name.c_str(), static_cast<size_type>(name.size()), found.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type widget_count = 0;
    ASSERT_EQ(Field_GetWidgetCount(found, &widget_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(widget_count, 1u);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> found_widget;
    ASSERT_EQ(Field_GetWidget(found, 0, found_widget.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type has_appearance = VANILLAPDF_RV_FALSE;
    WidgetHasEntry(found_widget, "AP", &has_appearance);
    EXPECT_EQ(has_appearance, VANILLAPDF_RV_TRUE);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> found_signature_field;
    ASSERT_EQ(SignatureField_FromField(found, found_signature_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> signature;
    ASSERT_EQ(SignatureField_GetValue(found_signature_field, signature.out()), VANILLAPDF_ERROR_SUCCESS);

    // Self-signed test certificate: the first pass extracts it, the second trusts it
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> result;
    HandleGuard<BufferHandle, Buffer_Release> signer_certificate;
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DigitalSignatureExtensions_Verify(signature, reopened, trust_store, nullptr, result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureVerificationResult_GetSignerCertificate(result, signer_certificate.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, signer_certificate), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DigitalSignatureExtensions_Verify(signature, reopened, trust_store, nullptr, result.out()), VANILLAPDF_ERROR_SUCCESS);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(result, &status), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(status, SignatureStatus_Valid);
}

TEST(SignatureField, NullChecks) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<StringObjectHandle, StringObject_Release> name;
    CreateName("Signature1", name);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    EXPECT_EQ(SignatureField_CreateFromDocument(nullptr, name, signature_field.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(SignatureField_CreateFromDocument(document, nullptr, signature_field.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(SignatureField_CreateFromDocument(document, name, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    ASSERT_EQ(SignatureField_CreateFromDocument(document, name, signature_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(SignatureField_ToField(signature_field, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type count = 0;
    boolean_type removed = VANILLAPDF_RV_FALSE;
    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> found;
    EXPECT_EQ(Field_GetWidgetCount(nullptr, &count), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetWidgetCount(field, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetWidget(nullptr, 0, found.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_GetWidget(field, 0, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_AddWidget(nullptr, widget), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_AddWidget(field, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_RemoveWidget(nullptr, widget, &removed), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_RemoveWidget(field, nullptr, &removed), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Field_RemoveWidget(field, widget, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // namespace signature_field
