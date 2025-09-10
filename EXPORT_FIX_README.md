# Export Mangling Fix

This document describes the fix for the export name mangling issue with `NameConstant_LZWDecode` and `NameConstant_JPXDecode`.

## Problem

The symbols `NameConstant_LZWDecode` and `NameConstant_JPXDecode` were being exported with mangled names like:
- `?NameConstant_LZWDecode@@3PEBUNameObjectHandleTag@@EB`
- `?NameConstant_JPXDecode@@3PEBUNameObjectHandleTag@@EB`

This was due to missing declarations in the C API header file, causing the symbols to be exported with C++ name mangling instead of C linkage.

## Root Cause

1. **Missing C API declarations**: The constants `NameConstant_LZWDecode` and `NameConstant_JPXDecode` were implemented in the C++ code but not declared in the public C API header file (`include/vanillapdf/utils/c_name_constants.h`).

2. **Syntax error**: There was a missing semicolon in the implementation file that could have caused compilation issues.

## Solution

### 1. Added Missing C API Declarations

Added the following declarations to `include/vanillapdf/utils/c_name_constants.h`:

```c
/**
 * \brief Represents LZWDecode filter, see 7.4.5.
 */
VANILLAPDF_API extern const NameObjectHandle* NameConstant_LZWDecode;

/**
 * \brief Represents JPXDecode filter, see 7.4.9.
 */
VANILLAPDF_API extern const NameObjectHandle* NameConstant_JPXDecode;
```

### 2. Fixed Syntax Error

Fixed the missing semicolon in `src/vanillapdf/syntax/utils/name_constants.cpp`:

```cpp
// Before (line 195):
DECLARE_CONST_NAME(Text)
DECLARE_CONST_NAME(FreeText);

// After:
DECLARE_CONST_NAME(Text);
DECLARE_CONST_NAME(FreeText);
```

### 3. Added Export Verification Tests

Created comprehensive tests to verify that exported symbols don't contain special characters:

1. **Runtime Export Verification** (`src/vanillapdf.unittest/export_test.cpp`):
   - Windows-specific test that inspects DLL exports at runtime
   - Verifies no symbols contain mangling characters (`@`, `?`, etc.)
   - Specifically checks for proper export of target constants

2. **Compile-time Verification** (`src/vanillapdf.unittest/export_verification_simple.cpp`):
   - Cross-platform test that verifies declarations exist and compile
   - Ensures constants are properly declared and accessible
   - Verifies constants are different objects

3. **Python Verification Script** (`scripts/verify_exports.py`):
   - Standalone script for manual DLL export verification
   - Uses `dumpbin` or `pefile` library to extract exports
   - Provides detailed analysis of export symbols

## Usage

### Building and Testing

1. **Build the project**:
   ```bash
   mkdir build && cd build
   cmake .. -DBUILD_SHARED_LIBS=ON -DVANILLAPDF_ENABLE_TESTS=ON
   make
   ```

2. **Run unit tests**:
   ```bash
   ctest --verbose
   ```

### Manual Verification (Windows)

Use the Python script to verify DLL exports:

```bash
# Install dependencies (optional, for pefile method)
pip install pefile

# Verify exports
python scripts/verify_exports.py path/to/vanillapdf.dll
```

Expected output:
```
✅ No mangled exports found!

Target symbol analysis:
  ✅ NameConstant_LZWDecode - Found and properly exported
  ✅ NameConstant_JPXDecode - Found and properly exported

✅ All checks passed!
```

### Using dumpbin (Visual Studio)

```cmd
dumpbin /exports vanillapdf.dll | findstr "NameConstant"
```

Expected output should show clean symbol names without `@` or `?` characters:
```
NameConstant_LZWDecode
NameConstant_JPXDecode
NameConstant_DCTDecode
...
```

## Files Modified

1. `include/vanillapdf/utils/c_name_constants.h` - Added missing C API declarations
2. `src/vanillapdf/syntax/utils/name_constants.cpp` - Fixed missing semicolon
3. `src/vanillapdf.unittest/CMakeLists.txt` - Added new test files
4. `src/vanillapdf.unittest/export_test.cpp` - New runtime export verification test
5. `src/vanillapdf.unittest/export_verification_simple.cpp` - New compile-time verification test
6. `scripts/verify_exports.py` - New manual verification script

## Prevention

The new tests will prevent regression of this issue by:
1. **Compile-time checks**: Ensuring all expected constants are declared and accessible
2. **Runtime checks**: Verifying actual DLL exports don't contain mangling characters
3. **Manual verification**: Providing tools for developers to check exports manually

## Notes

- The issue was specific to Windows DLL builds where C++ name mangling occurs
- The fix ensures proper C linkage for all exported name constants
- The verification tests work on both Windows and other platforms
- The Python script requires Windows with Visual Studio tools or the `pefile` library