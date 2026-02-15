#include "benchmark.h"
#include "handle_guard.h"

#include <string>

static const std::string TEST_DIR = VANILLAPDF_TEST_DIR;

// Benchmark for content stream serialization via PageContents_RecalculateStreamData.
// This exercises the ToPdf() methods of all content stream operations and objects,
// which internally use StreamUtils::InputOutputStreamFromMemory().
//
// Setup: open PDF, parse page content instructions, dirty the stream object.
// Loop: RecalculateStreamData re-serializes all instructions via ToPdf().
//
// Environment: Windows x64, MSVC 18, Release build
//   CPU: 16 x 3792 MHz, L1 32 KiB (x8), L2 256 KiB (x8), L3 16384 KiB (x1)
//
// Baseline (std::stringstream in ToPdf + RecalculateStreamData):
//   BM_ContentStreamSerialize/Report_mean    11794881 ns    instructions=5.797k
//   BM_ContentStreamSerialize/example_mean    1125828 ns    instructions=34
//
// After (MemoryBufferInputOutputStream in ToPdf via InputOutputStreamFromMemory):
//   BM_ContentStreamSerialize/Report_mean     9374896 ns    instructions=5.797k  (~21% faster)
//   BM_ContentStreamSerialize/example_mean     771272 ns    instructions=34       (~32% faster)

static bool SetupPageContents(
    benchmark::State& state,
    const std::string& path,
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<DocumentHandle, Document_Release>& doc,
    HandleGuard<PageContentsHandle, PageContents_Release>& contents,
    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release>& instructions
) {
    auto rc = File_Open(path.c_str(), file.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("File_Open failed");
        return false;
    }

    rc = File_Initialize(file);
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("File_Initialize failed");
        return false;
    }

    rc = Document_OpenFile(file, doc.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("Document_OpenFile failed");
        return false;
    }

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    rc = Document_GetCatalog(doc, catalog.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("Document_GetCatalog failed");
        return false;
    }

    HandleGuard<PageTreeHandle, PageTree_Release> pages;
    rc = Catalog_GetPages(catalog, pages.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("Catalog_GetPages failed");
        return false;
    }

    HandleGuard<PageObjectHandle, PageObject_Release> page;
    rc = PageTree_GetPage(pages, 1, page.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("PageTree_GetPage failed");
        return false;
    }

    rc = PageObject_GetContents(page, contents.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("PageObject_GetContents failed");
        return false;
    }

    // Parse instructions (populates internal m_instructions)
    rc = PageContents_GetInstructionCollection(contents, instructions.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("PageContents_GetInstructionCollection failed");
        return false;
    }

    // Report instruction count
    size_type instruction_count = 0;
    ContentInstructionCollection_GetSize(instructions, &instruction_count);
    state.counters["instructions"] = static_cast<double>(instruction_count);

    // Force dirty: get base stream object and modify its header dictionary
    HandleGuard<ObjectHandle, Object_Release> base_obj;
    rc = PageContents_GetBaseObject(contents, base_obj.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("PageContents_GetBaseObject failed");
        return false;
    }

    HandleGuard<StreamObjectHandle, StreamObject_Release> stream_obj;
    rc = StreamObject_FromObject(base_obj, stream_obj.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("StreamObject_FromObject failed (content may be array)");
        return false;
    }

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> header;
    rc = StreamObject_GetHeader(stream_obj, header.out());
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("StreamObject_GetHeader failed");
        return false;
    }

    // Insert a dummy key to dirty the dictionary, which dirties the stream.
    // After first RecalculateStreamData call, SetBody increments the version,
    // keeping the stream dirty for subsequent iterations.
    HandleGuard<NameObjectHandle, NameObject_Release> dummy_key;
    NameObject_CreateFromEncodedString("BenchmarkDirty", dummy_key.out());

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> dummy_value;
    IntegerObject_Create(dummy_value.out());

    HandleGuard<ObjectHandle, Object_Release> dummy_value_obj;
    IntegerObject_ToObject(dummy_value, dummy_value_obj.out());

    DictionaryObject_Insert(header, dummy_key, dummy_value_obj, VANILLAPDF_RV_TRUE);

    return true;
}

static void BM_ContentStreamSerialize(benchmark::State& state, const std::string& path) {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> doc;
    HandleGuard<PageContentsHandle, PageContents_Release> contents;
    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> instructions;

    if (!SetupPageContents(state, path, file, doc, contents, instructions)) {
        return;
    }

    for (auto _ : state) {
        boolean_type recalculated = VANILLAPDF_RV_FALSE;
        PageContents_RecalculateStreamData(contents, &recalculated);
    }
}

BENCHMARK_CAPTURE(BM_ContentStreamSerialize, Report, TEST_DIR + "/Report.pdf");
BENCHMARK_CAPTURE(BM_ContentStreamSerialize, example, TEST_DIR + "/example.pdf");
