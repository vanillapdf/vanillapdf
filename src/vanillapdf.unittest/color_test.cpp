#include "unittest.h"

namespace color {

TEST(Color, CreateTransparent) {
    ColorHandle* color = nullptr;
    ColorSpaceType color_space = ColorSpace_DeviceRGB;

    ASSERT_EQ(Color_CreateTransparent(&color), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(color, nullptr);

    ASSERT_EQ(Color_GetColorSpace(color, &color_space), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(color_space, ColorSpace_Transparent);

    ASSERT_EQ(Color_Release(color), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Color, CreateGray) {
    ColorHandle* color = nullptr;
    ColorSpaceType color_space = ColorSpace_Transparent;
    real_type gray = 0.0;

    ASSERT_EQ(Color_CreateGray(0.5, &color), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(color, nullptr);

    ASSERT_EQ(Color_GetColorSpace(color, &color_space), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(color_space, ColorSpace_DeviceGray);

    ASSERT_EQ(Color_GetGray(color, &gray), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(gray, 0.5);

    ASSERT_EQ(Color_Release(color), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Color, CreateRGB) {
    ColorHandle* color = nullptr;
    ColorSpaceType color_space = ColorSpace_Transparent;
    real_type red = 0.0, green = 0.0, blue = 0.0;

    ASSERT_EQ(Color_CreateRGB(1.0, 0.5, 0.25, &color), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(color, nullptr);

    ASSERT_EQ(Color_GetColorSpace(color, &color_space), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(color_space, ColorSpace_DeviceRGB);

    ASSERT_EQ(Color_GetRed(color, &red), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(red, 1.0);

    ASSERT_EQ(Color_GetGreen(color, &green), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(green, 0.5);

    ASSERT_EQ(Color_GetBlue(color, &blue), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(blue, 0.25);

    ASSERT_EQ(Color_Release(color), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Color, CreateCMYK) {
    ColorHandle* color = nullptr;
    ColorSpaceType color_space = ColorSpace_Transparent;
    real_type cyan = 0.0, magenta = 0.0, yellow = 0.0, black = 0.0;

    ASSERT_EQ(Color_CreateCMYK(0.1, 0.2, 0.3, 0.4, &color), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(color, nullptr);

    ASSERT_EQ(Color_GetColorSpace(color, &color_space), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(color_space, ColorSpace_DeviceCMYK);

    ASSERT_EQ(Color_GetCyan(color, &cyan), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(cyan, 0.1);

    ASSERT_EQ(Color_GetMagenta(color, &magenta), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(magenta, 0.2);

    ASSERT_EQ(Color_GetYellow(color, &yellow), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(yellow, 0.3);

    ASSERT_EQ(Color_GetBlack(color, &black), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(black, 0.4);

    ASSERT_EQ(Color_Release(color), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Color, ToAndFromUnknown) {
    ColorHandle* color = nullptr;
    IUnknownHandle* unknown = nullptr;
    ColorHandle* converted = nullptr;
    ColorSpaceType color_space = ColorSpace_Transparent;

    ASSERT_EQ(Color_CreateRGB(0.0, 1.0, 0.0, &color), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(color, nullptr);

    ASSERT_EQ(Color_ToUnknown(color, &unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown, nullptr);

    ASSERT_EQ(Color_FromUnknown(unknown, &converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted, nullptr);

    ASSERT_EQ(Color_GetColorSpace(converted, &color_space), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(color_space, ColorSpace_DeviceRGB);

    ASSERT_EQ(Color_Release(converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IUnknown_Release(unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_Release(color), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Color, GrayBoundaryValues) {
    ColorHandle* black = nullptr;
    ColorHandle* white = nullptr;
    real_type gray = 0.0;

    // Black (0.0)
    ASSERT_EQ(Color_CreateGray(0.0, &black), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(black, nullptr);
    ASSERT_EQ(Color_GetGray(black, &gray), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(gray, 0.0);

    // White (1.0)
    ASSERT_EQ(Color_CreateGray(1.0, &white), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(white, nullptr);
    ASSERT_EQ(Color_GetGray(white, &gray), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(gray, 1.0);

    ASSERT_EQ(Color_Release(white), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_Release(black), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Color, RGBPureColors) {
    ColorHandle* color = nullptr;
    real_type r = 0.0, g = 0.0, b = 0.0;

    // Pure red
    ASSERT_EQ(Color_CreateRGB(1.0, 0.0, 0.0, &color), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_GetRed(color, &r), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_GetGreen(color, &g), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_GetBlue(color, &b), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(r, 1.0);
    EXPECT_DOUBLE_EQ(g, 0.0);
    EXPECT_DOUBLE_EQ(b, 0.0);
    ASSERT_EQ(Color_Release(color), VANILLAPDF_ERROR_SUCCESS);

    // Pure blue
    ASSERT_EQ(Color_CreateRGB(0.0, 0.0, 1.0, &color), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_GetRed(color, &r), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_GetGreen(color, &g), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Color_GetBlue(color, &b), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(r, 0.0);
    EXPECT_DOUBLE_EQ(g, 0.0);
    EXPECT_DOUBLE_EQ(b, 1.0);
    ASSERT_EQ(Color_Release(color), VANILLAPDF_ERROR_SUCCESS);
}

} // namespace color
