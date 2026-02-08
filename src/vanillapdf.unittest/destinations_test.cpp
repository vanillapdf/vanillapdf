#include "unittest.h"
#include "handle_guard.h"

namespace destinations {

// Helper function to create a name object for destination types
HandleGuard<NameObjectHandle, NameObject_Release> CreateNameObject(const char* name) {
    HandleGuard<NameObjectHandle, NameObject_Release> name_handle;
    EXPECT_EQ(NameObject_CreateFromDecodedString(name, name_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(name_handle.get(), nullptr);
    return name_handle;
}

// Helper function to create an integer object for page numbers
HandleGuard<IntegerObjectHandle, IntegerObject_Release> CreateIntegerObject(int32_t value) {
    HandleGuard<IntegerObjectHandle, IntegerObject_Release> int_handle;
    EXPECT_EQ(IntegerObject_CreateFromIntegerValue(value, int_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(int_handle.get(), nullptr);
    return int_handle;
}

// Helper function to create a real object for coordinates
HandleGuard<RealObjectHandle, RealObject_Release> CreateRealObject(double value) {
    HandleGuard<RealObjectHandle, RealObject_Release> real_handle;
    EXPECT_EQ(RealObject_CreateFromData(value, 2, real_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(real_handle.get(), nullptr);
    return real_handle;
}

TEST(DestinationArray, CreateXYZDestination) {
    // Create XYZ destination array: [0 /XYZ 100.0 200.0 1.5]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(array_handle.get(), nullptr);

    // Add page number (0)
    auto page_handle = CreateIntegerObject(0);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type (/XYZ)
    auto type_handle = CreateNameObject("XYZ");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add left coordinate (100.0)
    auto left_handle = CreateRealObject(100.0);
    HandleGuard<ObjectHandle, Object_Release> left_obj;
    ASSERT_EQ(RealObject_ToObject(left_handle, left_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add top coordinate (200.0)
    auto top_handle = CreateRealObject(200.0);
    HandleGuard<ObjectHandle, Object_Release> top_obj;
    ASSERT_EQ(RealObject_ToObject(top_handle, top_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add zoom factor (1.5)
    auto zoom_handle = CreateRealObject(1.5);
    HandleGuard<ObjectHandle, Object_Release> zoom_obj;
    ASSERT_EQ(RealObject_ToObject(zoom_handle, zoom_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, zoom_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dest_handle.get(), nullptr);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_XYZ);

    // Convert to XYZ destination and test parameters
    HandleGuard<XYZDestinationHandle, XYZDestination_Release> xyz_handle;
    ASSERT_EQ(XYZDestination_FromDestination(dest_handle, xyz_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xyz_handle.get(), nullptr);

    // Test left coordinate
    HandleGuard<ObjectHandle, Object_Release> result_left;
    ASSERT_EQ(XYZDestination_GetLeft(xyz_handle, result_left.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_left_real;
    ASSERT_EQ(RealObject_FromObject(result_left, result_left_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 100.0);

    // Test top coordinate
    HandleGuard<ObjectHandle, Object_Release> result_top;
    ASSERT_EQ(XYZDestination_GetTop(xyz_handle, result_top.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_top_real;
    ASSERT_EQ(RealObject_FromObject(result_top, result_top_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 200.0);

    // Test zoom factor
    HandleGuard<ObjectHandle, Object_Release> result_zoom;
    ASSERT_EQ(XYZDestination_GetZoom(xyz_handle, result_zoom.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_zoom_real;
    ASSERT_EQ(RealObject_FromObject(result_zoom, result_zoom_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double zoom_value;
    ASSERT_EQ(RealObject_GetValue(result_zoom_real, &zoom_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(zoom_value, 1.5);
}

TEST(DestinationDictionary, CreateXYZDestination) {
    // Create XYZ destination dictionary: {/D [0 /XYZ 100.0 200.0 1.5]}

    // First create the destination array
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number, type, and coordinates
    auto page_handle = CreateIntegerObject(0);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    auto type_handle = CreateNameObject("XYZ");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    auto left_handle = CreateRealObject(150.0);
    HandleGuard<ObjectHandle, Object_Release> left_obj;
    ASSERT_EQ(RealObject_ToObject(left_handle, left_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    auto top_handle = CreateRealObject(250.0);
    HandleGuard<ObjectHandle, Object_Release> top_obj;
    ASSERT_EQ(RealObject_ToObject(top_handle, top_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    auto zoom_handle = CreateRealObject(2.0);
    HandleGuard<ObjectHandle, Object_Release> zoom_obj;
    ASSERT_EQ(RealObject_ToObject(zoom_handle, zoom_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, zoom_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create dictionary
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict_handle;
    ASSERT_EQ(DictionaryObject_Create(dict_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict_handle.get(), nullptr);

    // Add array to dictionary with /D key
    auto d_key_handle = CreateNameObject("D");
    HandleGuard<ObjectHandle, Object_Release> array_obj;
    ASSERT_EQ(ArrayObject_ToObject(array_handle, array_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict_handle, d_key_handle, array_obj, false), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from dictionary
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromDictionary(dict_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dest_handle.get(), nullptr);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_XYZ);

    // Convert to XYZ destination and test parameters
    HandleGuard<XYZDestinationHandle, XYZDestination_Release> xyz_handle;
    ASSERT_EQ(XYZDestination_FromDestination(dest_handle, xyz_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    HandleGuard<ObjectHandle, Object_Release> result_left;
    ASSERT_EQ(XYZDestination_GetLeft(xyz_handle, result_left.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_left_real;
    ASSERT_EQ(RealObject_FromObject(result_left, result_left_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 150.0);
}

TEST(DestinationArray, CreateFitDestination) {
    // Create Fit destination array: [0 /Fit]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(1);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("Fit");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_Fit);

    // Convert to Fit destination
    HandleGuard<FitDestinationHandle, FitDestination_Release> fit_handle;
    ASSERT_EQ(FitDestination_FromDestination(dest_handle, fit_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(fit_handle.get(), nullptr);
}

TEST(DestinationArray, CreateFitRectangleDestination) {
    // Create FitR destination array: [0 /FitR 50.0 100.0 200.0 300.0]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(2);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("FitR");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add rectangle coordinates: left, bottom, right, top
    auto left_handle = CreateRealObject(50.0);
    HandleGuard<ObjectHandle, Object_Release> left_obj;
    ASSERT_EQ(RealObject_ToObject(left_handle, left_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    auto bottom_handle = CreateRealObject(100.0);
    HandleGuard<ObjectHandle, Object_Release> bottom_obj;
    ASSERT_EQ(RealObject_ToObject(bottom_handle, bottom_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, bottom_obj), VANILLAPDF_ERROR_SUCCESS);

    auto right_handle = CreateRealObject(200.0);
    HandleGuard<ObjectHandle, Object_Release> right_obj;
    ASSERT_EQ(RealObject_ToObject(right_handle, right_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, right_obj), VANILLAPDF_ERROR_SUCCESS);

    auto top_handle = CreateRealObject(300.0);
    HandleGuard<ObjectHandle, Object_Release> top_obj;
    ASSERT_EQ(RealObject_ToObject(top_handle, top_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitRectangle);

    // Convert to FitRectangle destination and test parameters
    HandleGuard<FitRectangleDestinationHandle, FitRectangleDestination_Release> fitr_handle;
    ASSERT_EQ(FitRectangleDestination_FromDestination(dest_handle, fitr_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    HandleGuard<ObjectHandle, Object_Release> result_left;
    ASSERT_EQ(FitRectangleDestination_GetLeft(fitr_handle, result_left.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_left_real;
    ASSERT_EQ(RealObject_FromObject(result_left, result_left_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 50.0);

    // Test bottom coordinate
    HandleGuard<ObjectHandle, Object_Release> result_bottom;
    ASSERT_EQ(FitRectangleDestination_GetBottom(fitr_handle, result_bottom.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_bottom_real;
    ASSERT_EQ(RealObject_FromObject(result_bottom, result_bottom_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double bottom_value;
    ASSERT_EQ(RealObject_GetValue(result_bottom_real, &bottom_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(bottom_value, 100.0);

    // Test right coordinate
    HandleGuard<ObjectHandle, Object_Release> result_right;
    ASSERT_EQ(FitRectangleDestination_GetRight(fitr_handle, result_right.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_right_real;
    ASSERT_EQ(RealObject_FromObject(result_right, result_right_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double right_value;
    ASSERT_EQ(RealObject_GetValue(result_right_real, &right_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(right_value, 200.0);

    // Test top coordinate
    HandleGuard<ObjectHandle, Object_Release> result_top;
    ASSERT_EQ(FitRectangleDestination_GetTop(fitr_handle, result_top.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_top_real;
    ASSERT_EQ(RealObject_FromObject(result_top, result_top_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 300.0);
}

TEST(DestinationValidation, InvalidArraySize) {
    // Create array with only one element (should fail - need at least page and type)
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    auto page_handle = CreateIntegerObject(0);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail)
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    EXPECT_NE(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, InvalidDestinationType) {
    // Create array with invalid destination type
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    auto page_handle = CreateIntegerObject(0);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add invalid destination type
    auto type_handle = CreateNameObject("InvalidType");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail)
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    EXPECT_NE(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, MissingDictionaryDEntry) {
    // Create dictionary without /D entry
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict_handle;
    ASSERT_EQ(DictionaryObject_Create(dict_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail)
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    EXPECT_NE(Destination_CreateFromDictionary(dict_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitHorizontalDestination) {
    // Create FitH destination array: [0 /FitH 150.0]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(1);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("FitH");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add top coordinate
    auto top_handle = CreateRealObject(150.0);
    HandleGuard<ObjectHandle, Object_Release> top_obj;
    ASSERT_EQ(RealObject_ToObject(top_handle, top_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitHorizontal);

    // Convert to FitHorizontal destination and test parameter
    HandleGuard<FitHorizontalDestinationHandle, FitHorizontalDestination_Release> fith_handle;
    ASSERT_EQ(FitHorizontalDestination_FromDestination(dest_handle, fith_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Test top coordinate
    HandleGuard<ObjectHandle, Object_Release> result_top;
    ASSERT_EQ(FitHorizontalDestination_GetTop(fith_handle, result_top.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_top_real;
    ASSERT_EQ(RealObject_FromObject(result_top, result_top_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 150.0);
}

TEST(DestinationArray, CreateFitVerticalDestination) {
    // Create FitV destination array: [0 /FitV 75.0]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(2);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("FitV");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add left coordinate
    auto left_handle = CreateRealObject(75.0);
    HandleGuard<ObjectHandle, Object_Release> left_obj;
    ASSERT_EQ(RealObject_ToObject(left_handle, left_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitVertical);

    // Convert to FitVertical destination and test parameter
    HandleGuard<FitVerticalDestinationHandle, FitVerticalDestination_Release> fitv_handle;
    ASSERT_EQ(FitVerticalDestination_FromDestination(dest_handle, fitv_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    HandleGuard<ObjectHandle, Object_Release> result_left;
    ASSERT_EQ(FitVerticalDestination_GetLeft(fitv_handle, result_left.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_left_real;
    ASSERT_EQ(RealObject_FromObject(result_left, result_left_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 75.0);
}

TEST(DestinationArray, CreateFitBoundingBoxDestination) {
    // Create FitB destination array: [0 /FitB]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(3);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("FitB");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitBoundingBox);

    // Convert to FitBoundingBox destination
    HandleGuard<FitBoundingBoxDestinationHandle, FitBoundingBoxDestination_Release> fitb_handle;
    ASSERT_EQ(FitBoundingBoxDestination_FromDestination(dest_handle, fitb_handle.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitBoundingBoxHorizontalDestination) {
    // Create FitBH destination array: [0 /FitBH 200.0]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(4);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("FitBH");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add top coordinate
    auto top_handle = CreateRealObject(200.0);
    HandleGuard<ObjectHandle, Object_Release> top_obj;
    ASSERT_EQ(RealObject_ToObject(top_handle, top_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitBoundingBoxHorizontal);

    // Convert to FitBoundingBoxHorizontal destination and test parameter
    HandleGuard<FitBoundingBoxHorizontalDestinationHandle, FitBoundingBoxHorizontalDestination_Release> fitbh_handle;
    ASSERT_EQ(FitBoundingBoxHorizontalDestination_FromDestination(dest_handle, fitbh_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Test top coordinate
    HandleGuard<ObjectHandle, Object_Release> result_top;
    ASSERT_EQ(FitBoundingBoxHorizontalDestination_GetTop(fitbh_handle, result_top.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_top_real;
    ASSERT_EQ(RealObject_FromObject(result_top, result_top_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 200.0);
}

TEST(DestinationArray, CreateFitBoundingBoxVerticalDestination) {
    // Create FitBV destination array: [0 /FitBV 125.0]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(5);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("FitBV");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add left coordinate
    auto left_handle = CreateRealObject(125.0);
    HandleGuard<ObjectHandle, Object_Release> left_obj;
    ASSERT_EQ(RealObject_ToObject(left_handle, left_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitBoundingBoxVertical);

    // Convert to FitBoundingBoxVertical destination and test parameter
    HandleGuard<FitBoundingBoxVerticalDestinationHandle, FitBoundingBoxVerticalDestination_Release> fitbv_handle;
    ASSERT_EQ(FitBoundingBoxVerticalDestination_FromDestination(dest_handle, fitbv_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    HandleGuard<ObjectHandle, Object_Release> result_left;
    ASSERT_EQ(FitBoundingBoxVerticalDestination_GetLeft(fitbv_handle, result_left.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<RealObjectHandle, RealObject_Release> result_left_real;
    ASSERT_EQ(RealObject_FromObject(result_left, result_left_real.out()), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 125.0);
}

TEST(DestinationValidation, XYZDestinationWithNullParameters) {
    // Test XYZ destination with null left coordinate (should return false from GetLeft)
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(0);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("XYZ");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add null object for left coordinate
    HandleGuard<NullObjectHandle, NullObject_Release> null_handle;
    ASSERT_EQ(NullObject_Create(null_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<ObjectHandle, Object_Release> null_obj;
    ASSERT_EQ(NullObject_ToObject(null_handle, null_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, null_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Convert to XYZ destination
    HandleGuard<XYZDestinationHandle, XYZDestination_Release> xyz_handle;
    ASSERT_EQ(XYZDestination_FromDestination(dest_handle, xyz_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Test that GetLeft returns error for null parameter
    ObjectHandle* result_left = nullptr;
    EXPECT_EQ(XYZDestination_GetLeft(xyz_handle, &result_left), VANILLAPDF_ERROR_OBJECT_MISSING);
}

TEST(DestinationValidation, InvalidPageObjectType) {
    // Create array with string as page number (should fail)
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add invalid page object (string instead of integer/reference)
    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> string_handle;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("invalid", string_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<StringObjectHandle, StringObject_Release> string_as_string;
    ASSERT_EQ(LiteralStringObject_ToStringObject(string_handle, string_as_string.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<ObjectHandle, Object_Release> string_obj;
    ASSERT_EQ(StringObject_ToObject(string_as_string, string_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, string_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    auto type_handle = CreateNameObject("XYZ");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail due to invalid page object type)
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    EXPECT_NE(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, NonNameDestinationType) {
    // Create array with integer as destination type (should fail)
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    auto page_handle = CreateIntegerObject(0);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add invalid destination type (integer instead of name)
    auto type_handle = CreateIntegerObject(123);
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(IntegerObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail due to invalid type object)
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    EXPECT_NE(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Destination, ToAndFromUnknown) {
    // Create XYZ destination array: [0 /XYZ 100.0 200.0 1.5]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array_handle;
    ASSERT_EQ(ArrayObject_Create(array_handle.out()), VANILLAPDF_ERROR_SUCCESS);

    // Add page number (0)
    auto page_handle = CreateIntegerObject(0);
    HandleGuard<ObjectHandle, Object_Release> page_obj;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, page_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type (/XYZ)
    auto type_handle = CreateNameObject("XYZ");
    HandleGuard<ObjectHandle, Object_Release> type_obj;
    ASSERT_EQ(NameObject_ToObject(type_handle, type_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add left coordinate (100.0)
    auto left_handle = CreateRealObject(100.0);
    HandleGuard<ObjectHandle, Object_Release> left_obj;
    ASSERT_EQ(RealObject_ToObject(left_handle, left_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add top coordinate (200.0)
    auto top_handle = CreateRealObject(200.0);
    HandleGuard<ObjectHandle, Object_Release> top_obj;
    ASSERT_EQ(RealObject_ToObject(top_handle, top_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add zoom factor (1.5)
    auto zoom_handle = CreateRealObject(1.5);
    HandleGuard<ObjectHandle, Object_Release> zoom_obj;
    ASSERT_EQ(RealObject_ToObject(zoom_handle, zoom_obj.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, zoom_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    HandleGuard<DestinationHandle, Destination_Release> dest_handle;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, dest_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dest_handle.get(), nullptr);

    // Convert to IUnknown
    HandleGuard<IUnknownHandle, IUnknown_Release> unknown_handle;
    ASSERT_EQ(Destination_ToUnknown(dest_handle, unknown_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown_handle.get(), nullptr);

    // Convert back from IUnknown
    HandleGuard<DestinationHandle, Destination_Release> converted_dest;
    ASSERT_EQ(Destination_FromUnknown(unknown_handle, converted_dest.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted_dest.get(), nullptr);

    // Verify destination type is preserved
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(converted_dest, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_XYZ);
}

} // namespace destinations
