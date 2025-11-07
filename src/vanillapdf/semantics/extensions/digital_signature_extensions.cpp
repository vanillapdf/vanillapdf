#include "precompiled.h"

#include "semantics/extensions/digital_signature_extensions.h"
#include "semantics/objects/digital_signature.h"
#include "semantics/objects/document.h"

#include "syntax/files/file.h"
#include "syntax/objects/integer_object.h"
#include "syntax/objects/string_object.h"

#include "utils/buffer.h"
#include "utils/exceptions.h"
#include "utils/signature_verifier.h"
#include "utils/math_utils.h"

namespace vanillapdf {
namespace semantics {

SignatureVerificationResultPtr DigitalSignatureExtensions::Verify(
    DigitalSignaturePtr signature,
    DocumentPtr document,
    TrustedCertificateStorePtr trusted_store,
    VerificationFlags flags
) {
    // Get the file from the document
    auto file = document->GetFile();
    if (file.empty()) {
        LOG_ERROR_AND_THROW_GENERAL("Document file is not available");
    }

    // Extract ByteRange from signature
    OuputByteRangeCollectionPtr byte_range_collection;
    if (!signature->ByteRange(byte_range_collection) || byte_range_collection.empty()) {
        LOG_ERROR_AND_THROW_GENERAL("Signature does not contain valid ByteRange");
    }

    // Read and concatenate all byte ranges
    auto range_count = byte_range_collection->GetSize();
    if (range_count == 0) {
        LOG_ERROR_AND_THROW_GENERAL("ByteRange collection is empty");
    }

    // Calculate total size of signed data
    types::size_type total_size = 0;
    for (types::size_type i = 0; i < range_count; ++i) {
        auto byte_range = byte_range_collection->GetValue(i);
        auto length = byte_range->Length();
        auto length_converted = ValueConvertUtils::SafeConvert<types::size_type>(length->GetIntegerValue());

        // Perform safe addition to prevent overflows
        total_size = SafeAddition<types::size_type>(total_size, length_converted);
    }

    // Create buffer for signed data
    auto signed_data_buffer = make_deferred_container<Buffer>();

    // Make the allocation reservation, so that we can fit entire content in one allocation
    signed_data_buffer->reserve(total_size);

    // Read and concatenate byte ranges
    for (types::size_type i = 0; i < range_count; ++i) {
        auto byte_range = byte_range_collection->GetValue(i);
        auto byte_offset = byte_range->ByteOffset();
        auto length = byte_range->Length();

        auto range_offset = ValueConvertUtils::SafeConvert<types::stream_size>(byte_offset->GetIntegerValue());
        auto range_length = ValueConvertUtils::SafeConvert<types::size_type>(length->GetIntegerValue());

        // Read this range from the file
        auto range_data = file->GetByteRange(range_offset, range_length);

        // Insert into the concatenated buffer
        signed_data_buffer->insert(signed_data_buffer->end(), range_data->begin(), range_data->end());
    }

    // Extract signature contents (PKCS#7 blob)
    auto signature_contents_obj = signature->Contents();
    if (signature_contents_obj.empty()) {
        LOG_ERROR_AND_THROW_GENERAL("Signature does not contain valid Contents");
    }

    // GetValue() returns the decoded bytes (not hex-encoded)
    auto signature_contents = signature_contents_obj->GetValue();

    // Call the low-level signature verifier
    return SignatureVerifier::Verify(
        *signed_data_buffer,
        *signature_contents,
        trusted_store,
        flags
    );
}

} // semantics
} // vanillapdf
