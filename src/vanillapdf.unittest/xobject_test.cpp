#include "unittest.h"
#include "handle_guard.h"

#include <string>

namespace xobjects {

// Creates an empty in-memory document for tests that need
// document-registered indirect objects
static void CreateMemoryDocument(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document
) {
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
}

// Verifies that a name object holds the expected decoded contents.
// Buffer data carries no null terminator, so the length is checked
// explicitly and the contents compared element wise.
static void ExpectDecodedName(NameObjectHandle* handle, const std::string& expected) {
    HandleGuard<BufferHandle, Buffer_Release> buffer;
    ASSERT_EQ(NameObject_GetValue(handle, buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type data = nullptr;
    size_type size = 0;
    ASSERT_EQ(Buffer_GetData(buffer, &data, &size), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(size, expected.size());

    for (size_type i = 0; i < size; ++i) {
        EXPECT_EQ(data[i], expected[i]);
    }
}

TEST(FormXObject, CreateFromDocument) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<FormXObjectHandle, FormXObject_Release> form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_TRUE(form);

    // Verify derived type through the base class
    HandleGuard<XObjectHandle, XObject_Release> base_xobject;
    ASSERT_EQ(FormXObject_ToBaseXObject(form, base_xobject.out()), VANILLAPDF_ERROR_SUCCESS);

    XObjectType xobject_type = XObjectType_Undefined;
    ASSERT_EQ(XObject_GetXObjectType(base_xobject, &xobject_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(xobject_type, XObjectType_Form);

    // Down-conversion recovers the concrete type
    HandleGuard<FormXObjectHandle, FormXObject_Release> converted_form;
    ASSERT_EQ(FormXObject_FromBaseXObject(base_xobject, converted_form.out()), VANILLAPDF_ERROR_SUCCESS);

    // The new form carries the default zero bounding box
    HandleGuard<RectangleHandle, Rectangle_Release> bounding_box;
    ASSERT_EQ(FormXObject_GetBoundingBox(form, bounding_box.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type coordinate_value = 0.0;
    ASSERT_EQ(Rectangle_GetUpperRightXReal(bounding_box, &coordinate_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coordinate_value, 0.0);
}

TEST(FormXObject, BoundingBoxRoundtrip) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<FormXObjectHandle, FormXObject_Release> form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<RectangleHandle, Rectangle_Release> bounding_box;
    ASSERT_EQ(Rectangle_Create(bounding_box.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftXReal(bounding_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftYReal(bounding_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightXReal(bounding_box, 200.5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightYReal(bounding_box, 50.25), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(FormXObject_SetBoundingBox(form, bounding_box), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<RectangleHandle, Rectangle_Release> retrieved_box;
    ASSERT_EQ(FormXObject_GetBoundingBox(form, retrieved_box.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type coordinate_value = 0.0;
    ASSERT_EQ(Rectangle_GetUpperRightXReal(retrieved_box, &coordinate_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coordinate_value, 200.5);

    ASSERT_EQ(Rectangle_GetUpperRightYReal(retrieved_box, &coordinate_value), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coordinate_value, 50.25);
}

TEST(FormXObject, MatrixRoundtrip) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<FormXObjectHandle, FormXObject_Release> form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);

    // Matrix is optional and missing on a new form
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> missing_matrix;
    ASSERT_EQ(FormXObject_GetMatrix(form, missing_matrix.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Identity matrix [1 0 0 1 0 0]
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> matrix;
    ASSERT_EQ(ArrayObject_Create(matrix.out()), VANILLAPDF_ERROR_SUCCESS);

    const bigint_type matrix_values[] = { 1, 0, 0, 1, 0, 0 };
    for (auto matrix_value : matrix_values) {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer_value;
        ASSERT_EQ(IntegerObject_Create(integer_value.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(integer_value, matrix_value), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ObjectHandle, Object_Release> base_value;
        ASSERT_EQ(IntegerObject_ToObject(integer_value, base_value.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ArrayObject_Append(matrix, base_value), VANILLAPDF_ERROR_SUCCESS);
    }

    ASSERT_EQ(FormXObject_SetMatrix(form, matrix), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ArrayObjectHandle, ArrayObject_Release> retrieved_matrix;
    ASSERT_EQ(FormXObject_GetMatrix(form, retrieved_matrix.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type matrix_size = 0;
    ASSERT_EQ(ArrayObject_GetSize(retrieved_matrix, &matrix_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(matrix_size, 6u);
}

TEST(FormXObject, ResourcesRoundtrip) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<FormXObjectHandle, FormXObject_Release> form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);

    // Resources are optional and missing on a new form
    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> missing_resources;
    ASSERT_EQ(FormXObject_GetResources(form, missing_resources.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> resources_dictionary;
    ASSERT_EQ(DictionaryObject_Create(resources_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FormXObject_SetResources(form, resources_dictionary), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> retrieved_resources;
    ASSERT_EQ(FormXObject_GetResources(form, retrieved_resources.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(WidgetAnnotation, CreateFromDocument) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_TRUE(widget);

    HandleGuard<AnnotationHandle, Annotation_Release> base_annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, base_annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    AnnotationType annotation_type = AnnotationType_Undefined;
    ASSERT_EQ(Annotation_GetAnnotationType(base_annotation, &annotation_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(annotation_type, AnnotationType_Widget);

    // The new widget carries the default zero rectangle
    HandleGuard<RectangleHandle, Rectangle_Release> default_rect;
    ASSERT_EQ(Annotation_GetRect(base_annotation, default_rect.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type upper_right_x = -1;
    ASSERT_EQ(Rectangle_GetUpperRightXReal(default_rect, &upper_right_x), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(upper_right_x, 0);

    // The rectangle is set through the base annotation accessor
    HandleGuard<RectangleHandle, Rectangle_Release> rect;
    ASSERT_EQ(Rectangle_Create(rect.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftXReal(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftYReal(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightXReal(rect, 200), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightYReal(rect, 750), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetRect(base_annotation, rect), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<RectangleHandle, Rectangle_Release> retrieved_rect;
    ASSERT_EQ(Annotation_GetRect(base_annotation, retrieved_rect.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type lower_left_y = 0;
    ASSERT_EQ(Rectangle_GetLowerLeftYReal(retrieved_rect, &lower_left_y), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(lower_left_y, 700);
}

TEST(WidgetAnnotation, CreateFromDocumentNullParameters) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    WidgetAnnotationHandle* widget = nullptr;
    EXPECT_EQ(WidgetAnnotation_CreateFromDocument(nullptr, &widget), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(WidgetAnnotation_CreateFromDocument(document, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(widget, nullptr);
}

TEST(WidgetAnnotation, NormalAppearanceRoundtrip) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    // Appearance is missing on a new widget
    HandleGuard<FormXObjectHandle, FormXObject_Release> missing_appearance;
    ASSERT_EQ(WidgetAnnotation_GetNormalAppearance(widget, missing_appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<FormXObjectHandle, FormXObject_Release> appearance;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(WidgetAnnotation_SetNormalAppearance(widget, appearance), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> retrieved_appearance;
    ASSERT_EQ(WidgetAnnotation_GetNormalAppearance(widget, retrieved_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    // The retrieved appearance resolves to the same content stream
    HandleGuard<RectangleHandle, Rectangle_Release> bounding_box;
    ASSERT_EQ(FormXObject_GetBoundingBox(retrieved_appearance, bounding_box.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(WidgetAnnotation, AppearanceCharacteristicsRoundtrip) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> missing_characteristics;
    ASSERT_EQ(WidgetAnnotation_GetAppearanceCharacteristics(widget, missing_characteristics.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> characteristics;
    ASSERT_EQ(DictionaryObject_Create(characteristics.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(WidgetAnnotation_SetAppearanceCharacteristics(widget, characteristics), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> retrieved_characteristics;
    ASSERT_EQ(WidgetAnnotation_GetAppearanceCharacteristics(widget, retrieved_characteristics.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(ResourceDictionary, XObjectEnumeration) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> resources;
    ASSERT_EQ(ResourceDictionary_Create(resources.out()), VANILLAPDF_ERROR_SUCCESS);

    // Empty dictionary has no /XObject entry
    size_type xobject_count = 0;
    ASSERT_EQ(ResourceDictionary_GetXObjectCount(resources, &xobject_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(xobject_count, 0u);

    HandleGuard<NameObjectHandle, NameObject_Release> first_name;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Frm0", first_name.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XObjectHandle, XObject_Release> missing_xobject;
    ASSERT_EQ(ResourceDictionary_FindXObject(resources, first_name, missing_xobject.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Register two forms
    HandleGuard<FormXObjectHandle, FormXObject_Release> first_form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, first_form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XObjectHandle, XObject_Release> first_form_base;
    ASSERT_EQ(FormXObject_ToBaseXObject(first_form, first_form_base.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ResourceDictionary_AddXObject(resources, first_name, first_form_base), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> second_name;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Frm1", second_name.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> second_form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, second_form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XObjectHandle, XObject_Release> second_form_base;
    ASSERT_EQ(FormXObject_ToBaseXObject(second_form, second_form_base.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ResourceDictionary_AddXObject(resources, second_name, second_form_base), VANILLAPDF_ERROR_SUCCESS);

    // Mutation is reflected in the enumeration
    ASSERT_EQ(ResourceDictionary_GetXObjectCount(resources, &xobject_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(xobject_count, 2u);

    HandleGuard<NameObjectHandle, NameObject_Release> enumerated_name;
    ASSERT_EQ(ResourceDictionary_GetXObjectName(resources, 0, enumerated_name.out()), VANILLAPDF_ERROR_SUCCESS);
    ExpectDecodedName(enumerated_name, "Frm0");

    // Index outside the enumeration is rejected
    HandleGuard<NameObjectHandle, NameObject_Release> out_of_range_name;
    ASSERT_EQ(ResourceDictionary_GetXObjectName(resources, 2, out_of_range_name.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Lookup resolves the registered form through the base class
    HandleGuard<XObjectHandle, XObject_Release> found_xobject;
    ASSERT_EQ(ResourceDictionary_FindXObject(resources, first_name, found_xobject.out()), VANILLAPDF_ERROR_SUCCESS);

    XObjectType xobject_type = XObjectType_Undefined;
    ASSERT_EQ(XObject_GetXObjectType(found_xobject, &xobject_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(xobject_type, XObjectType_Form);

    HandleGuard<FormXObjectHandle, FormXObject_Release> found_form;
    ASSERT_EQ(FormXObject_FromBaseXObject(found_xobject, found_form.out()), VANILLAPDF_ERROR_SUCCESS);

    // Removal is reflected in the enumeration
    boolean_type removed = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(ResourceDictionary_RemoveXObject(resources, first_name, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(ResourceDictionary_GetXObjectCount(resources, &xobject_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(xobject_count, 1u);

    ASSERT_EQ(ResourceDictionary_RemoveXObject(resources, first_name, &removed), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(removed, VANILLAPDF_RV_FALSE);
}

} // namespace xobjects
