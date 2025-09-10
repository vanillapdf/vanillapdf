/**
 * \file CharacterEncodingExample.cs
 * \brief C# example demonstrating the new character encoding API for PDF text decoding
 * 
 * This example shows how .NET applications can use the enhanced font encoding 
 * system to decode PDF text content with proper character mapping.
 */

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace VanillaPDF.Examples
{
    /// <summary>
    /// P/Invoke declarations for the VanillaPDF character encoding API
    /// </summary>
    public static class VanillaPdfNative
    {
        private const string DllName = "vanillapdf.dll";

        // High-level text decoding function - primary function for .NET applications
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int PDF_DecodeTextWithFont(
            IntPtr fontHandle,
            byte[] byteData,
            UIntPtr byteCount,
            out IntPtr decodedText,
            out UIntPtr textLength);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int PDF_FreeDecodedText(IntPtr text);

        // Font encoding functions
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int FontEncoding_CreateByType(
            FontEncodingType type,
            out IntPtr encodingHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int FontEncoding_CodeToUnicode(
            IntPtr encodingHandle,
            byte code,
            out uint unicode);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int FontEncoding_CodeToGlyphName(
            IntPtr encodingHandle,
            byte code,
            out IntPtr glyphName);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int FontEncoding_FreeGlyphName(IntPtr glyphName);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int FontEncoding_Release(IntPtr encodingHandle);

        // Font functions
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Font_DecodeText(
            IntPtr fontHandle,
            byte[] codes,
            UIntPtr count,
            out IntPtr result,
            out UIntPtr resultLength);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Font_DecodeCharacter(
            IntPtr fontHandle,
            byte code,
            out uint result);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Font_GetEncoding(
            IntPtr fontHandle,
            out IntPtr encodingHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Font_FreeDecodedText(IntPtr text);
    }

    /// <summary>
    /// Font encoding types supported by VanillaPDF
    /// </summary>
    public enum FontEncodingType
    {
        Undefined = 0,
        StandardEncoding,
        MacRomanEncoding,
        WinAnsiEncoding,
        PDFDocEncoding,
        MacExpertEncoding,
        SymbolEncoding,
        ZapfDingbatsEncoding,
        Custom
    }

    /// <summary>
    /// Managed wrapper for VanillaPDF font encoding functionality
    /// </summary>
    public class FontEncoding : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        public FontEncoding(FontEncodingType type)
        {
            int result = VanillaPdfNative.FontEncoding_CreateByType(type, out _handle);
            if (result != 0)
            {
                throw new InvalidOperationException($"Failed to create font encoding: {result}");
            }
        }

        /// <summary>
        /// Convert character code to Unicode code point
        /// </summary>
        public uint CodeToUnicode(byte code)
        {
            ThrowIfDisposed();
            
            int result = VanillaPdfNative.FontEncoding_CodeToUnicode(_handle, code, out uint unicode);
            if (result != 0)
            {
                throw new InvalidOperationException($"Failed to convert code to Unicode: {result}");
            }
            
            return unicode;
        }

        /// <summary>
        /// Convert character code to PostScript glyph name
        /// </summary>
        public string CodeToGlyphName(byte code)
        {
            ThrowIfDisposed();
            
            int result = VanillaPdfNative.FontEncoding_CodeToGlyphName(_handle, code, out IntPtr glyphNamePtr);
            if (result != 0)
            {
                throw new InvalidOperationException($"Failed to convert code to glyph name: {result}");
            }

            try
            {
                return Marshal.PtrToStringAnsi(glyphNamePtr) ?? string.Empty;
            }
            finally
            {
                VanillaPdfNative.FontEncoding_FreeGlyphName(glyphNamePtr);
            }
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(FontEncoding));
            }
        }

        public void Dispose()
        {
            if (!_disposed && _handle != IntPtr.Zero)
            {
                VanillaPdfNative.FontEncoding_Release(_handle);
                _handle = IntPtr.Zero;
                _disposed = true;
            }
        }
    }

    /// <summary>
    /// Example class demonstrating character encoding usage
    /// </summary>
    public class CharacterEncodingExample
    {
        /// <summary>
        /// Demonstrate basic encoding functionality
        /// </summary>
        public static void BasicEncodingExample()
        {
            Console.WriteLine("=== Basic Character Encoding Example ===");

            // Example: Czech text with special characters
            byte[] czechBytes = { 128, 129, 130, 131, 132, 133, 134, 135, 136, 137 };

            using (var winAnsiEncoding = new FontEncoding(FontEncodingType.WinAnsiEncoding))
            using (var standardEncoding = new FontEncoding(FontEncodingType.StandardEncoding))
            {
                Console.WriteLine("Byte sequence: " + string.Join(" ", czechBytes));
                Console.WriteLine();

                Console.WriteLine("Character-by-character decoding:");
                Console.WriteLine("Code | WinAnsi | Standard | Glyph Name");
                Console.WriteLine("-----|---------|----------|------------");

                foreach (byte code in czechBytes)
                {
                    uint winUnicode = winAnsiEncoding.CodeToUnicode(code);
                    uint stdUnicode = standardEncoding.CodeToUnicode(code);
                    string glyphName = winAnsiEncoding.CodeToGlyphName(code);

                    Console.WriteLine($"{code,3}  | U+{winUnicode:X4}  | U+{stdUnicode:X4}   | {glyphName}");
                }
            }
        }

        /// <summary>
        /// Demonstrate high-level text decoding with a font
        /// </summary>
        public static void HighLevelDecodingExample(IntPtr fontHandle)
        {
            Console.WriteLine("\n=== High-Level Text Decoding Example ===");

            if (fontHandle == IntPtr.Zero)
            {
                Console.WriteLine("Note: This example requires a valid font handle from a PDF document.");
                Console.WriteLine("The font handle would typically be obtained by parsing a PDF file.");
                return;
            }

            // Example byte data from PDF content stream
            byte[] pdfTextBytes = { 72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100 }; // "Hello World"

            try
            {
                // Use the high-level decoding function
                int result = VanillaPdfNative.PDF_DecodeTextWithFont(
                    fontHandle,
                    pdfTextBytes,
                    (UIntPtr)pdfTextBytes.Length,
                    out IntPtr decodedTextPtr,
                    out UIntPtr textLength);

                if (result == 0 && decodedTextPtr != IntPtr.Zero)
                {
                    string decodedText = Marshal.PtrToStringUTF8(decodedTextPtr, (int)textLength);
                    Console.WriteLine($"Decoded text: '{decodedText}'");
                    
                    // Always free the allocated memory
                    VanillaPdfNative.PDF_FreeDecodedText(decodedTextPtr);
                }
                else
                {
                    Console.WriteLine($"Failed to decode text: {result}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error during text decoding: {ex.Message}");
            }
        }

        /// <summary>
        /// Demonstrate encoding differences handling
        /// </summary>
        public static void EncodingDifferencesExample()
        {
            Console.WriteLine("\n=== Encoding Differences Example ===");

            Console.WriteLine("PDF fonts can specify encoding differences to modify base encodings:");
            Console.WriteLine();
            Console.WriteLine("Example PDF font dictionary:");
            Console.WriteLine("20 0 obj");
            Console.WriteLine("<</Type /Font");
            Console.WriteLine("  /Subtype /TrueType");
            Console.WriteLine("  /BaseFont /Arial,BoldItalic");
            Console.WriteLine("  /Encoding <</Type /Encoding");
            Console.WriteLine("            /BaseEncoding /WinAnsiEncoding");
            Console.WriteLine("            /Differences [128 /aacute 129 /ccaron 130 /tcaron ...]>>");
            Console.WriteLine("  /ToUnicode 37 0 R");
            Console.WriteLine(">>");
            Console.WriteLine("endobj");
            Console.WriteLine();
            Console.WriteLine("The VanillaPDF encoding system automatically handles these differences");
            Console.WriteLine("when creating encodings from PDF font dictionaries.");
        }

        /// <summary>
        /// Show performance benefits of native decoding
        /// </summary>
        public static void PerformanceExample()
        {
            Console.WriteLine("\n=== Performance Benefits ===");

            Console.WriteLine("Benefits of native character encoding:");
            Console.WriteLine("• Embedded encoding tables - no external files needed");
            Console.WriteLine("• Optimized Unicode conversion in native code");
            Console.WriteLine("• Reduced .NET/native interop overhead");
            Console.WriteLine("• Consistent behavior across platforms");
            Console.WriteLine("• Automatic handling of encoding differences");
            Console.WriteLine("• Support for all PDF encoding types");
            Console.WriteLine();
            Console.WriteLine("Before: Heavy text processing logic in .NET");
            Console.WriteLine("After: Simple byte array → UTF-8 string conversion");
        }

        public static void Main(string[] args)
        {
            Console.WriteLine("VanillaPDF Character Encoding API - C# Examples");
            Console.WriteLine("=================================================");
            Console.WriteLine();

            try
            {
                BasicEncodingExample();
                HighLevelDecodingExample(IntPtr.Zero); // Pass actual font handle in real usage
                EncodingDifferencesExample();
                PerformanceExample();

                Console.WriteLine("\n=== Integration Summary ===");
                Console.WriteLine("The character encoding system provides a comprehensive solution for");
                Console.WriteLine("PDF text extraction with proper Unicode handling and encoding support.");
                Console.WriteLine("All encoding complexity is handled in the native library, providing");
                Console.WriteLine("a simple and efficient API for .NET applications.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }

            Console.WriteLine("\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}