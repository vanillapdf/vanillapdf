# CRL/OCSP Certificate Revocation Checking - Design Document

**Status**: Planned
**Prerequisites**: [Signature Verification](signature-verification.md) (complete)
**GitHub Issue**: [#157](https://github.com/vanillapdf/vanillapdf/issues/157)

## Executive Summary

This document provides a complete architectural design and implementation specification for adding Certificate Revocation List (CRL) and Online Certificate Status Protocol (OCSP) checking to vanillapdf's signature verification system. The design emphasizes security, performance, reliability, and standards compliance while maintaining backward compatibility with existing APIs.

## Table of Contents

1. [Current Architecture Analysis](#current-architecture-analysis)
2. [Design Requirements and Constraints](#design-requirements-and-constraints)
3. [Proposed Architecture](#proposed-architecture)
4. [Detailed Component Design](#detailed-component-design)
5. [Implementation Roadmap](#implementation-roadmap)
6. [Performance and Optimization](#performance-and-optimization)
7. [Security Analysis](#security-analysis)
8. [Testing Strategy](#testing-strategy)
9. [Migration and Backward Compatibility](#migration-and-backward-compatibility)
10. [Future Enhancements](#future-enhancements)

## Current Architecture Analysis

### SignatureVerifier Processing Pipeline

The current signature verification flow (`signature_verifier.cpp`) follows a linear pipeline:

```cpp
SignatureVerificationResult* Verify(data, signature, trust_store, settings) {
    // Stage 1: PKCS#7 Parsing
    PKCS7* p7 = d2i_PKCS7(signature);  // Parse signature blob

    // Stage 2: Certificate Extraction
    ExtractCertificateChain(p7, result);  // Extract certs, CN, chain

    // Stage 3: Cryptographic Verification
    PKCS7_verify(p7, nullptr, nullptr, data_bio, nullptr,
                 PKCS7_DETACHED | PKCS7_NOVERIFY);  // Verify signature

    // Stage 4: Chain Validation
    VerifyCertificateChain(p7, store, result, settings);  // X509_verify_cert

    // Stage 5: Algorithm Strength Check
    IsWeakAlgorithm(p7, result);  // Check MD5, SHA1, RSA<2048

    // Stage 6: Result Assembly
    return result;  // Status, flags, message, chain
}
```

**Integration Point**: Revocation checking should be inserted between stages 4 and 5, after successful chain validation but before algorithm checks.

### SignatureVerificationSettings State

Current configuration flags:
- `m_check_revocation` (bool): Placeholder for CRL/OCSP, currently unused
- `m_skip_certificate_validation` (bool): Bypass X509 chain validation
- `m_check_signing_time` (bool): Validate cert at signing time
- `m_allow_weak_algorithms` (bool): Allow MD5, SHA-1, RSA < 2048

### TrustedCertificateStore Implementation

The store wraps OpenSSL's `X509_STORE`:
- `AddCertificateFromPEM/DER()`: Add trusted roots
- `LoadFromDirectory()`: Load cert directory
- `LoadSystemDefaults()`: Platform-specific system roots
- `GetNativeHandle()`: Returns `X509_STORE*` for OpenSSL operations

## Design Requirements and Constraints

### Functional Requirements

1. **CRL Support**
   - Download and parse CRLs from HTTP/HTTPS URLs
   - Validate CRL signatures and validity periods
   - Support multiple CRL distribution points
   - Handle indirect CRLs and delta CRLs
   - Respect CRL reason codes

2. **OCSP Support**
   - Send OCSP requests to responder URLs
   - Validate OCSP response signatures
   - Support OCSP nonces for replay protection
   - Handle delegated OCSP responders
   - Process multi-certificate OCSP responses

3. **Fallback and Resilience**
   - OCSP -> CRL fallback on failure
   - CRL -> OCSP fallback (configurable)
   - Soft-fail and hard-fail modes
   - Network timeout handling
   - Retry logic with exponential backoff

4. **Performance Requirements**
   - Sub-second verification for cached responses
   - Parallel revocation checks for certificate chains
   - Async/non-blocking operation mode
   - Memory-efficient caching

### Non-Functional Requirements

1. **Security**
   - No MITM vulnerability in revocation checks
   - Protect against cache poisoning
   - Validate all cryptographic signatures
   - Support OCSP stapling

2. **Compatibility**
   - OpenSSL 1.1.1+ support
   - Optional libcurl integration
   - Cross-platform (Windows, Linux, macOS)
   - C++17 standard compliance

3. **Configurability**
   - Per-verification settings override
   - Global default configuration
   - Environment variable support
   - Configuration file support

## Proposed Architecture

### High-Level Component Architecture

```
+----------------------------------------------------------------------+
|                          SignatureVerifier                            |
+------------------------------------------------------------------------+
|                                                                        |
|  +--------------+    +------------------+    +------------------+     |
|  |    Parse     |--->|     Verify       |--->|    Validate      |     |
|  |   PKCS#7     |    |   Signature      |    |     Chain        |     |
|  +--------------+    +------------------+    +------------------+     |
|                                                         |             |
|                                                         v             |
|  +--------------------------------------------------------------+    |
|  |                  RevocationManager                           |    |
|  +--------------------------------------------------------------+    |
|  |  +---------+  +---------+  +----------+  +--------------+    |    |
|  |  | Policy  |  | Router  |  | Parallel |  |   Fallback   |    |    |
|  |  | Engine  |--|  Logic  |--| Executor |--|   Handler    |    |    |
|  |  +---------+  +---------+  +----------+  +--------------+    |    |
|  +------------------------+-------------------------------------+    |
|                           |                                           |
+---------------------------+-------------------------------------------+
                            |
    +-----------------------+------------------------+
    |                       |                        |
+---v----------+    +-------v---------+    +--------v------+
| CrlProcessor |    | OcspProcessor   |    | StapledOcsp  |
+--------------+    +-----------------+    +---------------+
| - Parser     |    | - Request Gen   |    | - Extract     |
| - Validator  |    | - Response Parse|    | - Validate    |
| - Cache      |    | - Nonce Handler |    | - Cache       |
| - Downloader |    | - Cache         |    +---------------+
+--------------+    +-----------------+
        |                   |
        +---------+---------+
                  |
        +---------v---------+
        |   NetworkManager  |
        +-------------------+
        | - HTTP/HTTPS      |
        | - Connection Pool |
        | - Retry Logic     |
        | - Timeout Handler |
        +-------------------+
```

### Layered Architecture

```
+---------------------------------------------+
|            Application Layer                 |  <- CLI tools, API users
+---------------------------------------------+
|            C API Layer                       |  <- c_signature_verifier.h
+---------------------------------------------+
|         Signature Verification Layer         |  <- SignatureVerifier
+---------------------------------------------+
|         Revocation Checking Layer            |  <- RevocationManager (NEW)
+---------------------------------------------+
|         Protocol Implementation Layer        |  <- CRL/OCSP/Stapling (NEW)
+---------------------------------------------+
|            Network Layer                     |  <- HTTP/HTTPS client (NEW)
+---------------------------------------------+
|           Cryptographic Layer                |  <- OpenSSL wrappers
+---------------------------------------------+
|             Cache Layer                      |  <- Memory/Disk cache (NEW)
+---------------------------------------------+
```

## Detailed Component Design

### 1. RevocationManager (Central Coordinator)

```cpp
// File: src/vanillapdf/utils/revocation_manager.h

namespace vanillapdf {

/**
 * Central coordinator for all revocation checking operations.
 * Manages policy decisions, routing, parallelization, and fallback logic.
 */
class RevocationManager {
public:
    /**
     * Check revocation status for an entire certificate chain
     * @param chain Complete certificate chain to verify
     * @param store Trust store for validation
     * @param settings Verification settings and policy
     * @return Chain revocation result with per-certificate status
     */
    static ChainRevocationResult CheckChain(
        const std::vector<X509*>& chain,
        X509_STORE* store,
        SignatureVerificationSettingsPtr settings);

    /**
     * Check revocation for a single certificate
     * @param cert Certificate to check
     * @param issuer Issuer certificate
     * @param store Trust store
     * @param policy Revocation checking policy
     * @return Certificate revocation status with details
     */
    static CertificateRevocationResult CheckCertificate(
        X509* cert,
        X509* issuer,
        X509_STORE* store,
        const RevocationPolicy& policy);

private:
    /**
     * Execute parallel revocation checks
     * @param tasks Vector of revocation check tasks
     * @param timeout Maximum time for all checks
     * @return Vector of results matching task order
     */
    static std::vector<RevocationCheckResult> ExecuteParallel(
        const std::vector<RevocationTask>& tasks,
        std::chrono::milliseconds timeout);

    /**
     * Apply fallback strategy when primary check fails
     * @param primary_result Failed primary check result
     * @param cert Certificate being checked
     * @param issuer Issuer certificate
     * @param policy Current revocation policy
     * @return Result from fallback method or failure
     */
    static RevocationCheckResult ApplyFallback(
        const RevocationCheckResult& primary_result,
        X509* cert,
        X509* issuer,
        const RevocationPolicy& policy);

    /**
     * Route revocation check based on policy and certificate extensions
     * @param cert Certificate to route
     * @param policy Active revocation policy
     * @return Preferred checking method
     */
    static RevocationMethod DetermineMethod(
        X509* cert,
        const RevocationPolicy& policy);
};

/**
 * Revocation checking policy configuration
 */
struct RevocationPolicy {
    enum class Method {
        PreferOCSP,      // Try OCSP first, fall back to CRL
        PreferCRL,       // Try CRL first, fall back to OCSP
        OCSPOnly,        // Only use OCSP
        CRLOnly,         // Only use CRL
        RequireBoth,     // Both must succeed
        BestEffort       // Try all available methods
    };

    enum class FailureMode {
        HardFail,        // Treat check failures as revoked
        SoftFail,        // Treat check failures as unknown
        IgnoreFail       // Ignore check failures
    };

    Method method = Method::PreferOCSP;
    FailureMode failure_mode = FailureMode::SoftFail;
    bool check_entire_chain = true;
    bool allow_cached = true;
    bool require_nonce = false;
    std::chrono::milliseconds timeout{5000};
    int max_retries = 2;
    std::chrono::milliseconds retry_delay{1000};
};

/**
 * Result of chain revocation checking
 */
struct ChainRevocationResult {
    bool all_valid = false;
    std::vector<CertificateRevocationResult> certificate_results;
    std::chrono::milliseconds total_time;
    int checks_performed = 0;
    int checks_failed = 0;
};

/**
 * Detailed result for single certificate
 */
struct CertificateRevocationResult {
    RevocationStatus status = RevocationStatus::Unknown;
    RevocationMethod method_used = RevocationMethod::None;
    std::string failure_reason;
    std::chrono::system_clock::time_point check_time;
    std::chrono::milliseconds check_duration;
    bool from_cache = false;
    std::optional<std::chrono::system_clock::time_point> next_update;
    std::optional<CRLReason> revocation_reason;
    std::optional<std::chrono::system_clock::time_point> revocation_time;
};

} // namespace vanillapdf
```

### 2. CrlProcessor (CRL Implementation)

```cpp
// File: src/vanillapdf/utils/crl_processor.h

namespace vanillapdf {

/**
 * Complete CRL processing implementation including download,
 * parsing, validation, caching, and delta CRL support.
 */
class CrlProcessor {
public:
    /**
     * Main entry point for CRL checking
     */
    static RevocationCheckResult CheckRevocation(
        X509* cert,
        X509* issuer,
        X509_STORE* store,
        const CrlOptions& options);

    /**
     * Download CRL from URL with retry logic
     */
    static std::unique_ptr<CrlData> DownloadCrl(
        const std::string& url,
        const NetworkOptions& options);

    /**
     * Parse and validate CRL
     */
    static std::unique_ptr<ParsedCrl> ParseCrl(
        const CrlData& data,
        X509* expected_issuer);

    /**
     * Check if certificate is in CRL
     */
    static CrlEntryResult CheckCertificate(
        const ParsedCrl& crl,
        X509* cert);

private:
    /**
     * Extract all CRL distribution points from certificate
     */
    static std::vector<CrlDistributionPoint> ExtractDistributionPoints(X509* cert);

    /**
     * Apply delta CRL to base CRL
     */
    static std::unique_ptr<ParsedCrl> ApplyDeltaCrl(
        const ParsedCrl& base,
        const ParsedCrl& delta);

    /**
     * Validate CRL signature and dates
     */
    static bool ValidateCrl(
        X509_CRL* crl,
        X509* issuer,
        std::chrono::system_clock::time_point validation_time);

    /**
     * Handle indirect CRL (issued by different CA)
     */
    static bool ValidateIndirectCrl(
        X509_CRL* crl,
        X509_STORE* store);
};

/**
 * CRL distribution point information
 */
struct CrlDistributionPoint {
    enum class Type { FullName, RelativeName };
    Type type;
    std::vector<std::string> urls;
    std::optional<std::string> crl_issuer;
    std::bitset<9> reasons;  // Key compromise, CA compromise, etc.

    bool supports_delta() const;
    bool is_indirect() const;
};

/**
 * Parsed CRL with metadata
 */
struct ParsedCrl {
    X509_CRL* crl = nullptr;  // OpenSSL CRL object
    std::string issuer_dn;
    std::chrono::system_clock::time_point this_update;
    std::chrono::system_clock::time_point next_update;
    std::unordered_map<std::string, CrlEntry> revoked_serials;
    bool is_delta = false;
    std::optional<int64_t> crl_number;
    std::optional<int64_t> delta_crl_number;

    ~ParsedCrl() { if (crl) X509_CRL_free(crl); }
};

/**
 * Individual CRL entry
 */
struct CrlEntry {
    std::string serial_number;
    std::chrono::system_clock::time_point revocation_date;
    CRLReason reason = CRLReason::Unspecified;
    std::optional<std::chrono::system_clock::time_point> invalidity_date;
    std::optional<std::string> hold_instruction;  // For certificateHold
};

/**
 * CRL revocation reasons (RFC 5280)
 */
enum class CRLReason {
    Unspecified = 0,
    KeyCompromise = 1,
    CACompromise = 2,
    AffiliationChanged = 3,
    Superseded = 4,
    CessationOfOperation = 5,
    CertificateHold = 6,
    RemoveFromCRL = 8,
    PrivilegeWithdrawn = 9,
    AACompromise = 10
};

} // namespace vanillapdf
```

### 3. OcspProcessor (OCSP Implementation)

```cpp
// File: src/vanillapdf/utils/ocsp_processor.h

namespace vanillapdf {

/**
 * Complete OCSP implementation including request generation,
 * response processing, nonce handling, and caching.
 */
class OcspProcessor {
public:
    /**
     * Main entry point for OCSP checking
     */
    static RevocationCheckResult CheckRevocation(
        X509* cert,
        X509* issuer,
        X509_STORE* store,
        const OcspOptions& options);

    /**
     * Generate OCSP request with optional nonce
     */
    static std::unique_ptr<OcspRequest> CreateRequest(
        X509* cert,
        X509* issuer,
        bool include_nonce = true,
        const std::vector<OcspExtension>& extensions = {});

    /**
     * Send OCSP request and receive response
     */
    static std::unique_ptr<OcspResponse> SendRequest(
        const OcspRequest& request,
        const std::string& responder_url,
        const NetworkOptions& options);

    /**
     * Parse and validate OCSP response
     */
    static OcspValidationResult ValidateResponse(
        const OcspResponse& response,
        const OcspRequest& request,
        X509* cert,
        X509* issuer,
        X509_STORE* store);

private:
    /**
     * Extract OCSP responder URLs from AIA extension
     */
    static std::vector<std::string> ExtractOcspUrls(X509* cert);

    /**
     * Verify OCSP response signature
     */
    static bool VerifyResponseSignature(
        OCSP_BASICRESP* basic_resp,
        X509_STORE* store,
        X509* issuer);

    /**
     * Check nonce matching between request and response
     */
    static bool VerifyNonce(
        const OcspRequest& request,
        const OcspResponse& response);

    /**
     * Handle delegated OCSP responder certificate
     */
    static bool VerifyDelegatedResponder(
        X509* responder_cert,
        X509* issuer);
};

/**
 * OCSP request wrapper with metadata
 */
struct OcspRequest {
    OCSP_REQUEST* request = nullptr;
    std::vector<uint8_t> nonce;
    std::chrono::system_clock::time_point created;
    std::vector<CertId> cert_ids;  // Support batch requests

    ~OcspRequest() { if (request) OCSP_REQUEST_free(request); }

    std::vector<uint8_t> to_der() const;
    std::string to_base64() const;
};

/**
 * OCSP response wrapper with metadata
 */
struct OcspResponse {
    OCSP_RESPONSE* response = nullptr;
    OcspResponseStatus status;
    std::chrono::system_clock::time_point received;
    std::optional<std::vector<uint8_t>> nonce;
    std::vector<SingleResponse> responses;

    ~OcspResponse() { if (response) OCSP_RESPONSE_free(response); }

    bool is_successful() const { return status == OcspResponseStatus::Successful; }
    std::optional<SingleResponse> find_response(const CertId& id) const;
};

/**
 * Single certificate response within OCSP response
 */
struct SingleResponse {
    CertId cert_id;
    CertStatus cert_status;
    std::chrono::system_clock::time_point this_update;
    std::optional<std::chrono::system_clock::time_point> next_update;
    std::optional<CRLReason> revocation_reason;
    std::optional<std::chrono::system_clock::time_point> revocation_time;
};

/**
 * Certificate identifier for OCSP
 */
struct CertId {
    std::string hash_algorithm;
    std::vector<uint8_t> issuer_name_hash;
    std::vector<uint8_t> issuer_key_hash;
    std::string serial_number;

    bool operator==(const CertId& other) const;
    size_t hash() const;
};

/**
 * OCSP response status codes
 */
enum class OcspResponseStatus {
    Successful = 0,
    MalformedRequest = 1,
    InternalError = 2,
    TryLater = 3,
    SigRequired = 5,
    Unauthorized = 6
};

/**
 * Certificate status in OCSP response
 */
enum class CertStatus {
    Good,
    Revoked,
    Unknown
};

} // namespace vanillapdf
```

### 4. NetworkManager (HTTP/HTTPS Client)

```cpp
// File: src/vanillapdf/utils/network_manager.h

namespace vanillapdf {

/**
 * Network abstraction layer supporting multiple backends
 */
class NetworkManager {
public:
    enum class Backend {
        OpenSSL_BIO,  // Default, always available
        LibCurl,      // If VANILLAPDF_HAVE_CURL
        WinHTTP,      // Windows-specific
        System        // Platform default
    };

    /**
     * Perform HTTP GET request
     */
    static HttpResponse Get(
        const std::string& url,
        const HttpOptions& options = {});

    /**
     * Perform HTTP POST request
     */
    static HttpResponse Post(
        const std::string& url,
        const std::vector<uint8_t>& data,
        const std::string& content_type,
        const HttpOptions& options = {});

    /**
     * Check if URL is reachable
     */
    static bool IsReachable(
        const std::string& url,
        std::chrono::milliseconds timeout = std::chrono::seconds(2));

private:
    /**
     * Parse URL into components
     */
    static UrlComponents ParseUrl(const std::string& url);

    /**
     * Create connection with retry logic
     */
    static std::unique_ptr<Connection> CreateConnection(
        const UrlComponents& url,
        const HttpOptions& options);

    /**
     * Apply exponential backoff for retries
     */
    static std::chrono::milliseconds CalculateBackoff(
        int attempt,
        std::chrono::milliseconds base_delay);
};

/**
 * HTTP request options
 */
struct HttpOptions {
    Backend backend = Backend::System;
    std::chrono::milliseconds timeout{5000};
    std::chrono::milliseconds connect_timeout{2000};
    int max_retries = 3;
    std::chrono::milliseconds retry_delay{1000};
    bool follow_redirects = true;
    int max_redirects = 5;
    std::unordered_map<std::string, std::string> headers;
    std::optional<std::string> proxy_url;
    std::optional<std::pair<std::string, std::string>> auth;  // user, pass
    bool verify_ssl = true;
    std::optional<std::string> ca_bundle_path;
};

/**
 * HTTP response
 */
struct HttpResponse {
    int status_code = 0;
    std::unordered_map<std::string, std::string> headers;
    std::vector<uint8_t> body;
    std::chrono::milliseconds elapsed_time;
    std::string error_message;

    bool is_success() const { return status_code >= 200 && status_code < 300; }
    std::string get_header(const std::string& name) const;
};

/**
 * Connection abstraction
 */
class Connection {
public:
    virtual ~Connection() = default;
    virtual HttpResponse Execute(const HttpRequest& request) = 0;
    virtual void Close() = 0;
};

/**
 * OpenSSL BIO implementation
 */
class BioConnection : public Connection {
public:
    BioConnection(const UrlComponents& url, const HttpOptions& options);
    ~BioConnection() override;
    HttpResponse Execute(const HttpRequest& request) override;
    void Close() override;

private:
    BIO* bio_ = nullptr;
    SSL_CTX* ssl_ctx_ = nullptr;
    SSL* ssl_ = nullptr;
};

#ifdef VANILLAPDF_HAVE_CURL
/**
 * libcurl implementation
 */
class CurlConnection : public Connection {
public:
    CurlConnection(const UrlComponents& url, const HttpOptions& options);
    ~CurlConnection() override;
    HttpResponse Execute(const HttpRequest& request) override;
    void Close() override;

private:
    CURL* curl_ = nullptr;
    struct curl_slist* headers_ = nullptr;
};
#endif

} // namespace vanillapdf
```

### 5. RevocationCache (Caching Layer)

```cpp
// File: src/vanillapdf/utils/revocation_cache.h

namespace vanillapdf {

/**
 * Multi-level cache for revocation data
 */
class RevocationCache {
public:
    /**
     * Get CRL from cache
     */
    static std::optional<CachedCrl> GetCrl(const std::string& url);

    /**
     * Store CRL in cache
     */
    static void PutCrl(
        const std::string& url,
        const ParsedCrl& crl,
        std::chrono::system_clock::time_point expires);

    /**
     * Get OCSP response from cache
     */
    static std::optional<CachedOcspResponse> GetOcspResponse(
        const CertId& cert_id);

    /**
     * Store OCSP response in cache
     */
    static void PutOcspResponse(
        const CertId& cert_id,
        const OcspResponse& response,
        std::chrono::system_clock::time_point expires);

    /**
     * Clear expired entries
     */
    static void Purge();

    /**
     * Get cache statistics
     */
    static CacheStatistics GetStatistics();

    /**
     * Configure cache settings
     */
    static void Configure(const CacheConfiguration& config);

private:
    /**
     * Memory cache (L1)
     */
    class MemoryCache {
    public:
        template<typename T>
        std::optional<T> Get(const std::string& key);

        template<typename T>
        void Put(const std::string& key, const T& value, std::chrono::seconds ttl);

        void Clear();
        size_t Size() const;

    private:
        struct Entry {
            std::any data;
            std::chrono::system_clock::time_point expires;
            std::chrono::system_clock::time_point last_access;
            size_t access_count = 0;
        };

        std::unordered_map<std::string, Entry> entries_;
        mutable std::shared_mutex mutex_;
        size_t max_entries_ = 1000;
        size_t max_memory_ = 100 * 1024 * 1024;  // 100MB
    };

    /**
     * Disk cache (L2)
     */
    class DiskCache {
    public:
        DiskCache(const std::filesystem::path& cache_dir);

        std::optional<std::vector<uint8_t>> Get(const std::string& key);
        void Put(const std::string& key, const std::vector<uint8_t>& data, std::chrono::seconds ttl);
        void Clear();
        size_t Size() const;

    private:
        std::filesystem::path cache_dir_;
        std::filesystem::path GetCachePath(const std::string& key) const;
        void CleanupExpired();
    };

    static MemoryCache memory_cache_;
    static std::unique_ptr<DiskCache> disk_cache_;
    static CacheConfiguration config_;
};

/**
 * Cache configuration
 */
struct CacheConfiguration {
    bool enabled = true;
    bool use_memory_cache = true;
    bool use_disk_cache = false;
    std::filesystem::path disk_cache_path;
    size_t max_memory_entries = 1000;
    size_t max_memory_size = 100 * 1024 * 1024;
    size_t max_disk_size = 1024 * 1024 * 1024;
    std::chrono::seconds default_ttl{3600};
    std::chrono::seconds max_ttl{86400};
    bool respect_next_update = true;
};

/**
 * Cache statistics
 */
struct CacheStatistics {
    size_t memory_entries = 0;
    size_t memory_size = 0;
    size_t disk_entries = 0;
    size_t disk_size = 0;
    size_t hits = 0;
    size_t misses = 0;
    double hit_rate = 0.0;
    std::chrono::system_clock::time_point last_purge;
};

} // namespace vanillapdf
```

### 6. StapledOcspProcessor (OCSP Stapling)

```cpp
// File: src/vanillapdf/utils/stapled_ocsp_processor.h

namespace vanillapdf {

/**
 * Extract and validate OCSP responses stapled in signatures
 */
class StapledOcspProcessor {
public:
    /**
     * Check for stapled OCSP response in PKCS#7 signature
     */
    static std::optional<OcspResponse> ExtractStapledResponse(PKCS7* p7);

    /**
     * Validate stapled response
     */
    static OcspValidationResult ValidateStapledResponse(
        const OcspResponse& response,
        X509* cert,
        X509* issuer,
        X509_STORE* store);

    /**
     * Check if signature should contain stapled response
     */
    static bool IsStaplingExpected(PKCS7* p7);

private:
    /**
     * Extract OCSP response from signed attributes
     */
    static std::optional<std::vector<uint8_t>> ExtractFromAttributes(
        STACK_OF(X509_ATTRIBUTE)* attrs);

    /**
     * Extract from CMS SignerInfo
     */
    static std::optional<std::vector<uint8_t>> ExtractFromCms(
        CMS_ContentInfo* cms);
};

} // namespace vanillapdf
```

## Implementation Roadmap

### Phase 1: Foundation

**Goals**: Establish core infrastructure and basic CRL support

1. **Infrastructure**
   - [ ] Implement `RevocationStatus` enum and result structures
   - [ ] Create `RevocationManager` skeleton with routing logic
   - [ ] Implement `NetworkManager` with OpenSSL BIO backend
   - [ ] Add basic HTTP GET support with timeout handling
   - [ ] Create unit tests for network layer

2. **CRL Core**
   - [ ] Implement `CrlProcessor::ExtractDistributionPoints()`
   - [ ] Implement `CrlProcessor::DownloadCrl()` with retry logic
   - [ ] Implement `CrlProcessor::ParseCrl()` for basic CRLs
   - [ ] Implement `CrlProcessor::ValidateCrl()` signature verification
   - [ ] Add CRL parsing unit tests

3. **CRL Integration**
   - [ ] Implement `CrlProcessor::CheckRevocation()` main logic
   - [ ] Integrate into `SignatureVerifier::VerifyCertificateChain()`
   - [ ] Add memory-based CRL caching
   - [ ] Create integration tests with test CRLs
   - [ ] Update `SignatureVerificationSettings` with CRL flags

### Phase 2: OCSP Implementation

**Goals**: Complete OCSP support with nonce handling

1. **OCSP Request**
   - [ ] Implement `OcspProcessor::ExtractOcspUrls()`
   - [ ] Implement `OcspProcessor::CreateRequest()` with nonce
   - [ ] Add HTTP POST support to `NetworkManager`
   - [ ] Create OCSP request serialization
   - [ ] Unit tests for request generation

2. **OCSP Response**
   - [ ] Implement `OcspProcessor::SendRequest()`
   - [ ] Implement `OcspProcessor::ValidateResponse()`
   - [ ] Add nonce verification
   - [ ] Handle delegated responders
   - [ ] Unit tests for response parsing

3. **OCSP Integration**
   - [ ] Implement `OcspProcessor::CheckRevocation()` main logic
   - [ ] Add OCSP response caching
   - [ ] Integrate with `RevocationManager`
   - [ ] Implement OCSP->CRL fallback logic
   - [ ] Integration tests with mock OCSP responder

4. **Advanced OCSP**
   - [ ] Support batch OCSP requests
   - [ ] Implement OCSP stapling extraction
   - [ ] Add stapled response validation
   - [ ] Performance testing and optimization
   - [ ] Security audit of OCSP implementation

### Phase 3: Advanced Features

**Goals**: Production-ready features and optimizations

1. **Advanced CRL**
   - [ ] Implement delta CRL support
   - [ ] Handle indirect CRLs
   - [ ] Add CRL reason code processing
   - [ ] Implement CRL partitioning support
   - [ ] Advanced CRL caching with disk persistence

2. **Performance & Reliability**
   - [ ] Implement parallel revocation checking
   - [ ] Add connection pooling to `NetworkManager`
   - [ ] Implement circuit breaker pattern
   - [ ] Add metrics and monitoring hooks
   - [ ] Load testing with concurrent verifications

3. **Configuration & Polish**
   - [ ] Add environment variable configuration
   - [ ] Implement configuration file support
   - [ ] Add detailed logging throughout
   - [ ] Create performance benchmarks
   - [ ] Documentation and examples

### Phase 4: C API and CLI

**Goals**: Expose functionality through C API and command line

1. **C API**
   - [ ] Uncomment revocation settings in C API
   - [ ] Add new configuration functions:
     - `SignatureVerificationSettings_SetRevocationMethod()`
     - `SignatureVerificationSettings_SetRevocationTimeout()`
     - `SignatureVerificationSettings_SetRevocationFailureMode()`
   - [ ] Update result API with revocation details
   - [ ] C API testing

2. **CLI Integration**
   - [ ] Add `--check-revocation` flag to `verify.c`
   - [ ] Add `--revocation-method` option (ocsp/crl/both)
   - [ ] Add `--revocation-timeout` option
   - [ ] Add verbose revocation logging mode
   - [ ] Update help documentation

## Performance and Optimization

### Optimization Strategies

1. **Parallel Processing**
   ```cpp
   // Parallel chain checking
   std::vector<std::future<RevocationResult>> futures;
   for (const auto& cert : chain) {
       futures.push_back(std::async(std::launch::async, [&]() {
           return CheckCertificate(cert, issuer, store, policy);
       }));
   }
   ```

2. **Connection Pooling**
   ```cpp
   class ConnectionPool {
       std::unordered_map<std::string, std::queue<std::unique_ptr<Connection>>> pools_;
       std::mutex mutex_;

   public:
       std::unique_ptr<Connection> Acquire(const std::string& host);
       void Release(const std::string& host, std::unique_ptr<Connection> conn);
   };
   ```

3. **Smart Caching**
   - Pre-fetch CRLs during idle time
   - Share CRL cache across verifications
   - Implement negative caching for unreachable endpoints
   - Use ETags for conditional requests

4. **Batch Operations**
   - Combine multiple OCSP requests to same responder
   - Download CRLs once for multiple certificates
   - Amortize network overhead

### Performance Targets

| Operation | Target | Maximum |
|-----------|--------|---------|
| Cached lookup | < 1ms | 5ms |
| OCSP check (network) | < 500ms | 2000ms |
| CRL download (1MB) | < 1000ms | 5000ms |
| Full chain (3 certs) | < 1500ms | 6000ms |
| Parallel chain check | < 600ms | 2000ms |

### Memory Management

1. **CRL Size Limits**
   - Maximum CRL size: 100MB (configurable)
   - Streaming parser for large CRLs
   - Compress cached CRLs with zlib

2. **Cache Eviction**
   - LRU eviction for memory cache
   - TTL-based eviction
   - Size-based limits

3. **Resource Pooling**
   - Reuse OpenSSL contexts
   - Pool HTTP connections
   - Reuse parser buffers

## Security Analysis

### Threat Model

1. **Network Attacks**
   - **MITM on CRL/OCSP**: Mitigated by signature verification
   - **DNS hijacking**: Use HTTPS where possible
   - **Response replay**: OCSP nonce validation
   - **DoS via slow response**: Timeouts and limits

2. **Cache Poisoning**
   - **Invalid data injection**: Cryptographic validation
   - **Timestamp manipulation**: Validate against system time
   - **Resource exhaustion**: Size and count limits

3. **Implementation Vulnerabilities**
   - **Buffer overflows**: Use safe string/buffer classes
   - **Integer overflows**: Check arithmetic operations
   - **Race conditions**: Proper synchronization
   - **Memory leaks**: RAII and smart pointers

### Security Controls

1. **Cryptographic Validation**
   ```cpp
   bool ValidateSignature(X509_CRL* crl, X509* issuer) {
       EVP_PKEY* key = X509_get0_pubkey(issuer);
       if (!key) return false;

       int result = X509_CRL_verify(crl, key);
       EVP_PKEY_free(key);

       return result == 1;
   }
   ```

2. **Time-based Validation**
   ```cpp
   bool ValidateTime(X509_CRL* crl) {
       ASN1_TIME* this_update = X509_CRL_get0_lastUpdate(crl);
       ASN1_TIME* next_update = X509_CRL_get0_nextUpdate(crl);

       time_t now = time(nullptr);

       if (ASN1_TIME_compare(this_update, nullptr) > 0) {
           return false;  // Not yet valid
       }

       if (next_update && ASN1_TIME_compare(next_update, nullptr) < 0) {
           return false;  // Expired
       }

       return true;
   }
   ```

3. **Input Validation**
   - Validate all URLs before use
   - Limit response sizes
   - Validate certificate formats
   - Sanitize error messages

## Testing Strategy

### Unit Testing

```cpp
// Test CRL parsing
TEST(CrlProcessor, ParseValidCrl) {
    auto crl_data = LoadTestCrl("valid_crl.pem");
    auto issuer = LoadTestCert("ca_cert.pem");

    auto parsed = CrlProcessor::ParseCrl(crl_data, issuer);

    ASSERT_NE(parsed, nullptr);
    EXPECT_FALSE(parsed->revoked_serials.empty());
    EXPECT_GT(parsed->next_update, std::chrono::system_clock::now());
}

// Test OCSP request generation
TEST(OcspProcessor, CreateRequestWithNonce) {
    auto cert = LoadTestCert("end_entity.pem");
    auto issuer = LoadTestCert("intermediate.pem");

    auto request = OcspProcessor::CreateRequest(cert, issuer, true);

    ASSERT_NE(request, nullptr);
    EXPECT_FALSE(request->nonce.empty());
    EXPECT_EQ(request->nonce.size(), 16);  // 128-bit nonce
}

// Test parallel checking
TEST(RevocationManager, ParallelChainCheck) {
    auto chain = LoadTestChain();
    auto store = CreateTestStore();
    auto settings = CreateTestSettings();

    auto start = std::chrono::steady_clock::now();
    auto result = RevocationManager::CheckChain(chain, store, settings);
    auto elapsed = std::chrono::steady_clock::now() - start;

    EXPECT_TRUE(result.all_valid);
    EXPECT_LT(elapsed, std::chrono::seconds(2));  // Should be fast due to parallelism
}
```

### Integration Testing

```cpp
// Mock OCSP responder
class MockOcspResponder {
public:
    void Start(uint16_t port);
    void Stop();
    void SetResponse(const std::string& serial, CertStatus status);
    void SetDelay(std::chrono::milliseconds delay);
    void SetErrorRate(double rate);
};

// Mock CRL server
class MockCrlServer {
public:
    void Start(uint16_t port);
    void ServeCrl(const std::string& path, X509_CRL* crl);
    void SetBandwidthLimit(size_t bytes_per_second);
};

// End-to-end test
TEST(SignatureVerifier, RevokedCertificateDetection) {
    // Setup mock servers
    MockOcspResponder ocsp_server;
    ocsp_server.Start(8080);
    ocsp_server.SetResponse("12345", CertStatus::Revoked);

    // Create signed document with revoked cert
    auto document = CreateTestDocument();
    auto signature = SignWithRevokedCert(document);

    // Verify with revocation checking
    auto settings = CreateSettings();
    settings->SetCheckRevocationFlag(true);

    auto result = SignatureVerifier::Verify(
        document, signature, trust_store, settings);

    EXPECT_EQ(result->GetStatus(), SignatureStatus_CertificateRevoked);
}
```

### Fuzz Testing

```cpp
// Fuzz CRL parser
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    try {
        CrlData crl_data{std::vector<uint8_t>(data, data + size)};
        auto parsed = CrlProcessor::ParseCrl(crl_data, nullptr);
        // Should not crash
    } catch (...) {
        // Exceptions are ok
    }
    return 0;
}
```

## Migration and Backward Compatibility

### API Compatibility

1. **Additive changes only** - No breaking changes to existing APIs
2. **Default behavior unchanged** - Revocation checking off by default
3. **New settings are optional** - All new settings have defaults
4. **Result structure extended** - New fields added, old fields preserved

### Migration Path

```cpp
// Phase 1: Add new APIs without enforcement
SignatureVerificationSettings_SetCheckRevocationFlag(settings, true);  // Opt-in

// Phase 2: Add warnings for missing revocation checks
if (!settings->GetCheckRevocationFlag()) {
    spdlog::warn("Signature verification without revocation checking");
}

// Phase 3: Change defaults (major version)
bool m_check_revocation = true;  // On by default in v3.0
```

### Configuration Migration

```yaml
# v2.x configuration
signature_verification:
  check_revocation: false

# v3.x configuration
signature_verification:
  revocation:
    enabled: true
    method: prefer_ocsp
    timeout_ms: 5000
    failure_mode: soft_fail
    cache:
      enabled: true
      memory_size_mb: 100
```

## Future Enhancements

### Short Term (Next Release)

1. **LibCurl backend** for better HTTP handling
2. **Prometheus metrics** for monitoring
3. **Redis cache backend** for distributed caching
4. **Async API** for non-blocking verification

### Medium Term

1. **Certificate Transparency** log checking
2. **Lightweight OCSP** (RFC 5019) support
3. **OCSP multi-stapling** (RFC 6961)
4. **CRL sharding** for large CRLs
5. **Machine learning** for anomaly detection

### Long Term

1. **Blockchain-based revocation** research
2. **Post-quantum** signature support
3. **Hardware security module** integration
4. **Distributed revocation** checking
5. **Zero-knowledge proofs** for privacy

## Appendix A: Implementation Examples

### Example 1: CRL Download with Retry

```cpp
std::unique_ptr<CrlData> CrlProcessor::DownloadCrl(
    const std::string& url,
    const NetworkOptions& options) {

    int attempts = 0;
    std::chrono::milliseconds delay = options.retry_delay;

    while (attempts <= options.max_retries) {
        try {
            spdlog::debug("Downloading CRL from {} (attempt {}/{})",
                         url, attempts + 1, options.max_retries + 1);

            auto response = NetworkManager::Get(url, {
                .timeout = options.timeout,
                .follow_redirects = true,
                .max_redirects = 5
            });

            if (response.is_success()) {
                spdlog::info("Successfully downloaded CRL from {} ({} bytes)",
                            url, response.body.size());

                return std::make_unique<CrlData>(CrlData{
                    .data = response.body,
                    .url = url,
                    .download_time = std::chrono::system_clock::now(),
                    .content_type = response.get_header("Content-Type")
                });
            }

            spdlog::warn("CRL download failed with status {}: {}",
                        response.status_code, response.error_message);

            if (response.status_code >= 400 && response.status_code < 500) {
                // Client error, don't retry
                break;
            }

        } catch (const std::exception& e) {
            spdlog::error("CRL download exception: {}", e.what());
        }

        if (++attempts <= options.max_retries) {
            spdlog::debug("Retrying CRL download after {} ms", delay.count());
            std::this_thread::sleep_for(delay);
            delay *= 2;  // Exponential backoff
        }
    }

    spdlog::error("Failed to download CRL from {} after {} attempts",
                 url, attempts);
    return nullptr;
}
```

### Example 2: OCSP Request with Nonce

```cpp
std::unique_ptr<OcspRequest> OcspProcessor::CreateRequest(
    X509* cert,
    X509* issuer,
    bool include_nonce,
    const std::vector<OcspExtension>& extensions) {

    auto request = std::make_unique<OcspRequest>();
    request->request = OCSP_REQUEST_new();
    if (!request->request) {
        throw std::runtime_error("Failed to create OCSP request");
    }

    // Add certificate ID
    OCSP_CERTID* cert_id = OCSP_cert_to_id(
        EVP_sha256(), cert, issuer);
    if (!cert_id) {
        throw std::runtime_error("Failed to create certificate ID");
    }

    if (!OCSP_request_add0_id(request->request, cert_id)) {
        OCSP_CERTID_free(cert_id);
        throw std::runtime_error("Failed to add certificate ID to request");
    }

    // Add nonce for replay protection
    if (include_nonce) {
        unsigned char nonce_buf[16];
        if (RAND_bytes(nonce_buf, sizeof(nonce_buf)) != 1) {
            throw std::runtime_error("Failed to generate nonce");
        }

        request->nonce = std::vector<uint8_t>(
            nonce_buf, nonce_buf + sizeof(nonce_buf));

        if (!OCSP_request_add1_nonce(
                request->request, nonce_buf, sizeof(nonce_buf))) {
            throw std::runtime_error("Failed to add nonce to request");
        }

        spdlog::debug("Added {} byte nonce to OCSP request",
                     request->nonce.size());
    }

    // Add custom extensions
    for (const auto& ext : extensions) {
        // Add extension to request
        // ...
    }

    request->created = std::chrono::system_clock::now();

    spdlog::debug("Created OCSP request for certificate serial {}",
                 GetSerialNumber(cert));

    return request;
}
```

### Example 3: Parallel Chain Verification

```cpp
ChainRevocationResult RevocationManager::CheckChain(
    const std::vector<X509*>& chain,
    X509_STORE* store,
    SignatureVerificationSettingsPtr settings) {

    ChainRevocationResult result;
    auto start = std::chrono::steady_clock::now();

    // Prepare revocation policy
    RevocationPolicy policy;
    if (settings) {
        policy.method = settings->GetPreferOcsp() ?
            RevocationPolicy::Method::PreferOCSP :
            RevocationPolicy::Method::PreferCRL;
        policy.timeout = std::chrono::milliseconds(
            settings->GetRevocationTimeout());
    }

    // Create tasks for parallel execution
    std::vector<RevocationTask> tasks;
    for (size_t i = 0; i < chain.size() - 1; ++i) {
        tasks.push_back({
            .cert = chain[i],
            .issuer = chain[i + 1],
            .store = store,
            .policy = policy,
            .index = i
        });
    }

    // Execute in parallel with timeout
    auto futures = std::vector<std::future<CertificateRevocationResult>>();

    for (const auto& task : tasks) {
        futures.push_back(std::async(std::launch::async, [task]() {
            try {
                return CheckCertificate(
                    task.cert, task.issuer, task.store, task.policy);
            } catch (const std::exception& e) {
                spdlog::error("Revocation check failed: {}", e.what());
                CertificateRevocationResult error_result;
                error_result.status = RevocationStatus::CheckFailed;
                error_result.failure_reason = e.what();
                return error_result;
            }
        }));
    }

    // Collect results with timeout
    result.certificate_results.reserve(futures.size());
    for (auto& future : futures) {
        if (future.wait_for(policy.timeout) == std::future_status::ready) {
            result.certificate_results.push_back(future.get());
        } else {
            CertificateRevocationResult timeout_result;
            timeout_result.status = RevocationStatus::CheckFailed;
            timeout_result.failure_reason = "Timeout";
            result.certificate_results.push_back(timeout_result);
            result.checks_failed++;
        }
        result.checks_performed++;
    }

    // Determine overall result
    result.all_valid = std::all_of(
        result.certificate_results.begin(),
        result.certificate_results.end(),
        [](const auto& r) {
            return r.status == RevocationStatus::Good;
        });

    result.total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);

    spdlog::info("Chain revocation check completed: {} certificates, "
                "{} failed, {} ms total",
                result.checks_performed,
                result.checks_failed,
                result.total_time.count());

    return result;
}
```

## Appendix B: Standards Compliance

### RFC Compliance

1. **RFC 5280** - Internet X.509 Public Key Infrastructure
   - Section 5: CRL and CRL Extensions Profile
   - Section 6: Certification Path Validation

2. **RFC 6960** - X.509 Internet PKI Online Certificate Status Protocol
   - Section 2: Protocol Overview
   - Section 4: Detailed Protocol

3. **RFC 5019** - Lightweight OCSP Profile
   - Simplified OCSP for constrained environments

4. **RFC 8954** - Online Certificate Status Protocol (OCSP) Nonce Extension
   - Updated nonce handling

### PDF Standards

1. **ISO 32000-2:2020** - PDF 2.0
   - Section 12.8: Digital Signatures
   - Annex B: Revocation Information

2. **ETSI EN 319 142** - PAdES (PDF Advanced Electronic Signatures)
   - Part 1: Building blocks and PAdES baseline signatures
   - Part 2: Additional PAdES signatures profiles

## Appendix C: Error Codes

```cpp
enum class RevocationErrorCode {
    SUCCESS = 0,

    // Network errors (1000-1999)
    NETWORK_TIMEOUT = 1001,
    NETWORK_CONNECTION_FAILED = 1002,
    NETWORK_DNS_FAILED = 1003,
    NETWORK_SSL_FAILED = 1004,

    // CRL errors (2000-2999)
    CRL_DOWNLOAD_FAILED = 2001,
    CRL_PARSE_FAILED = 2002,
    CRL_SIGNATURE_INVALID = 2003,
    CRL_EXPIRED = 2004,
    CRL_NOT_YET_VALID = 2005,
    CRL_ISSUER_MISMATCH = 2006,

    // OCSP errors (3000-3999)
    OCSP_REQUEST_FAILED = 3001,
    OCSP_RESPONSE_MALFORMED = 3002,
    OCSP_RESPONSE_UNAUTHORIZED = 3003,
    OCSP_RESPONSE_TRY_LATER = 3004,
    OCSP_SIGNATURE_INVALID = 3005,
    OCSP_NONCE_MISMATCH = 3006,

    // Certificate errors (4000-4999)
    CERT_NO_REVOCATION_INFO = 4001,
    CERT_REVOKED = 4002,
    CERT_STATUS_UNKNOWN = 4003,

    // Configuration errors (5000-5999)
    CONFIG_INVALID = 5001,
    CONFIG_CONFLICTING = 5002
};
```

## References

1. [RFC 5280 - Internet X.509 PKI Certificate and CRL Profile](https://tools.ietf.org/html/rfc5280)
2. [RFC 6960 - X.509 OCSP](https://tools.ietf.org/html/rfc6960)
3. [RFC 8954 - OCSP Nonce Extension](https://tools.ietf.org/html/rfc8954)
4. [RFC 5019 - Lightweight OCSP](https://tools.ietf.org/html/rfc5019)
5. [ISO 32000-2:2020 - PDF 2.0](https://www.iso.org/standard/75839.html)
6. [ETSI EN 319 142 - PAdES](https://www.etsi.org/deliver/etsi_en/319100_319199/31914201/)
7. [OpenSSL Documentation](https://www.openssl.org/docs/)
8. [NIST SP 800-57 - Key Management](https://csrc.nist.gov/publications/detail/sp/800-57-part-1/rev-5/final)

---

**Document Version**: 2.0
**Last Updated**: November 2025
**Authors**: Vanilla.PDF Development Team
**Status**: Design Specification - Pending Implementation
