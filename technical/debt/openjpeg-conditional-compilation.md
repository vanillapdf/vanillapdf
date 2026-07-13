# Technical Debt: OPENJPEG Conditional Compilation

## Overview

Vanilla.PDF currently requires OpenJPEG (JPEG2000) dependencies to be available even when `VANILLAPDF_ENABLE_JPEG2000=OFF`. This prevents compilation without OpenJPEG and causes build failures in scenarios where JPEG2000 support is intentionally disabled.

## Problem

The JPEG2000/JPX decode filter implementation in `src/vanillapdf/syntax/filters/jpx_decode_filter.cpp` unconditionally includes `<openjpeg.h>` and implements OpenJPEG-specific functionality, regardless of the `VANILLAPDF_ENABLE_JPEG2000` CMake option.

### Current Issues:
1. **Build failures**: When `VANILLAPDF_ENABLE_JPEG2000=OFF`, compilation fails with:
   ```
   fatal error: openjpeg.h: No such file or directory
   ```

2. **Inconsistent behavior**: Other optional dependencies (OpenSSL, JPEG, ZLIB) properly support conditional compilation, but JPEG2000 does not.

3. **Package workflow limitations**: System dependency testing workflows cannot disable JPEG2000 support without build failures.

## Root Cause

The `JPXDecodeFilter` class and its implementation are not wrapped with conditional compilation guards based on the `VANILLAPDF_HAVE_OPENJPEG` preprocessor definition.

## Solution Required

Implement conditional compilation guards similar to other optional dependencies:

### 1. Header File Guards
```cpp
#ifdef VANILLAPDF_HAVE_OPENJPEG
class JPXDecodeFilter : public FilterBase {
    // ... existing implementation
};
#endif // VANILLAPDF_HAVE_OPENJPEG
```

### 2. Implementation File Guards
```cpp
#ifdef VANILLAPDF_HAVE_OPENJPEG
#include <openjpeg.h>
// ... existing implementation
#else
// Fallback implementation that throws appropriate error
#endif
```

### 3. Factory Function Updates
```cpp
} else if (name.Equals(constant::Name::JPXDecode)) {
#ifdef VANILLAPDF_HAVE_OPENJPEG
    return JPXDecodeFilterPtr();
#else
    LOG_ERROR_AND_THROW_GENERAL("JPX/JPEG2000 support disabled at compile time");
#endif
}
```

## Files Affected

- `src/vanillapdf/syntax/filters/jpx_decode_filter.h`
- `src/vanillapdf/syntax/filters/jpx_decode_filter.cpp`
- `src/vanillapdf/syntax/filters/filter.cpp`
- `src/vanillapdf/implementation/syntax/c_filter.cpp` (if applicable)

## Benefits of Resolution

1. **Consistent dependency handling**: All optional dependencies behave uniformly
2. **Flexible build configurations**: Users can build without JPEG2000 support when not needed
3. **Reduced dependency overhead**: Smaller builds and fewer dependencies for use cases that don't require JPEG2000
4. **Improved CI testing**: System dependency workflows can properly test without OpenJPEG

## Priority

**Medium** - Affects build flexibility and consistency but has workarounds (always enabling JPEG2000).

## Effort Estimate

**Low** - Straightforward implementation following established patterns from other optional dependencies.

## Related

- Issue: [GitHub Issue #137](https://github.com/vanillapdf/vanillapdf/issues/137)
- Similar implementation: OpenSSL conditional compilation in encryption features
- CMake option: `VANILLAPDF_ENABLE_JPEG2000`
- Compile definition: `VANILLAPDF_HAVE_OPENJPEG`