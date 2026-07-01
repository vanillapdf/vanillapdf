#include "unittest.h"
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

} /* thread_safety */
