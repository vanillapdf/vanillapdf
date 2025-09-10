#include "unittest.h"
#include "vanillapdf/utils/c_name_constants.h"

namespace export_verification_simple {

// Simple compile-time test to verify that the constants are properly declared
TEST(ExportDeclarationTest, VerifyLZWDecodeConstantExists) {
    // This test verifies that NameConstant_LZWDecode is properly declared
    // If the symbol is not declared, this will fail to compile
    EXPECT_NE(NameConstant_LZWDecode, nullptr) << "NameConstant_LZWDecode should be properly declared and not null";
    
    // Verify it's a valid NameObjectHandle pointer
    const NameObjectHandle* handle = NameConstant_LZWDecode;
    EXPECT_NE(handle, nullptr) << "NameConstant_LZWDecode should point to a valid NameObjectHandle";
}

TEST(ExportDeclarationTest, VerifyJPXDecodeConstantExists) {
    // This test verifies that NameConstant_JPXDecode is properly declared
    // If the symbol is not declared, this will fail to compile
    EXPECT_NE(NameConstant_JPXDecode, nullptr) << "NameConstant_JPXDecode should be properly declared and not null";
    
    // Verify it's a valid NameObjectHandle pointer
    const NameObjectHandle* handle = NameConstant_JPXDecode;
    EXPECT_NE(handle, nullptr) << "NameConstant_JPXDecode should point to a valid NameObjectHandle";
}

TEST(ExportDeclarationTest, VerifyOtherDecodeConstants) {
    // Verify that other decode constants are also properly declared
    EXPECT_NE(NameConstant_DCTDecode, nullptr) << "NameConstant_DCTDecode should be properly declared";
    EXPECT_NE(NameConstant_FlateDecode, nullptr) << "NameConstant_FlateDecode should be properly declared";
    EXPECT_NE(NameConstant_ASCII85Decode, nullptr) << "NameConstant_ASCII85Decode should be properly declared";
    EXPECT_NE(NameConstant_ASCIIHexDecode, nullptr) << "NameConstant_ASCIIHexDecode should be properly declared";
}

TEST(ExportDeclarationTest, VerifyConstantsAreDifferent) {
    // Verify that the constants are different objects
    EXPECT_NE(NameConstant_LZWDecode, NameConstant_JPXDecode) 
        << "NameConstant_LZWDecode and NameConstant_JPXDecode should be different objects";
    EXPECT_NE(NameConstant_LZWDecode, NameConstant_DCTDecode) 
        << "NameConstant_LZWDecode and NameConstant_DCTDecode should be different objects";
    EXPECT_NE(NameConstant_JPXDecode, NameConstant_DCTDecode) 
        << "NameConstant_JPXDecode and NameConstant_DCTDecode should be different objects";
}

} // namespace export_verification_simple