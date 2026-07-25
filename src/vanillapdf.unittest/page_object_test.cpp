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

} // namespace page_object
