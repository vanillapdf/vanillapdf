#ifndef _VANILLAPDF_UNITTEST_H
#define _VANILLAPDF_UNITTEST_H

#include "vanillapdf/c_vanillapdf_api.h"

#include <gtest/gtest.h>

class TestEnvironment : public ::testing::Environment {
public:
	void SetUp() override;
	void TearDown() override;

	~TestEnvironment();
};

#endif /* _VANILLAPDF_UNITTEST_H */
