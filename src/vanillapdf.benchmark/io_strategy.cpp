// Benchmark results comparing FileStream (fstream) vs Memory (fmt::memory_buffer) I/O strategies.
// Each iteration: open file with strategy, initialize (parse xref/trailer), open document, iterate all pages.
//
// Environment: Windows x64, MSVC 17, Release build
//   CPU: 16 x 3792 MHz, L1 32 KiB (x8), L2 256 KiB (x8), L3 16384 KiB (x1)
//
// File sizes: minimalist.pdf ~334B, sample-document.pdf ~6KB, flat-pages.pdf ~136KB
//
// Baseline (fstream - File_Open + File_Initialize only):
//   Benchmark                         Time             CPU   Iterations
//   -----------------------------------------------------------------
//   BM_FileOpen/FileStream/0     426900 ns       423825 ns         1659
//   BM_FileOpen/FileStream/1    5967429 ns      5859375 ns          112
//   BM_FileOpen/FileStream/2 3554495200 ns   3546875000 ns            1
//   BM_FileOpen/Memory/0         123816 ns       125558 ns         5600
//   BM_FileOpen/Memory/1        1283197 ns      1255580 ns          560
//   BM_FileOpen/Memory/2     2239468800 ns   2234375000 ns            1
//
// With page iteration (File_Open + File_Initialize + Document_Open + iterate all pages):
//   Benchmark                                   Time             CPU   Iterations
//   ---------------------------------------------------------------------------
//   BM_FileOpen/FileStream_minimalist      429997 ns       433243 ns         1659
//   BM_FileOpen/FileStream_Report         6001251 ns      5998884 ns          112
//   BM_FileOpen/FileStream_MPK_SLOVLEX 3229765600 ns   3218750000 ns            1
//   BM_FileOpen/Memory_minimalist          117112 ns       117188 ns         5600
//   BM_FileOpen/Memory_Report             1213462 ns      1227679 ns          560
//   BM_FileOpen/Memory_MPK_SLOVLEX     2099723100 ns   2078125000 ns            1

#include "benchmark.h"
#include "handle_guard.h"

#include <string>

static const std::string TEST_DIR = VANILLAPDF_TEST_DIR;

static bool OpenAndIteratePages(benchmark::State& state, const std::string& path, IOStrategyType strategy) {
    HandleGuard<FileHandle, File_Release> file;

    auto result = File_OpenWithStrategy(path.c_str(), strategy, file.out());
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("File_OpenWithStrategy failed");
        return false;
    }

    result = File_Initialize(file);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("File_Initialize failed");
        return false;
    }

    HandleGuard<DocumentHandle, Document_Release> doc;
    result = Document_OpenFile(file, doc.out());
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("Document_OpenFile failed");
        return false;
    }

    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    result = Document_GetCatalog(doc, catalog.out());
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("Document_GetCatalog failed");
        return false;
    }

    HandleGuard<PageTreeHandle, PageTree_Release> pages;
    result = Catalog_GetPages(catalog, pages.out());
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("Catalog_GetPages failed");
        return false;
    }

    size_type page_count = 0;
    result = PageTree_GetPageCount(pages, &page_count);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("PageTree_GetPageCount failed");
        return false;
    }

    // Pages are 1-based in PDF
    for (size_type i = 1; i <= page_count; ++i) {
        HandleGuard<PageObjectHandle, PageObject_Release> page;
        result = PageTree_GetPage(pages, i, page.out());
        if (result != VANILLAPDF_ERROR_SUCCESS) {
            state.SkipWithError("PageTree_GetPage failed");
            return false;
        }
    }

    return true;
}

static void BM_FileOpen(benchmark::State& state, IOStrategyType strategy, const std::string& path) {
    for (auto _ : state) {
        if (!OpenAndIteratePages(state, path, strategy)) {
            break;
        }
    }
}

BENCHMARK_CAPTURE(BM_FileOpen, FileStream_minimalist, IOStrategy_FileStream, TEST_DIR + "/minimalist.pdf");
BENCHMARK_CAPTURE(BM_FileOpen, FileStream_sample, IOStrategy_FileStream, TEST_DIR + "/sample-document.pdf");
BENCHMARK_CAPTURE(BM_FileOpen, FileStream_flat_pages, IOStrategy_FileStream, TEST_DIR + "/flat-pages.pdf");

BENCHMARK_CAPTURE(BM_FileOpen, Memory_minimalist, IOStrategy_Memory, TEST_DIR + "/minimalist.pdf");
BENCHMARK_CAPTURE(BM_FileOpen, Memory_sample, IOStrategy_Memory, TEST_DIR + "/sample-document.pdf");
BENCHMARK_CAPTURE(BM_FileOpen, Memory_flat_pages, IOStrategy_Memory, TEST_DIR + "/flat-pages.pdf");
