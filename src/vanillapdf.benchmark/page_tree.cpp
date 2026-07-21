// Benchmark: PageTree sequential and random page access
//
// Covers the hot path in process_document: iterating all pages via
// PageTree_GetPage(pages, i, ...) for i = 1..N. For a flat page tree
// (all N pages as direct children of the root /Pages node) the current
// implementation re-traverses from the root on every call, costing
// O(1+2+...+N) = O(N²) total.
//
// flat-pages.pdf: 1,450 pages, single flat /Pages node.
//
// Environment: Windows 11 x64, 16 CPUs @ 3792 MHz, MSVC 18, Release
// L1D 32 KiB, L2 256 KiB, L3 16 MiB, Google Benchmark v1.9.4
//
// Baseline (no cache, main branch):
//
// | Benchmark                           | Time      | CPU       |
// |-------------------------------------|-----------|-----------|
// | SequentialAccess/50_mean            |   2.89 ms |   2.91 ms |
// | SequentialAccess/500_mean           |    548 ms |    547 ms |
// | SequentialAccess/1450_mean          |   3392 ms |   3391 ms |
// | SingleAccess/50_mean                |  58076 ns |  58710 ns |
// | SingleAccess/500_mean               | 745630 ns | 744048 ns |
// | SingleAccess/1450_mean              |1830885 ns |1827567 ns |
//
// 50→500 pages (10×): sequential time grows ~190× (O(N²) confirmed).
// 500→1450 pages (2.9×): sequential time grows ~6.2× (also O(N²)).
//
// After eager cache (previous iteration of this branch — BuildPageCache on first
// access; warmup called GetPage(1) which triggered the full eager build):
//
// | Benchmark                           | Time      | CPU       |
// |-------------------------------------|-----------|-----------|
// | SequentialAccess/50_mean            | 0.002 ms  | 0.002 ms  |
// | SequentialAccess/500_mean           | 0.019 ms  | 0.019 ms  |
// | SequentialAccess/1450_mean          | 0.058 ms  | 0.058 ms  |
// | SingleAccess/50_mean                |    36.8 ns|    35.8 ns|
// | SingleAccess/500_mean               |    38.0 ns|    38.1 ns|
// | SingleAccess/1450_mean              |    42.0 ns|    41.7 ns|
// | ColdFirstAccess_mean (1450 pages)   |    1616 ms|           |
//
// Improvement (sequential, 1450 pages): 3392 ms → 0.058 ms (~58,000×).
// Cold-start regression: 15.9 ms → 1616 ms (~100×) because BuildPageCache()
// resolved all 1450 indirect page references up-front on the very first call.
//
// After lazy cache (this branch — PageTreeWalker; warmup still calls GetPage(1)
// which now only warms page 1; first benchmark iteration pays the lazy-fill cost):
//
// | Benchmark                           | Time      | CPU       |
// |-------------------------------------|-----------|-----------|
// | SequentialAccess/50_mean            | 0.003 ms  | 0.003 ms  |
// | SequentialAccess/500_mean           | 0.056 ms  | 0.056 ms  |
// | SequentialAccess/1450_mean          |   1006 ms |    959 ms |
// | SingleAccess/50_mean                |    60.7 ns|    60.3 ns|
// | SingleAccess/500_mean               |    97.8 ns|    98.3 ns|
// | SingleAccess/1450_mean              | 517062 ns |518750 ns  |
// | ColdFirstAccess_mean (1450 pages)   |    15.6 ms|           |
//
// Cold-start regression eliminated: 1616 ms → 15.6 ms (≈ baseline 15.9 ms).
// SequentialAccess/1450 reflects the lazy-fill cost paid in the first iteration.
// Calling WarmPageCache() before use restores eager-cache steady-state numbers
// (0.058 ms sequential, 42 ns single access).

#include "benchmark.h"
#include "handle_guard.h"

#include <string>

// ---------------------------------------------------------------------------
// Fixture helpers
// ---------------------------------------------------------------------------

static std::string TestFile(const char* name) {
    return std::string(VANILLAPDF_TEST_DIR) + "/" + name;
}

struct PageTreeFixture {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<PageTreeHandle, PageTree_Release> pages;
    size_type page_count = 0;

    bool Open(const char* filename) {
        auto path = TestFile(filename);
        if (File_Open(path.c_str(), file.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (File_Initialize(file) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (Document_OpenFile(file, document.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (Document_GetCatalog(document, catalog.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (Catalog_GetPages(catalog, pages.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (PageTree_GetPageCount(pages, &page_count) != VANILLAPDF_ERROR_SUCCESS) return false;
        return true;
    }
};

// ---------------------------------------------------------------------------
// BM_PageTreeSequentialAccess
//
// Accesses the first `page_limit` pages in order (1, 2, ..., page_limit).
// This is what process_document does and what exhibits the O(N²) behaviour.
// ---------------------------------------------------------------------------

static void BM_PageTreeSequentialAccess(benchmark::State& state) {
    PageTreeFixture fixture;
    if (!fixture.Open("flat-pages.pdf")) {
        state.SkipWithError("Could not open flat-pages.pdf");
        return;
    }

    auto page_limit = static_cast<size_type>(state.range(0));
    if (page_limit > fixture.page_count) {
        page_limit = fixture.page_count;
    }

    // Pre-warm the cache so benchmark iterations measure steady-state access,
    // not the one-time O(N) tree walk that builds the flat page vector.
    {
        HandleGuard<PageObjectHandle, PageObject_Release> page;
        PageTree_GetPage(fixture.pages, 1, page.out());
    }

    for (auto _ : state) {
        for (size_type i = 1; i <= page_limit; i += 1) {
            HandleGuard<PageObjectHandle, PageObject_Release> page;
            PageTree_GetPage(fixture.pages, i, page.out());
            benchmark::DoNotOptimize(page.get());
        }
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(page_limit));
}

BENCHMARK(BM_PageTreeSequentialAccess)
    ->Arg(50)
    ->Arg(500)
    ->Arg(1450)
    ->Unit(benchmark::kMillisecond);

// ---------------------------------------------------------------------------
// BM_PageTreeSingleAccess
//
// Accesses a single page by index each iteration, cycling through all pages.
// Isolates the per-call cost of GetCachedPage without the cumulative O(N²)
// sweep, making it easier to compare before/after for a single lookup.
// ---------------------------------------------------------------------------

static void BM_PageTreeSingleAccess(benchmark::State& state) {
    PageTreeFixture fixture;
    if (!fixture.Open("flat-pages.pdf")) {
        state.SkipWithError("Could not open flat-pages.pdf");
        return;
    }

    auto page_limit = static_cast<size_type>(state.range(0));
    if (page_limit > fixture.page_count) {
        page_limit = fixture.page_count;
    }

    // Pre-warm the cache so benchmark iterations measure steady-state access,
    // not the one-time O(N) tree walk that builds the flat page vector.
    {
        HandleGuard<PageObjectHandle, PageObject_Release> page;
        PageTree_GetPage(fixture.pages, 1, page.out());
    }

    int64_t i = 0;
    for (auto _ : state) {
        HandleGuard<PageObjectHandle, PageObject_Release> page;
        PageTree_GetPage(fixture.pages, i % page_limit + 1, page.out());
        benchmark::DoNotOptimize(page.get());
        i += 1;
    }
}

BENCHMARK(BM_PageTreeSingleAccess)
    ->Arg(50)
    ->Arg(500)
    ->Arg(1450);

// ---------------------------------------------------------------------------
// BM_PageTreeColdFirstAccess
//
// Measures the cost of the very first PageTree_GetPage call on a freshly-
// opened document. With the page cache this triggers BuildPageCache() — an
// O(N) walk that resolves all N indirect page references and stores them in
// the flat vector. Without the cache the first call was a plain O(1) tree
// walk straight to page 1.
//
// The document is re-opened each iteration so the cache is always cold.
// File open and xref parsing are excluded from timing via
// PauseTiming / ResumeTiming; fixture teardown happens while the timer is
// paused at the end of each iteration.
// ---------------------------------------------------------------------------

static void BM_PageTreeColdFirstAccess(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        PageTreeFixture fixture;
        if (!fixture.Open("flat-pages.pdf")) {
            state.SkipWithError("Could not open flat-pages.pdf");
            return;
        }

        state.ResumeTiming();
        {
            HandleGuard<PageObjectHandle, PageObject_Release> page;
            PageTree_GetPage(fixture.pages, 1, page.out());
            benchmark::DoNotOptimize(page.get());
        }

        state.PauseTiming();
        // fixture destructor runs here with timer paused
    }
}

BENCHMARK(BM_PageTreeColdFirstAccess)
    ->Unit(benchmark::kMillisecond);
