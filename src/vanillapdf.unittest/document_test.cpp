#include "unittest.h"

#include <gtest/gtest.h>

namespace documents {

	TEST(Document, AddEncryption) {

		const char USER_PASSWORD[] = "user_password";
		const char OWNER_PASSWORD[] = "owner_password";

		const integer_type ENCRYPTION_KEY_LENGTH = 40;
		const EncryptionAlgorithmType ENCRYPTION_ALGORITHM = EncryptionAlgorithmType_RC4;

		FileHandle* memory_file = nullptr;
		DocumentHandle* memory_document = nullptr;
		InputOutputStreamHandle* io_stream = nullptr;
		DocumentEncryptionSettingsHandle* encryption_settings = nullptr;

		FileHandle* destination_file = nullptr;
		InputOutputStreamHandle* destination_io_stream = nullptr;

		ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(io_stream, nullptr);

		ASSERT_EQ(File_CreateStream(io_stream, "temp", &memory_file), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(memory_file, nullptr);

		ASSERT_EQ(Document_CreateFile(memory_file, &memory_document), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(memory_document, nullptr);

		ASSERT_EQ(DocumentEncryptionSettings_Create(&encryption_settings), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(encryption_settings, nullptr);

		ASSERT_EQ(DocumentEncryptionSettings_SetAlgorithm(encryption_settings, ENCRYPTION_ALGORITHM), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(DocumentEncryptionSettings_SetKeyLength(encryption_settings, ENCRYPTION_KEY_LENGTH), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(DocumentEncryptionSettings_SetUserPermissions(encryption_settings, UserAccessPermissionFlag_None), VANILLAPDF_ERROR_SUCCESS);

		BufferHandle* owner_password = nullptr;
		BufferHandle* user_password = nullptr;

		ASSERT_EQ(Buffer_CreateFromData(&owner_password, OWNER_PASSWORD, sizeof(OWNER_PASSWORD)), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(Buffer_CreateFromData(&user_password, USER_PASSWORD, sizeof(USER_PASSWORD)), VANILLAPDF_ERROR_SUCCESS);

		ASSERT_NE(owner_password, nullptr);
		ASSERT_NE(user_password, nullptr);

		ASSERT_EQ(DocumentEncryptionSettings_SetOwnerPassword(encryption_settings, owner_password), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(DocumentEncryptionSettings_SetUserPassword(encryption_settings, user_password), VANILLAPDF_ERROR_SUCCESS);

		// Add encryption dictionary and initialize keys
		ASSERT_EQ(Document_AddEncryption(memory_document, encryption_settings), VANILLAPDF_ERROR_SUCCESS);

		// Create the destination stream file for output
		ASSERT_EQ(InputOutputStream_CreateFromMemory(&destination_io_stream), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(destination_io_stream, nullptr);

		ASSERT_EQ(File_CreateStream(destination_io_stream, "temp_destination", &destination_file), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(destination_file, nullptr);

		// Save the file into destination memory stream
		ASSERT_EQ(Document_SaveFile(memory_document, destination_file), VANILLAPDF_ERROR_SUCCESS);

		// Cleanup

		ASSERT_EQ(File_Release(destination_file), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(InputOutputStream_Release(destination_io_stream), VANILLAPDF_ERROR_SUCCESS);

		ASSERT_EQ(Buffer_Release(owner_password), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(Buffer_Release(user_password), VANILLAPDF_ERROR_SUCCESS);

		ASSERT_EQ(DocumentEncryptionSettings_Release(encryption_settings), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(Document_Release(memory_document), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(File_Release(memory_file), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
	}

}
