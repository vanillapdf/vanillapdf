#include "unittest.h"
#include "handle_guard.h"

namespace attributes {

TEST(ObjectAttributeList, CreateRelease) {

    HandleGuard<ObjectAttributeListHandle, ObjectAttributeList_Release> attribute_list;

    ASSERT_EQ(ObjectAttributeList_Create(attribute_list.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(attribute_list.get(), nullptr);
}

TEST(ObjectAttributeList, NullCheck) {
    EXPECT_NE(ObjectAttributeList_Create(nullptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(ObjectAttributeList_Release(nullptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(ObjectAttributeList, Clear) {

    HandleGuard<ObjectAttributeListHandle, ObjectAttributeList_Release> attribute_list;

    ASSERT_EQ(ObjectAttributeList_Create(attribute_list.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(attribute_list.get(), nullptr);

    EXPECT_EQ(ObjectAttributeList_Clear(attribute_list), VANILLAPDF_ERROR_SUCCESS);
}

TEST(ObjectAttributeList, MissingKey) {

    BaseObjectAttributeHandle* found_attribute = NULL;
    HandleGuard<ObjectAttributeListHandle, ObjectAttributeList_Release> attribute_list;

    ASSERT_EQ(ObjectAttributeList_Create(attribute_list.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(attribute_list.get(), nullptr);

    EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_Undefined, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_Empty, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_SerializationOverride, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_TrackingIdentifier, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(ObjectAttributeList_Get(attribute_list, ObjectAttributeType_ImageMetadata, &found_attribute), VANILLAPDF_ERROR_SUCCESS);
}

TEST(SerializationOverrideObjectAttribute, CreateRelease) {

    HandleGuard<SerializationOverrideObjectAttributeHandle, SerializationOverrideObjectAttribute_Release> serialization_override_attribute;

    ASSERT_EQ(SerializationOverrideObjectAttribute_Create(serialization_override_attribute.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(serialization_override_attribute.get(), nullptr);
}

TEST(SerializationOverrideObjectAttribute, NullCheck) {
    EXPECT_NE(SerializationOverrideObjectAttribute_Create(nullptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(SerializationOverrideObjectAttribute_Release(nullptr), VANILLAPDF_ERROR_SUCCESS);
}

TEST(ImageMetadataObjectAttribute, CreateRelease) {

    HandleGuard<ImageMetadataObjectAttributeHandle, ImageMetadataObjectAttribute_Release> image_metadata_attribute;

    ASSERT_EQ(ImageMetadataObjectAttribute_Create(image_metadata_attribute.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(image_metadata_attribute.get(), nullptr);
}

TEST(ImageMetadataObjectAttribute, NullCheck) {
    EXPECT_NE(ImageMetadataObjectAttribute_Create(nullptr), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(ImageMetadataObjectAttribute_Release(nullptr), VANILLAPDF_ERROR_SUCCESS);
}

} /* attributes */
