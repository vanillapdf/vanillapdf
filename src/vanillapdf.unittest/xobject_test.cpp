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

// Registers a form XObject under an appearance state name within an
// appearance state subdictionary, mirroring the /AP /N << /Off ... >> shape
static void AppendAppearanceState(
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release>& states,
    const std::string& state,
    HandleGuard<FormXObjectHandle, FormXObject_Release>& appearance
) {
    HandleGuard<NameObjectHandle, NameObject_Release> state_key;
    ASSERT_EQ(NameObject_CreateFromEncodedString(state.c_str(), state_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XObjectHandle, XObject_Release> base_xobject;
    ASSERT_EQ(FormXObject_ToBaseXObject(appearance, base_xobject.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> appearance_object;
    ASSERT_EQ(XObject_GetBaseObject(base_xobject, appearance_object.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DictionaryObject_Insert(states, state_key, appearance_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
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
    HandleGuard<MatrixHandle, Matrix_Release> missing_matrix;
    ASSERT_EQ(FormXObject_GetMatrix(form, missing_matrix.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // A new matrix is the identity [1 0 0 1 0 0]
    HandleGuard<MatrixHandle, Matrix_Release> matrix;
    ASSERT_EQ(Matrix_Create(matrix.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type identity_scale = 0.0;
    ASSERT_EQ(Matrix_GetA(matrix, &identity_scale), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(identity_scale, 1.0);

    // Scale by half and translate, exercising fractional coefficients
    ASSERT_EQ(Matrix_SetA(matrix, 0.5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Matrix_SetD(matrix, 0.5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Matrix_SetE(matrix, 10.25), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(FormXObject_SetMatrix(form, matrix), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<MatrixHandle, Matrix_Release> retrieved_matrix;
    ASSERT_EQ(FormXObject_GetMatrix(form, retrieved_matrix.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type coefficient = 0.0;
    ASSERT_EQ(Matrix_GetA(retrieved_matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 0.5);
    ASSERT_EQ(Matrix_GetB(retrieved_matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 0.0);
    ASSERT_EQ(Matrix_GetC(retrieved_matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 0.0);
    ASSERT_EQ(Matrix_GetD(retrieved_matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 0.5);
    ASSERT_EQ(Matrix_GetE(retrieved_matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 10.25);
    ASSERT_EQ(Matrix_GetF(retrieved_matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 0.0);
}

TEST(FormXObject, MatrixReadsIntegerCoefficients) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<FormXObjectHandle, FormXObject_Release> form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, form.out()), VANILLAPDF_ERROR_SUCCESS);

    // Matrices in real documents are commonly written as integers - [1 0 0 1 0 0].
    // Inject one through the low-level API and read it back as a matrix.
    HandleGuard<ArrayObjectHandle, ArrayObject_Release> matrix_array;
    ASSERT_EQ(ArrayObject_Create(matrix_array.out()), VANILLAPDF_ERROR_SUCCESS);

    const bigint_type matrix_values[] = { 1, 0, 0, 1, 0, 0 };
    for (auto matrix_value : matrix_values) {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer_value;
        ASSERT_EQ(IntegerObject_Create(integer_value.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(IntegerObject_SetIntegerValue(integer_value, matrix_value), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ObjectHandle, Object_Release> base_value;
        ASSERT_EQ(IntegerObject_ToObject(integer_value, base_value.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ArrayObject_Append(matrix_array, base_value), VANILLAPDF_ERROR_SUCCESS);
    }

    HandleGuard<XObjectHandle, XObject_Release> base_xobject;
    ASSERT_EQ(FormXObject_ToBaseXObject(form, base_xobject.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> form_object;
    ASSERT_EQ(XObject_GetBaseObject(base_xobject, form_object.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StreamObjectHandle, StreamObject_Release> form_stream;
    ASSERT_EQ(StreamObject_FromObject(form_object, form_stream.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> form_header;
    ASSERT_EQ(StreamObject_GetHeader(form_stream, form_header.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> matrix_key;
    ASSERT_EQ(NameObject_CreateFromEncodedString("Matrix", matrix_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> matrix_value;
    ASSERT_EQ(ArrayObject_ToObject(matrix_array, matrix_value.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(form_header, matrix_key, matrix_value, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Integer coefficients are read through the shared numeric backend
    HandleGuard<MatrixHandle, Matrix_Release> matrix;
    ASSERT_EQ(FormXObject_GetMatrix(form, matrix.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type coefficient = 0.0;
    ASSERT_EQ(Matrix_GetA(matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 1.0);
    ASSERT_EQ(Matrix_GetD(matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 1.0);
    ASSERT_EQ(Matrix_GetF(matrix, &coefficient), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(coefficient, 0.0);
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

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> resources;
    ASSERT_EQ(ResourceDictionary_Create(resources.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FormXObject_SetResources(form, resources), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> retrieved_resources;
    ASSERT_EQ(FormXObject_GetResources(form, retrieved_resources.out()), VANILLAPDF_ERROR_SUCCESS);

    // The round-tripped handle addresses the same resource dictionary
    HandleGuard<FormXObjectHandle, FormXObject_Release> registered_form;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, registered_form.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<XObjectHandle, XObject_Release> registered_xobject;
    ASSERT_EQ(FormXObject_ToBaseXObject(registered_form, registered_xobject.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> resource_name;
    ASSERT_EQ(NameObject_CreateFromEncodedString("Fm0", resource_name.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ResourceDictionary_AddXObject(resources, resource_name, registered_xobject), VANILLAPDF_ERROR_SUCCESS);

    size_type xobject_count = 0;
    ASSERT_EQ(ResourceDictionary_GetXObjectCount(retrieved_resources, &xobject_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(xobject_count, 1u);
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

TEST(Annotation, AppearanceRoundtrip) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    // The appearance dictionary lives on the base annotation
    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    // Every slot is missing on a new annotation
    HandleGuard<FormXObjectHandle, FormXObject_Release> missing_appearance;
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Normal, missing_appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Rollover, missing_appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Down, missing_appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // An unnamed slot is rejected rather than silently treated as normal
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Undefined, missing_appearance.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<FormXObjectHandle, FormXObject_Release> appearance;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetAppearance(annotation, AppearanceType_Normal, appearance), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> retrieved_appearance;
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Normal, retrieved_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    // The retrieved appearance resolves to the same content stream
    HandleGuard<RectangleHandle, Rectangle_Release> bounding_box;
    ASSERT_EQ(FormXObject_GetBoundingBox(retrieved_appearance, bounding_box.out()), VANILLAPDF_ERROR_SUCCESS);

    // Writing one slot leaves the others alone
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Down, missing_appearance.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<FormXObjectHandle, FormXObject_Release> down_appearance;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, down_appearance.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetAppearance(annotation, AppearanceType_Down, down_appearance), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> retrieved_down;
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Down, retrieved_down.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> still_normal;
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Normal, still_normal.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Annotation, AppearanceStateResolvesStateKeyedSlot) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<AnnotationHandle, Annotation_Release> annotation;
    ASSERT_EQ(WidgetAnnotation_ToBaseAnnotation(widget, annotation.out()), VANILLAPDF_ERROR_SUCCESS);

    // Build the /AP /N << /Off ... /Yes ... >> shape a check box widget carries
    HandleGuard<FormXObjectHandle, FormXObject_Release> off_appearance;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, off_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> yes_appearance;
    ASSERT_EQ(FormXObject_CreateFromDocument(document, yes_appearance.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> states;
    ASSERT_EQ(DictionaryObject_Create(states.out()), VANILLAPDF_ERROR_SUCCESS);
    AppendAppearanceState(states, "Off", off_appearance);
    AppendAppearanceState(states, "Yes", yes_appearance);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> appearance_dictionary;
    ASSERT_EQ(DictionaryObject_Create(appearance_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> normal_key;
    ASSERT_EQ(NameObject_CreateFromEncodedString("N", normal_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> states_object;
    ASSERT_EQ(DictionaryObject_ToObject(states, states_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(appearance_dictionary, normal_key, states_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> annotation_dictionary;
    ASSERT_EQ(Annotation_GetBaseObject(annotation, annotation_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> appearance_key;
    ASSERT_EQ(NameObject_CreateFromEncodedString("AP", appearance_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> appearance_object;
    ASSERT_EQ(DictionaryObject_ToObject(appearance_dictionary, appearance_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(annotation_dictionary, appearance_key, appearance_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    // Without /AS no state is in effect, so the slot reports nothing
    HandleGuard<FormXObjectHandle, FormXObject_Release> unresolved;
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Normal, unresolved.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<NameObjectHandle, NameObject_Release> missing_state;
    ASSERT_EQ(Annotation_GetAppearanceState(annotation, missing_state.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Selecting a state resolves the slot to that state's stream
    HandleGuard<NameObjectHandle, NameObject_Release> yes_state;
    ASSERT_EQ(NameObject_CreateFromEncodedString("Yes", yes_state.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetAppearanceState(annotation, yes_state), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> retrieved_state;
    ASSERT_EQ(Annotation_GetAppearanceState(annotation, retrieved_state.out()), VANILLAPDF_ERROR_SUCCESS);
    ExpectDecodedName(retrieved_state, "Yes");

    HandleGuard<FormXObjectHandle, FormXObject_Release> resolved;
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Normal, resolved.out()), VANILLAPDF_ERROR_SUCCESS);

    // A state that is not present in the subdictionary reports nothing
    HandleGuard<NameObjectHandle, NameObject_Release> unknown_state;
    ASSERT_EQ(NameObject_CreateFromEncodedString("Maybe", unknown_state.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_SetAppearanceState(annotation, unknown_state), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FormXObjectHandle, FormXObject_Release> unknown;
    ASSERT_EQ(Annotation_GetAppearance(annotation, AppearanceType_Normal, unknown.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    // Overwriting a state-keyed slot with a single stream would drop the
    // other states, so it is refused
    ASSERT_EQ(Annotation_SetAppearance(annotation, AppearanceType_Normal, off_appearance), VANILLAPDF_ERROR_NOT_SUPPORTED);
}

TEST(WidgetAnnotation, AppearanceCharacteristicsRoundtrip) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    CreateMemoryDocument(io_stream, file, document);

    HandleGuard<WidgetAnnotationHandle, WidgetAnnotation_Release> widget;
    ASSERT_EQ(WidgetAnnotation_CreateFromDocument(document, widget.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<AppearanceCharacteristicsHandle, AppearanceCharacteristics_Release> missing_characteristics;
    ASSERT_EQ(WidgetAnnotation_GetAppearanceCharacteristics(widget, missing_characteristics.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<AppearanceCharacteristicsHandle, AppearanceCharacteristics_Release> characteristics;
    ASSERT_EQ(AppearanceCharacteristics_Create(characteristics.out()), VANILLAPDF_ERROR_SUCCESS);

    // Every entry is optional and absent on a new dictionary
    bigint_type missing_rotation = 0;
    ASSERT_EQ(AppearanceCharacteristics_GetRotation(characteristics, &missing_rotation), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<ColorHandle, Color_Release> missing_border_color;
    ASSERT_EQ(AppearanceCharacteristics_GetBorderColor(characteristics, missing_border_color.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    HandleGuard<StringObjectHandle, StringObject_Release> missing_caption;
    ASSERT_EQ(AppearanceCharacteristics_GetNormalCaption(characteristics, missing_caption.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    ASSERT_EQ(AppearanceCharacteristics_SetRotation(characteristics, 90), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ColorHandle, Color_Release> border_color;
    ASSERT_EQ(Color_CreateRGB(1.0, 0.0, 0.0, border_color.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(AppearanceCharacteristics_SetBorderColor(characteristics, border_color), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ColorHandle, Color_Release> background_color;
    ASSERT_EQ(Color_CreateGray(0.75, background_color.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(AppearanceCharacteristics_SetBackgroundColor(characteristics, background_color), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> caption;
    ASSERT_EQ(LiteralStringObject_CreateFromEncodedString("Submit", caption.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<StringObjectHandle, StringObject_Release> base_caption;
    ASSERT_EQ(LiteralStringObject_ToStringObject(caption, base_caption.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(AppearanceCharacteristics_SetNormalCaption(characteristics, base_caption), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(WidgetAnnotation_SetAppearanceCharacteristics(widget, characteristics), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<AppearanceCharacteristicsHandle, AppearanceCharacteristics_Release> retrieved_characteristics;
    ASSERT_EQ(WidgetAnnotation_GetAppearanceCharacteristics(widget, retrieved_characteristics.out()), VANILLAPDF_ERROR_SUCCESS);

    bigint_type rotation = 0;
    ASSERT_EQ(AppearanceCharacteristics_GetRotation(retrieved_characteristics, &rotation), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(rotation, 90);

    HandleGuard<ColorHandle, Color_Release> retrieved_border_color;
    ASSERT_EQ(AppearanceCharacteristics_GetBorderColor(retrieved_characteristics, retrieved_border_color.out()), VANILLAPDF_ERROR_SUCCESS);

    ColorSpaceType border_color_space = ColorSpace_Transparent;
    ASSERT_EQ(Color_GetColorSpace(retrieved_border_color, &border_color_space), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(border_color_space, ColorSpace_DeviceRGB);

    real_type red_component = 0.0;
    ASSERT_EQ(Color_GetRed(retrieved_border_color, &red_component), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(red_component, 1.0);

    HandleGuard<ColorHandle, Color_Release> retrieved_background_color;
    ASSERT_EQ(AppearanceCharacteristics_GetBackgroundColor(retrieved_characteristics, retrieved_background_color.out()), VANILLAPDF_ERROR_SUCCESS);

    real_type gray_component = 0.0;
    ASSERT_EQ(Color_GetGray(retrieved_background_color, &gray_component), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(gray_component, 0.75);

    HandleGuard<StringObjectHandle, StringObject_Release> retrieved_caption;
    ASSERT_EQ(AppearanceCharacteristics_GetNormalCaption(retrieved_characteristics, retrieved_caption.out()), VANILLAPDF_ERROR_SUCCESS);

    // The unmodelled entries stay reachable through the low-level dictionary
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> characteristics_dictionary;
    ASSERT_EQ(AppearanceCharacteristics_GetBaseObject(retrieved_characteristics, characteristics_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);
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
