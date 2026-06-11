#include "unittest.h"
#include "handle_guard.h"

#include <thread>
#include <vector>
#include <atomic>

namespace thread_safety {

struct ThreadTestParams {
    int num_threads;
    int iterations_per_thread;
};

class DocumentOpenFileTest : public ::testing::TestWithParam<ThreadTestParams> {};

// Test that demonstrates the race condition in Document::OpenFile
// Multiple threads opening the same file should not cause "overriding" warnings
// or result in orphaned Document objects
TEST_P(DocumentOpenFileTest, ConcurrentOpenSameFile) {
    const auto& params = GetParam();
    const int NUM_THREADS = params.num_threads;
    const int ITERATIONS_PER_THREAD = params.iterations_per_thread;

    std::atomic<int> success_count{0};
    std::atomic<int> error_count{0};

    // Create a shared file that all threads will try to open as Document
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> shared_file;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "shared_test_file", shared_file.out()), VANILLAPDF_ERROR_SUCCESS);

    std::vector<std::thread> threads;
    std::vector<DocumentHandle*> documents(NUM_THREADS * ITERATIONS_PER_THREAD, nullptr);

    // Launch multiple threads that all try to open the same file as a Document
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&, t]() {
            for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
                DocumentHandle* doc = nullptr;
                error_type result = Document_OpenFile(shared_file, &doc);

                if (result == VANILLAPDF_ERROR_SUCCESS && doc != nullptr) {
                    documents[t * ITERATIONS_PER_THREAD + i] = doc;
                    success_count++;
                } else {
                    error_count++;
                }
            }
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // All calls should succeed
    EXPECT_EQ(success_count.load(), NUM_THREADS * ITERATIONS_PER_THREAD);
    EXPECT_EQ(error_count.load(), 0);

    // All returned documents should be the SAME document instance
    // (since they all opened the same file)
    DocumentHandle* first_doc = nullptr;
    for (auto* doc : documents) {
        if (doc != nullptr) {
            if (first_doc == nullptr) {
                first_doc = doc;
            } else {
                // All documents should be the same instance
                EXPECT_EQ(doc, first_doc) << "Race condition detected: different Document instances for same File";
            }
        }
    }

    // Cleanup - release all document handles
    // These are raw pointers stored in a vector by threads, so they can't use HandleGuard
    for (auto* doc : documents) {
        if (doc != nullptr) {
            Document_Release(doc);
        }
    }
}

std::string ThreadTestParamsName(const ::testing::TestParamInfo<ThreadTestParams>& info) {
    return std::to_string(info.param.num_threads) + "threads_" +
           std::to_string(info.param.iterations_per_thread) + "iterations";
}

INSTANTIATE_TEST_SUITE_P(
    ThreadSafety,
    DocumentOpenFileTest,
    ::testing::Values(
        ThreadTestParams{50, 2000}
    ),
    ThreadTestParamsName
);

// Test concurrent open of different files (should work fine)
TEST(DocumentOpenFile, ConcurrentOpenDifferentFiles) {
    constexpr int NUM_THREADS = 10;

    std::atomic<int> success_count{0};
    std::atomic<int> error_count{0};

    // These use raw pointer vectors because they're populated in loops/threads
    std::vector<InputOutputStreamHandle*> streams(NUM_THREADS, nullptr);
    std::vector<FileHandle*> files(NUM_THREADS, nullptr);
    std::vector<DocumentHandle*> documents(NUM_THREADS, nullptr);

    // Create separate files for each thread
    for (int i = 0; i < NUM_THREADS; ++i) {
        ASSERT_EQ(InputOutputStream_CreateFromMemory(&streams[i]), VANILLAPDF_ERROR_SUCCESS);
        std::string filename = "test_file_" + std::to_string(i);
        ASSERT_EQ(File_CreateStream(streams[i], filename.c_str(), &files[i]), VANILLAPDF_ERROR_SUCCESS);
    }

    std::vector<std::thread> threads;

    // Launch threads that each open their own file
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&, t]() {
            DocumentHandle* doc = nullptr;
            error_type result = Document_OpenFile(files[t], &doc);

            if (result == VANILLAPDF_ERROR_SUCCESS && doc != nullptr) {
                documents[t] = doc;
                success_count++;
            } else {
                error_count++;
            }
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // All calls should succeed
    EXPECT_EQ(success_count.load(), NUM_THREADS);
    EXPECT_EQ(error_count.load(), 0);

    // All documents should be different instances
    for (int i = 0; i < NUM_THREADS; ++i) {
        EXPECT_NE(documents[i], nullptr);
        for (int j = i + 1; j < NUM_THREADS; ++j) {
            EXPECT_NE(documents[i], documents[j]) << "Different files should have different Document instances";
        }
    }

    // Cleanup - raw pointer vectors require manual release
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (documents[i] != nullptr) {
            ASSERT_EQ(Document_Release(documents[i]), VANILLAPDF_ERROR_SUCCESS);
        }
        ASSERT_EQ(File_Release(files[i]), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_Release(streams[i]), VANILLAPDF_ERROR_SUCCESS);
    }
}

// Test concurrent Append to a shared MixedArrayObject
TEST(ArrayObjectThreadSafety, ConcurrentAppend) {
    constexpr int NUM_THREADS = 8;
    constexpr int ITEMS_PER_THREAD = 500;

    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array;
    ASSERT_EQ(ArrayObject_Create(array.out()), VANILLAPDF_ERROR_SUCCESS);

    std::vector<std::thread> threads;
    std::atomic<int> error_count{0};

    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < ITEMS_PER_THREAD; ++i) {
                IntegerObjectHandle* int_obj = nullptr;
                if (IntegerObject_Create(&int_obj) != VANILLAPDF_ERROR_SUCCESS) {
                    error_count++;
                    continue;
                }

                ObjectHandle* obj = nullptr;
                if (IntegerObject_ToObject(int_obj, &obj) != VANILLAPDF_ERROR_SUCCESS) {
                    IntegerObject_Release(int_obj);
                    error_count++;
                    continue;
                }

                if (ArrayObject_Append(array, obj) != VANILLAPDF_ERROR_SUCCESS) {
                    error_count++;
                }

                Object_Release(obj);
                IntegerObject_Release(int_obj);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(error_count.load(), 0);

    size_type final_size = 0;
    ASSERT_EQ(ArrayObject_GetSize(array, &final_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(final_size, static_cast<size_type>(NUM_THREADS * ITEMS_PER_THREAD));
}

// Test concurrent reads while another thread appends
TEST(ArrayObjectThreadSafety, ConcurrentReadWrite) {
    constexpr int NUM_READERS = 4;
    constexpr int NUM_APPENDS = 1000;

    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array;
    ASSERT_EQ(ArrayObject_Create(array.out()), VANILLAPDF_ERROR_SUCCESS);

    // Pre-populate with one element so readers always have something
    {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> seed;
        ASSERT_EQ(IntegerObject_Create(seed.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ObjectHandle, Object_Release> seed_obj;
        ASSERT_EQ(IntegerObject_ToObject(seed, seed_obj.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ArrayObject_Append(array, seed_obj), VANILLAPDF_ERROR_SUCCESS);
    }

    std::atomic<bool> done{false};
    std::atomic<int> read_errors{0};
    std::atomic<int> write_errors{0};

    // Writer thread: continuously appends
    std::thread writer([&]() {
        for (int i = 0; i < NUM_APPENDS; ++i) {
            IntegerObjectHandle* int_obj = nullptr;
            if (IntegerObject_Create(&int_obj) != VANILLAPDF_ERROR_SUCCESS) {
                write_errors++;
                continue;
            }

            ObjectHandle* obj = nullptr;
            if (IntegerObject_ToObject(int_obj, &obj) != VANILLAPDF_ERROR_SUCCESS) {
                IntegerObject_Release(int_obj);
                write_errors++;
                continue;
            }

            if (ArrayObject_Append(array, obj) != VANILLAPDF_ERROR_SUCCESS) {
                write_errors++;
            }

            Object_Release(obj);
            IntegerObject_Release(int_obj);
        }
        done = true;
    });

    // Reader threads: continuously read size and first element
    std::vector<std::thread> readers;
    for (int t = 0; t < NUM_READERS; ++t) {
        readers.emplace_back([&]() {
            while (!done.load()) {
                size_type size = 0;
                if (ArrayObject_GetSize(array, &size) != VANILLAPDF_ERROR_SUCCESS) {
                    read_errors++;
                    continue;
                }

                if (size > 0) {
                    ObjectHandle* obj = nullptr;
                    if (ArrayObject_GetValue(array, 0, &obj) != VANILLAPDF_ERROR_SUCCESS) {
                        read_errors++;
                        continue;
                    }
                    Object_Release(obj);
                }
            }
        });
    }

    writer.join();
    for (auto& reader : readers) {
        reader.join();
    }

    EXPECT_EQ(read_errors.load(), 0);
    EXPECT_EQ(write_errors.load(), 0);

    size_type final_size = 0;
    ASSERT_EQ(ArrayObject_GetSize(array, &final_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(final_size, static_cast<size_type>(NUM_APPENDS + 1));
}

// Test concurrent Remove operations
TEST(ArrayObjectThreadSafety, ConcurrentRemove) {
    constexpr int INITIAL_SIZE = 2000;
    constexpr int NUM_THREADS = 4;

    HandleGuard<ArrayObjectHandle, ArrayObject_Release> array;
    ASSERT_EQ(ArrayObject_Create(array.out()), VANILLAPDF_ERROR_SUCCESS);

    // Pre-populate
    for (int i = 0; i < INITIAL_SIZE; ++i) {
        HandleGuard<IntegerObjectHandle, IntegerObject_Release> int_obj;
        ASSERT_EQ(IntegerObject_Create(int_obj.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ObjectHandle, Object_Release> obj;
        ASSERT_EQ(IntegerObject_ToObject(int_obj, obj.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(ArrayObject_Append(array, obj), VANILLAPDF_ERROR_SUCCESS);
    }

    std::atomic<int> success_count{0};
    std::atomic<int> expected_fail_count{0};

    std::vector<std::thread> threads;
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&]() {
            // Each thread tries to remove from index 0 repeatedly
            for (int i = 0; i < INITIAL_SIZE / NUM_THREADS; ++i) {
                error_type result = ArrayObject_Remove(array, 0);
                if (result == VANILLAPDF_ERROR_SUCCESS) {
                    success_count++;
                } else {
                    // Expected: another thread may have already emptied the array
                    expected_fail_count++;
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Total successful removes should not exceed initial size
    EXPECT_LE(success_count.load(), INITIAL_SIZE);

    size_type final_size = 0;
    ASSERT_EQ(ArrayObject_GetSize(array, &final_size), VANILLAPDF_ERROR_SUCCESS);

    // Remaining size + successful removes should equal initial size
    EXPECT_EQ(static_cast<int>(final_size) + success_count.load(), INITIAL_SIZE);
}

// Test concurrent reads while another thread inserts entries
TEST(DictionaryObjectThreadSafety, ConcurrentReadWrite) {
    constexpr int NUM_READERS = 4;
    constexpr int NUM_INSERTS = 1000;

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> dict;
    ASSERT_EQ(DictionaryObject_Create(dict.out()), VANILLAPDF_ERROR_SUCCESS);

    // Pre-populate with one entry so readers always have something to find
    {
        HandleGuard<NameObjectHandle, NameObject_Release> seed_key;
        ASSERT_EQ(NameObject_CreateFromDecodedString("seed", seed_key.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<IntegerObjectHandle, IntegerObject_Release> seed_val;
        ASSERT_EQ(IntegerObject_Create(seed_val.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ObjectHandle, Object_Release> seed_obj;
        ASSERT_EQ(IntegerObject_ToObject(seed_val, seed_obj.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DictionaryObject_Insert(dict, seed_key, seed_obj, VANILLAPDF_RV_FALSE), VANILLAPDF_ERROR_SUCCESS);
    }

    std::atomic<bool> done{false};
    std::atomic<int> read_errors{0};
    std::atomic<int> write_errors{0};

    // Writer thread: continuously inserts entries with unique keys
    std::thread writer([&]() {
        for (int i = 0; i < NUM_INSERTS; ++i) {
            auto key_str = "key_" + std::to_string(i);

            NameObjectHandle* key = nullptr;
            if (NameObject_CreateFromDecodedString(key_str.c_str(), &key) != VANILLAPDF_ERROR_SUCCESS) {
                write_errors++;
                continue;
            }

            IntegerObjectHandle* int_obj = nullptr;
            if (IntegerObject_Create(&int_obj) != VANILLAPDF_ERROR_SUCCESS) {
                NameObject_Release(key);
                write_errors++;
                continue;
            }

            ObjectHandle* obj = nullptr;
            if (IntegerObject_ToObject(int_obj, &obj) != VANILLAPDF_ERROR_SUCCESS) {
                IntegerObject_Release(int_obj);
                NameObject_Release(key);
                write_errors++;
                continue;
            }

            if (DictionaryObject_Insert(dict, key, obj, VANILLAPDF_RV_FALSE) != VANILLAPDF_ERROR_SUCCESS) {
                write_errors++;
            }

            Object_Release(obj);
            IntegerObject_Release(int_obj);
            NameObject_Release(key);
        }
        done = true;
    });

    // Reader threads: continuously read size and check for the seed entry
    std::vector<std::thread> readers;
    for (int t = 0; t < NUM_READERS; ++t) {
        readers.emplace_back([&]() {
            while (!done.load()) {
                size_type size = 0;
                if (DictionaryObject_GetSize(dict, &size) != VANILLAPDF_ERROR_SUCCESS) {
                    read_errors++;
                    continue;
                }

                if (size > 0) {
                    NameObjectHandle* key = nullptr;
                    if (NameObject_CreateFromDecodedString("seed", &key) != VANILLAPDF_ERROR_SUCCESS) {
                        read_errors++;
                        continue;
                    }

                    boolean_type contains = VANILLAPDF_RV_FALSE;
                    if (DictionaryObject_Contains(dict, key, &contains) != VANILLAPDF_ERROR_SUCCESS) {
                        read_errors++;
                    }

                    NameObject_Release(key);
                }
            }
        });
    }

    writer.join();
    for (auto& reader : readers) {
        reader.join();
    }

    EXPECT_EQ(read_errors.load(), 0);
    EXPECT_EQ(write_errors.load(), 0);

    size_type final_size = 0;
    ASSERT_EQ(DictionaryObject_GetSize(dict, &final_size), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(final_size, static_cast<size_type>(NUM_INSERTS + 1));
}

// Reproduction for the reported native crash in a multi-threaded environment
// (last frame: LinkAnnotation_GetDestination). Resolving a named destination
// reaches Document::GetDocumentCatalog, which lazily initializes a cached
// Catalog (mutable m_catalog) with no synchronization.
//
// This test isolates that member: a freshly created document leaves m_catalog
// cold (CreateCatalog only inserts /Root into the trailer, it does not populate
// the cache), so the first concurrent Document_GetCatalog calls all hit the
// lazy-init path simultaneously. A correct implementation caches exactly one
// instance, so every concurrent call MUST return the same Catalog handle.
// The double-init race produces distinct Catalog instances (and races on the
// underlying Deferred<> pointer / refcount, which can also crash).
//
// Each document only races on its first access, so we repeat over many fresh
// documents to get many cold-cache windows, and use a spin barrier so all
// threads are released into the cold cache at the same moment.
TEST(CatalogThreadSafety, ConcurrentLazyInitReturnsSameInstance) {
    constexpr int NUM_DOCUMENTS = 500;
    constexpr int NUM_THREADS = 16;

    std::atomic<int> mismatch_count{0};
    std::atomic<int> error_count{0};

    for (int d = 0; d < NUM_DOCUMENTS; ++d) {
        HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
        HandleGuard<FileHandle, File_Release> file;
        HandleGuard<DocumentHandle, Document_Release> doc;

        ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_CreateStream(io_stream, "catalog_race", file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_CreateFile(file, doc.out()), VANILLAPDF_ERROR_SUCCESS);

        // Spin barrier so every thread hits the cold cache simultaneously
        std::atomic<int> ready{0};
        std::atomic<bool> go{false};

        std::vector<CatalogHandle*> catalogs(NUM_THREADS, nullptr);
        std::vector<std::thread> threads;

        for (int t = 0; t < NUM_THREADS; ++t) {
            threads.emplace_back([&, t]() {
                ready.fetch_add(1);
                while (!go.load()) { /* spin until released */ }

                CatalogHandle* catalog = nullptr;
                if (Document_GetCatalog(doc, &catalog) == VANILLAPDF_ERROR_SUCCESS) {
                    catalogs[t] = catalog;
                } else {
                    error_count.fetch_add(1);
                }
            });
        }

        while (ready.load() < NUM_THREADS) { /* wait for all threads to spin up */ }
        go.store(true);

        for (auto& thread : threads) {
            thread.join();
        }

        // Every thread must observe the same cached Catalog instance
        CatalogHandle* first = nullptr;
        for (auto* catalog : catalogs) {
            if (catalog == nullptr) {
                continue;
            }

            if (first == nullptr) {
                first = catalog;
            } else if (catalog != first) {
                mismatch_count.fetch_add(1);
            }
        }

        for (auto* catalog : catalogs) {
            if (catalog != nullptr) {
                Catalog_Release(catalog);
            }
        }
    }

    EXPECT_EQ(error_count.load(), 0);
    EXPECT_EQ(mismatch_count.load(), 0)
        << "Document::GetDocumentCatalog returned multiple Catalog instances under concurrency";
}

} /* thread_safety */
