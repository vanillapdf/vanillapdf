#include "unittest.h"
#include "handle_guard.h"

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

} // namespace page_object
