#ifndef _VANILLAPDF_TOOLS_HANDLE_GUARD_H
#define _VANILLAPDF_TOOLS_HANDLE_GUARD_H

#include "vanillapdf/c_vanillapdf_api.h"

/**
 * RAII wrapper for C API handle lifetime management in the tools.
 *
 * Automatically calls the release function when the guard goes out of scope,
 * so every early return frees the handle. The former C implementation chained
 * manual Release calls and leaked on its error paths (LeakSanitizer caught one
 * in remove_page).
 *
 * Usage:
 *   DocumentGuard document;
 *   RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(path, document.out()));
 *   RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, destination));
 *   // document is automatically released at scope exit
 */
template<typename Handle, error_type (CALLING_CONVENTION *ReleaseFn)(Handle*)>
class HandleGuard {
    Handle* handle_ = nullptr;

public:
    HandleGuard() = default;
    explicit HandleGuard(Handle* h) : handle_(h) {}

    ~HandleGuard() {
        reset();
    }

    // Move-only semantics to prevent double-release
    HandleGuard(HandleGuard&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }

    HandleGuard& operator=(HandleGuard&& other) noexcept {
        if (this != &other) {
            reset();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    HandleGuard(const HandleGuard&) = delete;
    HandleGuard& operator=(const HandleGuard&) = delete;

    /**
     * Returns a pointer to the internal handle pointer.
     * Used with C API Create/Get functions: Create(guard.out()).
     * A handle already owned by the guard is released first, so a guard
     * declared outside a loop can be refilled on every iteration.
     */
    Handle** out() {
        reset();
        return &handle_;
    }

    /**
     * Returns the raw handle pointer.
     * Used when passing to C API functions that take Handle*.
     */
    Handle* get() const { return handle_; }

    /**
     * Implicit conversion to raw handle pointer.
     * Allows passing directly to C API functions.
     */
    operator Handle*() const { return handle_; }

    /**
     * Explicit bool conversion for null checks.
     */
    explicit operator bool() const { return handle_ != nullptr; }

    /**
     * Releases the owned handle now. The release result is deliberately
     * ignored, matching the cleanup blocks it replaces.
     */
    void reset() {
        if (handle_) {
            ReleaseFn(handle_);
            handle_ = nullptr;
        }
    }
};

// Files and documents
using FileGuard = HandleGuard<FileHandle, File_Release>;
using FileWriterGuard = HandleGuard<FileWriterHandle, FileWriter_Release>;
using FileWriterObserverGuard = HandleGuard<FileWriterObserverHandle, FileWriterObserver_Release>;
using DocumentGuard = HandleGuard<DocumentHandle, Document_Release>;
using CatalogGuard = HandleGuard<CatalogHandle, Catalog_Release>;
using PageTreeGuard = HandleGuard<PageTreeHandle, PageTree_Release>;
using PageObjectGuard = HandleGuard<PageObjectHandle, PageObject_Release>;
using PageContentsGuard = HandleGuard<PageContentsHandle, PageContents_Release>;

// Cross-reference tables
using XrefChainGuard = HandleGuard<XrefChainHandle, XrefChain_Release>;
using XrefChainIteratorGuard = HandleGuard<XrefChainIteratorHandle, XrefChainIterator_Release>;
using XrefGuard = HandleGuard<XrefHandle, Xref_Release>;
using XrefIteratorGuard = HandleGuard<XrefIteratorHandle, XrefIterator_Release>;
using XrefEntryGuard = HandleGuard<XrefEntryHandle, XrefEntry_Release>;
using XrefUsedEntryGuard = HandleGuard<XrefUsedEntryHandle, XrefUsedEntry_Release>;
using XrefCompressedEntryGuard = HandleGuard<XrefCompressedEntryHandle, XrefCompressedEntry_Release>;

// Objects
using ObjectGuard = HandleGuard<ObjectHandle, Object_Release>;
using NameObjectGuard = HandleGuard<NameObjectHandle, NameObject_Release>;
using IntegerObjectGuard = HandleGuard<IntegerObjectHandle, IntegerObject_Release>;
using DictionaryObjectGuard = HandleGuard<DictionaryObjectHandle, DictionaryObject_Release>;
using StreamObjectGuard = HandleGuard<StreamObjectHandle, StreamObject_Release>;

// Content streams
using ContentInstructionCollectionGuard = HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release>;
using ContentInstructionGuard = HandleGuard<ContentInstructionHandle, ContentInstruction_Release>;
using ContentObjectGuard = HandleGuard<ContentObjectHandle, ContentObject_Release>;
using ContentObjectInlineImageGuard = HandleGuard<ContentObjectInlineImageHandle, ContentObjectInlineImage_Release>;

// Streams, buffers and filters
using BufferGuard = HandleGuard<BufferHandle, Buffer_Release>;
using InputStreamGuard = HandleGuard<InputStreamHandle, InputStream_Release>;
using OutputStreamGuard = HandleGuard<OutputStreamHandle, OutputStream_Release>;
using FlateDecodeFilterGuard = HandleGuard<FlateDecodeFilterHandle, FlateDecodeFilter_Release>;
using DCTDecodeFilterGuard = HandleGuard<DCTDecodeFilterHandle, DCTDecodeFilter_Release>;
using JPXDecodeFilterGuard = HandleGuard<JPXDecodeFilterHandle, JPXDecodeFilter_Release>;
using ASCII85DecodeFilterGuard = HandleGuard<ASCII85DecodeFilterHandle, ASCII85DecodeFilter_Release>;
using ASCIIHexDecodeFilterGuard = HandleGuard<ASCIIHexDecodeFilterHandle, ASCIIHexDecodeFilter_Release>;

// Interactive forms and signatures
using InteractiveFormGuard = HandleGuard<InteractiveFormHandle, InteractiveForm_Release>;
using FieldCollectionGuard = HandleGuard<FieldCollectionHandle, FieldCollection_Release>;
using FieldGuard = HandleGuard<FieldHandle, Field_Release>;
using SignatureFieldGuard = HandleGuard<SignatureFieldHandle, SignatureField_Release>;
using DigitalSignatureGuard = HandleGuard<DigitalSignatureHandle, DigitalSignature_Release>;
using TrustedCertificateStoreGuard = HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release>;
using SignatureVerificationSettingsGuard = HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release>;
using SignatureVerificationResultGuard = HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release>;
using DocumentSignatureSettingsGuard = HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release>;
using PKCS12KeyGuard = HandleGuard<PKCS12KeyHandle, PKCS12Key_Release>;
using SigningKeyGuard = HandleGuard<SigningKeyHandle, SigningKey_Release>;
using EncryptionKeyGuard = HandleGuard<EncryptionKeyHandle, EncryptionKey_Release>;
using DocumentEncryptionSettingsGuard = HandleGuard<DocumentEncryptionSettingsHandle, DocumentEncryptionSettings_Release>;

// Validation and utilities
using FileStructureValidationResultGuard = HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release>;
using FileStructureIssueGuard = HandleGuard<FileStructureIssueHandle, FileStructureIssue_Release>;
using DateGuard = HandleGuard<DateHandle, Date_Release>;

#endif /* _VANILLAPDF_TOOLS_HANDLE_GUARD_H */
