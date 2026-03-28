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

PDF encryption relies on algorithms that restrictive OpenSSL crypto policies may block (MD5 for key derivation, RC4 for legacy PDFs, SHA-1 for older signatures). On RHEL-based distributions (Rocky Linux, Fedora, AlmaLinux) the `DEFAULT` policy restricts these algorithms.

To enable PDF encryption support on these systems:
```bash
# System-wide: switch to LEGACY crypto policy
sudo update-crypto-policies --set LEGACY
```

The library loads the OpenSSL `legacy` provider at startup (`OSSL_PROVIDER_load`) to make MD5, RC4, and other legacy algorithms available. However, the system crypto policy can still block their use at a higher level.

For signature verification, the `SignatureVerificationSettings` API provides `AllowWeakAlgorithmsFlag` to control whether weak algorithms (MD5, SHA-1, RSA < 2048 bits) are accepted:
```c
SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(settings, VANILLAPDF_RV_TRUE);
```

## Test Failures

If tests fail unexpectedly:
1. Ensure all dependencies are properly installed
2. Check that the correct preset is being used for your platform
3. Run tests with verbose output: `ctest --preset your-preset --verbose`
4. Check for memory issues with sanitizers: `-DVANILLAPDF_ENABLE_ASAN=ON`
