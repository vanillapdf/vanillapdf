#include "precompiled.h"

#include "utils/misc_utils.h"
#include "utils/license_info.h"
#include "utils/streams/input_stream.h"

#include <nlohmann_json/json.hpp>

#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <chrono>
#include <fstream>

namespace gotchangpdf {

using namespace nlohmann;

static const char VERSION_NODE[] =			"version";
static const char DATA_NODE[] =				"data";
static const char OWNER_NODE[] =			"owner";
static const char SERIAL_NODE[] =			"serial";
static const char EXPIRATION_NODE[] =		"expiration";
static const char SIGNATURE_NODE[] =		"signature";
static const char CERTIFICATES_NODE[] =		"certificates";

// Initialize static members
bool LicenseInfo::m_valid = false;

void LicenseInfo::SetLicense(IInputStreamPtr stream, types::stream_size length) {
	Buffer buffer(length);
	auto read = stream->Read(buffer, length);
	if (read != length) {
		throw GeneralException("Could not read license file");
	}

	// Forward content
	SetLicense(buffer);
}

void LicenseInfo::SetLicense(const Buffer& data) {

	// Parse JSON
	auto json_data = json::parse(data);

	// Check license version
	auto version_node = json_data[VERSION_NODE];
	if (!version_node.is_string()) {
		throw InvalidLicenseException("Missing license version");
	}

	// Check license version format
	auto version_string = version_node.get<std::string>();
	auto delimiter_offset = version_string.find('.');
	if (delimiter_offset == std::string::npos) {
		throw InvalidLicenseException("Invalid license version format");
	}

	// Obtain version details
	auto version_major_string = version_string.substr(0, delimiter_offset);
	auto version_minor_string = version_string.substr(delimiter_offset + 1);

	auto version_major = std::stoi(version_major_string);
	auto version_minor = std::stoi(version_minor_string);

	// Version format 1
	if (version_major == 1) {
		auto data_node = json_data[DATA_NODE];
		auto owner_node = data_node[OWNER_NODE];
		auto serial_node = data_node[SERIAL_NODE];
		auto expiration_node = data_node[EXPIRATION_NODE];

		if (!owner_node.is_string() ||
			!serial_node.is_string() ||
			!expiration_node.is_string()) {
			throw InvalidLicenseException("Invalid license data format");
		}

		// Check serial on blacklist
		auto serial_string = serial_node.get<std::string>();
		bool blacklisted = CheckBlacklist(serial_string);
		if (blacklisted) {
			std::stringstream msg;
			msg << "Your license with serial " << serial_string << " is blacklisted";
			throw InvalidLicenseException(msg.str());
		}

		// Check expiration
		auto expiration_string = expiration_node.get<std::string>();
		bool expired = CheckExpiration(expiration_string);
		if (expired) {
			std::stringstream msg;
			msg << "Your license expired on " << expiration_string;
			throw InvalidLicenseException(msg.str());
		}

		// Check signature
		auto signature_node = json_data[SIGNATURE_NODE];
		auto certificates_node = json_data[CERTIFICATES_NODE];

		if (!signature_node.is_string() ||
			!certificates_node.is_array() ||
			certificates_node.size() == 0) {
			throw InvalidLicenseException("Invalid license signature format");
		}

		std::stringstream signed_content;
		signed_content << owner_node.get<std::string>();
		signed_content << serial_node.get<std::string>();
		signed_content << expiration_node.get<std::string>();

		auto signature_value = signature_node.get<std::string>();
		auto signing_certificate = certificates_node[0].get<std::string>();
		bool signature_valid = CheckSignature(signed_content.str(), signature_value, signing_certificate, MessageDigestAlgorithm::SHA256);

		// Could not verify license signature
		if (!signature_valid) {
			throw InvalidLicenseException("Invalid license signature");
		}

		// Verify certificate
		std::vector<std::string> certificates = certificates_node.get<std::vector<std::string>>();
		bool chain_valid = CheckCertificateChain(certificates);

		// Invalid certificate chain
		if (!chain_valid) {
			throw InvalidLicenseException("Invalid certificate chain");
		}

		// Successfully validated
		m_valid = true;
		return;
	}

	// None of the known license format matches
	throw GeneralException("Unknown license version");
}

void LicenseInfo::SetLicense(const char * filename) {

	// Determine file size
	auto file = std::make_shared<std::ifstream>(filename, std::ios::binary | std::ios::ate);
	auto length = file->tellg();

	// Seek back to the beginning
	file->seekg(0, std::ios::beg);

	// Forward content
	auto stream = make_deferred<InputStream>(file);
	SetLicense(stream, length);
}

bool LicenseInfo::IsValid() noexcept {
	return m_valid;
}

bool LicenseInfo::CheckBlacklist(const std::string& serial) {
	// TODO check blacklist

	return false;
}

bool LicenseInfo::CheckSignature(
	const std::string& signed_content,
	const std::string& signature_value,
	const std::string& signing_certificate,
	MessageDigestAlgorithm digest_algorithm) {

	auto signing_certificate_bio = BIO_new(BIO_s_mem());
	if (signing_certificate_bio == nullptr) {
		return false;
	}

	SCOPE_GUARD([signing_certificate_bio]() { BIO_free(signing_certificate_bio); });

	auto bytes_written = BIO_write(signing_certificate_bio, signing_certificate.data(), signing_certificate.size());
	if (bytes_written <= 0) {
		return false;
	}

	auto signing_certificate_x509 = PEM_read_bio_X509(signing_certificate_bio, NULL, NULL, NULL);
	if (signing_certificate_x509 == nullptr) {
		return false;
	}

	SCOPE_GUARD([signing_certificate_x509]() { X509_free(signing_certificate_x509); });

	auto signing_certificate_x509_pubkey = X509_get_pubkey(signing_certificate_x509);
	if (signing_certificate_x509_pubkey == nullptr) {
		return false;
	}

	SCOPE_GUARD([signing_certificate_x509_pubkey]() { EVP_PKEY_free(signing_certificate_x509_pubkey); });

	auto signing_certificate_x509_rsa_pubkey = EVP_PKEY_get1_RSA(signing_certificate_x509_pubkey);
	if (signing_certificate_x509_rsa_pubkey == nullptr) {
		return false;
	}

	SCOPE_GUARD([signing_certificate_x509_rsa_pubkey]() { RSA_free(signing_certificate_x509_rsa_pubkey); });

	EVP_MD_CTX mdctx = {0};
	auto algorithm = MiscUtils::GetAlgorithm(digest_algorithm);

	auto initialized = EVP_DigestVerifyInit(&mdctx, nullptr, algorithm, nullptr, signing_certificate_x509_pubkey);
	if (initialized != 1) {
		return false;
	}

	auto updated = EVP_DigestVerifyUpdate(&mdctx, signed_content.data(), signed_content.size());
	if (updated != 1) {
		return false;
	}

	// Signature value if base 64 encoded
	auto signature_value_decoded = MiscUtils::FromBase64(signature_value);
	auto signature_value_decoded_ptr = reinterpret_cast<const unsigned char *>(signature_value_decoded->data());

	auto finalized = EVP_DigestVerifyFinal(&mdctx, signature_value_decoded_ptr, signature_value_decoded->size());
	if (finalized != 1) {
		return false;
	}

	return true;
}

bool LicenseInfo::CheckExpiration(const std::string& expiration) {

	std::stringstream expiration_stream;
	expiration_stream << expiration;

	std::tm expiration_tm = { 0 };
	expiration_stream >> std::get_time(&expiration_tm, "%Y-%m-%d %H:%M:%SZ");

	auto expiration_since_epoch = std::mktime(&expiration_tm);
	auto expiration_time = std::chrono::system_clock::from_time_t(expiration_since_epoch);

	// Returns true if already expired
	return (expiration_time < std::chrono::system_clock::now());
}

bool LicenseInfo::CheckCertificateChain(const std::vector<std::string>& certificates) {
	// TODO check chain

	return true;
}

} // gotchangpdf
