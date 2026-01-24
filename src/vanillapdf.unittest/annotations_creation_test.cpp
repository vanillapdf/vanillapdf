#include "unittest.h"

namespace annotations {

TEST(TextAnnotation, CreateBasic) {
    RectangleHandle* rect = nullptr;
    TextAnnotationHandle* annot = nullptr;
    AnnotationHandle* base_annot = nullptr;
    AnnotationType annot_type = AnnotationType_Undefined;

    // Create rectangle
    ASSERT_EQ(Rectangle_Create(&rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(rect, nullptr);

    ASSERT_EQ(Rectangle_SetLowerLeftX(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(rect, 200), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(rect, 750), VANILLAPDF_ERROR_SUCCESS);

    // Create text annotation
    ASSERT_EQ(TextAnnotation_Create(rect, &annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(annot, nullptr);

    // Convert to base and verify type
    ASSERT_EQ(TextAnnotation_ToBaseAnnotation(annot, &base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_annot, nullptr);

    ASSERT_EQ(Annotation_GetAnnotationType(base_annot, &annot_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(annot_type, AnnotationType_Text);

    // Cleanup
    ASSERT_EQ(Annotation_Release(base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextAnnotation_Release(annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(rect), VANILLAPDF_ERROR_SUCCESS);
}

TEST(TextAnnotation, CreateWithContents) {
    RectangleHandle* rect = nullptr;
    LiteralStringObjectHandle* contents = nullptr;
    TextAnnotationHandle* annot = nullptr;
    AnnotationHandle* base_annot = nullptr;
    LiteralStringObjectHandle* retrieved_contents = nullptr;

    // Create rectangle
    ASSERT_EQ(Rectangle_Create(&rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(rect, nullptr);

    ASSERT_EQ(Rectangle_SetLowerLeftX(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(rect, 200), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(rect, 750), VANILLAPDF_ERROR_SUCCESS);

    // Create contents string
    const char* text = "This is a test comment";
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString(text, &contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(contents, nullptr);

    // Create text annotation with contents
    ASSERT_EQ(TextAnnotation_CreateWithContents(rect, contents, &annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(annot, nullptr);

    // Convert to base and retrieve contents
    ASSERT_EQ(TextAnnotation_ToBaseAnnotation(annot, &base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_annot, nullptr);

    ASSERT_EQ(Annotation_GetContents(base_annot, &retrieved_contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(retrieved_contents, nullptr);

    // Cleanup
    ASSERT_EQ(LiteralStringObject_Release(retrieved_contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_Release(base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextAnnotation_Release(annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(rect), VANILLAPDF_ERROR_SUCCESS);
}

TEST(HighlightAnnotation, CreateWithQuadPoints) {
    RectangleHandle* rect = nullptr;
    ArrayObjectHandle* quad_points = nullptr;
    HighlightAnnotationHandle* annot = nullptr;
    AnnotationHandle* base_annot = nullptr;
    AnnotationType annot_type = AnnotationType_Undefined;

    // Create rectangle
    ASSERT_EQ(Rectangle_Create(&rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(rect, nullptr);

    ASSERT_EQ(Rectangle_SetLowerLeftX(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(rect, 200), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(rect, 750), VANILLAPDF_ERROR_SUCCESS);

    // Create quad points array (8 numbers for one quadrilateral)
    ASSERT_EQ(ArrayObject_Create(&quad_points), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(quad_points, nullptr);

    // Add 8 real values for the quad points
    RealObjectHandle* real_val = nullptr;
    double values[] = {100.0, 700.0, 200.0, 700.0, 200.0, 750.0, 100.0, 750.0};
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(RealObject_Create(&real_val), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(RealObject_SetValue(real_val, values[i]), VANILLAPDF_ERROR_SUCCESS);
        ObjectHandle* obj = nullptr;
        ASSERT_EQ(RealObject_ToObject(real_val, &obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ArrayObject_Append(quad_points, obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Object_Release(obj), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(RealObject_Release(real_val), VANILLAPDF_ERROR_SUCCESS);
    }

    // Create highlight annotation
    ASSERT_EQ(HighlightAnnotation_Create(rect, quad_points, &annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(annot, nullptr);

    // Convert to base and verify type
    ASSERT_EQ(HighlightAnnotation_ToBaseAnnotation(annot, &base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_annot, nullptr);

    ASSERT_EQ(Annotation_GetAnnotationType(base_annot, &annot_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(annot_type, AnnotationType_Highlight);

    // Cleanup
    ASSERT_EQ(Annotation_Release(base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(HighlightAnnotation_Release(annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ArrayObject_Release(quad_points), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(rect), VANILLAPDF_ERROR_SUCCESS);
}

TEST(FreeTextAnnotation, CreateWithDefaultAppearance) {
    RectangleHandle* rect = nullptr;
    LiteralStringObjectHandle* contents = nullptr;
    LiteralStringObjectHandle* default_appearance = nullptr;
    FreeTextAnnotationHandle* annot = nullptr;
    AnnotationHandle* base_annot = nullptr;
    AnnotationType annot_type = AnnotationType_Undefined;
    LiteralStringObjectHandle* retrieved_da = nullptr;

    // Create rectangle
    ASSERT_EQ(Rectangle_Create(&rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(rect, nullptr);

    ASSERT_EQ(Rectangle_SetLowerLeftX(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(rect, 300), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(rect, 750), VANILLAPDF_ERROR_SUCCESS);

    // Create contents and default appearance strings
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("Free text content", &contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(contents, nullptr);

    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("/Helv 12 Tf 0 g", &default_appearance), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(default_appearance, nullptr);

    // Create free text annotation
    ASSERT_EQ(FreeTextAnnotation_Create(rect, contents, default_appearance, &annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(annot, nullptr);

    // Convert to base and verify type
    ASSERT_EQ(FreeTextAnnotation_ToBaseAnnotation(annot, &base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_annot, nullptr);

    ASSERT_EQ(Annotation_GetAnnotationType(base_annot, &annot_type), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(annot_type, AnnotationType_FreeText);

    // Get default appearance
    ASSERT_EQ(FreeTextAnnotation_GetDefaultAppearance(annot, &retrieved_da), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(retrieved_da, nullptr);

    // Cleanup
    ASSERT_EQ(LiteralStringObject_Release(retrieved_da), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Annotation_Release(base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FreeTextAnnotation_Release(annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(default_appearance), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(rect), VANILLAPDF_ERROR_SUCCESS);
}

TEST(PageAnnotations, AppendAnnotation) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    PageObjectHandle* page = nullptr;
    PageAnnotationsHandle* annots = nullptr;
    RectangleHandle* rect = nullptr;
    TextAnnotationHandle* text_annot = nullptr;
    AnnotationHandle* base_annot = nullptr;
    size_type annots_size = 0;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(io_stream, nullptr);

    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(file, nullptr);

    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(doc, nullptr);

    // Get catalog and page tree
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(catalog, nullptr);

    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page_tree, nullptr);

    // Create a new page
    ASSERT_EQ(PageObject_CreateFromDocument(doc, &page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page, nullptr);

    // Set media box
    RectangleHandle* media_box = nullptr;
    ASSERT_EQ(Rectangle_Create(&media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftX(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(media_box, 612), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(media_box, 792), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetMediaBox(page, media_box), VANILLAPDF_ERROR_SUCCESS);

    // Append page to tree
    ASSERT_EQ(PageTree_AppendPage(page_tree, page), VANILLAPDF_ERROR_SUCCESS);

    // Create annotations array for the page
    ASSERT_EQ(PageObject_CreateAnnotations(page, &annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(annots, nullptr);

    // Create rectangle for annotation
    ASSERT_EQ(Rectangle_Create(&rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftX(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(rect, 200), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(rect, 750), VANILLAPDF_ERROR_SUCCESS);

    // Create text annotation
    ASSERT_EQ(TextAnnotation_Create(rect, &text_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(text_annot, nullptr);

    // Convert to base annotation
    ASSERT_EQ(TextAnnotation_ToBaseAnnotation(text_annot, &base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_annot, nullptr);

    // Append annotation to page
    ASSERT_EQ(PageAnnotations_Append(annots, base_annot), VANILLAPDF_ERROR_SUCCESS);

    // Verify size increased
    ASSERT_EQ(PageAnnotations_GetSize(annots, &annots_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(annots_size, 1u);

    // Cleanup
    ASSERT_EQ(Annotation_Release(base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextAnnotation_Release(text_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageAnnotations_Release(annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}

// TODO: This test causes infinite memory growth when saving to the same stream.
// See https://github.com/vanillapdf/vanillapdf/issues/156
// Saving to the same InputOutputStream that was used for document creation
// causes an infinite loop in the file writer.
#if 0
TEST(AnnotationIntegration, CreateAndSave) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    PageObjectHandle* page = nullptr;
    PageAnnotationsHandle* annots = nullptr;
    RectangleHandle* rect = nullptr;
    LiteralStringObjectHandle* contents = nullptr;
    TextAnnotationHandle* text_annot = nullptr;
    AnnotationHandle* base_annot = nullptr;

    FileHandle* reload_file = nullptr;
    DocumentHandle* reload_doc = nullptr;
    CatalogHandle* reload_catalog = nullptr;
    PageTreeHandle* reload_page_tree = nullptr;
    PageObjectHandle* reload_page = nullptr;
    PageAnnotationsHandle* reload_annots = nullptr;
    size_type reload_annots_size = 0;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(io_stream, nullptr);

    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(file, nullptr);

    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(doc, nullptr);

    // Get catalog and page tree
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(catalog, nullptr);

    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page_tree, nullptr);

    // Create a new page
    ASSERT_EQ(PageObject_CreateFromDocument(doc, &page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page, nullptr);

    // Set media box
    RectangleHandle* media_box = nullptr;
    ASSERT_EQ(Rectangle_Create(&media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftX(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(media_box, 612), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(media_box, 792), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetMediaBox(page, media_box), VANILLAPDF_ERROR_SUCCESS);

    // Append page to tree
    ASSERT_EQ(PageTree_AppendPage(page_tree, page), VANILLAPDF_ERROR_SUCCESS);

    // Create annotations array for the page
    ASSERT_EQ(PageObject_CreateAnnotations(page, &annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(annots, nullptr);

    // Create rectangle for annotation
    ASSERT_EQ(Rectangle_Create(&rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftX(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(rect, 200), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(rect, 750), VANILLAPDF_ERROR_SUCCESS);

    // Create contents string
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("Test annotation", &contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(contents, nullptr);

    // Create text annotation with contents
    ASSERT_EQ(TextAnnotation_CreateWithContents(rect, contents, &text_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(text_annot, nullptr);

    // Convert to base annotation and append
    ASSERT_EQ(TextAnnotation_ToBaseAnnotation(text_annot, &base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_annot, nullptr);

    ASSERT_EQ(PageAnnotations_Append(annots, base_annot), VANILLAPDF_ERROR_SUCCESS);

    // Save document
    ASSERT_EQ(Document_SaveFile(doc, file), VANILLAPDF_ERROR_SUCCESS);

    // Reload and verify
    ASSERT_EQ(File_OpenStream(io_stream, "temp", &reload_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_file, nullptr);

    ASSERT_EQ(File_Initialize(reload_file), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Document_OpenFile(reload_file, &reload_doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_doc, nullptr);

    ASSERT_EQ(Document_GetCatalog(reload_doc, &reload_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_catalog, nullptr);

    ASSERT_EQ(Catalog_GetPages(reload_catalog, &reload_page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_page_tree, nullptr);

    ASSERT_EQ(PageTree_GetPage(reload_page_tree, 0, &reload_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_page, nullptr);

    ASSERT_EQ(PageObject_GetAnnotations(reload_page, &reload_annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_annots, nullptr);

    ASSERT_EQ(PageAnnotations_GetSize(reload_annots, &reload_annots_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(reload_annots_size, 1u);

    // Cleanup reload
    ASSERT_EQ(PageAnnotations_Release(reload_annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(reload_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(reload_page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(reload_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(reload_doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(reload_file), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup original
    ASSERT_EQ(Annotation_Release(base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextAnnotation_Release(text_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageAnnotations_Release(annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
}
#endif

TEST(AnnotationIntegration, CreateAndSaveToSeparateStream) {
    FileHandle* file = nullptr;
    DocumentHandle* doc = nullptr;
    InputOutputStreamHandle* io_stream = nullptr;
    CatalogHandle* catalog = nullptr;
    PageTreeHandle* page_tree = nullptr;
    PageObjectHandle* page = nullptr;
    PageAnnotationsHandle* annots = nullptr;
    RectangleHandle* rect = nullptr;
    LiteralStringObjectHandle* contents = nullptr;
    TextAnnotationHandle* text_annot = nullptr;
    AnnotationHandle* base_annot = nullptr;

    // Separate stream for saving (to avoid infinite loop when source == destination)
    InputOutputStreamHandle* output_stream = nullptr;
    FileHandle* output_file = nullptr;

    FileHandle* reload_file = nullptr;
    DocumentHandle* reload_doc = nullptr;
    CatalogHandle* reload_catalog = nullptr;
    PageTreeHandle* reload_page_tree = nullptr;
    PageObjectHandle* reload_page = nullptr;
    PageAnnotationsHandle* reload_annots = nullptr;
    size_type reload_annots_size = 0;

    // Create in-memory document
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(io_stream, nullptr);

    ASSERT_EQ(File_CreateStream(io_stream, "temp", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(file, nullptr);

    ASSERT_EQ(Document_CreateFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(doc, nullptr);

    // Get catalog and page tree
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(catalog, nullptr);

    ASSERT_EQ(Catalog_GetPages(catalog, &page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page_tree, nullptr);

    // Create a new page
    ASSERT_EQ(PageObject_CreateFromDocument(doc, &page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page, nullptr);

    // Set media box
    RectangleHandle* media_box = nullptr;
    ASSERT_EQ(Rectangle_Create(&media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftX(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(media_box, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(media_box, 612), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(media_box, 792), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetMediaBox(page, media_box), VANILLAPDF_ERROR_SUCCESS);

    // Append page to tree
    ASSERT_EQ(PageTree_AppendPage(page_tree, page), VANILLAPDF_ERROR_SUCCESS);

    // Create annotations array for the page
    ASSERT_EQ(PageObject_CreateAnnotations(page, &annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(annots, nullptr);

    // Create rectangle for annotation
    ASSERT_EQ(Rectangle_Create(&rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftX(rect, 100), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetLowerLeftY(rect, 700), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightX(rect, 200), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_SetUpperRightY(rect, 750), VANILLAPDF_ERROR_SUCCESS);

    // Create contents string
    ASSERT_EQ(LiteralStringObject_CreateFromDecodedString("Test annotation", &contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(contents, nullptr);

    // Create text annotation with contents
    ASSERT_EQ(TextAnnotation_CreateWithContents(rect, contents, &text_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(text_annot, nullptr);

    // Convert to base annotation and append
    ASSERT_EQ(TextAnnotation_ToBaseAnnotation(text_annot, &base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(base_annot, nullptr);

    ASSERT_EQ(PageAnnotations_Append(annots, base_annot), VANILLAPDF_ERROR_SUCCESS);

    // Create separate output stream for saving
    // (saving to the same stream causes infinite loop - see issue #156)
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&output_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(output_stream, nullptr);

    ASSERT_EQ(File_CreateStream(output_stream, "output", &output_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(output_file, nullptr);

    // Save document to separate output file
    ASSERT_EQ(Document_SaveFile(doc, output_file), VANILLAPDF_ERROR_SUCCESS);

    // Reload from output stream and verify
    ASSERT_EQ(File_OpenStream(output_stream, "output", &reload_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_file, nullptr);

    ASSERT_EQ(File_Initialize(reload_file), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Document_OpenFile(reload_file, &reload_doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_doc, nullptr);

    ASSERT_EQ(Document_GetCatalog(reload_doc, &reload_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_catalog, nullptr);

    ASSERT_EQ(Catalog_GetPages(reload_catalog, &reload_page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_page_tree, nullptr);

    // Note: Page indices are 1-based in VanillaPDF
    // Document_CreateFile creates 1 empty page, and we appended another, so we have 2 pages
    // Our annotation is on page 2
    ASSERT_EQ(PageTree_GetPage(reload_page_tree, 2, &reload_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_page, nullptr);

    ASSERT_EQ(PageObject_GetAnnotations(reload_page, &reload_annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(reload_annots, nullptr);

    ASSERT_EQ(PageAnnotations_GetSize(reload_annots, &reload_annots_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(reload_annots_size, 1u);

    // Cleanup reload
    ASSERT_EQ(PageAnnotations_Release(reload_annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(reload_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(reload_page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(reload_catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(reload_doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(reload_file), VANILLAPDF_ERROR_SUCCESS);

    // Cleanup original
    ASSERT_EQ(Annotation_Release(base_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TextAnnotation_Release(text_annot), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(LiteralStringObject_Release(contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(rect), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageAnnotations_Release(annots), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_Release(media_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_Release(page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_Release(page_tree), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_Release(catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(output_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(output_stream), VANILLAPDF_ERROR_SUCCESS);
}

} // namespace annotations
