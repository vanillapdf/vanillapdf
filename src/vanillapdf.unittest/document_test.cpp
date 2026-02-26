#include "unittest.h"
#include "handle_guard.h"
#include "test_data.h"

namespace documents {

TEST(DocumentEncryptionSettings, CreateRelease) {
    HandleGuard<DocumentEncryptionSettingsHandle, DocumentEncryptionSettings_Release> encryption_settings;

    ASSERT_EQ(DocumentEncryptionSettings_Create(encryption_settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(encryption_settings.get(), nullptr);
}

TEST(DocumentEncryptionSettings, PermissionsFlagMaxValue) {
    HandleGuard<DocumentEncryptionSettingsHandle, DocumentEncryptionSettings_Release> encryption_settings;

    UserAccessPermissionFlags permissions_flags = static_cast<UserAccessPermissionFlags>(
        UserAccessPermissionFlag_PrintDegraded | UserAccessPermissionFlag_ModifyContents |
        UserAccessPermissionFlag_CopyAndExtract | UserAccessPermissionFlag_AddAnnotations |
        UserAccessPermissionFlag_FillForms | UserAccessPermissionFlag_ExtractText |
        UserAccessPermissionFlag_AssembleDocument | UserAccessPermissionFlag_PrintFaithful);
    UserAccessPermissionFlags permissions_flags_check = UserAccessPermissionFlag_None;

    ASSERT_EQ(DocumentEncryptionSettings_Create(encryption_settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(encryption_settings.get(), nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, permissions_flags), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_GetUserAccessPermissions(encryption_settings, &permissions_flags_check), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(permissions_flags, permissions_flags_check);
}

void EncryptDocument(
    std::string owner_password,
    std::string user_password,
    EncryptionAlgorithmType encryption_algorithm,
    integer_type encryption_key_length,
    UserAccessPermissionFlags user_permissions) {

    HandleGuard<FileHandle, File_Release> memory_file;
    HandleGuard<DocumentHandle, Document_Release> memory_document;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<DocumentEncryptionSettingsHandle, DocumentEncryptionSettings_Release> encryption_settings;

    HandleGuard<FileHandle, File_Release> destination_save_file;
    HandleGuard<FileHandle, File_Release> destination_load_file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_io_stream;

    boolean_type destination_is_encrypted = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(io_stream.get(), nullptr);

    ASSERT_EQ(File_CreateStream(io_stream, "temp", memory_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(memory_file.get(), nullptr);

    ASSERT_EQ(Document_CreateFile(memory_file, memory_document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(memory_document.get(), nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_Create(encryption_settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(encryption_settings.get(), nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_SetAlgorithm(encryption_settings, encryption_algorithm), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetKeyLength(encryption_settings, encryption_key_length), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, user_permissions), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> owner_password_buffer;
    HandleGuard<BufferHandle, Buffer_Release> user_password_buffer;

    ASSERT_EQ(Buffer_CreateFromData(owner_password.data(), owner_password.length(), owner_password_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_CreateFromData(user_password.data(), user_password.length(), user_password_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_NE(owner_password_buffer.get(), nullptr);
    ASSERT_NE(user_password_buffer.get(), nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_SetOwnerPassword(encryption_settings, owner_password_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetUserPassword(encryption_settings, user_password_buffer), VANILLAPDF_ERROR_SUCCESS);

    // Add encryption dictionary and initialize keys
    ASSERT_EQ(Document_AddEncryption(memory_document, encryption_settings), VANILLAPDF_ERROR_SUCCESS);

    // Create the destination stream file for output
    ASSERT_EQ(InputOutputStream_CreateFromMemory(destination_io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_io_stream.get(), nullptr);

    ASSERT_EQ(File_CreateStream(destination_io_stream, "temp_destination", destination_save_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_save_file.get(), nullptr);

    // Save the file into destination memory stream
    ASSERT_EQ(Document_SaveFile(memory_document, destination_save_file), VANILLAPDF_ERROR_SUCCESS);

    // Check the destination file for consistency
    ASSERT_EQ(File_OpenStream(destination_io_stream, "temp_destination", destination_load_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_load_file.get(), nullptr);

    ASSERT_EQ(File_Initialize(destination_load_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_IsEncrypted(destination_load_file, &destination_is_encrypted), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(destination_is_encrypted, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(File_SetEncryptionPassword(destination_load_file, owner_password.data()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_SetEncryptionPassword(destination_load_file, user_password.data()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Document, Encrypt_RC4_40) {
    EncryptDocument("owner", "user", EncryptionAlgorithmType_RC4, 40, UserAccessPermissionFlag_None);
}

TEST(Document, Encrypt_RC4_128) {
    EncryptDocument("owner", "user", EncryptionAlgorithmType_RC4, 128, UserAccessPermissionFlag_None);
}

TEST(Document, Encrypt_AES_40) {
    EncryptDocument("owner", "user", EncryptionAlgorithmType_AES, 40, UserAccessPermissionFlag_None);
}

TEST(Document, Encrypt_AES_128) {
    EncryptDocument("owner", "user", EncryptionAlgorithmType_AES, 128, UserAccessPermissionFlag_None);
}

// TODO: FIX AES-256
//TEST(Document, Encrypt_AES_256) {
//	EncryptDocument("owner", "user", EncryptionAlgorithmType_AES, 256, UserAccessPermissionFlag_None);
//}

TEST(Document, Sign) {

    HandleGuard<FileHandle, File_Release> source_memory_file;
    HandleGuard<DocumentHandle, Document_Release> source_memory_document;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;

    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> signature_settings;
    HandleGuard<DateHandle, Date_Release> signing_time;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> signature_pkcs12_key;
    HandleGuard<BufferHandle, Buffer_Release> signing_key_data;

    HandleGuard<FileHandle, File_Release> destination_file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_io_stream;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(io_stream.get(), nullptr);

    ASSERT_EQ(File_CreateStream(io_stream, "temp", source_memory_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_memory_file.get(), nullptr);

    ASSERT_EQ(Document_CreateFile(source_memory_file, source_memory_document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_memory_document.get(), nullptr);

    // Create the destination stream file for output
    ASSERT_EQ(InputOutputStream_CreateFromMemory(destination_io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_io_stream.get(), nullptr);

    ASSERT_EQ(File_CreateStream(destination_io_stream, "temp_destination", destination_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_file.get(), nullptr);

    // Configure the signature settings
    ASSERT_EQ(DocumentSignatureSettings_Create(signature_settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_settings.get(), nullptr);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE), sizeof(SIGNING_CERTIFICATE), signing_key_data.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key_data.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(signing_key_data, nullptr, signature_pkcs12_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(signature_pkcs12_key, signing_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    ASSERT_EQ(Date_CreateCurrent(signing_time.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_time.get(), nullptr);

    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time), VANILLAPDF_ERROR_SUCCESS);

    // Sign the document and save it to the destination file
    ASSERT_EQ(Document_Sign(source_memory_document, destination_file, signature_settings), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Document, ButtonFieldConversion) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    HandleGuard<NameObjectHandle, NameObject_Release> ft_key;
    HandleGuard<NameObjectHandle, NameObject_Release> btn_value;
    HandleGuard<FieldHandle, Field_Release> field;
    HandleGuard<ButtonFieldHandle, ButtonField_Release> button_field;
    HandleGuard<FieldHandle, Field_Release> field_back;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict.get(), nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", ft_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Btn", btn_value.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(btn_value.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field.get(), nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Button);

    ASSERT_EQ(ButtonField_FromField(field, button_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(button_field.get(), nullptr);

    ASSERT_EQ(ButtonField_ToField(button_field, field_back.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back.get(), nullptr);
}

TEST(Document, TextFieldConversion) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    HandleGuard<NameObjectHandle, NameObject_Release> ft_key;
    HandleGuard<NameObjectHandle, NameObject_Release> tx_value;
    HandleGuard<FieldHandle, Field_Release> field;
    HandleGuard<TextFieldHandle, TextField_Release> text_field;
    HandleGuard<FieldHandle, Field_Release> field_back;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict.get(), nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", ft_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Tx", tx_value.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(tx_value.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field.get(), nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Text);

    ASSERT_EQ(TextField_FromField(field, text_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(text_field.get(), nullptr);

    ASSERT_EQ(TextField_ToField(text_field, field_back.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back.get(), nullptr);
}

TEST(Document, ChoiceFieldConversion) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    HandleGuard<NameObjectHandle, NameObject_Release> ft_key;
    HandleGuard<NameObjectHandle, NameObject_Release> ch_value;
    HandleGuard<FieldHandle, Field_Release> field;
    HandleGuard<ChoiceFieldHandle, ChoiceField_Release> choice_field;
    HandleGuard<FieldHandle, Field_Release> field_back;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict.get(), nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", ft_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Ch", ch_value.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(ch_value.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field.get(), nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Choice);

    ASSERT_EQ(ChoiceField_FromField(field, choice_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(choice_field.get(), nullptr);

    ASSERT_EQ(ChoiceField_ToField(choice_field, field_back.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back.get(), nullptr);
}

TEST(Document, SignatureFieldConversion) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    HandleGuard<NameObjectHandle, NameObject_Release> ft_key;
    HandleGuard<NameObjectHandle, NameObject_Release> sig_value;
    HandleGuard<FieldHandle, Field_Release> field;
    HandleGuard<SignatureFieldHandle, SignatureField_Release> signature_field;
    HandleGuard<FieldHandle, Field_Release> field_back;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict.get(), nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", ft_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Sig", sig_value.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(sig_value.get()), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field.get(), nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Signature);

    ASSERT_EQ(SignatureField_FromField(field, signature_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_field.get(), nullptr);

    ASSERT_EQ(SignatureField_ToField(signature_field, field_back.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back.get(), nullptr);
}

TEST(Document, NonTerminalFieldCreation) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    HandleGuard<FieldHandle, Field_Release> field;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict.get(), nullptr);

    ASSERT_EQ(Field_CreateFromDictionary(dict, field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field.get(), nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_NonTerminal);
}

} /* documents */
