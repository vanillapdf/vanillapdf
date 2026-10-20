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

The library does not load the OpenSSL 3 `legacy` provider unless built with `-DVANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER=ON` (set by the NuGet workflow, which links OpenSSL statically). Without it these fail with `VANILLAPDF_ERROR_NOT_SUPPORTED`:
- RC2 envelopes of certificate encrypted documents (PDFBox writes them)
- PKCS#12 files protected with RC2/RC4 (OpenSSL 1.x export default) — re-export: `openssl pkcs12 -legacy -in old.pfx -nodes | openssl pkcs12 -export -out new.pfx`
- MD2/MD4 signatures

With a **shared** OpenSSL the provider is a separate module (`legacy.dll`/`legacy.so`) looked up in a directory fixed at OpenSSL build time, which is why the option is meant for static OpenSSL. To try the option against a shared vcpkg OpenSSL locally, point `OPENSSL_MODULES` at `<build>/vcpkg_installed/<triplet>/bin`. Applications can also activate `legacy` + `default` themselves via `OSSL_PROVIDER_load` or an `OPENSSL_CONF` file — loading any provider explicitly disables the implicit default provider, so always load both.

Tests needing the provider are gated on the option: unit tests use `VANILLAPDF_TEST_OPENSSL_LEGACY`, and the `Tools.sign/verify.legacy-rc2-key` tests are only registered with it ON. The testdata corpus holds no RC2 material, so every corpus test runs in the default build.

## Test Failures

If tests fail unexpectedly:
1. Ensure all dependencies are properly installed
2. Check that the correct preset is being used for your platform
3. Run tests with verbose output: `ctest --preset your-preset --verbose`
4. Check for memory issues with sanitizers: `-DVANILLAPDF_ENABLE_ASAN=ON`
