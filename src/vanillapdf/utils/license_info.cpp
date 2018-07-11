#include "precompiled.h"

#include "utils/resource.h"
#include "utils/misc_utils.h"
#include "utils/library_info.h"
#include "utils/license_info.h"
#include "utils/streams/input_stream.h"

#include <nlohmann_json/json.hpp>

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#include <chrono>
#include <fstream>

namespace vanillapdf {

using namespace nlohmann;

static const char VERSION_NODE[] =				"version";
static const char DATA_NODE[] =					"data";
static const char OWNER_NODE[] =				"owner";
static const char NOTE_NODE[] =					"note";
static const char SERIAL_NODE[] =				"serial";
static const char UPDATES_EXPIRATION_NODE[] =	"updates_expiration";
static const char TEMPORARY_EXPIRATION_NODE[] =	"temporary_expiration";
static const char SIGNATURE_NODE[] =			"signature";
static const char CERTIFICATES_NODE[] =			"certificates";

// Initialize static members
bool LicenseInfo::m_update_valid = false;
std::string LicenseInfo::m_temporary_expiration;

static X509* LoadCertificate(const std::string& certificate) {
	auto signing_certificate_bio = BIO_new(BIO_s_mem());
	if (signing_certificate_bio == nullptr) {
		throw GeneralException("Could not create memory buffer");
	}

	SCOPE_GUARD([signing_certificate_bio]() { BIO_free(signing_certificate_bio); });

	auto signing_certificate_size = ValueConvertUtils::SafeConvert<int>(certificate.size());
	auto bytes_written = BIO_write(signing_certificate_bio, certificate.data(), signing_certificate_size);
	if (bytes_written <= 0) {
		throw GeneralException("Could not write certificate data");
	}

	auto certificate_x509 = PEM_read_bio_X509(signing_certificate_bio, nullptr, nullptr, nullptr);
	if (certificate_x509 == nullptr) {
		throw GeneralException("Could not read PEM certificate");
	}

	return certificate_x509;
}

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

	// Will be used in the future
	UNUSED(version_minor);

	// Version format 1
	if (version_major == 1) {
		auto data_node = json_data[DATA_NODE];
		auto owner_node = data_node[OWNER_NODE];
		auto note_node = data_node[NOTE_NODE];
		auto serial_node = data_node[SERIAL_NODE];
		auto updates_expiration_node = data_node[UPDATES_EXPIRATION_NODE];
		auto temporary_expiration_node = data_node[TEMPORARY_EXPIRATION_NODE];

		if (!owner_node.is_string() ||
			!note_node.is_string() ||
			!serial_node.is_string() ||
			!updates_expiration_node.is_string()) {
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

		// Check updates expiration
		auto updates_expiration_string = updates_expiration_node.get<std::string>();
		bool updates_expired = CheckUpdateExpiration(updates_expiration_string);
		if (updates_expired) {
			std::stringstream msg;
			msg << "Your license updates expired on " << updates_expiration_string;
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
		signed_content << note_node.get<std::string>();
		signed_content << serial_node.get<std::string>();
		signed_content << updates_expiration_node.get<std::string>();

		if (temporary_expiration_node.is_string()) {
			signed_content << temporary_expiration_node.get<std::string>();
		}

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

		m_temporary_expiration.clear();
		if (temporary_expiration_node.is_string()) {
			m_temporary_expiration = temporary_expiration_node.get<std::string>();
		}

		// Successfully validated
		m_update_valid = true;
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

bool LicenseInfo::IsValid() {
	if (!m_update_valid) {
		return false;
	}

	if (!m_temporary_expiration.empty()) {
		bool temporary_expired = CheckTemporaryExpiration(m_temporary_expiration);
		if (temporary_expired) {
			return false;
		}
	}

	return true;
}

bool LicenseInfo::IsTemporary() {
	return !m_temporary_expiration.empty();
}

bool LicenseInfo::CheckBlacklist(const std::string& serial) {

	// Serial blacklist is embedded in the library resources
	auto serial_blacklist_raw = Resource::Load(ResourceID::SERIAL_BLACKLIST);

	// Parse list of blacklisted serials
	auto serial_blacklist_json = json::parse(serial_blacklist_raw);
	auto serial_blacklist = serial_blacklist_json.get<std::vector<std::string>>();

	// Check if current serial appears within the list
	for (auto& blacklisted_serial : serial_blacklist) {
		if (MiscUtils::CaseInsensitiveCompare(serial, blacklisted_serial)) {
			return true;
		}
	}

	// Presented serial does not appear on blacklist
	return false;
}

bool LicenseInfo::CheckSignature(
	const std::string& signed_content,
	const std::string& signature_value,
	const std::string& signing_certificate,
	MessageDigestAlgorithm digest_algorithm) {

	auto signing_certificate_x509 = LoadCertificate(signing_certificate);

	SCOPE_GUARD([signing_certificate_x509]() { X509_free(signing_certificate_x509); });

	auto signing_certificate_x509_pubkey = X509_get_pubkey(signing_certificate_x509);
	if (signing_certificate_x509_pubkey == nullptr) {
		return false;
	}

	SCOPE_GUARD([signing_certificate_x509_pubkey]() { EVP_PKEY_free(signing_certificate_x509_pubkey); });

	auto digest_context = EVP_MD_CTX_create();
	if (digest_context == nullptr) {
		return false;
	}

	SCOPE_GUARD([digest_context]() { EVP_MD_CTX_destroy(digest_context); });

	auto algorithm = MiscUtils::GetAlgorithm(digest_algorithm);

	auto initialized = EVP_DigestVerifyInit(digest_context, nullptr, algorithm, nullptr, signing_certificate_x509_pubkey);
	if (initialized != 1) {
		return false;
	}

	auto updated = EVP_DigestVerifyUpdate(digest_context, signed_content.data(), signed_content.size());
	if (updated != 1) {
		return false;
	}

	// Signature value if base 64 encoded
	auto signature_value_buffer = make_deferred_container<Buffer>(signature_value);
	auto signature_value_decoded = MiscUtils::FromBase64(signature_value_buffer);
	auto signature_value_decoded_ptr = reinterpret_cast<unsigned char *>(signature_value_decoded->data());

	auto finalized = EVP_DigestVerifyFinal(digest_context, signature_value_decoded_ptr, signature_value_decoded->size());
	if (finalized != 1) {
		return false;
	}

	return true;
}

bool LicenseInfo::CheckTemporaryExpiration(const std::string& expiration) {
	std::stringstream expiration_stream;
	expiration_stream << expiration;

	tm expiration_tm = { 0 };
	expiration_stream >> std::get_time(&expiration_tm, "%Y-%m-%d");

	auto expiration_since_epoch = std::mktime(&expiration_tm);
	if (expiration_since_epoch == -1) {
		throw GeneralException("Could not interpret expiration time as valid date time");
	}

	auto expiration_time = std::chrono::system_clock::from_time_t(expiration_since_epoch);
	auto current_time = std::chrono::system_clock::now();

	// Returns true if already expired
	return (expiration_time < current_time);
}

bool LicenseInfo::CheckUpdateExpiration(const std::string& expiration) {

	std::stringstream expiration_stream;
	expiration_stream << expiration;

	tm expiration_tm = { 0 };
	expiration_stream >> std::get_time(&expiration_tm, "%Y-%m-%d");

	auto expiration_since_epoch = std::mktime(&expiration_tm);
	if (expiration_since_epoch == -1) {
		throw GeneralException("Could not interpret expiration time as valid date time");
	}

	auto expiration_time = std::chrono::system_clock::from_time_t(expiration_since_epoch);

	auto build_year = LibraryInfo::BuildYear();
	auto build_month = LibraryInfo::BuildMonth();
	auto build_day = LibraryInfo::BuildDay();

	tm build_tm = { 0 };
	build_tm.tm_year = build_year - 1900;
	build_tm.tm_mon = build_month - 1;
	build_tm.tm_mday = build_day;

	auto build_since_epoch = std::mktime(&build_tm);
	if (build_since_epoch == -1) {
		throw GeneralException("Could not interpret build time as valid date time");
	}

	auto build_time = std::chrono::system_clock::from_time_t(build_since_epoch);

	// Returns true if already expired
	return (expiration_time < build_time);
}

bool LicenseInfo::CheckCertificateChain(const std::vector<std::string>& certificates) {

	// Initialize algorithms is required before validating certificates
	MiscUtils::InitializeOpenSSL();

	// Create trusted certificate store
	auto store = X509_STORE_new();
	if (store == nullptr) {
		return false;
	}

	SCOPE_GUARD([store]() { X509_STORE_free(store); });

	auto store_ctx = X509_STORE_CTX_new();
	if (store_ctx == nullptr) {
		return false;
	}

	SCOPE_GUARD([store_ctx]() { X509_STORE_CTX_free(store_ctx); });

	// Signing certificate is the first one in the chain
	auto signing_certificate_string = certificates[0];

	// Master certificate is embedded in the library resources
	auto master_certificate_buffer = Resource::Load(ResourceID::MASTER_CERTIFICATE);

	auto signing_certificate_x509 = LoadCertificate(signing_certificate_string);
	SCOPE_GUARD([signing_certificate_x509]() { X509_free(signing_certificate_x509); });

	auto master_certificate_x509 = LoadCertificate(master_certificate_buffer->ToString());
	SCOPE_GUARD([master_certificate_x509]() { X509_free(master_certificate_x509); });

	// Add master certificate to the trusted store
	auto master_certififcate_added = X509_STORE_add_cert(store, master_certificate_x509);
	if (master_certififcate_added != 1) {
		return false;
	}

	auto certificate_chain = sk_X509_new_null();
	if (certificate_chain == nullptr) {
		return false;
	}

	SCOPE_GUARD([certificate_chain]() {
		auto length = sk_X509_num(certificate_chain);
		for (decltype(length) i = 0; i < length; ++i) {
			auto certificate_x509 = sk_X509_value(certificate_chain, i);

			assert(certificate_x509 != nullptr && "Is this a bug in openssl?");
			if (certificate_x509 == nullptr) {
				continue;
			}

			X509_free(certificate_x509);
		}

		sk_X509_free(certificate_chain);
	});

	// Add other intermediate certificates to the validation chain
	for (auto& certificate_string : certificates) {
		auto certificate_x509 = LoadCertificate(certificate_string);

		auto certififcate_added = sk_X509_push(certificate_chain, certificate_x509);
		if (certififcate_added == 0) {
			return false;
		}
	}

	auto initialized = X509_STORE_CTX_init(store_ctx, store, signing_certificate_x509, certificate_chain);
	if (initialized != 1) {
		return false;
	}

	// Perform the certificate validation
	auto verified = X509_verify_cert(store_ctx);
	if (verified != 1) {
		return false;
	}

	// The signing certificate is valid and was issued with the master certificate
	return true;
}

} // vanillapdf
