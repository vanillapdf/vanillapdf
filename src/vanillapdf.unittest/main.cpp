#include "unittest.h"

#include <gtest/gtest.h>

TEST(Buffer, CreateRelease) {

	BufferHandle* buffer_ptr = nullptr;
	auto create_result = Buffer_Create(&buffer_ptr);

	EXPECT_EQ(create_result, VANILLAPDF_ERROR_SUCCESS);
	EXPECT_NE(buffer_ptr, nullptr);

	auto release_result = Buffer_Release(buffer_ptr);

	EXPECT_EQ(release_result, VANILLAPDF_ERROR_SUCCESS);
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
