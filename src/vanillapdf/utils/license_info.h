#ifndef _LICENSE_INFO_H
#define _LICENSE_INFO_H

#include "utils/buffer.h"
#include "utils/message_digest_algorithm.h"
#include "utils/streams/input_stream_interface.h"

#include <string>

namespace vanillapdf {

class LicenseInfo {
public:
	static void SetLicense(IInputStreamPtr stream, types::stream_size length);
	static void SetLicense(const Buffer& data);
	static void SetLicense(const char * filename);

	static bool IsValid() noexcept;

private:
	LicenseInfo() = default;

	static bool CheckBlacklist(const std::string& serial);
	static bool CheckExpiration(const std::string& expiration);
	static bool CheckSignature(
		const std::string& signed_content,
		const std::string& signature_value,
		const std::string& signing_certificate,
		MessageDigestAlgorithm digest_algorithm);

	static bool CheckCertificateChain(const std::vector<std::string>& certificates);

	static bool m_valid;
};

} // vanillapdf

#endif /* _LICENSE_INFO_H */
