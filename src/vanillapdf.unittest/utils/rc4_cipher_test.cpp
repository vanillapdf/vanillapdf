#include "unittest.h"
#include "handle_guard.h"

#include <cstdint>
#include <string>
#include <vector>

namespace rc4_cipher {

using BufferGuard = HandleGuard<BufferHandle, Buffer_Release>;
using RC4CipherGuard = HandleGuard<RC4CipherHandle, RC4Cipher_Release>;

static std::vector<uint8_t> FromHex(const std::string& hex) {
    std::vector<uint8_t> result;
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        result.push_back(static_cast<uint8_t>(std::stoul(hex.substr(i, 2), nullptr, 16)));
    }

    return result;
}

static void CreateBuffer(const std::vector<uint8_t>& data, BufferGuard& result) {
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(data.data()),
        static_cast<size_type>(data.size()), result.out()), VANILLAPDF_ERROR_SUCCESS);
}

static std::vector<uint8_t> GetBytes(BufferHandle* buffer) {
    string_type data = nullptr;
    size_type size = 0;
    EXPECT_EQ(Buffer_GetData(buffer, &data, &size), VANILLAPDF_ERROR_SUCCESS);

    auto bytes = reinterpret_cast<const uint8_t*>(data);
    return std::vector<uint8_t>(bytes, bytes + size);
}

static void CreateCipher(const std::vector<uint8_t>& key, RC4CipherGuard& result) {
    BufferGuard key_buffer;
    CreateBuffer(key, key_buffer);
    ASSERT_EQ(RC4Cipher_CreateFromKey(key_buffer, result.out()), VANILLAPDF_ERROR_SUCCESS);
}

static std::vector<uint8_t> Process(RC4CipherHandle* cipher, const std::vector<uint8_t>& data) {
    BufferGuard input;
    CreateBuffer(data, input);

    BufferGuard output;
    EXPECT_EQ(RC4Cipher_Process(cipher, input, output.out()), VANILLAPDF_ERROR_SUCCESS);
    return GetBytes(output);
}

// ===== RFC 6229 keystream vectors =====

struct KeystreamVector {
    const char* key;
    size_t offset;
    const char* keystream;
};

class RC4CipherKeystream : public ::testing::TestWithParam<KeystreamVector> {
};

TEST_P(RC4CipherKeystream, MatchesRFC6229) {
    const auto& param = GetParam();

    RC4CipherGuard cipher;
    CreateCipher(FromHex(param.key), cipher);

    // Encrypting zeroes yields the raw keystream
    auto expected = FromHex(param.keystream);
    auto keystream = Process(cipher, std::vector<uint8_t>(param.offset + expected.size(), 0));

    std::vector<uint8_t> actual(keystream.begin() + param.offset, keystream.end());
    EXPECT_EQ(actual, expected);
}

// Keys of the PDF range: 40-bit (revision 2) up to 128-bit (revisions 3 and 4)
INSTANTIATE_TEST_SUITE_P(RFC6229, RC4CipherKeystream, ::testing::Values(
    KeystreamVector{ "0102030405", 0, "b2396305f03dc027ccc3524a0a1118a8" },
    KeystreamVector{ "0102030405", 16, "6982944f18fc82d589c403a47a0d0919" },
    KeystreamVector{ "0102030405", 240, "28cb1132c96ce286421dcaadb8b69eae" },
    KeystreamVector{ "0102030405", 4096, "ff25b58995996707e51fbdf08b34d875" },
    KeystreamVector{ "01020304050607", 0, "293f02d47f37c9b633f2af5285feb46b" },
    KeystreamVector{ "0102030405060708", 0, "97ab8a1bf0afb96132f2f67258da15a8" },
    KeystreamVector{ "0102030405060708090a", 0, "ede3b04643e586cc907dc21851709902" },
    KeystreamVector{ "0102030405060708090a0b0c0d0e0f10", 0, "9ac7cc9a609d1ef7b2932899cde41b97" },
    KeystreamVector{ "0102030405060708090a0b0c0d0e0f10", 16, "5248c4959014126a6e8a84f11d1a9e1c" },
    KeystreamVector{ "0102030405060708090a0b0c0d0e0f10", 240, "065902e4b620f6cc36c8589f66432f2b" },
    KeystreamVector{ "0102030405060708090a0b0c0d0e0f10", 4096, "a36a4c301ae8ac13610ccbc12256cacc" }
));

// ===== Plaintext vectors =====

TEST(RC4Cipher, EncryptsKnownPlaintexts) {
    struct PlaintextVector {
        const char* key;
        const char* plaintext;
        const char* ciphertext;
    };

    const PlaintextVector vectors[] = {
        { "Key", "Plaintext", "bbf316e8d940af0ad3" },
        { "Wiki", "pedia", "1021bf0420" },
        { "Secret", "Attack at dawn", "45a01f645fc35b383552544b9bf5" }
    };

    for (const auto& vector : vectors) {
        std::string key(vector.key);
        std::string plaintext(vector.plaintext);

        RC4CipherGuard cipher;
        CreateCipher(std::vector<uint8_t>(key.begin(), key.end()), cipher);

        auto ciphertext = Process(cipher, std::vector<uint8_t>(plaintext.begin(), plaintext.end()));
        EXPECT_EQ(ciphertext, FromHex(vector.ciphertext)) << "Key: " << vector.key;
    }
}

// ===== Behavior =====

TEST(RC4Cipher, DecryptsItsOwnOutput) {
    auto key = FromHex("0102030405");
    std::vector<uint8_t> plaintext = { 'v', 'a', 'n', 'i', 'l', 'l', 'a', 'p', 'd', 'f' };

    RC4CipherGuard encryptor;
    CreateCipher(key, encryptor);
    auto ciphertext = Process(encryptor, plaintext);
    EXPECT_NE(ciphertext, plaintext);

    RC4CipherGuard decryptor;
    CreateCipher(key, decryptor);
    EXPECT_EQ(Process(decryptor, ciphertext), plaintext);
}

TEST(RC4Cipher, KeystreamContinuesAcrossCalls) {
    auto key = FromHex("0102030405060708090a0b0c0d0e0f10");
    std::vector<uint8_t> data(300, 0x5A);

    RC4CipherGuard whole_cipher;
    CreateCipher(key, whole_cipher);
    auto whole = Process(whole_cipher, data);

    // Uneven parts, including an empty one
    RC4CipherGuard split_cipher;
    CreateCipher(key, split_cipher);

    std::vector<uint8_t> split;
    for (size_t part_length : { 1u, 0u, 15u, 64u, 220u }) {
        auto part = Process(split_cipher, std::vector<uint8_t>(part_length, 0x5A));
        split.insert(split.end(), part.begin(), part.end());
    }

    EXPECT_EQ(split, whole);
}

TEST(RC4Cipher, EmptyDataYieldsEmptyOutput) {
    RC4CipherGuard cipher;
    CreateCipher(FromHex("0102030405"), cipher);
    EXPECT_TRUE(Process(cipher, std::vector<uint8_t>()).empty());
}

TEST(RC4Cipher, AcceptsKeyLengthBounds) {
    RC4CipherGuard shortest;
    CreateCipher(std::vector<uint8_t>(1, 0x01), shortest);

    RC4CipherGuard longest;
    CreateCipher(std::vector<uint8_t>(256, 0x01), longest);
}

TEST(RC4Cipher, RejectsEmptyKey) {
    BufferGuard key;
    ASSERT_EQ(Buffer_Create(key.out()), VANILLAPDF_ERROR_SUCCESS);

    RC4CipherGuard cipher;
    EXPECT_EQ(RC4Cipher_CreateFromKey(key, cipher.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(RC4Cipher, RejectsOverlongKey) {
    BufferGuard key;
    CreateBuffer(std::vector<uint8_t>(257, 0x01), key);

    RC4CipherGuard cipher;
    EXPECT_EQ(RC4Cipher_CreateFromKey(key, cipher.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(RC4Cipher, NullParams) {
    BufferGuard key;
    CreateBuffer(FromHex("0102030405"), key);

    RC4CipherGuard cipher;
    EXPECT_EQ(RC4Cipher_CreateFromKey(nullptr, cipher.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(RC4Cipher_CreateFromKey(key, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    ASSERT_EQ(RC4Cipher_CreateFromKey(key, cipher.out()), VANILLAPDF_ERROR_SUCCESS);

    BufferGuard output;
    EXPECT_EQ(RC4Cipher_Process(nullptr, key, output.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(RC4Cipher_Process(cipher, nullptr, output.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(RC4Cipher_Process(cipher, key, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(RC4Cipher, UnknownRoundTrip) {
    RC4CipherGuard cipher;
    CreateCipher(FromHex("0102030405"), cipher);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(RC4Cipher_ToUnknown(cipher, unknown.out()), VANILLAPDF_ERROR_SUCCESS);

    RC4CipherGuard converted;
    ASSERT_EQ(RC4Cipher_FromUnknown(unknown, converted.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(converted.get(), cipher.get());
}

} // namespace rc4_cipher
