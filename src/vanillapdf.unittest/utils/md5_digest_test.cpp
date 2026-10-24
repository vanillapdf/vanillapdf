#include "unittest.h"
#include "handle_guard.h"

#include <cstdint>
#include <string>
#include <vector>

namespace md5_digest {

using BufferGuard = HandleGuard<BufferHandle, Buffer_Release>;
using MD5DigestGuard = HandleGuard<MD5DigestHandle, MD5Digest_Release>;

static std::string ToHex(BufferHandle* buffer) {
    string_type data = nullptr;
    size_type size = 0;
    EXPECT_EQ(Buffer_GetData(buffer, &data, &size), VANILLAPDF_ERROR_SUCCESS);

    static const char HEX_DIGITS[] = "0123456789abcdef";

    std::string result;
    for (size_type i = 0; i < size; ++i) {
        auto byte = static_cast<uint8_t>(data[i]);
        result.push_back(HEX_DIGITS[byte >> 4]);
        result.push_back(HEX_DIGITS[byte & 0x0F]);
    }

    return result;
}

static void Update(MD5DigestHandle* digest, const std::string& data) {
    BufferGuard buffer;
    ASSERT_EQ(Buffer_CreateFromData(data.data(), static_cast<size_type>(data.size()), buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(MD5Digest_Update(digest, buffer), VANILLAPDF_ERROR_SUCCESS);
}

static std::string Final(MD5DigestHandle* digest) {
    BufferGuard result;
    EXPECT_EQ(MD5Digest_Final(digest, result.out()), VANILLAPDF_ERROR_SUCCESS);
    return ToHex(result);
}

static std::string Compute(const std::string& data) {
    MD5DigestGuard digest;
    EXPECT_EQ(MD5Digest_Create(digest.out()), VANILLAPDF_ERROR_SUCCESS);

    Update(digest, data);
    return Final(digest);
}

// Deterministic input whose length crosses the padding boundaries
static std::string Pattern(size_t length) {
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        result.push_back(static_cast<char>(i % 251));
    }

    return result;
}

// ===== RFC 1321 test suite =====

TEST(MD5Digest, MatchesRFC1321TestSuite) {
    EXPECT_EQ(Compute(""), "d41d8cd98f00b204e9800998ecf8427e");
    EXPECT_EQ(Compute("a"), "0cc175b9c0f1b6a831c399e269772661");
    EXPECT_EQ(Compute("abc"), "900150983cd24fb0d6963f7d28e17f72");
    EXPECT_EQ(Compute("message digest"), "f96b697d7cb7938d525a2f31aaf161d0");
    EXPECT_EQ(Compute("abcdefghijklmnopqrstuvwxyz"), "c3fcd3d76192e4007dfb496cca67e13b");
    EXPECT_EQ(Compute("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), "d174ab98d277d9f5a5611c2c9f419d9f");
    EXPECT_EQ(Compute("12345678901234567890123456789012345678901234567890123456789012345678901234567890"), "57edf4a22be3c955ac49da2e2107b67a");
}

// ===== Padding boundaries =====

struct BoundaryVector {
    size_t length;
    const char* digest;
};

class MD5DigestBoundary : public ::testing::TestWithParam<BoundaryVector> {
};

TEST_P(MD5DigestBoundary, MatchesReference) {
    const auto& param = GetParam();
    EXPECT_EQ(Compute(Pattern(param.length)), param.digest);
}

// 55 bytes still fit the length into the last block, 56 to 63 need an extra block
INSTANTIATE_TEST_SUITE_P(Lengths, MD5DigestBoundary, ::testing::Values(
    BoundaryVector{ 55, "6912ee65fff2d9f9ce2508cddf8bcda0" },
    BoundaryVector{ 56, "51fdd1acda72405dfdfa03fcb85896d7" },
    BoundaryVector{ 57, "5320ef4c17ef34a0cf2db763338d25eb" },
    BoundaryVector{ 63, "48a6295221902e8e0938f773a7185e72" },
    BoundaryVector{ 64, "b2d3f56bc197fd985d5965079b5e7148" },
    BoundaryVector{ 65, "8bd7053801c768420faf816fadba971c" },
    BoundaryVector{ 119, "1c772251899a7ff007400b888d6b2042" },
    BoundaryVector{ 120, "b7ba1efc6022e9ed272f00b8831e26e6" },
    BoundaryVector{ 128, "37eff01866ba3f538421b30b7cbefcac" },
    BoundaryVector{ 1000, "a24f1e3ef66950e1327f210e3997ba2c" }
));

// ===== Behavior =====

TEST(MD5Digest, IncrementalUpdatesMatchSingleUpdate) {
    auto data = Pattern(200);
    auto expected = Compute(data);

    // Every split point, including empty parts at both ends
    for (size_t split = 0; split <= data.size(); ++split) {
        MD5DigestGuard digest;
        ASSERT_EQ(MD5Digest_Create(digest.out()), VANILLAPDF_ERROR_SUCCESS);

        Update(digest, data.substr(0, split));
        Update(digest, data.substr(split));
        EXPECT_EQ(Final(digest), expected) << "Split at " << split;
    }
}

TEST(MD5Digest, ByteByByteUpdatesMatchSingleUpdate) {
    auto data = Pattern(130);

    MD5DigestGuard digest;
    ASSERT_EQ(MD5Digest_Create(digest.out()), VANILLAPDF_ERROR_SUCCESS);

    for (char byte : data) {
        Update(digest, std::string(1, byte));
    }

    EXPECT_EQ(Final(digest), Compute(data));
}

TEST(MD5Digest, RejectsSecondFinal) {
    MD5DigestGuard digest;
    ASSERT_EQ(MD5Digest_Create(digest.out()), VANILLAPDF_ERROR_SUCCESS);
    Final(digest);

    BufferGuard result;
    EXPECT_EQ(MD5Digest_Final(digest, result.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(MD5Digest, RejectsUpdateAfterFinal) {
    MD5DigestGuard digest;
    ASSERT_EQ(MD5Digest_Create(digest.out()), VANILLAPDF_ERROR_SUCCESS);
    Final(digest);

    BufferGuard data;
    ASSERT_EQ(Buffer_CreateFromData("a", 1, data.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(MD5Digest_Update(digest, data), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(MD5Digest, NullParams) {
    EXPECT_EQ(MD5Digest_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    MD5DigestGuard digest;
    ASSERT_EQ(MD5Digest_Create(digest.out()), VANILLAPDF_ERROR_SUCCESS);

    BufferGuard data;
    ASSERT_EQ(Buffer_Create(data.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(MD5Digest_Update(nullptr, data), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(MD5Digest_Update(digest, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

    BufferGuard result;
    EXPECT_EQ(MD5Digest_Final(nullptr, result.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(MD5Digest_Final(digest, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(MD5Digest, UnknownRoundTrip) {
    MD5DigestGuard digest;
    ASSERT_EQ(MD5Digest_Create(digest.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(MD5Digest_ToUnknown(digest, unknown.out()), VANILLAPDF_ERROR_SUCCESS);

    MD5DigestGuard converted;
    ASSERT_EQ(MD5Digest_FromUnknown(unknown, converted.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(converted.get(), digest.get());
}

} // namespace md5_digest
