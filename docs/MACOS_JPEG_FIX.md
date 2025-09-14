# macOS JPEG Library Version Mismatch Fix

## Problem Description

On macOS x64 systems, you may encounter the following error when running tests:

```
Error: JPEG decompression exited with error: Wrong JPEG library version: library is 62, caller expects 80
```

This error occurs due to a version mismatch between:
- **libjpeg-turbo** (version 8.0, reports version 80) - used during compilation
- **libjpeg** (version 6.2, reports version 62) - found at runtime

## Root Cause

The issue typically happens when:
1. The project is compiled with libjpeg-turbo from vcpkg
2. At runtime, the system picks up the macOS system libjpeg instead
3. The version check in libjpeg fails due to incompatible version numbers

## Solutions

### Option 1: Automated Fix Script (Recommended)

Run the provided fix script:

```bash
./scripts/fix_macos_jpeg.sh
```

This script will:
- Install/update libjpeg-turbo via Homebrew
- Clean the build directory
- Configure proper environment variables
- Rebuild the project with consistent JPEG libraries

### Option 2: Manual Fix

1. **Clean build directory:**
   ```bash
   rm -rf build/
   ```

2. **Install libjpeg-turbo:**
   ```bash
   brew install libjpeg-turbo
   ```

3. **Set environment variables:**
   
   For Apple Silicon (arm64):
   ```bash
   export LDFLAGS="-L/opt/homebrew/lib"
   export CPPFLAGS="-I/opt/homebrew/include"
   export PKG_CONFIG_PATH="/opt/homebrew/lib/pkgconfig"
   ```
   
   For Intel (x86_64):
   ```bash
   export LDFLAGS="-L/usr/local/lib"
   export CPPFLAGS="-I/usr/local/include"
   export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig"
   ```

4. **Reconfigure and rebuild:**
   ```bash
   cmake --preset macos-x64  # or macos-arm64 for Apple Silicon
   cmake --build build/macos-x64
   ```

### Option 3: Use System JPEG Library

Configure CMake to use the system JPEG library instead of vcpkg's:

```bash
cmake --preset macos-x64 -DVANILLAPDF_EXTERNAL_JPEG=ON
```

## Verification

Test the fix by running the DCTDecodeFilter test:

```bash
cd build/macos-x64  # or build/macos-arm64
ctest -R DCTDecodeFilter.Decode
```

You should see:
```
Test project /path/to/build
    Start 1: DCTDecodeFilter.Decode
1/1 Test #1: DCTDecodeFilter.Decode ...........   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1 total.
```

## Prevention

To prevent this issue in future builds:

1. **Use consistent build environment:** Always build in a clean environment with consistent library versions.

2. **Use vcpkg overlays:** The project now includes macOS-specific overlays that ensure consistent JPEG library usage.

3. **Static linking:** The overlays configure static linking for JPEG libraries to avoid runtime conflicts.

## Technical Details

The fix works by:

1. **Improved Error Handling:** Enhanced error messages that provide clear guidance when version mismatches occur.

2. **Build Configuration:** Added macOS-specific vcpkg overlays (`cmake/overlays/x64-osx.cmake` and `cmake/overlays/arm64-osx.cmake`) that ensure consistent JPEG library usage.

3. **Runtime Detection:** Better logging and error reporting to help diagnose version issues.

4. **Fallback Options:** Multiple configuration options to handle different deployment scenarios.

## Related Files

- `src/vanillapdf/syntax/filters/dct_decode_filter.cpp` - Enhanced error handling
- `cmake/overlays/x64-osx.cmake` - macOS x64 build configuration
- `cmake/overlays/arm64-osx.cmake` - macOS ARM64 build configuration
- `scripts/fix_macos_jpeg.sh` - Automated fix script
- `src/vanillapdf/CMakeLists.txt` - Build warnings and guidance

## Troubleshooting

If the issue persists after applying the fixes:

1. **Check for multiple JPEG installations:**
   ```bash
   find /usr/local/lib /opt/homebrew/lib -name "*jpeg*"
   ```

2. **Verify library linking:**
   ```bash
   otool -L /path/to/your/test/binary
   ```

3. **Check Homebrew installation:**
   ```bash
   brew doctor
   ```

4. **Try a completely clean build:**
   ```bash
   rm -rf build/ external/vcpkg/buildtrees/ external/vcpkg/packages/
   ```

For additional help, please check the project's issue tracker or create a new issue with your specific build environment details.