#include "unittest.h"
#include "test_data.h"

namespace documents {

TEST(DocumentEncryptionSettings, CreateRelease) {
    DocumentEncryptionSettingsHandle* encryption_settings = nullptr;

    ASSERT_EQ(DocumentEncryptionSettings_Create(&encryption_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(encryption_settings, nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_Release(encryption_settings), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DocumentEncryptionSettings, PermissionsFlagMaxValue) {
    DocumentEncryptionSettingsHandle* encryption_settings = nullptr;

    UserAccessPermissionFlags permissions_flags = static_cast<UserAccessPermissionFlags>(-1);
    UserAccessPermissionFlags permissions_flags_check = UserAccessPermissionFlag_None;

    ASSERT_EQ(DocumentEncryptionSettings_Create(&encryption_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(encryption_settings, nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, permissions_flags), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_GetUserAccessPermissions(encryption_settings, &permissions_flags_check), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(permissions_flags, permissions_flags_check);

    ASSERT_EQ(DocumentEncryptionSettings_Release(encryption_settings), VANILLAPDF_ERROR_SUCCESS);
}

void EncryptDocument(
    std::string owner_password,
    std::string user_password,
    EncryptionAlgorithmType encryption_algorithm,
    integer_type encryption_key_length,
    UserAccessPermissionFlags user_permissions) {

    FileHandle* memory_file = nullptr;
    DocumentHandle* memory_document = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    DocumentEncryptionSettingsHandle* encryption_settings = nullptr;

    FileHandle* destination_save_file = nullptr;
    FileHandle* destination_load_file = nullptr;
    InputOutputStreamHandle* destination_io_stream = nullptr;

    boolean_type destination_is_encrypted = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(io_stream, nullptr);

    ASSERT_EQ(File_CreateStream(io_stream, "temp", &memory_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(memory_file, nullptr);

    ASSERT_EQ(Document_CreateFile(memory_file, &memory_document), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(memory_document, nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_Create(&encryption_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(encryption_settings, nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_SetAlgorithm(encryption_settings, encryption_algorithm), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetKeyLength(encryption_settings, encryption_key_length), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, user_permissions), VANILLAPDF_ERROR_SUCCESS);

    BufferHandle* owner_password_buffer = nullptr;
    BufferHandle* user_password_buffer = nullptr;

    ASSERT_EQ(Buffer_CreateFromData(owner_password.data(), owner_password.length(), &owner_password_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_CreateFromData(user_password.data(), user_password.length(), &user_password_buffer), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_NE(owner_password_buffer, nullptr);
    ASSERT_NE(user_password_buffer, nullptr);

    ASSERT_EQ(DocumentEncryptionSettings_SetOwnerPassword(encryption_settings, owner_password_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetUserPassword(encryption_settings, user_password_buffer), VANILLAPDF_ERROR_SUCCESS);

    // Add encryption dictionary and initialize keys
    ASSERT_EQ(Document_AddEncryption(memory_document, encryption_settings), VANILLAPDF_ERROR_SUCCESS);

    // Create the destination stream file for output
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&destination_io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_io_stream, nullptr);

    ASSERT_EQ(File_CreateStream(destination_io_stream, "temp_destination", &destination_save_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_save_file, nullptr);

    // Save the file into destination memory stream
    ASSERT_EQ(Document_SaveFile(memory_document, destination_save_file), VANILLAPDF_ERROR_SUCCESS);

    // Check the destination file for consistency
    ASSERT_EQ(File_OpenStream(destination_io_stream, "temp_destination", &destination_load_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_load_file, nullptr);

    ASSERT_EQ(File_Initialize(destination_load_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_IsEncrypted(destination_load_file, &destination_is_encrypted), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(destination_is_encrypted, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(File_SetEncryptionPassword(destination_load_file, owner_password.data()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_SetEncryptionPassword(destination_load_file, user_password.data()), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup

    ASSERT_EQ(File_Release(destination_load_file), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(File_Release(destination_save_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(destination_io_stream), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_Release(owner_password_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(user_password_buffer), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DocumentEncryptionSettings_Release(encryption_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(memory_document), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(memory_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
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

    FileHandle* source_memory_file = nullptr;
    DocumentHandle* source_memory_document = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;

    DocumentSignatureSettingsHandle* signature_settings = nullptr;
    DateHandle* signing_time = nullptr;
    SigningKeyHandle* signing_key = nullptr;
    PKCS12KeyHandle* signature_pkcs12_key = nullptr;
    BufferHandle* signing_key_data = nullptr;

    FileHandle* destination_file = nullptr;
    InputOutputStreamHandle* destination_io_stream = nullptr;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(io_stream, nullptr);

    ASSERT_EQ(File_CreateStream(io_stream, "temp", &source_memory_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_memory_file, nullptr);

    ASSERT_EQ(Document_CreateFile(source_memory_file, &source_memory_document), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_memory_document, nullptr);

    // Create the destination stream file for output
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&destination_io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_io_stream, nullptr);

    ASSERT_EQ(File_CreateStream(destination_io_stream, "temp_destination", &destination_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(destination_file, nullptr);

    // Configure the signature settings
    ASSERT_EQ(DocumentSignatureSettings_Create(&signature_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_settings, nullptr);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE), sizeof(SIGNING_CERTIFICATE), &signing_key_data), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key_data, nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(signing_key_data, nullptr, &signature_pkcs12_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_pkcs12_key, nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(signature_pkcs12_key, &signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key, nullptr);

    ASSERT_EQ(Date_CreateCurrent(&signing_time), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_time, nullptr);

    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time), VANILLAPDF_ERROR_SUCCESS);

    // Sign the document and save it to the destination file
    ASSERT_EQ(Document_Sign(source_memory_document, destination_file, signature_settings), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup

    ASSERT_EQ(Date_Release(signing_time), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(File_Release(destination_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(destination_io_stream), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_Release(signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_Release(signature_pkcs12_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(signing_key_data), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DocumentSignatureSettings_Release(signature_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(source_memory_document), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(source_memory_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

} /* documents */
