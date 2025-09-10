/**
 * \file character_encoding_example.c
 * \brief Example demonstrating the new character encoding API for PDF text decoding
 * 
 * This example shows how to use the enhanced font encoding system to decode
 * PDF text content with proper character mapping and Unicode conversion.
 */

#include "vanillapdf/c_vanillapdf_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error(error_type error) {
    if (error != VANILLAPDF_ERROR_SUCCESS) {
        printf("Error: %d\n", error);
    }
}

/**
 * \brief Example: Decode text using a specific font encoding
 */
void example_decode_text_with_encoding() {
    printf("=== Character Encoding Example ===\n");
    
    // Example byte data that might come from a PDF content stream
    // This represents Czech text with special characters
    uint8_t czech_text[] = {128, 129, 130, 131, 132, 133, 134, 135, 136, 137};
    size_t text_length = sizeof(czech_text);
    
    // Create different encoding types to demonstrate
    FontEncodingHandle* win_ansi_encoding = NULL;
    FontEncodingHandle* standard_encoding = NULL;
    FontEncodingHandle* pdf_doc_encoding = NULL;
    
    error_type error;
    
    // Create WinAnsiEncoding
    error = FontEncoding_CreateByType(FontEncodingType_WinAnsiEncoding, &win_ansi_encoding);
    if (error != VANILLAPDF_ERROR_SUCCESS) {
        print_error(error);
        return;
    }
    
    // Create StandardEncoding
    error = FontEncoding_CreateByType(FontEncodingType_StandardEncoding, &standard_encoding);
    if (error != VANILLAPDF_ERROR_SUCCESS) {
        print_error(error);
        goto cleanup;
    }
    
    // Create PDFDocEncoding
    error = FontEncoding_CreateByType(FontEncodingType_PDFDocEncoding, &pdf_doc_encoding);
    if (error != VANILLAPDF_ERROR_SUCCESS) {
        print_error(error);
        goto cleanup;
    }
    
    printf("Decoding byte sequence: ");
    for (size_t i = 0; i < text_length; i++) {
        printf("%d ", czech_text[i]);
    }
    printf("\n\n");
    
    // Demonstrate character-by-character decoding with different encodings
    printf("Character-by-character decoding:\n");
    printf("Code | WinAnsi | Standard | PDFDoc\n");
    printf("-----|---------|----------|-------\n");
    
    for (size_t i = 0; i < text_length; i++) {
        uint32_t win_unicode, std_unicode, pdf_unicode;
        
        FontEncoding_CodeToUnicode(win_ansi_encoding, czech_text[i], &win_unicode);
        FontEncoding_CodeToUnicode(standard_encoding, czech_text[i], &std_unicode);
        FontEncoding_CodeToUnicode(pdf_doc_encoding, czech_text[i], &pdf_unicode);
        
        printf("%3d  | U+%04X  | U+%04X   | U+%04X\n", 
               czech_text[i], win_unicode, std_unicode, pdf_unicode);
    }
    
    printf("\nGlyph names for WinAnsi encoding:\n");
    for (size_t i = 0; i < text_length; i++) {
        char* glyph_name = NULL;
        error = FontEncoding_CodeToGlyphName(win_ansi_encoding, czech_text[i], &glyph_name);
        if (error == VANILLAPDF_ERROR_SUCCESS && glyph_name) {
            printf("Code %d: %s\n", czech_text[i], glyph_name);
            FontEncoding_FreeGlyphName(glyph_name);
        }
    }

cleanup:
    if (win_ansi_encoding) FontEncoding_Release(win_ansi_encoding);
    if (standard_encoding) FontEncoding_Release(standard_encoding);
    if (pdf_doc_encoding) FontEncoding_Release(pdf_doc_encoding);
}

/**
 * \brief Example: Demonstrate encoding differences
 */
void example_encoding_differences() {
    printf("\n=== Encoding Differences Example ===\n");
    
    // This would typically be done when parsing a PDF font dictionary
    // that contains a /Differences array like:
    // /Differences [128 /aacute 129 /ccaron 130 /tcaron ...]
    
    printf("This example shows how the Differences array modifies base encodings.\n");
    printf("In a real PDF, the differences would be parsed from the font dictionary.\n");
    printf("The API handles this automatically when creating encodings from PDF objects.\n");
}

/**
 * \brief Example: High-level text decoding function for .NET interop
 */
void example_high_level_decoding() {
    printf("\n=== High-Level Text Decoding Example ===\n");
    
    // This is the primary function that .NET applications would use
    printf("The PDF_DecodeTextWithFont function provides a simple interface for .NET:\n");
    printf("\n");
    printf("// C# P/Invoke declaration:\n");
    printf("[DllImport(\"vanillapdf.dll\")]\n");
    printf("public static extern int PDF_DecodeTextWithFont(\n");
    printf("    IntPtr fontHandle,\n");
    printf("    byte[] byteData,\n");
    printf("    UIntPtr byteCount,\n");
    printf("    out IntPtr decodedText,\n");
    printf("    out UIntPtr textLength);\n");
    printf("\n");
    printf("// Usage in C#:\n");
    printf("byte[] pdfBytes = GetBytesFromPDFContent();\n");
    printf("IntPtr decodedTextPtr;\n");
    printf("UIntPtr textLength;\n");
    printf("int result = PDF_DecodeTextWithFont(fontHandle, pdfBytes, \n");
    printf("    (UIntPtr)pdfBytes.Length, out decodedTextPtr, out textLength);\n");
    printf("string text = Marshal.PtrToStringUTF8(decodedTextPtr, (int)textLength);\n");
    printf("PDF_FreeDecodedText(decodedTextPtr);\n");
}

/**
 * \brief Example: Supported encodings overview
 */
void example_supported_encodings() {
    printf("\n=== Supported Encodings Overview ===\n");
    
    const char* encodings[] = {
        "StandardEncoding - Adobe Standard Encoding (default for Type1 fonts)",
        "MacRomanEncoding - Mac OS Roman encoding",
        "WinAnsiEncoding - Windows Code Page 1252 (default for TrueType fonts)", 
        "PDFDocEncoding - PDF document encoding (superset of WinAnsi)",
        "MacExpertEncoding - Mac Expert encoding for typography",
        "SymbolEncoding - Symbol font encoding",
        "ZapfDingbatsEncoding - ZapfDingbats symbol encoding"
    };
    
    for (size_t i = 0; i < sizeof(encodings) / sizeof(encodings[0]); i++) {
        printf("• %s\n", encodings[i]);
    }
    
    printf("\nAll encodings support:\n");
    printf("• Character code to Unicode conversion\n");
    printf("• Character code to glyph name mapping\n");
    printf("• Differences array processing\n");
    printf("• UTF-8 text output\n");
}

int main() {
    printf("VanillaPDF Character Encoding API Examples\n");
    printf("==========================================\n\n");
    
    example_decode_text_with_encoding();
    example_encoding_differences();
    example_high_level_decoding();
    example_supported_encodings();
    
    printf("\n=== Integration Notes ===\n");
    printf("The character encoding system integrates seamlessly with existing font classes:\n");
    printf("• Font_DecodeText() - Decode byte array to UTF-8 text\n");
    printf("• Font_DecodeCharacter() - Decode single character to Unicode\n");
    printf("• Font_GetEncoding() - Get the font's character encoding\n");
    printf("\nThis moves the heavy text decoding logic from .NET to the native library,\n");
    printf("providing better performance and consistent behavior across platforms.\n");
    
    return 0;
}