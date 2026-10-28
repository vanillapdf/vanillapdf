# Troubleshooting

## vcpkg Bootstrap Fails

```bash
# Ensure submodules are initialized
git submodule sync --recursive
git submodule update --init --recursive
```

## macOS JPEG Conflicts

The project includes a workaround for JPEG library conflicts on macOS (see GitHub issue #125):
```bash
brew unlink jpeg jpeg-turbo libjpeg 2>/dev/null || true
```

## Windows Debug Build Issues

Some x86 debug configurations may have issues. Use Release builds for x86 on Windows if encountering problems.

## Missing System Dependencies

For external dependency builds, ensure system packages are installed:
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev libjpeg-turbo8-dev zlib1g-dev

# macOS
brew install openssl libjpeg-turbo
```

## Build Configuration Issues

If CMake configuration fails, try cleaning the build directory:
```bash
rm -rf build/
cmake --preset your-preset
```

## OpenSSL Crypto Policy (RHEL/Rocky/Fedora)

Password protected PDFs do not depend on the crypto policy: RC4 and MD5 (standard security handler up to revision 4) are implemented in-house (`utils/rc4_cipher.*`, `utils/md5_digest.*`), and AES/SHA-2 come from the OpenSSL default provider. The policy still matters for SHA-1 signatures, which the RHEL `DEFAULT` policy rejects:
```bash
# System-wide: switch to LEGACY crypto policy
sudo update-crypto-policies --set LEGACY
```

For signature verification, the `SignatureVerificationSettings` API provides `AllowWeakAlgorithmsFlag` to control whether weak algorithms (MD5, SHA-1, RSA < 2048 bits) are accepted:
```c
SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(settings, VANILLAPDF_RV_TRUE);
```

## OpenSSL Legacy Provider

The library loads the OpenSSL 3 `legacy` provider by default. It supplies:
- RC2 envelopes of certificate encrypted documents (PDFBox, iText 5 write them)
- PKCS#12 files protected with RC2/RC4 (OpenSSL 1.x export default) — re-export: `openssl pkcs12 -legacy -in old.pfx -nodes | openssl pkcs12 -export -out new.pfx`
- MD2/MD4 signatures

Without it RC2 fails with `VANILLAPDF_ERROR_NOT_SUPPORTED`; MD2/MD4 signatures are still reported `SignatureStatus_Invalid` and MD4/MDC2/Whirlpool signing fails with a generic crypto error (#568). It is missing when the library is built with `-DVANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER=OFF`, when the runtime policy is `LegacyAlgorithmsPolicy_Disabled`, or when OpenSSL cannot find the module: with the default `Optional` policy that only logs a warning, `Required` fails `CryptoUtils_Initialize` instead. `CryptoUtils_GetLegacyAlgorithmsAvailable` tells which.

With a **shared** OpenSSL the provider is a separate module (`legacy.dll`/`legacy.so`) looked up in a directory fixed at OpenSSL build time; the dynamic vcpkg triplets have no usable one, so point `OPENSSL_MODULES` at `<build>/vcpkg_installed/<triplet>/bin` (the test environment does this automatically). A static OpenSSL compiles the provider in. `CryptoUtils_Cleanup` unloads only the providers the library loaded — it never calls `OPENSSL_cleanup`, so the library can be initialized again with a different policy.

Tests: `LegacyAlgorithmsPolicyTest` covers every policy in one binary; `VANILLAPDF_TEST_OPENSSL_LEGACY_PROVIDER` (defined when the option is ON) switches the expectations for builds without the provider, covered by the nightly `win.2025-x64-no-openssl-legacy` row. The `Tools.sign/verify.legacy-rc2-key` tests are only registered with the option ON. The testdata corpus holds no RC2 material.

## Test Failures

If tests fail unexpectedly:
1. Ensure all dependencies are properly installed
2. Check that the correct preset is being used for your platform
3. Run tests with verbose output: `ctest --preset your-preset --verbose`
4. Check for memory issues with sanitizers: `-DVANILLAPDF_ENABLE_ASAN=ON`
