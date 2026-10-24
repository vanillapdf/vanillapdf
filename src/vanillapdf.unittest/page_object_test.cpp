#include "unittest.h"
#include "handle_guard.h"

#include <limits>
#include <string>

namespace page_object {

// Creates an in-memory document together with its first page.
// Document_CreateFile always produces a document with exactly one page.
static void CreateMemoryDocumentWithPage(
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& document,
    HandleGuard<PageObjectHandle, PageObject_Release>& page
) {
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> page_tree;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "temp", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(file, document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(catalog, page_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_GetPage(page_tree, 1, page.out()), VANILLAPDF_ERROR_SUCCESS);
}

// Registers an object as a new indirect object within the file,
// which is a precondition for anything referenced by an indirect reference
static void RegisterIndirectObject(FileHandle* file, ObjectHandle* object) {
    HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> entry;
    ASSERT_EQ(File_AllocateNewEntry(file, entry.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(XrefUsedEntry_SetReference(entry, object), VANILLAPDF_ERROR_SUCCESS);
}

TEST(ResourceDictionary, CreateBlank) {
    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> resources;

    ASSERT_EQ(ResourceDictionary_Create(resources.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(resources.get(), nullptr);
}

TEST(ResourceDictionary, CreateFromDictionary) {
    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary;
    ASSERT_EQ(DictionaryObject_Create(dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> resources;
    ASSERT_EQ(ResourceDictionary_CreateFromDictionary(dictionary, resources.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(resources.get(), nullptr);
}

TEST(ResourceDictionary, CreateRejectsNullParameters) {
    EXPECT_EQ(ResourceDictionary_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(ResourceDictionary_CreateFromDictionary(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// A blank resource dictionary has no /Font entry
TEST(ResourceDictionary, GetFontMapMissing) {
    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> resources;
    ASSERT_EQ(ResourceDictionary_Create(resources.out()), VANILLAPDF_ERROR_SUCCESS);

    FontMapHandle* font_map = nullptr;
    EXPECT_EQ(ResourceDictionary_GetFontMap(resources, &font_map), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(font_map, nullptr);
}

// Document_CreateFile produces a page without a /Resources entry
TEST(PageObject, GetResourcesMissing) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    ResourceDictionaryHandle* resources = nullptr;
    EXPECT_EQ(PageObject_GetResources(page, &resources), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(resources, nullptr);
}

TEST(PageObject, SetAndGetResources) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> resources;
    ASSERT_EQ(ResourceDictionary_Create(resources.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(PageObject_SetResources(page, resources), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> found_resources;
    ASSERT_EQ(PageObject_GetResources(page, found_resources.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_resources.get(), nullptr);
}

// Setting resources twice must replace the previous entry rather than throw
TEST(PageObject, SetResourcesOverwrite) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> first;
    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> second;
    ASSERT_EQ(ResourceDictionary_Create(first.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(ResourceDictionary_Create(second.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(PageObject_SetResources(page, first), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetResources(page, second), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> found;
    ASSERT_EQ(PageObject_GetResources(page, found.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found.get(), nullptr);
}

TEST(PageObject, SetResourcesRejectsNullParameters) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    EXPECT_EQ(PageObject_SetResources(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetResources(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(PageContents, CreateFromStream) {
    HandleGuard<StreamObjectHandle, StreamObject_Release> stream;
    ASSERT_EQ(StreamObject_Create(stream.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PageContentsHandle, PageContents_Release> contents;
    ASSERT_EQ(PageContents_CreateFromStream(stream, contents.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(contents.get(), nullptr);
}

// The one call variant registers the content stream as an indirect object,
// so the caller does not have to allocate the xref entry by hand
TEST(PageContents, CreateFromDocument) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<PageContentsHandle, PageContents_Release> contents;
    ASSERT_EQ(PageContents_CreateFromDocument(document, contents.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(contents.get(), nullptr);

    ASSERT_EQ(PageObject_SetContents(page, contents), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PageContentsHandle, PageContents_Release> found_contents;
    ASSERT_EQ(PageObject_GetContents(page, found_contents.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_contents.get(), nullptr);
}

TEST(PageContents, CreateFromDocumentRejectsNullParameters) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    EXPECT_EQ(PageContents_CreateFromDocument(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageContents_CreateFromDocument(document, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// The indirect reference written by SetContents has to resolve after a save,
// which is what an unregistered content stream would fail to do
TEST(PageContents, CreateFromDocumentPersistsAcrossSave) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<PageContentsHandle, PageContents_Release> contents;
    ASSERT_EQ(PageContents_CreateFromDocument(document, contents.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetContents(page, contents), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_stream;
    HandleGuard<FileHandle, File_Release> destination_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(destination_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(destination_stream, "temp_destination", destination_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(document, destination_file), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> reloaded_file;
    HandleGuard<DocumentHandle, Document_Release> reloaded_document;
    HandleGuard<CatalogHandle, Catalog_Release> reloaded_catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> reloaded_page_tree;
    HandleGuard<PageObjectHandle, PageObject_Release> reloaded_page;
    ASSERT_EQ(File_OpenStream(destination_stream, "temp_destination", reloaded_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Initialize(reloaded_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(reloaded_file, reloaded_document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(reloaded_document, reloaded_catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(reloaded_catalog, reloaded_page_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_GetPage(reloaded_page_tree, 1, reloaded_page.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PageContentsHandle, PageContents_Release> reloaded_contents;
    EXPECT_EQ(PageObject_GetContents(reloaded_page, reloaded_contents.out()), VANILLAPDF_ERROR_SUCCESS);
}

TEST(PageContents, CreateFromStreamRejectsNullParameters) {
    EXPECT_EQ(PageContents_CreateFromStream(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    HandleGuard<StreamObjectHandle, StreamObject_Release> stream;
    ASSERT_EQ(StreamObject_Create(stream.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(PageContents_CreateFromStream(stream, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// PageObject::SetContents stores an indirect reference to the content stream,
// so the stream has to be registered as an indirect object first
TEST(PageObject, SetAndGetContents) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<StreamObjectHandle, StreamObject_Release> stream;
    ASSERT_EQ(StreamObject_Create(stream.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> stream_object;
    ASSERT_EQ(StreamObject_ToObject(stream, stream_object.out()), VANILLAPDF_ERROR_SUCCESS);
    RegisterIndirectObject(file, stream_object);

    const std::string body = "BT ET";
    HandleGuard<BufferHandle, Buffer_Release> body_buffer;
    ASSERT_EQ(Buffer_CreateFromData(body.data(), body.size(), body_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(StreamObject_SetBody(stream, body_buffer), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PageContentsHandle, PageContents_Release> contents;
    ASSERT_EQ(PageContents_CreateFromStream(stream, contents.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(PageObject_SetContents(page, contents), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PageContentsHandle, PageContents_Release> found_contents;
    ASSERT_EQ(PageObject_GetContents(page, found_contents.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(found_contents.get(), nullptr);

    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> instructions;
    ASSERT_EQ(PageContents_GetInstructionCollection(found_contents, instructions.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type instruction_count = 0;
    ASSERT_EQ(ContentInstructionCollection_GetSize(instructions, &instruction_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_GT(instruction_count, 0u);
}

TEST(PageObject, SetContentsRejectsNullParameters) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    EXPECT_EQ(PageObject_SetContents(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetContents(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// A blank resource dictionary is attached as a direct object, so it is
// serialized inline with the page - confirm it reaches the written file
TEST(PageObject, SetResourcesPersistAcrossSave) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> resources;
    ASSERT_EQ(ResourceDictionary_Create(resources.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetResources(page, resources), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> dest_stream;
    HandleGuard<FileHandle, File_Release> dest_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(dest_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(dest_stream, "temp_destination", dest_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(document, dest_file), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> reloaded_file;
    HandleGuard<DocumentHandle, Document_Release> reloaded_doc;
    HandleGuard<CatalogHandle, Catalog_Release> reloaded_catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> reloaded_tree;
    HandleGuard<PageObjectHandle, PageObject_Release> reloaded_page;
    ASSERT_EQ(File_OpenStream(dest_stream, "temp_destination", reloaded_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Initialize(reloaded_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(reloaded_file, reloaded_doc.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(reloaded_doc, reloaded_catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(reloaded_catalog, reloaded_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_GetPage(reloaded_tree, 1, reloaded_page.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ResourceDictionaryHandle, ResourceDictionary_Release> reloaded_resources;
    EXPECT_EQ(PageObject_GetResources(reloaded_page, reloaded_resources.out()), VANILLAPDF_ERROR_SUCCESS);
}

// Page geometry entries (Table 30)

static HandleGuard<RectangleHandle, Rectangle_Release> CreateRectangle(double llx, double lly, double urx, double ury) {
    HandleGuard<RectangleHandle, Rectangle_Release> rectangle;
    EXPECT_EQ(Rectangle_Create(rectangle.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(Rectangle_SetLowerLeftXReal(rectangle, llx), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(Rectangle_SetLowerLeftYReal(rectangle, lly), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(Rectangle_SetUpperRightXReal(rectangle, urx), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(Rectangle_SetUpperRightYReal(rectangle, ury), VANILLAPDF_ERROR_SUCCESS);
    return rectangle;
}

static void ExpectRectangle(RectangleHandle* rectangle, double llx, double lly, double urx, double ury) {
    real_type found_llx = 0;
    real_type found_lly = 0;
    real_type found_urx = 0;
    real_type found_ury = 0;
    ASSERT_EQ(Rectangle_GetLowerLeftXReal(rectangle, &found_llx), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_GetLowerLeftYReal(rectangle, &found_lly), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_GetUpperRightXReal(rectangle, &found_urx), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Rectangle_GetUpperRightYReal(rectangle, &found_ury), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_DOUBLE_EQ(found_llx, llx);
    EXPECT_DOUBLE_EQ(found_lly, lly);
    EXPECT_DOUBLE_EQ(found_urx, urx);
    EXPECT_DOUBLE_EQ(found_ury, ury);
}

using RectangleSetter = error_type (CALLING_CONVENTION *)(PageObjectHandle*, RectangleHandle*);
using RectangleGetter = error_type (CALLING_CONVENTION *)(PageObjectHandle*, RectangleHandle**);

static void SetAndExpectRectangle(PageObjectHandle* page, RectangleSetter setter, RectangleGetter getter) {
    auto source = CreateRectangle(10.5, 20.5, 300.0, 400.0);
    ASSERT_EQ(setter(page, source), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<RectangleHandle, Rectangle_Release> found;
    ASSERT_EQ(getter(page, found.out()), VANILLAPDF_ERROR_SUCCESS);
    ExpectRectangle(found, 10.5, 20.5, 300.0, 400.0);

    // A second set has to replace the entry rather than throw on the duplicate key
    auto replacement = CreateRectangle(0.0, 0.0, 100.0, 200.0);
    ASSERT_EQ(setter(page, replacement), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(getter(page, found.out()), VANILLAPDF_ERROR_SUCCESS);
    ExpectRectangle(found, 0.0, 0.0, 100.0, 200.0);

    EXPECT_EQ(setter(nullptr, source), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(setter(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(getter(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// Document_CreateFile produces a page with a media box only
TEST(PageObject, GeometryEntriesMissing) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    RectangleHandle* rectangle = nullptr;
    EXPECT_EQ(PageObject_GetCropBox(page, &rectangle), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(PageObject_GetBleedBox(page, &rectangle), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(PageObject_GetTrimBox(page, &rectangle), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(PageObject_GetArtBox(page, &rectangle), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(rectangle, nullptr);

    bigint_type rotate = 0;
    EXPECT_EQ(PageObject_GetRotate(page, &rotate), VANILLAPDF_ERROR_OBJECT_MISSING);

    real_type user_unit = 0;
    EXPECT_EQ(PageObject_GetUserUnit(page, &user_unit), VANILLAPDF_ERROR_OBJECT_MISSING);

    TransparencyGroupHandle* group = nullptr;
    EXPECT_EQ(PageObject_GetGroup(page, &group), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(group, nullptr);
}

TEST(PageObject, SetAndGetBoxes) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    SetAndExpectRectangle(page, PageObject_SetMediaBox, PageObject_GetMediaBox);
    SetAndExpectRectangle(page, PageObject_SetCropBox, PageObject_GetCropBox);
    SetAndExpectRectangle(page, PageObject_SetBleedBox, PageObject_GetBleedBox);
    SetAndExpectRectangle(page, PageObject_SetTrimBox, PageObject_GetTrimBox);
    SetAndExpectRectangle(page, PageObject_SetArtBox, PageObject_GetArtBox);
}

TEST(PageObject, SetAndGetRotate) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    bigint_type rotate = 0;

    ASSERT_EQ(PageObject_SetRotate(page, 90), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetRotate(page, &rotate), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(rotate, 90);

    ASSERT_EQ(PageObject_SetRotate(page, 270), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetRotate(page, &rotate), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(rotate, 270);

    // Negative multiples of 90 occur in real files and are still multiples of 90
    ASSERT_EQ(PageObject_SetRotate(page, -90), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetRotate(page, &rotate), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(rotate, -90);

    ASSERT_EQ(PageObject_SetRotate(page, 0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetRotate(page, &rotate), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(rotate, 0);
}

// Anything that is not a multiple of 90 is rejected and leaves the entry untouched
TEST(PageObject, SetRotateRejectsInvalidValues) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    ASSERT_EQ(PageObject_SetRotate(page, 180), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(PageObject_SetRotate(page, 45), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetRotate(page, 91), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetRotate(nullptr, 90), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_GetRotate(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    bigint_type rotate = 0;
    ASSERT_EQ(PageObject_GetRotate(page, &rotate), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(rotate, 180);
}

TEST(PageObject, SetAndGetUserUnit) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    real_type user_unit = 0;

    ASSERT_EQ(PageObject_SetUserUnit(page, 2.5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetUserUnit(page, &user_unit), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(user_unit, 2.5);

    ASSERT_EQ(PageObject_SetUserUnit(page, 1.0), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_GetUserUnit(page, &user_unit), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(user_unit, 1.0);

    EXPECT_EQ(PageObject_SetUserUnit(page, 0.0), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetUserUnit(page, -1.0), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetUserUnit(page, std::numeric_limits<real_type>::quiet_NaN()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetUserUnit(page, std::numeric_limits<real_type>::infinity()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetUserUnit(nullptr, 1.0), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_GetUserUnit(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // The rejected values must not have replaced the last valid one
    ASSERT_EQ(PageObject_GetUserUnit(page, &user_unit), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(user_unit, 1.0);
}

// A freshly created group carries the mandatory /S /Transparency subtype
// and none of the optional flags
TEST(TransparencyGroup, CreateEmpty) {
    HandleGuard<TransparencyGroupHandle, TransparencyGroup_Release> group;
    ASSERT_EQ(TransparencyGroup_Create(group.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(group.get(), nullptr);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> base_object;
    ASSERT_EQ(TransparencyGroup_GetBaseObject(group, base_object.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type contains = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(DictionaryObject_Contains(base_object, NameConstant_S, &contains), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(contains, VANILLAPDF_RV_TRUE);

    boolean_type flag = VANILLAPDF_RV_FALSE;
    EXPECT_EQ(TransparencyGroup_GetIsolated(group, &flag), VANILLAPDF_ERROR_OBJECT_MISSING);
    EXPECT_EQ(TransparencyGroup_GetKnockout(group, &flag), VANILLAPDF_ERROR_OBJECT_MISSING);

    EXPECT_EQ(TransparencyGroup_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TransparencyGroup, SetAndGetFlags) {
    HandleGuard<TransparencyGroupHandle, TransparencyGroup_Release> group;
    ASSERT_EQ(TransparencyGroup_Create(group.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type flag = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(TransparencyGroup_SetIsolated(group, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TransparencyGroup_GetIsolated(group, &flag), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(flag, VANILLAPDF_RV_TRUE);

    ASSERT_EQ(TransparencyGroup_SetIsolated(group, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TransparencyGroup_GetIsolated(group, &flag), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(flag, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(TransparencyGroup_SetKnockout(group, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TransparencyGroup_GetKnockout(group, &flag), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(flag, VANILLAPDF_RV_TRUE);

    EXPECT_EQ(TransparencyGroup_SetIsolated(nullptr, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TransparencyGroup_GetIsolated(group, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TransparencyGroup_SetKnockout(nullptr, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TransparencyGroup_GetKnockout(group, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TransparencyGroup_GetBaseObject(group, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(PageObject, SetAndGetGroup) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<TransparencyGroupHandle, TransparencyGroup_Release> group;
    ASSERT_EQ(TransparencyGroup_Create(group.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TransparencyGroup_SetIsolated(group, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(PageObject_SetGroup(page, group), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<TransparencyGroupHandle, TransparencyGroup_Release> found;
    ASSERT_EQ(PageObject_GetGroup(page, found.out()), VANILLAPDF_ERROR_SUCCESS);

    boolean_type isolated = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(TransparencyGroup_GetIsolated(found, &isolated), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(isolated, VANILLAPDF_RV_TRUE);

    // A second set has to replace the entry rather than throw on the duplicate key
    HandleGuard<TransparencyGroupHandle, TransparencyGroup_Release> replacement;
    ASSERT_EQ(TransparencyGroup_Create(replacement.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetGroup(page, replacement), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(PageObject_GetGroup(page, found.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(TransparencyGroup_GetIsolated(found, &isolated), VANILLAPDF_ERROR_OBJECT_MISSING);

    EXPECT_EQ(PageObject_SetGroup(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_SetGroup(nullptr, group), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PageObject_GetGroup(page, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// A /Group whose subtype is not /Transparency is not a transparency group
TEST(PageObject, GetGroupRejectsForeignSubtype) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> foreign_group;
    ASSERT_EQ(DictionaryObject_Create(foreign_group.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> subtype;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Unknown", subtype.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> subtype_object;
    ASSERT_EQ(NameObject_ToObject(subtype, subtype_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_InsertConst(foreign_group, NameConstant_S, subtype_object, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<NameObjectHandle, NameObject_Release> group_key;
    ASSERT_EQ(NameObject_CreateFromDecodedString("Group", group_key.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> page_dictionary;
    ASSERT_EQ(PageObject_GetBaseObject(page, page_dictionary.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<ObjectHandle, Object_Release> foreign_group_object;
    ASSERT_EQ(DictionaryObject_ToObject(foreign_group, foreign_group_object.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DictionaryObject_Insert(page_dictionary, group_key, foreign_group_object, VANILLAPDF_RV_TRUE), VANILLAPDF_ERROR_SUCCESS);

    TransparencyGroupHandle* group = nullptr;
    EXPECT_EQ(PageObject_GetGroup(page, &group), VANILLAPDF_ERROR_SEMANTIC_CONTEXT);
    EXPECT_EQ(group, nullptr);
}

// Geometry set through the setters has to survive a save/reload cycle
TEST(PageObject, GeometryPersistsAcrossSave) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<PageObjectHandle, PageObject_Release> page;
    CreateMemoryDocumentWithPage(io_stream, file, document, page);

    auto crop_box = CreateRectangle(1.0, 2.0, 3.0, 4.0);
    ASSERT_EQ(PageObject_SetCropBox(page, crop_box), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetRotate(page, 270), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageObject_SetUserUnit(page, 3.0), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> destination_stream;
    HandleGuard<FileHandle, File_Release> destination_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(destination_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(destination_stream, "temp_destination", destination_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(document, destination_file), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileHandle, File_Release> reloaded_file;
    HandleGuard<DocumentHandle, Document_Release> reloaded_document;
    HandleGuard<CatalogHandle, Catalog_Release> reloaded_catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> reloaded_page_tree;
    HandleGuard<PageObjectHandle, PageObject_Release> reloaded_page;
    ASSERT_EQ(File_OpenStream(destination_stream, "temp_destination", reloaded_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Initialize(reloaded_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(reloaded_file, reloaded_document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_GetCatalog(reloaded_document, reloaded_catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetPages(reloaded_catalog, reloaded_page_tree.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PageTree_GetPage(reloaded_page_tree, 1, reloaded_page.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<RectangleHandle, Rectangle_Release> reloaded_crop_box;
    ASSERT_EQ(PageObject_GetCropBox(reloaded_page, reloaded_crop_box.out()), VANILLAPDF_ERROR_SUCCESS);
    ExpectRectangle(reloaded_crop_box, 1.0, 2.0, 3.0, 4.0);

    bigint_type rotate = 0;
    ASSERT_EQ(PageObject_GetRotate(reloaded_page, &rotate), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(rotate, 270);

    real_type user_unit = 0;
    ASSERT_EQ(PageObject_GetUserUnit(reloaded_page, &user_unit), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_DOUBLE_EQ(user_unit, 3.0);
}

} // namespace page_object
