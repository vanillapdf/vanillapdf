#include "unittest.h"

namespace attributes {

	TEST(ObjectAttributeList, CreateRelease) {

		ObjectAttributeListHandle* attribute_list = NULL;

		ASSERT_EQ(ObjectAttributeList_Create(&attribute_list), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(attribute_list, nullptr);

		ASSERT_EQ(ObjectAttributeList_Release(attribute_list), VANILLAPDF_ERROR_SUCCESS);
	}

	TEST(ObjectAttributeList, NullCheck) {
		EXPECT_NE(ObjectAttributeList_Create(nullptr), VANILLAPDF_ERROR_SUCCESS);
		EXPECT_NE(ObjectAttributeList_Release(nullptr), VANILLAPDF_ERROR_SUCCESS);
	}

	TEST(ObjectAttributeList, Clear) {

		ObjectAttributeListHandle* attribute_list = NULL;

		ASSERT_EQ(ObjectAttributeList_Create(&attribute_list), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(attribute_list, nullptr);

		EXPECT_EQ(ObjectAttributeList_Clear(attribute_list), VANILLAPDF_ERROR_SUCCESS);

		ASSERT_EQ(ObjectAttributeList_Release(attribute_list), VANILLAPDF_ERROR_SUCCESS);
	}

	TEST(ObjectAttributeList, MissingKey) {

		BaseObjectAttributeHandle* found_attribute = NULL;
		ObjectAttributeListHandle* attribute_list = NULL;

		ASSERT_EQ(ObjectAttributeList_Create(&attribute_list), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(attribute_list, nullptr);

		EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_Undefined, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
		EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_Empty, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
		EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_SerializationOverride, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
		EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_TrackingIdentifier, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
		EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_ImageMetadata, &found_attribute), VANILLAPDF_ERROR_SUCCESS);

		ASSERT_EQ(ObjectAttributeList_Release(attribute_list), VANILLAPDF_ERROR_SUCCESS);
	}

	TEST(SerializationOverrideObjectAttribute, CreateRelease) {

		SerializationOverrideObjectAttributeHandle* serialization_override_attribute = NULL;

		ASSERT_EQ(SerializationOverrideObjectAttribute_Create(&serialization_override_attribute), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(serialization_override_attribute, nullptr);

		ASSERT_EQ(SerializationOverrideObjectAttribute_Release(serialization_override_attribute), VANILLAPDF_ERROR_SUCCESS);
	}

	TEST(SerializationOverrideObjectAttribute, NullCheck) {
		EXPECT_NE(SerializationOverrideObjectAttribute_Create(nullptr), VANILLAPDF_ERROR_SUCCESS);
		EXPECT_NE(SerializationOverrideObjectAttribute_Release(nullptr), VANILLAPDF_ERROR_SUCCESS);
	}

	TEST(ImageMetadataObjectAttribute, CreateRelease) {

		ImageMetadataObjectAttributeHandle* image_metadata_attribute = NULL;

		ASSERT_EQ(ImageMetadataObjectAttribute_Create(&image_metadata_attribute), VANILLAPDF_ERROR_SUCCESS);
		ASSERT_NE(image_metadata_attribute, nullptr);

		ASSERT_EQ(ImageMetadataObjectAttribute_Release(image_metadata_attribute), VANILLAPDF_ERROR_SUCCESS);
	}

	TEST(ImageMetadataObjectAttribute, NullCheck) {
		EXPECT_NE(ImageMetadataObjectAttribute_Create(nullptr), VANILLAPDF_ERROR_SUCCESS);
		EXPECT_NE(ImageMetadataObjectAttribute_Release(nullptr), VANILLAPDF_ERROR_SUCCESS);
	}

}
