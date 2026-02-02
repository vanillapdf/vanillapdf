#ifndef _VANILLAPDF_UNITTEST_H
#define _VANILLAPDF_UNITTEST_H

#include "vanillapdf/c_vanillapdf_api.h"

#include <gtest/gtest.h>
#include <string>

// Path to test data directory, set via --test_dir command-line argument.
// Populated in main() after InitGoogleTest parses its own flags.
extern std::string g_test_data_dir;

class TestEnvironment : public ::testing::Environment {
public:
    void SetUp() override;
    void TearDown() override;

    ~TestEnvironment();
};

#endif /* _VANILLAPDF_UNITTEST_H */
