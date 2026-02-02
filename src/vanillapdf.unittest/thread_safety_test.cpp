#include "unittest.h"

#include <thread>
#include <vector>
#include <atomic>
#include <string>

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
    InputOutputStreamHandle* io_stream = nullptr;
    FileHandle* shared_file = nullptr;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "shared_test_file", &shared_file), VANILLAPDF_ERROR_SUCCESS);

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
    for (auto* doc : documents) {
        if (doc != nullptr) {
            Document_Release(doc);
        }
    }

    ASSERT_EQ(File_Release(shared_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
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

    // Cleanup
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (documents[i] != nullptr) {
            ASSERT_EQ(Document_Release(documents[i]), VANILLAPDF_ERROR_SUCCESS);
        }
        ASSERT_EQ(File_Release(files[i]), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_Release(streams[i]), VANILLAPDF_ERROR_SUCCESS);
    }
}

// Self-contained reproduction of XrefChain data race (Issue #250)
//
// File::GetIndirectObjectInternal() calls XrefChain::Contains() and
// XrefChain::GetXrefEntry(), both of which iterate a std::list<XrefBasePtr>
// without synchronization. When multiple threads resolve indirect references
// concurrently, they iterate the same list simultaneously, causing:
//   - MSVC debug: "list iterators incompatible" assertion
//   - Release builds: undefined behavior (crash, corruption)
//
// This test creates a document in memory, saves and reopens it (forcing
// xref chain creation via PDF parsing), then has threads concurrently call
// File_GetIndirectObject which iterates the xref chain on every invocation.
TEST(ThreadSafety, ConcurrentXrefAccess_Issue250) {
    constexpr int NUM_THREADS = 20;
    constexpr int ITERATIONS = 200;
    constexpr int NUM_PAGES = 200;

    // --- Phase 1: Create a document with pages in memory ---
    InputOutputStreamHandle* create_stream = nullptr;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&create_stream), VANILLAPDF_ERROR_SUCCESS);

    FileHandle* create_file = nullptr;
    ASSERT_EQ(File_CreateStream(create_stream, "create", &create_file), VANILLAPDF_ERROR_SUCCESS);

    DocumentHandle* create_doc = nullptr;
    ASSERT_EQ(Document_CreateFile(create_file, &create_doc), VANILLAPDF_ERROR_SUCCESS);

    CatalogHandle* create_catalog = nullptr;
    ASSERT_EQ(Document_GetCatalog(create_doc, &create_catalog), VANILLAPDF_ERROR_SUCCESS);

    PageTreeHandle* create_pages = nullptr;
    ASSERT_EQ(Catalog_GetPages(create_catalog, &create_pages), VANILLAPDF_ERROR_SUCCESS);

    for (int i = 0; i < NUM_PAGES; ++i) {
        PageObjectHandle* page = nullptr;
        ASSERT_EQ(PageObject_CreateFromDocument(create_doc, &page), VANILLAPDF_ERROR_SUCCESS);

        RectangleHandle* box = nullptr;
        ASSERT_EQ(Rectangle_Create(&box), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Rectangle_SetLowerLeftX(box, 0), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Rectangle_SetLowerLeftY(box, 0), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Rectangle_SetUpperRightX(box, 612), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Rectangle_SetUpperRightY(box, 792), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(PageObject_SetMediaBox(page, box), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(PageTree_InsertPage(create_pages, i + 1, page), VANILLAPDF_ERROR_SUCCESS);

        Rectangle_Release(box);
        PageObject_Release(page);
    }

    // --- Phase 2: Save to memory stream ---
    InputOutputStreamHandle* save_stream = nullptr;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&save_stream), VANILLAPDF_ERROR_SUCCESS);

    FileHandle* save_file = nullptr;
    ASSERT_EQ(File_CreateStream(save_stream, "saved", &save_file), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Document_SaveFile(create_doc, save_file), VANILLAPDF_ERROR_SUCCESS);

    PageTree_Release(create_pages);
    Catalog_Release(create_catalog);
    Document_Release(create_doc);
    File_Release(create_file);
    InputOutputStream_Release(create_stream);
    File_Release(save_file);

    // --- Phase 3: Reopen from memory (forces xref chain parsing) ---
    FileHandle* file = nullptr;
    ASSERT_EQ(File_OpenStream(save_stream, "reopened", &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(file, nullptr);
    ASSERT_EQ(File_Initialize(file), VANILLAPDF_ERROR_SUCCESS);

    // --- Phase 4: Concurrent xref chain access ---
    // File_GetIndirectObject -> GetIndirectObjectInternal -> XrefChain::Contains()
    // + XrefChain::GetXrefEntry(), both iterate std::list on every call.
    // Object numbers 1..max_obj cover catalog, page tree, and page objects.
    const biguint_type max_obj = static_cast<biguint_type>(NUM_PAGES + 5);

    std::atomic<int> ready_count{0};
    std::atomic<bool> go{false};

    std::vector<std::thread> threads;
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&]() {
            ready_count.fetch_add(1, std::memory_order_release);
            while (!go.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }

            for (int iter = 0; iter < ITERATIONS; ++iter) {
                for (biguint_type obj = 1; obj <= max_obj; ++obj) {
                    ObjectHandle* result = nullptr;
                    File_GetIndirectObject(file, obj, 0, &result);
                    if (result != nullptr) {
                        Object_Release(result);
                    }
                }
            }
        });
    }

    // Release all threads simultaneously for maximum contention
    while (ready_count.load(std::memory_order_acquire) < NUM_THREADS) {
        std::this_thread::yield();
    }
    go.store(true, std::memory_order_release);

    for (auto& thread : threads) {
        thread.join();
    }

    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InputOutputStream_Release(save_stream), VANILLAPDF_ERROR_SUCCESS);
}

// Reproduce thread-safety issues in concurrent semantic API access (Issue #250)
//
// This companion test exercises the higher-level semantic API path:
//   C API (e.g. PageObject_GetResources)
//     -> FindAs<T>()
//       -> ObjectTypeFunctor::Convert()
//         -> DereferenceHelper::Get()
//           -> std::map::find() / operator[]
//             -> IndirectReferenceObject::operator<()
//               -> locks this->m_access_lock, then other.m_access_lock
//
// Requires a real PDF with fonts, content streams, and annotations to generate
// enough indirect reference complexity. Skipped when --test_dir is not provided.
//
// If this test hangs, the ABBA deadlock in operator<() has been triggered.
// If it crashes, a data race in the xref chain or semantic layer was triggered.
TEST(ThreadSafety, ConcurrentSemanticTraversal_Issue250) {
    constexpr int NUM_THREADS = 2;
    constexpr int ITERATIONS = 500;

    if (g_test_data_dir.empty()) {
        GTEST_SKIP() << "Requires --test_dir=<path> pointing to test data directory";
    }

    std::string pdf_path = g_test_data_dir + "/pdfjs/basicapi.pdf";

    FileHandle* file = nullptr;
    ASSERT_EQ(File_Open(pdf_path.c_str(), &file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(file, nullptr);
    ASSERT_EQ(File_Initialize(file), VANILLAPDF_ERROR_SUCCESS);

    DocumentHandle* doc = nullptr;
    ASSERT_EQ(Document_OpenFile(file, &doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(doc, nullptr);

    // Get the catalog, page tree, and page 1 in single-threaded setup.
    // This resolves all navigation objects so we can focus on
    // the specific concurrent GetMediaBox call.
    CatalogHandle* catalog = nullptr;
    ASSERT_EQ(Document_GetCatalog(doc, &catalog), VANILLAPDF_ERROR_SUCCESS);

    PageTreeHandle* pages = nullptr;
    ASSERT_EQ(Catalog_GetPages(catalog, &pages), VANILLAPDF_ERROR_SUCCESS);

    size_type page_count = 0;
    PageTree_GetPageCount(pages, &page_count);
    ASSERT_GT(page_count, static_cast<size_type>(0));

    // Get page 1 once and keep it for concurrent access
    PageObjectHandle* shared_page = nullptr;
    ASSERT_EQ(PageTree_GetPage(pages, 1, &shared_page), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(shared_page, nullptr);

    // Warmup: call GetMediaBox once to resolve all indirect references
    {
        RectangleHandle* box = nullptr;
        ASSERT_EQ(PageObject_GetMediaBox(shared_page, &box), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(box, nullptr);
        Rectangle_Release(box);
    }

    // Get the base dictionary for low-level access
    DictionaryObjectHandle* page_dict = nullptr;
    ASSERT_EQ(PageObject_GetBaseObject(shared_page, &page_dict), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(page_dict, nullptr);

    // Stage 1: Test concurrent Dictionary Find (no type conversion)
    // This tests only the dictionary lookup + indirect reference dereference
    {
        std::atomic<int> ready_count{0};
        std::atomic<bool> go{false};
        std::atomic<int> completed{0};
        std::atomic<int> success_count{0};

        std::vector<std::thread> threads;

        for (int t = 0; t < NUM_THREADS; ++t) {
            threads.emplace_back([&]() {
                ready_count.fetch_add(1, std::memory_order_release);
                while (!go.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                for (int iter = 0; iter < ITERATIONS; ++iter) {
                    ObjectHandle* obj = nullptr;
                    error_type err = DictionaryObject_Find(
                        page_dict, NameConstant_MediaBox, &obj);
                    if (err == VANILLAPDF_ERROR_SUCCESS && obj != nullptr) {
                        success_count.fetch_add(1, std::memory_order_relaxed);
                        Object_Release(obj);
                    }
                }

                completed.fetch_add(1, std::memory_order_relaxed);
            });
        }

        while (ready_count.load(std::memory_order_acquire) < NUM_THREADS) {
            std::this_thread::yield();
        }
        go.store(true, std::memory_order_release);

        for (auto& thread : threads) {
            thread.join();
        }

        EXPECT_EQ(completed.load(), NUM_THREADS);
        EXPECT_EQ(success_count.load(), NUM_THREADS * ITERATIONS);
    }

    // Stage 2: Concurrent GetMediaBox with library-side logging
    {
        std::atomic<int> ready_count{0};
        std::atomic<bool> go{false};
        std::atomic<int> completed{0};
        std::atomic<int> success{0};

        std::vector<std::thread> threads;

        for (int t = 0; t < NUM_THREADS; ++t) {
            threads.emplace_back([&]() {
                ready_count.fetch_add(1, std::memory_order_release);
                while (!go.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                for (int iter = 0; iter < ITERATIONS; ++iter) {
                    RectangleHandle* box = nullptr;
                    error_type err = PageObject_GetMediaBox(shared_page, &box);
                    if (err == VANILLAPDF_ERROR_SUCCESS && box != nullptr) {
                        success.fetch_add(1, std::memory_order_relaxed);
                        Rectangle_Release(box);
                    }
                }

                completed.fetch_add(1, std::memory_order_relaxed);
            });
        }

        while (ready_count.load(std::memory_order_acquire) < NUM_THREADS) {
            std::this_thread::yield();
        }
        go.store(true, std::memory_order_release);

        for (auto& thread : threads) {
            thread.join();
        }

        printf("GetMediaBox: completed=%d success=%d\n",
               completed.load(), success.load());
        EXPECT_EQ(completed.load(), NUM_THREADS);
        EXPECT_EQ(success.load(), NUM_THREADS * ITERATIONS);
    }

    DictionaryObject_Release(page_dict);

    PageObject_Release(shared_page);
    PageTree_Release(pages);
    Catalog_Release(catalog);

    ASSERT_EQ(Document_Release(doc), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Release(file), VANILLAPDF_ERROR_SUCCESS);
}

} /* thread_safety */
