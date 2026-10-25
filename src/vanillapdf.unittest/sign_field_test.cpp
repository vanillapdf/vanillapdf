#include "unittest.h"
#include "handle_guard.h"
#include "test_data.h"

#include "vanillapdf/c_vanillapdf_api.h"

#include <string>

namespace sign_field {

// Document_SignField signs into a signature field the caller has already built
// and attached, rather than creating an invisible one of its own. The field is
// addressed by its fully qualified name, so resolving it is what establishes
// that it belongs to this document and hangs off the interactive form.

static void InsertName(DictionaryObjectHandle* dictionary, const char* key, const char* value) {
    HandleGuard<NameObjectHandle, NameObject_Release> key_object;
    ASSERT_EQ(NameObject_CreateFromEncodedString(key, key_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> value_object;
    ASSERT_EQ(NameObject_CreateFromEncodedString(value, value_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> value_base;
    ASSERT_EQ(NameObject_ToObject(value_object, value_base.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dictionary, key_object, value_base, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

static void InsertPartialName(DictionaryObjectHandle* dictionary, const std::string& name) {
    HandleGuard<NameObjectHandle, NameObject_Release> key_object;
    ASSERT_EQ(NameObject_CreateFromEncodedString("T", key_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name_object;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString(name.c_str(), name_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> name_string;
    ASSERT_EQ(LiteralStringObject_ToStringObject(name_object, name_string.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> name_base;
    ASSERT_EQ(StringObject_ToObject(name_string, name_base.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dictionary, key_object, name_base, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
}

// A widget merged with a field of the given type, carrying an appearance, put
// into the document's field hierarchy - the shape a caller building a visible
// signature produces. The appearance is a bare form XObject; its content does
// not matter here, only that signing leaves it alone.
static void AddField(DocumentHandle* document, const std::string& name, const char* field_type) {
    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> appearance;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetAppearance(annotation, AppearanceType_Normal, appearance), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    ASSERT_EQ(Annotation_GetBaseObject(annotation, field_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    // No semantic accessor sets /FT yet, so the merge goes through the dictionary
    InsertName(field_dictionary, "FT", field_type);
    InsertPartialName(field_dictionary, name);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(field_dictionary, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> field_tree;

    if (Catalog_GetAcroForm(catalog, form.out()) == VANILLAPDF_ERROR_SUCCESS) {
        ASSERT_EQ(InteractiveForm_GetFieldTree(form, field_tree.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(FieldTree_AddRootChild(field_tree, field), VANILLAPDF_ERROR_SUCCESS);
        return;
    }

    ASSERT_EQ(InteractiveForm_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_CreateFromDocument(document, field_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_AddRootChild(field_tree, field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetFieldTree(form, field_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_SetAcroForm(catalog, form), VANILLAPDF_ERROR_SUCCESS);
}

static void CreateDocument(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(stream, "source.pdf", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
}

static void CreateSettings(HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release>& settings) {
    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
        SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DocumentSignatureSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);
}

static error_type SignFieldInto(
    DocumentHandle* document,
    const std::string& name,
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& signed_stream
) {
    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    CreateSettings(settings);

    HandleGuard<FileHandle, File_Release> signed_file;
    EXPECT_EQ(InputOutputStream_CreateFromMemory(signed_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(File_CreateStream(signed_stream, "signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);

    return Document_SignField(document, signed_file, name.c_str(),
        static_cast<size_type>(name.size()), settings);
}

// Resolves the named signature field in a signed stream
static void OpenSignedField(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& signed_stream,
    const std::string& name,
    HandleGuard<FileHandle, File_Release>& signed_file,
    HandleGuard<DocumentHandle, Document_Release>& signed_document,
    HandleGuard<FieldHandle, Field_Release>& field
) {
    ASSERT_EQ(File_OpenStream(signed_stream, "signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(signed_file, signed_document.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(signed_document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    ASSERT_EQ(Catalog_GetAcroForm(catalog, form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldTreeHandle, FieldTree_Release> field_tree;
    ASSERT_EQ(InteractiveForm_GetFieldTree(form, field_tree.out()), VANILLAPDF_ERROR_SUCCESS);

    // The caller's field is the only one - the library did not add its own
    size_type field_count = 0;
    ASSERT_EQ(FieldTree_GetFieldCount(field_tree, &field_count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_count, 1u);

    ASSERT_EQ(FieldTree_FindField(field_tree, name.c_str(),
        static_cast<size_type>(name.size()), field.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(SignField, SignsTheNamedFieldAndLeavesItsAppearance) {
    const std::string name = "VisibleSignature";

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);
    AddField(document, name, "Sig");

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    ASSERT_EQ(SignFieldInto(document, name, signed_stream), VANILLAPDF_ERROR_SUCCESS);


    HandleGuard<FileHandle, File_Release> signed_file;
    HandleGuard<DocumentHandle, Document_Release> signed_document;
    HandleGuard<FieldHandle, Field_Release> field;
    OpenSignedField(signed_stream, name, signed_file, signed_document, field);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    ASSERT_EQ(SignatureField_FromField(field, signature_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> signature;
    ASSERT_EQ(SignatureField_GetValue(signature_field, signature.out()), VANILLAPDF_ERROR_SUCCESS);

    // Signing writes /V and leaves the caller's appearance untouched
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    ASSERT_EQ(Field_GetBaseObject(field, field_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> appearance_key;
    ASSERT_EQ(NameObject_CreateFromEncodedString("AP", appearance_key.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type has_appearance = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DictionaryObject_Contains(field_dictionary, appearance_key, &has_appearance), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(has_appearance, VANILLAPDF_RV_TRUE);
}

TEST(SignField, SignatureVerifies) {
    const std::string name = "VisibleSignature";

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);
    AddField(document, name, "Sig");

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    ASSERT_EQ(SignFieldInto(document, name, signed_stream), VANILLAPDF_ERROR_SUCCESS);


    HandleGuard<FileHandle, File_Release> signed_file;
    HandleGuard<DocumentHandle, Document_Release> signed_document;
    HandleGuard<FieldHandle, Field_Release> field;
    OpenSignedField(signed_stream, name, signed_file, signed_document, field);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    ASSERT_EQ(SignatureField_FromField(field, signature_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> signature;
    ASSERT_EQ(SignatureField_GetValue(signature_field, signature.out()), VANILLAPDF_ERROR_SUCCESS);

    // The test certificate is self-signed, so the first pass extracts it and
    // the second trusts it
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> result;
    ASSERT_EQ(DigitalSignatureExtensions_Verify(signature, signed_document, trust_store, nullptr, result.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> signer_certificate;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCertificate(result, signer_certificate.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, signer_certificate), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DigitalSignatureExtensions_Verify(signature, signed_document, trust_store, nullptr, result.out()), VANILLAPDF_ERROR_SUCCESS);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(result, &status), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(status, SignatureStatus_Valid);
}

// The name is the fully qualified one, so a field below a group is reached
// through the group's partial name
TEST(SignField, SignsFieldBelowGroup) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> group_dictionary;
    HandleGuard<ObjectHandle, Object_Release> group_object;
    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> group_entry;
    ASSERT_EQ(DictionaryObject_Create(group_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertPartialName(group_dictionary, "Group");
    ASSERT_EQ(DictionaryObject_ToObject(group_dictionary, group_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_AllocateNewEntry(file, group_entry.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefUsedEntry_SetReference(group_entry, group_object), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> group;
    ASSERT_EQ(Field_CreateFromDictionary(group_dictionary, group.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> field_dictionary;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_GetBaseObject(annotation, field_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    InsertName(field_dictionary, "FT", "Sig");
    InsertPartialName(field_dictionary, "Signature1");

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(Field_CreateFromDictionary(field_dictionary, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> field_tree;
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_CreateFromDocument(document, field_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_AddRootChild(field_tree, group), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldTree_AddChild(field_tree, group, field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_SetFieldTree(form, field_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_SetAcroForm(catalog, form), VANILLAPDF_ERROR_SUCCESS);

    const std::string name = "Group.Signature1";

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    ASSERT_EQ(SignFieldInto(document, name, signed_stream), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> signed_file;
    HandleGuard<DocumentHandle, Document_Release> signed_document;
    HandleGuard<FieldHandle, Field_Release> signed_field;
    OpenSignedField(signed_stream, name, signed_file, signed_document, signed_field);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    ASSERT_EQ(SignatureField_FromField(signed_field, signature_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> signature;
    EXPECT_EQ(SignatureField_GetValue(signature_field, signature.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(SignField, UnknownNameIsRejected) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);
    AddField(document, "VisibleSignature", "Sig");

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    EXPECT_EQ(SignFieldInto(document, "NoSuchField", signed_stream), VANILLAPDF_ERROR_OBJECT_MISSING);
}

TEST(SignField, NonSignatureFieldIsRejected) {
    const std::string name = "PushButton";

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);
    AddField(document, name, "Btn");

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    EXPECT_EQ(SignFieldInto(document, name, signed_stream), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// The signed file is written whole, so a document carrying a signature is refused
TEST(SignField, AlreadySignedDocumentIsRejected) {
    const std::string name = "VisibleSignature";

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);
    AddField(document, name, "Sig");

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    ASSERT_EQ(SignFieldInto(document, name, signed_stream), VANILLAPDF_ERROR_SUCCESS);


    HandleGuard<FileHandle, File_Release> signed_file;
    HandleGuard<DocumentHandle, Document_Release> signed_document;
    ASSERT_EQ(File_OpenStream(signed_stream, "signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(signed_file, signed_document.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> resigned_stream;
    EXPECT_EQ(SignFieldInto(signed_document, name, resigned_stream), VANILLAPDF_ERROR_NOT_SUPPORTED);
}

TEST(SignField, NullChecks) {
    const std::string name = "VisibleSignature";
    const size_type name_size = static_cast<size_type>(name.size());

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateDocument(stream, file, document);
    AddField(document, name, "Sig");

    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    CreateSettings(settings);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    HandleGuard<FileHandle, File_Release> signed_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(signed_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(signed_stream, "signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(Document_SignField(nullptr, signed_file, name.c_str(), name_size, settings), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Document_SignField(document, nullptr, name.c_str(), name_size, settings), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Document_SignField(document, signed_file, nullptr, name_size, settings), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Document_SignField(document, signed_file, name.c_str(), name_size, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // namespace sign_field
