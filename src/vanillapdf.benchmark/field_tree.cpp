// Benchmark: FieldTree bulk authoring
//
// Covers the path a form author takes: attach an empty hierarchy, append
// N root-level fields through FieldTree_AddRootChild, then read the flat
// view once. Originally every mutator dropped the cache, and the
// validation of the next insert (membership, name uniqueness) forced a
// full rebuild before it, so appending N fields walked O(1+2+...+N) =
// O(N²) nodes.
//
// Environment: Windows 11 x64, MSVC 18, Release, L3 16 MiB,
// Google Benchmark, 3 repetitions, mean reported
//
// Baseline (Invalidate on every mutator, this branch before the change):
//
// | Benchmark                           | Time      | CPU       |
// |-------------------------------------|-----------|-----------|
// | AppendRootChildren/100_mean         |   51.9 ms |   53.0 ms |
// | AppendRootChildren/1000_mean        |   5192 ms |   5188 ms |
// | AppendRootChildren/5000_mean        | 148490 ms | 147734 ms |
//
// 100→1000 fields (10×): time grows 100× (O(N²) confirmed).
// 1000→5000 fields (5×): time grows 28.6× (also O(N²)).
//
// After the two-tier cache (this branch - the insert mutators extend the
// membership and name index with the subtree they validated and only mark
// the ordered views stale; the read at the end rebuilds them once):
//
// | Benchmark                           | Time      | CPU       |
// |-------------------------------------|-----------|-----------|
// | AppendRootChildren/100_mean         |   1.64 ms |   1.73 ms |
// | AppendRootChildren/1000_mean        |   16.3 ms |   16.7 ms |
// | AppendRootChildren/5000_mean        |   93.2 ms |   94.6 ms |
//
// 100→1000 fields (10×): time grows 9.9× (linear).
// 1000→5000 fields (5×): time grows 5.7× (linear).
// Improvement: 1000 fields 5192 ms → 16.3 ms (~320×),
// 5000 fields 148490 ms → 93.2 ms (~1600×).

#include "benchmark.h"
#include "handle_guard.h"

#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Fixture helpers
// ---------------------------------------------------------------------------

struct FieldTreeFixture {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> form;
    HandleGuard<FieldTreeHandle, FieldTree_Release> tree;
    std::vector<FieldHandle*> fields;

    ~FieldTreeFixture() {
        for (auto* field : fields) {
            Field_Release(field);
        }
    }

    bool Create(size_type field_count) {
        if (InputOutputStream_CreateFromMemory(io_stream.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (File_CreateStream(io_stream, "field_tree_benchmark", file.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (Document_CreateFile(file, document.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (InteractiveForm_CreateFromDocument(document, form.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (FieldTree_CreateFromDocument(document, tree.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (InteractiveForm_SetFieldTree(form, tree) != VANILLAPDF_ERROR_SUCCESS) return false;

        // Registering the dictionaries allocates cross-reference entries,
        // which is not the subject - every field is prepared up front, so
        // the timed loop is AddRootChild alone. Each needs a distinct name,
        // since duplicates are rejected on write.
        fields.reserve(field_count);
        for (size_type i = 0; i < field_count; i += 1) {
            FieldHandle* field = nullptr;
            if (!CreateTextField("field" + std::to_string(i), &field)) return false;
            fields.push_back(field);
        }

        return true;
    }

private:
    bool CreateTextField(const std::string& partial_name, FieldHandle** result) {
        HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dictionary;
        if (DictionaryObject_Create(dictionary.out()) != VANILLAPDF_ERROR_SUCCESS) return false;

        HandleGuard<NameObjectHandle, NameObject_Release> name_key;
        HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name_literal;
        HandleGuard<StringObjectHandle, StringObject_Release> name_string;
        HandleGuard<ObjectHandle, Object_Release> name_object;
        if (NameObject_CreateFromDecodedString("T", name_key.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (LiteralStringObject_CreateFromDecodedString(partial_name.c_str(), name_literal.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (LiteralStringObject_ToStringObject(name_literal, name_string.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (StringObject_ToObject(name_string, name_object.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (DictionaryObject_Insert(dictionary, name_key, name_object, VANILLAPDF_RV_TRUE) != VANILLAPDF_ERROR_SUCCESS) return false;

        HandleGuard<NameObjectHandle, NameObject_Release> type_key;
        HandleGuard<NameObjectHandle, NameObject_Release> type_value;
        HandleGuard<ObjectHandle, Object_Release> type_object;
        if (NameObject_CreateFromDecodedString("FT", type_key.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (NameObject_CreateFromDecodedString("Tx", type_value.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (NameObject_ToObject(type_value, type_object.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (DictionaryObject_Insert(dictionary, type_key, type_object, VANILLAPDF_RV_TRUE) != VANILLAPDF_ERROR_SUCCESS) return false;

        HandleGuard<ObjectHandle, Object_Release> dictionary_object;
        HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release> entry;
        if (DictionaryObject_ToObject(dictionary, dictionary_object.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (File_AllocateNewEntry(file, entry.out()) != VANILLAPDF_ERROR_SUCCESS) return false;
        if (XrefUsedEntry_SetReference(entry, dictionary_object) != VANILLAPDF_ERROR_SUCCESS) return false;

        return Field_CreateFromDictionary(dictionary, result) == VANILLAPDF_ERROR_SUCCESS;
    }
};

// ---------------------------------------------------------------------------
// BM_FieldTreeAppendRootChildren
//
// Appends `field_count` prepared fields to an empty hierarchy, then reads
// the flat view once - the cost an author pays for a form of that size.
// The fixture is rebuilt each iteration with the timer paused, so every
// iteration starts from an empty tree.
// ---------------------------------------------------------------------------

static void BM_FieldTreeAppendRootChildren(benchmark::State& state) {
    auto field_count = static_cast<size_type>(state.range(0));

    for (auto _ : state) {
        state.PauseTiming();
        FieldTreeFixture fixture;
        if (!fixture.Create(field_count)) {
            state.SkipWithError("Could not prepare the field tree fixture");
            return;
        }

        state.ResumeTiming();
        for (auto* field : fixture.fields) {
            if (FieldTree_AddRootChild(fixture.tree, field) != VANILLAPDF_ERROR_SUCCESS) {
                state.SkipWithError("FieldTree_AddRootChild failed");
                return;
            }
        }

        size_type count = 0;
        FieldTree_GetFieldCount(fixture.tree, &count);
        benchmark::DoNotOptimize(count);

        state.PauseTiming();
        // fixture destructor runs here with timer paused
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(field_count));
}

BENCHMARK(BM_FieldTreeAppendRootChildren)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000)
    ->Unit(benchmark::kMillisecond);
