# PDF Character Encoding Support

This document describes the comprehensive character encoding system added to VanillaPDF, providing native support for all PDF font encodings with efficient text decoding capabilities.

## Overview

The character encoding system implements the complete set of PDF character encodings as specified in the PDF specification ANNEX D, with embedded character mapping tables and support for encoding differences. This moves the heavy text decoding logic from .NET applications into the native library for better performance and consistency.

## Supported Encodings

### Base Encodings
- **StandardEncoding** - Adobe Standard Encoding (default for Type1 fonts)
- **MacRomanEncoding** - Mac OS Roman encoding
- **WinAnsiEncoding** - Windows Code Page 1252 (default for TrueType fonts)
- **PDFDocEncoding** - PDF document encoding (superset of WinAnsi)
- **MacExpertEncoding** - Mac Expert encoding for sophisticated typography

### Symbol Encodings
- **SymbolEncoding** - Mathematical and Greek symbols
- **ZapfDingbatsEncoding** - Decorative symbols and dingbats

### Custom Encodings
- Support for custom encodings with **Differences** arrays
- Automatic parsing of encoding modifications from PDF font dictionaries

## Architecture

### Core Classes

#### `CharacterEncoding` (Base Class)
```cpp
class CharacterEncoding : public IUnknown {
public:
    enum class Type {
        StandardEncoding, MacRomanEncoding, WinAnsiEncoding,
        PDFDocEncoding, MacExpertEncoding, SymbolEncoding,
        ZapfDingbatsEncoding, Custom
    };
    
    virtual uint32_t CodeToUnicode(uint8_t code) const;
    virtual std::string CodeToGlyphName(uint8_t code) const;
    virtual void ApplyDifferences(syntax::ArrayObjectPtr differences);
    
    static std::unique_ptr<CharacterEncoding> Create(syntax::ObjectPtr encoding_obj);
    static std::unique_ptr<CharacterEncoding> Create(Type type);
};
```

#### Specific Encoding Implementations
- `StandardEncoding` - Adobe Standard Encoding
- `MacRomanEncoding` - Mac Roman encoding
- `WinAnsiEncoding` - Windows ANSI encoding
- `PDFDocEncoding` - PDF document encoding
- `MacExpertEncoding` - Mac Expert encoding
- `SymbolEncoding` - Symbol font encoding
- `ZapfDingbatsEncoding` - ZapfDingbats encoding
- `CustomEncoding` - Custom encoding with base + differences

### Enhanced Font Classes

The existing font classes have been extended with encoding support:

```cpp
class FontBase {
public:
    // New encoding methods
    std::unique_ptr<CharacterEncoding> GetEncoding() const;
    std::string DecodeText(const uint8_t* codes, size_t count) const;
    uint32_t DecodeCharacter(uint8_t code) const;
    
protected:
    virtual CharacterEncoding::Type GetDefaultEncoding() const;
};
```

Font-specific default encodings:
- **Type1Font/MMType1Font**: StandardEncoding
- **TrueTypeFont**: WinAnsiEncoding
- **Type0Font**: Identity encoding (handled separately)

## C API

### High-Level Text Decoding (Primary .NET Interface)

```c
// Main function for .NET applications
error_type PDF_DecodeTextWithFont(
    FontHandle* font_handle,
    const uint8_t* byte_data,
    size_t byte_count,
    char** decoded_text,    // UTF-8 output
    size_t* text_length
);

error_type PDF_FreeDecodedText(char* text);
```

### Font Encoding Functions

```c
// Create encodings
error_type FontEncoding_CreateByType(FontEncodingType type, FontEncodingHandle** result);
error_type FontEncoding_CreateFromObject(ObjectHandle* encoding_obj, FontEncodingHandle** result);

// Character conversion
error_type FontEncoding_CodeToUnicode(FontEncodingHandle* handle, uint8_t code, uint32_t* result);
error_type FontEncoding_CodeToGlyphName(FontEncodingHandle* handle, uint8_t code, char** result);

// Apply differences
error_type FontEncoding_ApplyDifferences(FontEncodingHandle* handle, ArrayObjectHandle* differences);
```

### Font Text Decoding Functions

```c
// Font-specific decoding
error_type Font_DecodeText(FontHandle* handle, const uint8_t* codes, size_t count, char** result, size_t* result_length);
error_type Font_DecodeCharacter(FontHandle* handle, uint8_t code, uint32_t* result);
error_type Font_GetEncoding(FontHandle* handle, FontEncodingHandle** result);
```

## .NET Integration

### P/Invoke Declarations

```csharp
[DllImport("vanillapdf.dll")]
public static extern int PDF_DecodeTextWithFont(
    IntPtr fontHandle,
    byte[] byteData,
    UIntPtr byteCount,
    out IntPtr decodedText,
    out UIntPtr textLength);

[DllImport("vanillapdf.dll")]
public static extern int PDF_FreeDecodedText(IntPtr text);
```

### Usage Example

```csharp
// Get font handle from PDF parsing
IntPtr fontHandle = GetFontFromPDF();

// Raw bytes from PDF content stream
byte[] pdfBytes = GetBytesFromPDFContent();

// Decode text
int result = PDF_DecodeTextWithFont(
    fontHandle, 
    pdfBytes, 
    (UIntPtr)pdfBytes.Length, 
    out IntPtr textPtr, 
    out UIntPtr textLength);

if (result == 0) {
    string decodedText = Marshal.PtrToStringUTF8(textPtr, (int)textLength);
    Console.WriteLine($"Decoded: {decodedText}");
    PDF_FreeDecodedText(textPtr);
}
```

## Encoding Differences Support

The system automatically handles PDF encoding differences as specified in font dictionaries:

### PDF Font Dictionary Example
```pdf
20 0 obj
<</Type /Font
  /Subtype /TrueType
  /BaseFont /Arial,BoldItalic
  /Encoding <</Type /Encoding
            /BaseEncoding /WinAnsiEncoding
            /Differences [128 /aacute 129 /ccaron 130 /tcaron 131 /lcaron 
                         132 /iacute 133 /eacute 134 /ncaron 135 /zcaron]>>
  /ToUnicode 37 0 R
>>
endobj
```

The system:
1. Creates base WinAnsiEncoding
2. Applies differences starting at code 128
3. Maps codes to specified glyph names
4. Provides Unicode conversion for modified characters

## Character Mapping Tables

All character mappings are embedded in the binary with complete tables for:

### Standard Encoding (Adobe Standard)
- Codes 32-126: Standard ASCII
- Codes 161+: Extended characters, accents, symbols
- Special characters: quoteleft, quoteright, fi, fl ligatures

### WinAnsi Encoding (Windows CP1252)
- Codes 0-127: ASCII
- Codes 128-159: Windows-specific characters (Euro, smart quotes, etc.)
- Codes 160-255: Latin-1 Supplement

### Mac Roman Encoding
- Codes 0-127: ASCII  
- Codes 128-255: Mac-specific character mappings
- Includes Mac typography characters

### PDF Doc Encoding
- Based on WinAnsi with modifications for codes 128-160
- Optimized for PDF document text strings
- Enhanced symbol support

### Symbol/ZapfDingbats Encodings
- Mathematical symbols (Symbol)
- Decorative symbols and dingbats (ZapfDingbats)
- Unicode mappings for special characters

## Performance Benefits

### Before (Heavy .NET Processing)
```csharp
// Complex encoding logic in .NET
string DecodeText(byte[] bytes, FontInfo font) {
    // Load encoding tables from resources
    // Apply font-specific transformations
    // Handle differences arrays
    // Convert to Unicode
    // Build UTF-8 string
    // Handle special cases
}
```

### After (Simple Native Call)
```csharp
// Single native call
string DecodeText(byte[] bytes, IntPtr fontHandle) {
    PDF_DecodeTextWithFont(fontHandle, bytes, ...);
    return Marshal.PtrToStringUTF8(resultPtr);
}
```

### Performance Improvements
- **Embedded tables**: No file I/O or resource loading
- **Native optimization**: Optimized C++ character conversion
- **Reduced interop**: Single call instead of multiple operations
- **Memory efficiency**: Direct UTF-8 output, no intermediate conversions
- **Consistent behavior**: Same logic across all platforms

## Implementation Details

### File Structure
```
src/vanillapdf/semantics/objects/
├── character_encoding.h              # Base classes and interfaces
├── character_encoding.cpp            # Core implementation
├── character_encoding_tables.cpp     # StandardEncoding, MacRoman
└── character_encoding_tables2.cpp    # WinAnsi, PDFDoc, Symbol, etc.

include/vanillapdf/semantics/
└── c_font_encoding.h                 # C API declarations

src/vanillapdf/implementation/semantics/
└── c_font_encoding.cpp               # C API implementation
```

### Memory Management
- All encodings inherit from `IUnknown` for reference counting
- Automatic cleanup with RAII patterns
- Explicit memory management in C API with free functions
- Thread-safe reference counting

### Unicode Conversion
- Direct Unicode code point generation
- UTF-8 encoding for output strings
- Support for full Unicode range (U+0000 to U+10FFFF)
- Proper handling of multi-byte UTF-8 sequences

## Testing and Validation

### Test Cases Covered
- All base encoding types
- Differences array processing
- Unicode conversion accuracy
- Glyph name mappings
- Memory management
- Thread safety
- .NET interop

### Example Test Data
```cpp
// Czech text with special characters
uint8_t czech_text[] = {128, 129, 130, 131, 132, 133, 134, 135};
// Expected: áčťľíéňž in WinAnsi encoding
```

## Integration with Existing Code

### Backward Compatibility
- Existing `FontBase::ToUnicode()` method preserved
- New methods added without breaking changes
- C API follows existing patterns and conventions

### Font Class Extensions
```cpp
// New methods added to FontBase
std::unique_ptr<CharacterEncoding> GetEncoding() const;
std::string DecodeText(const uint8_t* codes, size_t count) const;
uint32_t DecodeCharacter(uint8_t code) const;
```

### Automatic Encoding Detection
The system automatically determines the appropriate encoding:
1. Check font dictionary for explicit `/Encoding` entry
2. Apply any `/Differences` modifications
3. Fall back to font-type default encoding
4. Handle special cases (Symbol fonts, etc.)

## Future Enhancements

### Potential Additions
- CMap support for CID fonts
- Additional expert encodings
- Custom glyph name dictionaries
- Encoding validation and diagnostics
- Performance profiling and optimization

### Extensibility
The modular design allows easy addition of new encodings by:
1. Inheriting from `CharacterEncoding`
2. Implementing `InitializeBaseEncoding()`
3. Adding to the factory methods
4. Updating C API enumerations

## Summary

This character encoding implementation provides a comprehensive, high-performance solution for PDF text extraction with:

- **Complete PDF specification compliance** - All standard encodings supported
- **Embedded character tables** - No external dependencies
- **Efficient native implementation** - Optimized for performance
- **Simple .NET integration** - Single function call for text decoding
- **Automatic difference handling** - Transparent encoding modifications
- **Memory safety** - Proper resource management and cleanup
- **Thread safety** - Safe for concurrent usage
- **Extensible design** - Easy to add new encodings

The system significantly simplifies PDF text extraction for .NET applications while providing better performance and consistency across platforms.