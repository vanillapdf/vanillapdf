#include "unittest.h"
#include "handle_guard.h"
#include "test_data.h"

#include "vanillapdf/c_vanillapdf_api.h"

namespace digital_signature_objects {

// DocumentSignatureSettings reserved signature size

TEST(DocumentSignatureSettings, ReservedSignatureSizeRoundtrip) {
    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    ASSERT_EQ(DocumentSignatureSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type reserved_size = 0;
    ASSERT_EQ(DocumentSignatureSettings_GetReservedSignatureSize(settings, &reserved_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(reserved_size, 16384u);

    // Long-term validation embeds revocation information and needs more room
    ASSERT_EQ(DocumentSignatureSettings_SetReservedSignatureSize(settings, 65536), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_GetReservedSignatureSize(settings, &reserved_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(reserved_size, 65536u);
}

// Signs an empty in-memory document with the given reservation and hands back
// the signed stream, so the /Contents actually written can be inspected
static void SignWithReservedSize(
    size_type reserved_size,
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& signed_stream,
    error_type* sign_result
) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> source_stream;
    HandleGuard<FileHandle, File_Release> source_file;
    HandleGuard<DocumentHandle, Document_Release> source_document;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(source_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(source_stream, "reserved_source.pdf", source_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(source_file, source_document.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
        SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    ASSERT_EQ(DocumentSignatureSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetReservedSignatureSize(settings, reserved_size), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> signed_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(signed_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(signed_stream, "reserved_signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);

    *sign_result = Document_Sign(source_document, signed_file, settings);
}

// Reads back the /Contents of the single signature in a signed stream
static void ReadSignatureContentsSize(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& signed_stream,
    size_type* contents_size
) {
    HandleGuard<FileHandle, File_Release> signed_file;
    HandleGuard<DocumentHandle, Document_Release> signed_document;

    ASSERT_EQ(File_OpenStream(signed_stream, "reserved_signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(signed_file, signed_document.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    ASSERT_EQ(Document_GetCatalog(signed_document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> acro_form;
    ASSERT_EQ(Catalog_GetAcroForm(catalog, acro_form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldTreeHandle, FieldTree_Release> field_tree;
    ASSERT_EQ(InteractiveForm_GetFieldTree(acro_form, field_tree.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FieldHandle, Field_Release> field;
    ASSERT_EQ(FieldTree_GetField(field_tree, 0, field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    ASSERT_EQ(SignatureField_FromField(field, signature_field.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> digital_signature;
    ASSERT_EQ(SignatureField_GetValue(signature_field, digital_signature.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> contents;
    ASSERT_EQ(DigitalSignature_GetContents(digital_signature, contents.out()), VANILLAPDF_ERROR_SUCCESS);

    // The decoded value - two hexadecimal characters collapse into one byte
    HandleGuard<BufferHandle, Buffer_Release> contents_value;
    ASSERT_EQ(HexadecimalStringObject_GetValue(contents, contents_value.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    ASSERT_EQ(Buffer_GetData(contents_value, &data, contents_size), VANILLAPDF_ERROR_SUCCESS);
}

// The setting has to reach the placeholder actually written, not just round
// trip through the settings object
TEST(DocumentSignatureSettings, ReservedSignatureSizeDeterminesWrittenContents) {
    const size_type reserved_size = 4096;

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    error_type sign_result = VANILLAPDF_ERROR_GENERAL;
    SignWithReservedSize(reserved_size, signed_stream, &sign_result);
    ASSERT_EQ(sign_result, VANILLAPDF_ERROR_SUCCESS);

    size_type contents_size = 0;
    ReadSignatureContentsSize(signed_stream, &contents_size);

    // The signature is padded out to fill the reservation, so the decoded
    // /Contents is exactly the reserved size
    EXPECT_EQ(contents_size, reserved_size);
}

TEST(DocumentSignatureSettings, DefaultReservedSignatureSizeDeterminesWrittenContents) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    error_type sign_result = VANILLAPDF_ERROR_GENERAL;
    SignWithReservedSize(16384, signed_stream, &sign_result);
    ASSERT_EQ(sign_result, VANILLAPDF_ERROR_SUCCESS);

    size_type contents_size = 0;
    ReadSignatureContentsSize(signed_stream, &contents_size);
    EXPECT_EQ(contents_size, 16384u);
}

// A reservation too small for the signature has to fail rather than truncate
TEST(DocumentSignatureSettings, ReservedSignatureSizeTooSmallFails) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    error_type sign_result = VANILLAPDF_ERROR_SUCCESS;
    SignWithReservedSize(64, signed_stream, &sign_result);

    EXPECT_NE(sign_result, VANILLAPDF_ERROR_SUCCESS);
}

TEST(DocumentSignatureSettings, ReservedSignatureSizeRejectsZero) {
    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    ASSERT_EQ(DocumentSignatureSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(DocumentSignatureSettings_SetReservedSignatureSize(settings, 0), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // The rejected value leaves the previous one in place
    size_type reserved_size = 0;
    ASSERT_EQ(DocumentSignatureSettings_GetReservedSignatureSize(settings, &reserved_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(reserved_size, 16384u);
}

TEST(DocumentSignatureSettings, ReservedSignatureSizeNullChecks) {
    size_type reserved_size = 0;

    EXPECT_EQ(DocumentSignatureSettings_GetReservedSignatureSize(nullptr, &reserved_size), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(DocumentSignatureSettings_SetReservedSignatureSize(nullptr, 1024), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    ASSERT_EQ(DocumentSignatureSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(DocumentSignatureSettings_GetReservedSignatureSize(settings, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// DigitalSignature ToUnknown/FromUnknown null checks

TEST(DigitalSignature, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(DigitalSignature_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(DigitalSignature, FromUnknown_NullChecks) {
    DigitalSignatureHandle* sig = nullptr;

    EXPECT_EQ(DigitalSignature_FromUnknown(nullptr, &sig), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// Field ToUnknown/FromUnknown null checks

TEST(Field, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(Field_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Field, FromUnknown_NullChecks) {
    FieldHandle* field = nullptr;

    EXPECT_EQ(Field_FromUnknown(nullptr, &field), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// OutlineItemColor ToUnknown/FromUnknown null checks

TEST(OutlineItemColor, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(OutlineItemColor_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(OutlineItemColor, FromUnknown_NullChecks) {
    OutlineItemColorHandle* color = nullptr;

    EXPECT_EQ(OutlineItemColor_FromUnknown(nullptr, &color), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// OutlineItemFlags ToUnknown/FromUnknown null checks

TEST(OutlineItemFlags, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(OutlineItemFlags_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(OutlineItemFlags, FromUnknown_NullChecks) {
    OutlineItemFlagsHandle* flags = nullptr;

    EXPECT_EQ(OutlineItemFlags_FromUnknown(nullptr, &flags), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// EncryptionKey ToUnknown/FromUnknown null checks

TEST(EncryptionKey, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(EncryptionKey_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(EncryptionKey, FromUnknown_NullChecks) {
    EncryptionKeyHandle* key = nullptr;

    EXPECT_EQ(EncryptionKey_FromUnknown(nullptr, &key), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // namespace digital_signature_objects
