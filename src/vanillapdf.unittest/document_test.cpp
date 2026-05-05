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

// Parameterized test for AES encryption roundtrip with various string sizes.
// Verifies that encrypt→save→reopen→decrypt preserves the original data.
// Uses sizes that are not multiples of the AES block size (16) to exercise
// PKCS#7 padding correctness.
struct AESRoundtripParam {
    std::string name;
    std::string test_data;
};

class AESEncryptionRoundtrip : public ::testing::TestWithParam<AESRoundtripParam> {
};

TEST_P(AESEncryptionRoundtrip, VerifyStringContent) {
    auto param = GetParam();

    // Create source document in memory
    InputOutputStreamHandle* src_io = nullptr;
    FileHandle* src_file = nullptr;
    DocumentHandle* src_doc = nullptr;
    DocumentEncryptionSettingsHandle* enc_settings = nullptr;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(&src_io), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(src_io, "src", &src_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(src_file, &src_doc), VANILLAPDF_ERROR_SUCCESS);

    // Configure AES-128 encryption
    BufferHandle* owner_pw = nullptr;
    BufferHandle* user_pw = nullptr;
    std::string owner_password = "owner";
    std::string user_password = "user";

    ASSERT_EQ(DocumentEncryptionSettings_Create(&enc_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetAlgorithm(enc_settings, EncryptionAlgorithmType_AES), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetKeyLength(enc_settings, 128), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetUserAccessPermissions(enc_settings, UserAccessPermissionFlag_None), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_CreateFromData(owner_password.data(), owner_password.length(), &owner_pw), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_CreateFromData(user_password.data(), user_password.length(), &user_pw), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetOwnerPassword(enc_settings, owner_pw), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_SetUserPassword(enc_settings, user_pw), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_AddEncryption(src_doc, enc_settings), VANILLAPDF_ERROR_SUCCESS);

    // Insert a test string into the page dictionary under a custom key.
    // Page dictionaries survive save/reopen and their string values are
    // encrypted, so this exercises the full AES encrypt→decrypt roundtrip.
    CatalogHandle* src_catalog = nullptr;
    PageTreeHandle* src_pages = nullptr;
    PageObjectHandle* src_page = nullptr;
    DictionaryObjectHandle* src_page_dict = nullptr;

    ASSERT_EQ(Document_GetCatalog(src_doc, &src_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(src_catalog, &src_pages), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_CreateFromDocument(src_doc, &src_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_AppendPage(src_pages, src_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetBaseObject(src_page, &src_page_dict), VANILLAPDF_ERROR_SUCCESS);

    NameObjectHandle* key_name = nullptr;
    BufferHandle* data_buf = nullptr;
    LiteralStringObjectHandle* str_obj = nullptr;
    StringObjectHandle* str_base = nullptr;
    ObjectHandle* str_as_obj = nullptr;

    ASSERT_EQ(NameObject_CreateFromEncodedString("TestData", &key_name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_CreateFromData(param.test_data.data(), param.test_data.size(), &data_buf), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedBuffer(data_buf, &str_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_ToStringObject(str_obj, &str_base), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_ToObject(str_base, &str_as_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(src_page_dict, key_name, str_as_obj, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Save encrypted document to memory
    InputOutputStreamHandle* dst_io = nullptr;
    FileHandle* dst_save_file = nullptr;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(&dst_io), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(dst_io, "dst", &dst_save_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(src_doc, dst_save_file), VANILLAPDF_ERROR_SUCCESS);

    // Reopen the encrypted document and navigate to the page
    FileHandle* dst_load_file = nullptr;
    DocumentHandle* dst_doc = nullptr;

    ASSERT_EQ(File_OpenStream(dst_io, "dst", &dst_load_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Initialize(dst_load_file), VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_encrypted = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(File_IsEncrypted(dst_load_file, &is_encrypted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(is_encrypted, VANILLAPDF_RV_TRUE);
    ASSERT_EQ(File_SetEncryptionPassword(dst_load_file, user_password.data()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Document_OpenFile(dst_load_file, &dst_doc), VANILLAPDF_ERROR_SUCCESS);

    CatalogHandle* dst_catalog = nullptr;
    PageTreeHandle* dst_pages = nullptr;
    PageObjectHandle* dst_page = nullptr;
    DictionaryObjectHandle* dst_page_dict = nullptr;

    ASSERT_EQ(Document_GetCatalog(dst_doc, &dst_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(dst_catalog, &dst_pages), VANILLAPDF_ERROR_SUCCESS);

    size_type page_count = 0;
    ASSERT_EQ(PageTree_GetPageCount(dst_pages, &page_count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_GE(page_count, 1u);

    ASSERT_EQ(PageTree_GetPage(dst_pages, page_count, &dst_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetBaseObject(dst_page, &dst_page_dict), VANILLAPDF_ERROR_SUCCESS);

    // Read back the test string and verify content matches
    NameObjectHandle* read_key = nullptr;
    ObjectHandle* read_obj = nullptr;
    ASSERT_EQ(NameObject_CreateFromEncodedString("TestData", &read_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Find(dst_page_dict, read_key, &read_obj), VANILLAPDF_ERROR_SUCCESS);

    ObjectType read_type = ObjectType_Undefined;
    ASSERT_EQ(Object_GetObjectType(read_obj, &read_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(read_type, ObjectType_String);

    StringObjectHandle* read_str = nullptr;
    ASSERT_EQ(StringObject_FromObject(read_obj, &read_str), VANILLAPDF_ERROR_SUCCESS);

    BufferHandle* read_buf = nullptr;
    ASSERT_EQ(StringObject_GetValue(read_str, &read_buf), VANILLAPDF_ERROR_SUCCESS);

    string_type read_data = nullptr;
    size_type read_len = 0;
    ASSERT_EQ(Buffer_GetData(read_buf, &read_data, &read_len), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(read_len, param.test_data.size());
    for (size_type i = 0; i < read_len; ++i) {
        EXPECT_EQ(read_data[i], param.test_data[i]);
    }

    // Cleanup
    ASSERT_EQ(Buffer_Release(read_buf), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(read_str), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(read_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(read_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(dst_page_dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(dst_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(dst_pages), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(dst_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(dst_doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(dst_load_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(dst_save_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(dst_io), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(str_as_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(str_base), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(str_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(data_buf), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(key_name), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(src_page_dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(src_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(src_pages), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(src_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(owner_pw), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(user_pw), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentEncryptionSettings_Release(enc_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(src_doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(src_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(src_io), VANILLAPDF_ERROR_SUCCESS);
}

INSTANTIATE_TEST_SUITE_P(
    EncryptionPadding,
    AESEncryptionRoundtrip,
    ::testing::Values(
        AESRoundtripParam{"1byte",  "X"},
        AESRoundtripParam{"5bytes", "Hello"},
        AESRoundtripParam{"15bytes", "FifteenBytesXX!"},
        AESRoundtripParam{"16bytes", "SixteenBytes!!!!"},
        AESRoundtripParam{"17bytes", "SeventeenBytesXX!"},
        AESRoundtripParam{"31bytes", "ThirtyOneBytesOfTestDataHere!!!"},
        AESRoundtripParam{"32bytes", "ThirtyTwoBytesOfTestDataGoHere!!"},
        AESRoundtripParam{"33bytes", "ThirtyThreeBytesOfTestDataGoHere!"}
    ),
    [](const ::testing::TestParamInfo<AESRoundtripParam>& info) {
        return info.param.name;
    }
);

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

TEST(Document, ButtonFieldConversion) {
    DictionaryObjectHandle* dict = nullptr;
    NameObjectHandle* ft_key = nullptr;
    NameObjectHandle* btn_value = nullptr;
    FieldHandle* field = nullptr;
    ButtonFieldHandle* button_field = nullptr;
    FieldHandle* field_back = nullptr;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(&dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict, nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", &ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Btn", &btn_value), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(btn_value), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, &field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field, nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Button);

    ASSERT_EQ(ButtonField_FromField(field, &button_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(button_field, nullptr);

    ASSERT_EQ(ButtonField_ToField(button_field, &field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back, nullptr);

    ASSERT_EQ(Field_Release(field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ButtonField_Release(button_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Field_Release(field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(btn_value), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Document, TextFieldConversion) {
    DictionaryObjectHandle* dict = nullptr;
    NameObjectHandle* ft_key = nullptr;
    NameObjectHandle* tx_value = nullptr;
    FieldHandle* field = nullptr;
    TextFieldHandle* text_field = nullptr;
    FieldHandle* field_back = nullptr;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(&dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict, nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", &ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Tx", &tx_value), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(tx_value), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, &field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field, nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Text);

    ASSERT_EQ(TextField_FromField(field, &text_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(text_field, nullptr);

    ASSERT_EQ(TextField_ToField(text_field, &field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back, nullptr);

    ASSERT_EQ(Field_Release(field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextField_Release(text_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Field_Release(field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(tx_value), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Document, ChoiceFieldConversion) {
    DictionaryObjectHandle* dict = nullptr;
    NameObjectHandle* ft_key = nullptr;
    NameObjectHandle* ch_value = nullptr;
    FieldHandle* field = nullptr;
    ChoiceFieldHandle* choice_field = nullptr;
    FieldHandle* field_back = nullptr;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(&dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict, nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", &ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Ch", &ch_value), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(ch_value), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, &field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field, nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Choice);

    ASSERT_EQ(ChoiceField_FromField(field, &choice_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(choice_field, nullptr);

    ASSERT_EQ(ChoiceField_ToField(choice_field, &field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back, nullptr);

    ASSERT_EQ(Field_Release(field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ChoiceField_Release(choice_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Field_Release(field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(ch_value), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Document, SignatureFieldConversion) {
    DictionaryObjectHandle* dict = nullptr;
    NameObjectHandle* ft_key = nullptr;
    NameObjectHandle* sig_value = nullptr;
    FieldHandle* field = nullptr;
    SignatureFieldHandle* signature_field = nullptr;
    FieldHandle* field_back = nullptr;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(&dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict, nullptr);

    ASSERT_EQ(NameObject_CreateFromDecodedString("FT", &ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_CreateFromDecodedString("Sig", &sig_value), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(dict, ft_key, reinterpret_cast<ObjectHandle*>(sig_value), VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Field_CreateFromDictionary(dict, &field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field, nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_Signature);

    ASSERT_EQ(SignatureField_FromField(field, &signature_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_field, nullptr);

    ASSERT_EQ(SignatureField_ToField(signature_field, &field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field_back, nullptr);

    ASSERT_EQ(Field_Release(field_back), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureField_Release(signature_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Field_Release(field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(sig_value), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(ft_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Document, NonTerminalFieldCreation) {
    DictionaryObjectHandle* dict = nullptr;
    FieldHandle* field = nullptr;
    FieldType field_type = FieldType_Undefined;

    ASSERT_EQ(DictionaryObject_Create(&dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict, nullptr);

    ASSERT_EQ(Field_CreateFromDictionary(dict, &field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field, nullptr);

    ASSERT_EQ(Field_GetType(field, &field_type), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(field_type, FieldType_NonTerminal);

    ASSERT_EQ(Field_Release(field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(dict), VANILLAPDF_ERROR_SUCCESS);
}

} /* documents */
