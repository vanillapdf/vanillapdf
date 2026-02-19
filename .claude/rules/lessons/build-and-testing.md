# Build and Testing Lessons

- Full test suite (`ctest`) takes ~40 minutes in Debug. Run the unit test executable directly for quick verification (~600ms): `build/windows-x64-msvc-17/src/vanillapdf.unittest/Debug/vanillapdf.unittest.exe`
- Never run the full test suite in Debug — it is extremely slow. Always use `-R "FilterPattern"` to target relevant tests. If the full suite is truly needed, run it in Release.
- Always run benchmarks in Release mode (`--build-config Release` / `--config Release`). Debug builds produce meaningless timings.
- Clang with `-Werror,-Wswitch` requires all enum values handled in switch statements (including `Undefined`). MSVC does not warn. Always add a `default:` case when switching on enums.
- `BIO_reset()` on a **writable** `BIO_s_mem()` clears the buffer (sets length=0), it does NOT rewind to position 0. To pass accumulated data to `CMS_final()`, use `BIO_get_mem_ptr()` + `BIO_new_mem_buf()` for a read-only view instead.
- Ed25519 CMS signing (RFC 8419): pass `EVP_sha512()` — NOT `NULL` — to `CMS_add1_signer()`. OpenSSL 3.x returns `CMS_R_NO_DEFAULT_DIGEST` when `NULL` is passed; the CMS layer uses SHA-512 for the `messageDigest` attribute and pure Ed25519 for the actual signing. Ed448 uses `EVP_shake256()`.
- After migrating from PKCS7 to CMS: `ExtractCertificateChain` must be called **after** `CMS_verify()` so that `CMS_SignerInfo_get0_algs(si, NULL, &signer_cert, ...)` returns the matched signer certificate.

## Performance Improvement Workflow

Every performance change must follow this workflow to justify the change with data:

1. **Add benchmark** — write a Google Benchmark test covering the hot path before making any changes
2. **Measure baseline** — build and run the benchmark in Release mode, record results
3. **Commit baseline** — commit the benchmark with baseline results in a comment
4. **Implement the change** — modify the code
5. **Measure impact** — run the same benchmark in Release mode, record new results
6. **Commit with comparison** — commit the change with both before/after results in the benchmark comment
