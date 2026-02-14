// Benchmark: unordered_set (baseline/main) vs unordered_map (this branch)
//
// Environment: Windows 11 x64, 16 CPUs @ 3792 MHz, MSVC 18, Release
// L1D 32 KiB, L2 256 KiB, L3 16 MiB, Google Benchmark v1.9.4
//
// Baseline (unordered_set, main branch):
//
// | Benchmark        | Time (ns) | CPU (ns) | Iterations |
// |------------------|-----------|----------|------------|
// | Find/50_mean     |       192 |      193 |          5 |
// | Find/500_mean    |       196 |      195 |          5 |
// | Find/5000_mean   |       210 |      210 |          5 |
// | Contains/50_mean |      86.1 |     85.8 |          5 |
// | Contains/500_mean|      88.8 |     88.7 |          5 |
// | Contains/5000_mean|     98.0 |     97.5 |          5 |
// | Insert/50_mean   |    19,122 |   19,496 |          5 |
// | Insert/500_mean  |   190,448 |  187,270 |          5 |
// | Insert/5000_mean | 2,091,127 |2,089,844 |          5 |
//
// After (unordered_map, this branch):
//
// | Benchmark        | Time (ns) | CPU (ns) | Iterations |
// |------------------|-----------|----------|------------|
// | Find/50_mean     |       121 |      121 |          5 |
// | Find/500_mean    |       124 |      124 |          5 |
// | Find/5000_mean   |       134 |      134 |          5 |
// | Contains/50_mean |      6.90 |     6.91 |          5 |
// | Contains/500_mean|      7.69 |     7.70 |          5 |
// | Contains/5000_mean|     12.2 |     12.3 |          5 |
// | Insert/50_mean   |    21,310 |   21,680 |          5 |
// | Insert/500_mean  |   224,597 |  225,446 |          5 |
// | Insert/5000_mean | 2,364,355 |2,343,750 |          5 |
//
// Summary: Find ~1.6x faster, Contains ~8-12x faster, Insert ~10-20% slower.
// Acceptable tradeoff: xref is built once, searched many times.

#include "benchmark.h"
#include "handle_guard.h"

#include <vector>

// Helper: populate an xref table with N used entries (object numbers 1..N)
static void PopulateXref(XrefHandle* xref, int64_t count) {
    for (int64_t i = 1; i <= count; i += 1) {
        HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> used_entry;
        HandleGuard<XrefEntryHandle, XrefEntry_Release> base_entry;

        XrefUsedEntry_Create(
            static_cast<biguint_type>(i),
            static_cast<ushort_type>(0),
            static_cast<offset_type>(i * 100),
            used_entry.out());

        XrefUsedEntry_ToEntry(used_entry, base_entry.out());
        Xref_Insert(xref, base_entry);
    }
}

// ----- Xref_Find -----

static void BM_XrefFind(benchmark::State& state) {
    auto table_size = state.range(0);

    HandleGuard<XrefHandle, Xref_Release> xref;
    Xref_Create(xref.out());
    PopulateXref(xref, table_size);

    // Cycle through all object numbers to avoid branch prediction bias
    biguint_type lookup = 1;

    for (auto _ : state) {
        HandleGuard<XrefEntryHandle, XrefEntry_Release> found_entry;
        Xref_Find(xref, lookup, found_entry.out());
        benchmark::DoNotOptimize(found_entry.get());

        lookup += 1;
        if (lookup > static_cast<biguint_type>(table_size)) {
            lookup = 1;
        }
    }
}

BENCHMARK(BM_XrefFind)
    ->Arg(50)
    ->Arg(500)
    ->Arg(5000);

// ----- Xref_Contains -----

static void BM_XrefContains(benchmark::State& state) {
    auto table_size = state.range(0);

    HandleGuard<XrefHandle, Xref_Release> xref;
    Xref_Create(xref.out());
    PopulateXref(xref, table_size);

    biguint_type lookup = 1;

    for (auto _ : state) {
        boolean_type result = VANILLAPDF_RV_FALSE;
        Xref_Contains(xref, lookup, &result);
        benchmark::DoNotOptimize(result);

        lookup += 1;
        if (lookup > static_cast<biguint_type>(table_size)) {
            lookup = 1;
        }
    }
}

BENCHMARK(BM_XrefContains)
    ->Arg(50)
    ->Arg(500)
    ->Arg(5000);

// ----- Xref_Insert -----

static void BM_XrefInsert(benchmark::State& state) {
    auto table_size = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HandleGuard<XrefHandle, Xref_Release> xref;
        Xref_Create(xref.out());
        state.ResumeTiming();

        PopulateXref(xref, table_size);
    }
}

BENCHMARK(BM_XrefInsert)
    ->Arg(50)
    ->Arg(500)
    ->Arg(5000);
