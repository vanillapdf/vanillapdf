#include "precompiled.h"

#include "utils/misc_utils.h"
#include "utils/crypto_utils.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <cctype>

namespace vanillapdf {

BufferPtr MiscUtils::ToBase64(const Buffer& value) {

    auto memory_bio = BIO_new(BIO_s_mem());
    if (memory_bio == nullptr) {
        throw IOErrorException("Could not create memory buffer");
    }

    SCOPE_GUARD([memory_bio]() { BIO_free_all(memory_bio); });

    auto base64_bio = BIO_new(BIO_f_base64());
    if (base64_bio == nullptr) {
        throw IOErrorException("Could not create base64 filter");
    }

    SCOPE_GUARD([base64_bio]() { BIO_free_all(base64_bio); });

    // Insert base64 filter into bio chain
    BIO_push(base64_bio, memory_bio);

    //Ignore newlines - write everything in one line
    BIO_set_flags(base64_bio, BIO_FLAGS_BASE64_NO_NL);

    auto value_size = ValueConvertUtils::SafeConvert<int>(value.size());
    auto bytes_written = BIO_write(memory_bio, value.data(), value_size);
    if (bytes_written <= 0) {
        throw IOErrorException("Could not write data");
    }

    auto flushed = BIO_flush(base64_bio);
    if (flushed != 1) {
        throw IOErrorException("Could not flush buffer");
    }

    BUF_MEM* memory_buffer = nullptr;
    auto have_mem_ptr = BIO_get_mem_ptr(memory_bio, &memory_buffer);
    if (have_mem_ptr != 1) {
        throw IOErrorException("Could not get memory pointer");
    }

    return make_deferred_container<Buffer>(memory_buffer->data, memory_buffer->length);
}

BufferPtr MiscUtils::FromBase64(const Buffer& value) {

    auto memory_bio = BIO_new(BIO_s_mem());
    if (memory_bio == nullptr) {
        throw IOErrorException("Could not create memory buffer");
    }

    SCOPE_GUARD([memory_bio]() { BIO_free(memory_bio); });

    auto base64_bio = BIO_new(BIO_f_base64());
    if (base64_bio == nullptr) {
        throw IOErrorException("Could not create base64 filter");
    }

    SCOPE_GUARD([base64_bio]() { BIO_free(base64_bio); });

    auto value_size = ValueConvertUtils::SafeConvert<int>(value.size());
    auto bytes_written = BIO_write(memory_bio, value.data(), value_size);
    if (bytes_written <= 0) {
        throw IOErrorException("Could not write data into buffer: " + std::to_string(bytes_written));
    }

    auto flushed = BIO_flush(memory_bio);
    if (flushed != 1) {
        throw IOErrorException("Could not flush buffer: " + std::to_string(flushed));
    }

    // Insert base64 filter into bio chain
    BIO_push(base64_bio, memory_bio);

    //Ignore newlines - write everything in one line
    BIO_set_flags(base64_bio, BIO_FLAGS_BASE64_NO_NL);

    BufferPtr result;
    Buffer read_buffer(constant::BUFFER_SIZE);

    for (;;) {
        auto read_buffer_size = ValueConvertUtils::SafeConvert<int>(read_buffer.size());
        auto bytes_read = BIO_read(base64_bio, read_buffer.data(), read_buffer_size);
        if (bytes_read == 0 || bytes_read == -1) {
            break;
        }

        if (bytes_read == -2) {
            throw IOErrorException("Could not read data");
        }

        // Assume the byte count is positive
        assert(bytes_read > 0);

        // Also not bigger than buffer size
        auto buffer_size_converted = ValueConvertUtils::SafeConvert<decltype(bytes_read)>(read_buffer.size());
        assert(bytes_read <= buffer_size_converted); UNUSED(buffer_size_converted);

        result->insert(result->end(), read_buffer.begin(), read_buffer.begin() + bytes_read);
    }

    return result;
}

BufferPtr MiscUtils::CalculateHash(const Buffer& data, MessageDigestAlgorithm digest_algorithm) {
    auto memory_bio = BIO_new(BIO_s_mem());
    if (memory_bio == nullptr) {
        throw IOErrorException("Could not create memory buffer");
    }

    SCOPE_GUARD([memory_bio]() { BIO_free(memory_bio); });

    auto md_bio = BIO_new(BIO_f_md());
    if (md_bio == nullptr) {
        throw IOErrorException("Could not create hash filter");
    }

    SCOPE_GUARD([md_bio]() { BIO_free(md_bio); });

    auto algorithm = CryptoUtils::GetAlgorithm(digest_algorithm);
    auto md_set = BIO_set_md(md_bio, algorithm);
    if (md_set != 1) {
        throw IOErrorException("Could not set digest algorithm");
    }

    // Insert md into bio chain
    BIO_push(md_bio, memory_bio);

    auto data_size = ValueConvertUtils::SafeConvert<int>(data.size());
    auto bytes_written = BIO_write(memory_bio, data.data(), data_size);
    if (bytes_written <= 0) {
        throw IOErrorException("");
    }

    auto flushed = BIO_flush(memory_bio);
    if (flushed != 1) {
        throw IOErrorException("Could not flush buffer");
    }

    BufferPtr hash_buffer = make_deferred_container<Buffer>(EVP_MAX_MD_SIZE);

    auto hash_buffer_size = ValueConvertUtils::SafeConvert<int>(hash_buffer->size());
    auto bytes_read = BIO_gets(md_bio, hash_buffer->data(), hash_buffer_size);
    if (bytes_read <= 0) {
        throw IOErrorException("Could not calculate hash");
    }

    return hash_buffer;
}

std::string MiscUtils::ExtractFilename(const std::string& path) {
    auto pos = path.rfind('\\');
    if (pos == std::string::npos) {
        pos = path.rfind('/');
        if (pos == std::string::npos) {
            return path;
        }
    }

    return std::string(path.begin() + (pos + 1), path.end());
}

bool MiscUtils::CaseInsensitiveCompare(const std::string& left, const std::string& right) {
    if (left.size() != right.size()) {
        return false;
    }

    auto comparison_predicate = [](unsigned char left, unsigned char right) {
        return std::toupper(left) == std::toupper(right);
    };

    return std::equal(left.begin(), left.end(), right.begin(), comparison_predicate);
}

} // vanillapdf
