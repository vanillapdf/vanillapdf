#include "unittest.h"
#include "handle_guard.h"
#include "test_data.h"

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

// Regression test for concurrent open AND release of a document over one
// shared file.
//
// The sibling test above never releases a document while other threads are
// running - every handle is parked in a vector and released after the join - so
// the reference counter only ever climbs and never returns to zero during the
// concurrent phase. That is precisely the window this bug needs, which is why
// 50 threads x 2000 iterations could not reach it while an open/release cycle
// reaches it at 2 threads.
//
// The failure is a heap corruption, not an assertion: a document whose counter
// had already reached zero was revived by a weak reference upgrade, leaving two
// owners of storage that was being torn down. Both eventually delete it. It
// surfaces as the test host dying (STATUS_HEAP_CORRUPTION / FailFast) with no
// managed exception, usually while some unrelated test is in flight, so treat
// any hard crash of this binary as this test failing.
void OpenReleaseCycleWorker(
    FileHandle* shared_file,
    int iterations,
    std::atomic<int>& success_count,
    std::atomic<int>& error_count) {

    for (int i = 0; i < iterations; ++i) {
        DocumentHandle* doc = nullptr;
        if (Document_OpenFile(shared_file, &doc) != VANILLAPDF_ERROR_SUCCESS || doc == nullptr) {
            error_count.fetch_add(1);
            continue;
        }

        // Touch the document, so a revived instance is actually used
        // and not merely held
        CatalogHandle* catalog = nullptr;
        if (Document_GetCatalog(doc, &catalog) == VANILLAPDF_ERROR_SUCCESS) {
            Catalog_Release(catalog);
        }

        Document_Release(doc);
        success_count.fetch_add(1);
    }
}

TEST(DocumentOpenFileThreadSafety, ConcurrentOpenReleaseCycleSameFile) {
    constexpr int NUM_THREADS = 4;
    constexpr int ITERATIONS_PER_THREAD = 250;

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> shared_file;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "shared_open_release_cycle", shared_file.out()), VANILLAPDF_ERROR_SUCCESS);

    // One cycle up front, so the file materializes whatever it builds lazily on
    // first use - the xref chain among it. Those live as long as the file, well
    // past the count check below, and would otherwise read as a leak.
    std::atomic<int> warmup_success{0};
    std::atomic<int> warmup_errors{0};
    OpenReleaseCycleWorker(shared_file.get(), 1, warmup_success, warmup_errors);
    ASSERT_EQ(warmup_errors.load(), 0);

    bigint_type baseline_objects = 0;
    ASSERT_EQ(ObjectDiagnostics_GetActiveObjectCount(&baseline_objects), VANILLAPDF_ERROR_SUCCESS);

    std::atomic<int> success_count{0};
    std::atomic<int> error_count{0};

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back(
            OpenReleaseCycleWorker,
            shared_file.get(),
            ITERATIONS_PER_THREAD,
            std::ref(success_count),
            std::ref(error_count));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(error_count.load(), 0);
    EXPECT_EQ(success_count.load(), NUM_THREADS * ITERATIONS_PER_THREAD);

    // Deliberately no assertion that every open returned the same instance.
    // A document that legitimately reached zero references is dead, and the
    // next open has to build a fresh one - demanding identity here would be
    // asserting the resurrection this test exists to prevent.

    // Every document opened above has been released, so the live object count
    // has to be back where it started. A lower count means something was freed
    // twice, a higher one means a registry entry outlived its document.
    bigint_type remaining_objects = 0;
    ASSERT_EQ(ObjectDiagnostics_GetActiveObjectCount(&remaining_objects), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(remaining_objects, baseline_objects);
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
                while (!go.load()) { std::this_thread::yield(); }

                CatalogHandle* catalog = nullptr;
                if (Document_GetCatalog(doc, &catalog) == VANILLAPDF_ERROR_SUCCESS) {
                    catalogs[t] = catalog;
                } else {
                    error_count.fetch_add(1);
                }
            });
        }

        while (ready.load() < NUM_THREADS) { std::this_thread::yield(); }
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

// Regression test for the shared input-stream cursor race in File::GetByteRange.
//
// A single PDF File is backed by one input stream with a single read cursor. Reading a
// signature's ByteRange (File::GetByteRange) seeks the cursor and then reads from it.
// If that seek+read is not performed atomically under the stream's exclusive lock, a
// second thread that is also reading the file (here: another signature validation, which
// is exactly the desktop-app scenario of validating signatures while the file is being
// used by another thread) moves the cursor in between, so the reader reconstructs the
// wrong signed bytes. The PKCS#7 digest then no longer matches and IsDocumentIntact
// becomes false intermittently (matching the observed CMS_SignerInfo_verify_content
// failures), even though the document on disk is perfectly intact.
//
// Note: this race is independent of the storage backend. Loading the whole file into
// memory does NOT avoid it, because the in-memory stream still exposes a single shared
// cursor - the fix is to make the seek+read atomic, not to change where the bytes live.
//
// Without the lock in File::GetByteRange this test fails (some validations report the
// document as not intact, or the verification call fails outright). With the lock it is
// deterministically stable.
TEST(GetByteRangeThreadSafety, ConcurrentSignatureValidationKeepsDocumentIntact) {
    constexpr int NUM_THREADS = 8;
    constexpr int ITERATIONS_PER_THREAD = 150;

    InputOutputStreamHandle* source_stream = nullptr;
    InputOutputStreamHandle* signed_stream = nullptr;
    FileHandle* source_file = nullptr;
    FileHandle* signed_file = nullptr;
    BufferHandle* pkcs12_buffer = nullptr;
    PKCS12KeyHandle* pkcs12_key = nullptr;
    SigningKeyHandle* signing_key = nullptr;
    DateHandle* signing_time = nullptr;
    DocumentSignatureSettingsHandle* signature_settings = nullptr;
    DocumentHandle* source_document = nullptr;
    DocumentHandle* signed_document = nullptr;
    CatalogHandle* catalog = nullptr;
    InteractiveFormHandle* acro_form = nullptr;
    FieldCollectionHandle* fields = nullptr;
    FieldHandle* field = nullptr;
    SignatureFieldHandle* sig_field = nullptr;
    DigitalSignatureHandle* digital_signature = nullptr;
    TrustedCertificateStoreHandle* trust_store = nullptr;

    // Step 1: Create a simple PDF document in memory and sign it, producing a
    // self-contained signed PDF in the signed_stream (no external file needed).
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&source_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(source_stream, "memory_source.pdf", &source_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(source_file, &source_document), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                    SIGNING_CERTIFICATE_SIZE, &pkcs12_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, &pkcs12_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, &signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_CreateCurrent(&signing_time), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DocumentSignatureSettings_Create(&signature_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(InputOutputStream_CreateFromMemory(&signed_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(signed_stream, "memory_signed.pdf", &signed_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Sign(source_document, signed_file, signature_settings), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Document_Release(source_document), VANILLAPDF_ERROR_SUCCESS);
    source_document = nullptr;
    ASSERT_EQ(File_Release(source_file), VANILLAPDF_ERROR_SUCCESS);
    source_file = nullptr;
    ASSERT_EQ(File_Release(signed_file), VANILLAPDF_ERROR_SUCCESS);
    signed_file = nullptr;

    // Step 2: Reopen the signed document and navigate to its digital signature.
    ASSERT_EQ(File_OpenStream(signed_stream, "memory_signed.pdf", &signed_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(signed_file, &signed_document), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Document_GetCatalog(signed_document, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetAcroForm(catalog, &acro_form), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetFields(acro_form, &fields), VANILLAPDF_ERROR_SUCCESS);

    size_type field_count = 0;
    ASSERT_EQ(FieldCollection_GetSize(fields, &field_count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_GT(field_count, 0);

    ASSERT_EQ(FieldCollection_At(fields, 0, &field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureField_FromField(field, &sig_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureField_GetValue(sig_field, &digital_signature), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(digital_signature, nullptr);

    ASSERT_EQ(TrustedCertificateStore_Create(&trust_store), VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Single-threaded warm-up. This both proves the document is valid when
    // accessed serially and forces all lazily-loaded objects referenced during
    // verification to be initialized, so the concurrent phase below exercises the
    // ByteRange file reads rather than first-touch object parsing. It also seeds the
    // trust store with the (self-signed) signer certificate.
    {
        SignatureVerificationResultHandle* warmup = nullptr;
        ASSERT_EQ(DigitalSignatureExtensions_Verify(digital_signature, signed_document, trust_store,
                  nullptr, &warmup), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(warmup, nullptr);

        boolean_type warmup_intact = VANILLAPDF_RV_FALSE;
        ASSERT_EQ(SignatureVerificationResult_IsDocumentIntact(warmup, &warmup_intact), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(warmup_intact, VANILLAPDF_RV_TRUE) << "Document must be intact when validated serially";

        BufferHandle* signer_cert = nullptr;
        ASSERT_EQ(SignatureVerificationResult_GetSignerCertificate(warmup, &signer_cert), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, signer_cert), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Buffer_Release(signer_cert), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(SignatureVerificationResult_Release(warmup), VANILLAPDF_ERROR_SUCCESS);
    }

    // Step 4: Concurrent phase. Many threads validate the SAME signature on the SAME
    // document at the same time. Each validation reconstructs the signed bytes via
    // File::GetByteRange. The document never changes, so every validation must report
    // the document as intact. Any "not intact" result (or failed call) means a
    // concurrent validation corrupted the shared stream cursor of another.
    std::atomic<int> not_intact_count{0};
    std::atomic<int> verify_error_count{0};

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
                SignatureVerificationResultHandle* result = nullptr;
                error_type rv = DigitalSignatureExtensions_Verify(
                    digital_signature, signed_document, trust_store, nullptr, &result);

                if (rv != VANILLAPDF_ERROR_SUCCESS || result == nullptr) {
                    verify_error_count++;
                    continue;
                }

                boolean_type is_intact = VANILLAPDF_RV_FALSE;
                if (SignatureVerificationResult_IsDocumentIntact(result, &is_intact) != VANILLAPDF_ERROR_SUCCESS
                    || is_intact != VANILLAPDF_RV_TRUE) {
                    not_intact_count++;
                }

                SignatureVerificationResult_Release(result);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(not_intact_count.load(), 0)
        << "Concurrent signature validations corrupted the shared input cursor: "
        << "a perfectly intact document was reported as tampered.";
    EXPECT_EQ(verify_error_count.load(), 0)
        << "Concurrent signature validations failed outright due to the shared input cursor race.";

    // Cleanup
    if (digital_signature) DigitalSignature_Release(digital_signature);
    if (sig_field) SignatureField_Release(sig_field);
    if (field) Field_Release(field);
    if (fields) FieldCollection_Release(fields);
    if (acro_form) InteractiveForm_Release(acro_form);
    if (catalog) Catalog_Release(catalog);
    if (trust_store) TrustedCertificateStore_Release(trust_store);
    if (signed_document) Document_Release(signed_document);
    if (signed_file) File_Release(signed_file);
    if (signature_settings) DocumentSignatureSettings_Release(signature_settings);
    if (signing_time) Date_Release(signing_time);
    if (signing_key) SigningKey_Release(signing_key);
    if (pkcs12_key) PKCS12Key_Release(pkcs12_key);
    if (pkcs12_buffer) Buffer_Release(pkcs12_buffer);
    if (signed_stream) InputOutputStream_Release(signed_stream);
    if (source_stream) InputOutputStream_Release(source_stream);
}

// Regression test for the Document create guard against a stale registry entry.
//
// Document::CreateFile guards on SemanticUtils::HasMappedDocument, which
// originally reported mere key-presence in the global File*-keyed document
// registry. The sibling open path (GetOrCreateDocument) additionally required
// found->second.IsActive(), so only the create path trusted dead entries.
//
// A stale, inactive entry is easy to produce: the Document constructor calls
// AddDocumentMapping BEFORE Initialize(), so opening a file that fails to
// initialize maps the File and then throws, and ~Document never runs to erase
// the entry. Once that File is released its address becomes free, and the next
// File the allocator hands out frequently reuses it. With a presence-only
// guard, Document_CreateFile for that brand-new File then wrongly fails with
// "the file instance was already opened".
//
// This does not require threads - it is a plain registry-consistency bug. It
// only surfaced through the Python binding once #40 let create/open/destroy
// overlap (previously the GIL serialized them), and it reproduced most easily
// on macOS whose allocator recycles freed addresses aggressively. The loop
// below reproduces it deterministically on any platform: each iteration seeds a
// stale entry via a failed open, frees the File, then immediately creates a new
// document whose File tends to land on the just-freed address.
TEST(DocumentCreateThreadSafety, StaleRegistryEntryDoesNotBlockCreate) {
    constexpr int ITERATIONS = 2000;

    for (int i = 0; i < ITERATIONS; ++i) {
        std::string name = "stale_registry_" + std::to_string(i);

        // Seed a stale, inactive registry entry: opening an empty stream as a
        // document maps the File (Document ctor) and then fails in Initialize(),
        // so the entry is never erased. Releasing the File frees its address
        // while the dead entry lingers in the registry.
        {
            HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
            HandleGuard<FileHandle, File_Release> file;

            ASSERT_EQ(InputOutputStream_CreateFromMemory(stream.out()), VANILLAPDF_ERROR_SUCCESS);
            ASSERT_EQ(File_OpenStream(stream, name.c_str(), file.out()), VANILLAPDF_ERROR_SUCCESS);

            DocumentHandle* doc = nullptr;
            error_type rv = Document_OpenFile(file, &doc);
            ASSERT_NE(rv, VANILLAPDF_ERROR_SUCCESS)
                << "empty file was expected to fail initialization and seed a stale registry entry";

            if (doc != nullptr) {
                Document_Release(doc);
            }
        }

        // Create a real document. Its fresh File frequently reuses the address
        // just freed above, colliding with the stale entry. The create guard
        // must treat that dead entry as absent; a presence-only guard reports
        // the fresh file as already opened.
        {
            HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
            HandleGuard<FileHandle, File_Release> file;
            HandleGuard<DocumentHandle, Document_Release> doc;

            ASSERT_EQ(InputOutputStream_CreateFromMemory(stream.out()), VANILLAPDF_ERROR_SUCCESS);
            ASSERT_EQ(File_CreateStream(stream, name.c_str(), file.out()), VANILLAPDF_ERROR_SUCCESS);

            error_type rv = Document_CreateFile(file, doc.out());
            ASSERT_EQ(rv, VANILLAPDF_ERROR_SUCCESS)
                << "iteration " << i << ": a stale registry entry blocked a valid "
                << "Document_CreateFile - the create guard is not liveness-aware";
        }
    }
}

// Positive counterpart to the guard: creating a second document for a file that
// already has a LIVE document must fail. This exercises the create guard's
// active-entry path - HasMappedDocument finds a present, IsActive() entry and
// reports the file as already opened - whereas StaleRegistryEntryDoesNotBlockCreate
// above covers the present-but-inactive case.
TEST(DocumentCreateThreadSafety, CreateOnAlreadyOpenFileFails) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<DocumentHandle, Document_Release> doc;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(stream, "already_open", file.out()), VANILLAPDF_ERROR_SUCCESS);

    // First create registers a live document mapping for the file.
    ASSERT_EQ(Document_CreateFile(file, doc.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(doc.get(), nullptr);

    // While that document is still alive, a second create on the same file must
    // be rejected - the registry already holds an active entry for it.
    HandleGuard<DocumentHandle, Document_Release> second_doc;
    error_type rv = Document_CreateFile(file, second_doc.out());
    EXPECT_NE(rv, VANILLAPDF_ERROR_SUCCESS)
        << "creating a second document for an already-open file must fail";
    EXPECT_EQ(second_doc.get(), nullptr);
}

} /* thread_safety */
