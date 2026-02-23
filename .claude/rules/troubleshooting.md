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

## Test Failures

If tests fail unexpectedly:
1. Ensure all dependencies are properly installed
2. Check that the correct preset is being used for your platform
3. Run tests with verbose output: `ctest --preset your-preset --verbose`
4. Check for memory issues with sanitizers: `-DVANILLAPDF_ENABLE_ASAN=ON`
