#include <stdio.h>
#include <string.h>
#include <vanillapdf/c_vanillapdf_api.h>

/* Store the result of a call and jump to the cleanup when it failed */
#define CHECK(call) \
    do { \
        rc = (call); \
        if (rc != VANILLAPDF_ERROR_SUCCESS) { \
            goto cleanup; \
        } \
    } while (0)

int main(int argc, char* argv[]) {
    const char* field_name = "Signature1";

    DocumentHandle* document = NULL;
    CatalogHandle* catalog = NULL;
    PageTreeHandle* page_tree = NULL;
    PageObjectHandle* page = NULL;
    PageAnnotationsHandle* page_annotations = NULL;
    LiteralStringObjectHandle* name_literal = NULL;
    StringObjectHandle* name = NULL;
    SignatureFieldHandle* signature_field = NULL;
    FieldHandle* field = NULL;
    WidgetAnnotationHandle* widget = NULL;
    AnnotationHandle* annotation = NULL;
    RectangleHandle* rect = NULL;
    FormXObjectHandle* appearance = NULL;
    PKCS12KeyHandle* key_file = NULL;
    SigningKeyHandle* signing_key = NULL;
    DocumentSignatureSettingsHandle* settings = NULL;
    FileHandle* destination = NULL;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input.pdf> <signed.pdf> <key.p12> <password>\n", argv[0]);
        return 1;
    }

    CHECK(Document_Open(argv[1], &document));

    /*
    * The signature field joins the field hierarchy of the interactive form,
    * which is created when the document has none. At the root of the
    * hierarchy its partial name is also its fully qualified name.
    */
    CHECK(LiteralStringObject_CreateFromEncodedString(field_name, &name_literal));
    CHECK(LiteralStringObject_ToStringObject(name_literal, &name));
    CHECK(SignatureField_CreateFromDocument(document, name, &signature_field));
    CHECK(SignatureField_ToField(signature_field, &field));

    /* The widget annotation is what a viewer shows: a rectangle on the page */
    CHECK(WidgetAnnotation_CreateFromDocument(document, &widget));
    CHECK(WidgetAnnotation_ToBaseAnnotation(widget, &annotation));

    CHECK(Rectangle_Create(&rect));
    CHECK(Rectangle_SetLowerLeftXReal(rect, 100));
    CHECK(Rectangle_SetLowerLeftYReal(rect, 100));
    CHECK(Rectangle_SetUpperRightXReal(rect, 300));
    CHECK(Rectangle_SetUpperRightYReal(rect, 160));
    CHECK(Annotation_SetRect(annotation, rect));

    /* The appearance is a form XObject, which an application draws into */
    CHECK(FormXObject_CreateFromDocument(document, &appearance));
    CHECK(Annotation_SetAppearance(annotation, AppearanceType_Normal, appearance));

    /* The page lists the widget among its annotations */
    CHECK(Document_GetCatalog(document, &catalog));
    CHECK(Catalog_GetPages(catalog, &page_tree));
    CHECK(PageTree_GetPage(page_tree, 1, &page));

    rc = PageObject_GetAnnotations(page, &page_annotations);
    if (rc == VANILLAPDF_ERROR_OBJECT_MISSING) {
        CHECK(PageAnnotations_Create(&page_annotations));
    }

    CHECK(rc);
    CHECK(PageAnnotations_Append(page_annotations, annotation));
    CHECK(PageObject_SetAnnotations(page, page_annotations));

    /* The field lists the widget as its own */
    CHECK(Field_AddWidget(field, widget));

    CHECK(PKCS12Key_CreateFromFile(argv[3], argv[4], &key_file));
    CHECK(PKCS12Key_ToSigningKey(key_file, &signing_key));

    CHECK(DocumentSignatureSettings_Create(&settings));
    CHECK(DocumentSignatureSettings_SetSigningKey(settings, signing_key));
    CHECK(DocumentSignatureSettings_SetDigest(settings, MessageDigestAlgorithmType_SHA256));

    /* The field is found by its fully qualified name, UTF-8 with its size in bytes */
    CHECK(File_Create(argv[2], &destination));
    CHECK(Document_SignField(document, destination, field_name, strlen(field_name), settings));

    printf("Signed field %s of %s into %s\n", field_name, argv[1], argv[2]);

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char message[256] = { 0 };
        Errors_GetLastErrorMessage(message, sizeof(message));
        fprintf(stderr, "Error: %s\n", message);
    }

    if (destination) File_Release(destination);
    if (settings) DocumentSignatureSettings_Release(settings);
    if (signing_key) SigningKey_Release(signing_key);
    if (key_file) PKCS12Key_Release(key_file);
    if (appearance) FormXObject_Release(appearance);
    if (rect) Rectangle_Release(rect);
    if (annotation) Annotation_Release(annotation);
    if (widget) WidgetAnnotation_Release(widget);
    if (field) Field_Release(field);
    if (signature_field) SignatureField_Release(signature_field);
    if (name) StringObject_Release(name);
    if (name_literal) LiteralStringObject_Release(name_literal);
    if (page_annotations) PageAnnotations_Release(page_annotations);
    if (page) PageObject_Release(page);
    if (page_tree) PageTree_Release(page_tree);
    if (catalog) Catalog_Release(catalog);
    if (document) Document_Release(document);

    return (rc == VANILLAPDF_ERROR_SUCCESS) ? 0 : 1;
}
