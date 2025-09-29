#include "unittest.h"

namespace destinations {

// Helper function to create a name object for destination types
NameObjectHandle* CreateNameObject(const char* name) {
    NameObjectHandle* name_handle = nullptr;
    EXPECT_EQ(NameObject_CreateFromDecodedString(name, &name_handle), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(name_handle, nullptr);
    return name_handle;
}

// Helper function to create an integer object for page numbers
IntegerObjectHandle* CreateIntegerObject(int32_t value) {
    IntegerObjectHandle* int_handle = nullptr;
    EXPECT_EQ(IntegerObject_CreateFromIntegerValue(value, &int_handle), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(int_handle, nullptr);
    return int_handle;
}

// Helper function to create a real object for coordinates
RealObjectHandle* CreateRealObject(double value) {
    RealObjectHandle* real_handle = nullptr;
    EXPECT_EQ(RealObject_CreateFromData(value, 2, &real_handle), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(real_handle, nullptr);
    return real_handle;
}

TEST(DestinationArray, CreateXYZDestination) {
    // Create XYZ destination array: [0 /XYZ 100.0 200.0 1.5]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(array_handle, nullptr);

    // Add page number (0)
    IntegerObjectHandle* page_handle = CreateIntegerObject(0);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type (/XYZ)
    NameObjectHandle* type_handle = CreateNameObject("XYZ");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add left coordinate (100.0)
    RealObjectHandle* left_handle = CreateRealObject(100.0);
    ObjectHandle* left_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(left_handle, &left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add top coordinate (200.0)
    RealObjectHandle* top_handle = CreateRealObject(200.0);
    ObjectHandle* top_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(top_handle, &top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add zoom factor (1.5)
    RealObjectHandle* zoom_handle = CreateRealObject(1.5);
    ObjectHandle* zoom_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(zoom_handle, &zoom_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, zoom_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dest_handle, nullptr);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_XYZ);

    // Convert to XYZ destination and test parameters
    XYZDestinationHandle* xyz_handle = nullptr;
    ASSERT_EQ(XYZDestination_FromDestination(dest_handle, &xyz_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(xyz_handle, nullptr);

    // Test left coordinate
    ObjectHandle* result_left = nullptr;
    ASSERT_EQ(XYZDestination_GetLeft(xyz_handle, &result_left), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_left_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_left, &result_left_real), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 100.0);

    // Test top coordinate
    ObjectHandle* result_top = nullptr;
    ASSERT_EQ(XYZDestination_GetTop(xyz_handle, &result_top), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_top_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_top, &result_top_real), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 200.0);

    // Test zoom factor
    ObjectHandle* result_zoom = nullptr;
    ASSERT_EQ(XYZDestination_GetZoom(xyz_handle, &result_zoom), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_zoom_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_zoom, &result_zoom_real), VANILLAPDF_ERROR_SUCCESS);
    double zoom_value;
    ASSERT_EQ(RealObject_GetValue(result_zoom_real, &zoom_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(zoom_value, 1.5);

    // Cleanup
    ASSERT_EQ(RealObject_Release(result_zoom_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_zoom), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(result_top_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_top), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(result_left_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_left), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XYZDestination_Release(xyz_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(zoom_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(zoom_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(top_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(left_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationDictionary, CreateXYZDestination) {
    // Create XYZ destination dictionary: {/D [0 /XYZ 100.0 200.0 1.5]}

    // First create the destination array
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number, type, and coordinates
    IntegerObjectHandle* page_handle = CreateIntegerObject(0);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    NameObjectHandle* type_handle = CreateNameObject("XYZ");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    RealObjectHandle* left_handle = CreateRealObject(150.0);
    ObjectHandle* left_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(left_handle, &left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    RealObjectHandle* top_handle = CreateRealObject(250.0);
    ObjectHandle* top_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(top_handle, &top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    RealObjectHandle* zoom_handle = CreateRealObject(2.0);
    ObjectHandle* zoom_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(zoom_handle, &zoom_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, zoom_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create dictionary
    DictionaryObjectHandle* dict_handle = nullptr;
    ASSERT_EQ(DictionaryObject_Create(&dict_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dict_handle, nullptr);

    // Add array to dictionary with /D key
    NameObjectHandle* d_key_handle = CreateNameObject("D");
    ObjectHandle* array_obj = nullptr;
    ASSERT_EQ(ArrayObject_ToObject(array_handle, &array_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(dict_handle, d_key_handle, array_obj, false), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from dictionary
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromDictionary(dict_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(dest_handle, nullptr);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_XYZ);

    // Convert to XYZ destination and test parameters
    XYZDestinationHandle* xyz_handle = nullptr;
    ASSERT_EQ(XYZDestination_FromDestination(dest_handle, &xyz_handle), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    ObjectHandle* result_left = nullptr;
    ASSERT_EQ(XYZDestination_GetLeft(xyz_handle, &result_left), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_left_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_left, &result_left_real), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 150.0);

    // Cleanup
    ASSERT_EQ(RealObject_Release(result_left_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_left), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XYZDestination_Release(xyz_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(array_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(d_key_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Release(dict_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(zoom_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(zoom_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(top_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(left_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitDestination) {
    // Create Fit destination array: [0 /Fit]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(1);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("Fit");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_Fit);

    // Convert to Fit destination
    FitDestinationHandle* fit_handle = nullptr;
    ASSERT_EQ(FitDestination_FromDestination(dest_handle, &fit_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(fit_handle, nullptr);

    // Cleanup
    ASSERT_EQ(FitDestination_Release(fit_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitRectangleDestination) {
    // Create FitR destination array: [0 /FitR 50.0 100.0 200.0 300.0]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(2);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("FitR");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add rectangle coordinates: left, bottom, right, top
    RealObjectHandle* left_handle = CreateRealObject(50.0);
    ObjectHandle* left_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(left_handle, &left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    RealObjectHandle* bottom_handle = CreateRealObject(100.0);
    ObjectHandle* bottom_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(bottom_handle, &bottom_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, bottom_obj), VANILLAPDF_ERROR_SUCCESS);

    RealObjectHandle* right_handle = CreateRealObject(200.0);
    ObjectHandle* right_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(right_handle, &right_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, right_obj), VANILLAPDF_ERROR_SUCCESS);

    RealObjectHandle* top_handle = CreateRealObject(300.0);
    ObjectHandle* top_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(top_handle, &top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitRectangle);

    // Convert to FitRectangle destination and test parameters
    FitRectangleDestinationHandle* fitr_handle = nullptr;
    ASSERT_EQ(FitRectangleDestination_FromDestination(dest_handle, &fitr_handle), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    ObjectHandle* result_left = nullptr;
    ASSERT_EQ(FitRectangleDestination_GetLeft(fitr_handle, &result_left), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_left_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_left, &result_left_real), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 50.0);

    // Test bottom coordinate
    ObjectHandle* result_bottom = nullptr;
    ASSERT_EQ(FitRectangleDestination_GetBottom(fitr_handle, &result_bottom), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_bottom_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_bottom, &result_bottom_real), VANILLAPDF_ERROR_SUCCESS);
    double bottom_value;
    ASSERT_EQ(RealObject_GetValue(result_bottom_real, &bottom_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(bottom_value, 100.0);

    // Test right coordinate
    ObjectHandle* result_right = nullptr;
    ASSERT_EQ(FitRectangleDestination_GetRight(fitr_handle, &result_right), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_right_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_right, &result_right_real), VANILLAPDF_ERROR_SUCCESS);
    double right_value;
    ASSERT_EQ(RealObject_GetValue(result_right_real, &right_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(right_value, 200.0);

    // Test top coordinate
    ObjectHandle* result_top = nullptr;
    ASSERT_EQ(FitRectangleDestination_GetTop(fitr_handle, &result_top), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_top_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_top, &result_top_real), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 300.0);

    // Cleanup
    ASSERT_EQ(RealObject_Release(result_top_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_top), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(result_right_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_right), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(result_bottom_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_bottom), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(result_left_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_left), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FitRectangleDestination_Release(fitr_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(top_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(right_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(right_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(bottom_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(bottom_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(left_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, InvalidArraySize) {
    // Create array with only one element (should fail - need at least page and type)
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    IntegerObjectHandle* page_handle = CreateIntegerObject(0);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail)
    DestinationHandle* dest_handle = nullptr;
    EXPECT_NE(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, InvalidDestinationType) {
    // Create array with invalid destination type
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    IntegerObjectHandle* page_handle = CreateIntegerObject(0);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add invalid destination type
    NameObjectHandle* type_handle = CreateNameObject("InvalidType");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail)
    DestinationHandle* dest_handle = nullptr;
    EXPECT_NE(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, MissingDictionaryDEntry) {
    // Create dictionary without /D entry
    DictionaryObjectHandle* dict_handle = nullptr;
    ASSERT_EQ(DictionaryObject_Create(&dict_handle), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail)
    DestinationHandle* dest_handle = nullptr;
    EXPECT_NE(Destination_CreateFromDictionary(dict_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    ASSERT_EQ(DictionaryObject_Release(dict_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitHorizontalDestination) {
    // Create FitH destination array: [0 /FitH 150.0]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(1);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("FitH");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add top coordinate
    RealObjectHandle* top_handle = CreateRealObject(150.0);
    ObjectHandle* top_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(top_handle, &top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitHorizontal);

    // Convert to FitHorizontal destination and test parameter
    FitHorizontalDestinationHandle* fith_handle = nullptr;
    ASSERT_EQ(FitHorizontalDestination_FromDestination(dest_handle, &fith_handle), VANILLAPDF_ERROR_SUCCESS);

    // Test top coordinate
    ObjectHandle* result_top = nullptr;
    ASSERT_EQ(FitHorizontalDestination_GetTop(fith_handle, &result_top), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_top_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_top, &result_top_real), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 150.0);

    // Cleanup
    ASSERT_EQ(RealObject_Release(result_top_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_top), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FitHorizontalDestination_Release(fith_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(top_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitVerticalDestination) {
    // Create FitV destination array: [0 /FitV 75.0]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(2);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("FitV");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add left coordinate
    RealObjectHandle* left_handle = CreateRealObject(75.0);
    ObjectHandle* left_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(left_handle, &left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitVertical);

    // Convert to FitVertical destination and test parameter
    FitVerticalDestinationHandle* fitv_handle = nullptr;
    ASSERT_EQ(FitVerticalDestination_FromDestination(dest_handle, &fitv_handle), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    ObjectHandle* result_left = nullptr;
    ASSERT_EQ(FitVerticalDestination_GetLeft(fitv_handle, &result_left), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_left_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_left, &result_left_real), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 75.0);

    // Cleanup
    ASSERT_EQ(RealObject_Release(result_left_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_left), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FitVerticalDestination_Release(fitv_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(left_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitBoundingBoxDestination) {
    // Create FitB destination array: [0 /FitB]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(3);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("FitB");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitBoundingBox);

    // Convert to FitBoundingBox destination
    FitBoundingBoxDestinationHandle* fitb_handle = nullptr;
    ASSERT_EQ(FitBoundingBoxDestination_FromDestination(dest_handle, &fitb_handle), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    ASSERT_EQ(FitBoundingBoxDestination_Release(fitb_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitBoundingBoxHorizontalDestination) {
    // Create FitBH destination array: [0 /FitBH 200.0]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(4);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("FitBH");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add top coordinate
    RealObjectHandle* top_handle = CreateRealObject(200.0);
    ObjectHandle* top_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(top_handle, &top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, top_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitBoundingBoxHorizontal);

    // Convert to FitBoundingBoxHorizontal destination and test parameter
    FitBoundingBoxHorizontalDestinationHandle* fitbh_handle = nullptr;
    ASSERT_EQ(FitBoundingBoxHorizontalDestination_FromDestination(dest_handle, &fitbh_handle), VANILLAPDF_ERROR_SUCCESS);

    // Test top coordinate
    ObjectHandle* result_top = nullptr;
    ASSERT_EQ(FitBoundingBoxHorizontalDestination_GetTop(fitbh_handle, &result_top), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_top_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_top, &result_top_real), VANILLAPDF_ERROR_SUCCESS);
    double top_value;
    ASSERT_EQ(RealObject_GetValue(result_top_real, &top_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(top_value, 200.0);

    // Cleanup
    ASSERT_EQ(RealObject_Release(result_top_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_top), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FitBoundingBoxHorizontalDestination_Release(fitbh_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(top_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(top_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationArray, CreateFitBoundingBoxVerticalDestination) {
    // Create FitBV destination array: [0 /FitBV 125.0]
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(5);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("FitBV");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add left coordinate
    RealObjectHandle* left_handle = CreateRealObject(125.0);
    ObjectHandle* left_obj = nullptr;
    ASSERT_EQ(RealObject_ToObject(left_handle, &left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, left_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Verify destination type
    DestinationType dest_type;
    ASSERT_EQ(Destination_GetDestinationType(dest_handle, &dest_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(dest_type, DestinationType_FitBoundingBoxVertical);

    // Convert to FitBoundingBoxVertical destination and test parameter
    FitBoundingBoxVerticalDestinationHandle* fitbv_handle = nullptr;
    ASSERT_EQ(FitBoundingBoxVerticalDestination_FromDestination(dest_handle, &fitbv_handle), VANILLAPDF_ERROR_SUCCESS);

    // Test left coordinate
    ObjectHandle* result_left = nullptr;
    ASSERT_EQ(FitBoundingBoxVerticalDestination_GetLeft(fitbv_handle, &result_left), VANILLAPDF_ERROR_SUCCESS);
    RealObjectHandle* result_left_real = nullptr;
    ASSERT_EQ(RealObject_FromObject(result_left, &result_left_real), VANILLAPDF_ERROR_SUCCESS);
    double left_value;
    ASSERT_EQ(RealObject_GetValue(result_left_real, &left_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(left_value, 125.0);

    // Cleanup
    ASSERT_EQ(RealObject_Release(result_left_real), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(result_left), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FitBoundingBoxVerticalDestination_Release(fitbv_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(left_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(RealObject_Release(left_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, XYZDestinationWithNullParameters) {
    // Test XYZ destination with null left coordinate (should return false from GetLeft)
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(0);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("XYZ");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add null object for left coordinate
    NullObjectHandle* null_handle = nullptr;
    ASSERT_EQ(NullObject_Create(&null_handle), VANILLAPDF_ERROR_SUCCESS);
    ObjectHandle* null_obj = nullptr;
    ASSERT_EQ(NullObject_ToObject(null_handle, &null_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, null_obj), VANILLAPDF_ERROR_SUCCESS);

    // Create destination from array
    DestinationHandle* dest_handle = nullptr;
    ASSERT_EQ(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Convert to XYZ destination
    XYZDestinationHandle* xyz_handle = nullptr;
    ASSERT_EQ(XYZDestination_FromDestination(dest_handle, &xyz_handle), VANILLAPDF_ERROR_SUCCESS);

    // Test that GetLeft returns error for null parameter
    ObjectHandle* result_left = nullptr;
    EXPECT_EQ(XYZDestination_GetLeft(xyz_handle, &result_left), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Cleanup
    ASSERT_EQ(XYZDestination_Release(xyz_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Destination_Release(dest_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(null_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NullObject_Release(null_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, InvalidPageObjectType) {
    // Create array with string as page number (should fail)
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add invalid page object (string instead of integer/reference)
    LiteralStringObjectHandle* string_handle = nullptr;
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("invalid", &string_handle), VANILLAPDF_ERROR_SUCCESS);
    StringObjectHandle* string_as_string = nullptr;
    ASSERT_EQ(LiteralStringObject_ToStringObject(string_handle, &string_as_string), VANILLAPDF_ERROR_SUCCESS);
    ObjectHandle* string_obj = nullptr;
    ASSERT_EQ(StringObject_ToObject(string_as_string, &string_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, string_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add destination type
    NameObjectHandle* type_handle = CreateNameObject("XYZ");
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(NameObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail due to invalid page object type)
    DestinationHandle* dest_handle = nullptr;
    EXPECT_NE(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(NameObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(string_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StringObject_Release(string_as_string), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(string_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DestinationValidation, NonNameDestinationType) {
    // Create array with integer as destination type (should fail)
    ArrayObjectHandle* array_handle = nullptr;
    ASSERT_EQ(ArrayObject_Create(&array_handle), VANILLAPDF_ERROR_SUCCESS);

    // Add page number
    IntegerObjectHandle* page_handle = CreateIntegerObject(0);
    ObjectHandle* page_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(page_handle, &page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, page_obj), VANILLAPDF_ERROR_SUCCESS);

    // Add invalid destination type (integer instead of name)
    IntegerObjectHandle* type_handle = CreateIntegerObject(123);
    ObjectHandle* type_obj = nullptr;
    ASSERT_EQ(IntegerObject_ToObject(type_handle, &type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Append(array_handle, type_obj), VANILLAPDF_ERROR_SUCCESS);

    // Try to create destination (should fail due to invalid type object)
    DestinationHandle* dest_handle = nullptr;
    EXPECT_NE(Destination_CreateFromArray(array_handle, &dest_handle), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup
    ASSERT_EQ(Object_Release(type_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(type_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Object_Release(page_obj), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IntegerObject_Release(page_handle), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(array_handle), VANILLAPDF_ERROR_SUCCESS);
}

} // namespace destinations