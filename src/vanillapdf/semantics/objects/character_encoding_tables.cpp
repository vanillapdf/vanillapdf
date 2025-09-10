#include "precompiled.h"
#include "semantics/objects/character_encoding.h"

namespace vanillapdf {
namespace semantics {

// Initialize glyph name to Unicode mappings
void CharacterEncoding::InitializeGlyphNameMappings() {
    if (s_glyph_mappings_initialized) return;
    
    // Common glyph names to Unicode mappings based on Adobe Glyph List
    s_glyph_name_to_unicode = {
        // Basic Latin
        {"space", 0x0020}, {"exclam", 0x0021}, {"quotedbl", 0x0022}, {"numbersign", 0x0023},
        {"dollar", 0x0024}, {"percent", 0x0025}, {"ampersand", 0x0026}, {"quotesingle", 0x0027},
        {"parenleft", 0x0028}, {"parenright", 0x0029}, {"asterisk", 0x002A}, {"plus", 0x002B},
        {"comma", 0x002C}, {"hyphen", 0x002D}, {"period", 0x002E}, {"slash", 0x002F},
        {"zero", 0x0030}, {"one", 0x0031}, {"two", 0x0032}, {"three", 0x0033},
        {"four", 0x0034}, {"five", 0x0035}, {"six", 0x0036}, {"seven", 0x0037},
        {"eight", 0x0038}, {"nine", 0x0039}, {"colon", 0x003A}, {"semicolon", 0x003B},
        {"less", 0x003C}, {"equal", 0x003D}, {"greater", 0x003E}, {"question", 0x003F},
        {"at", 0x0040}, {"A", 0x0041}, {"B", 0x0042}, {"C", 0x0043}, {"D", 0x0044},
        {"E", 0x0045}, {"F", 0x0046}, {"G", 0x0047}, {"H", 0x0048}, {"I", 0x0049},
        {"J", 0x004A}, {"K", 0x004B}, {"L", 0x004C}, {"M", 0x004D}, {"N", 0x004E},
        {"O", 0x004F}, {"P", 0x0050}, {"Q", 0x0051}, {"R", 0x0052}, {"S", 0x0053},
        {"T", 0x0054}, {"U", 0x0055}, {"V", 0x0056}, {"W", 0x0057}, {"X", 0x0058},
        {"Y", 0x0059}, {"Z", 0x005A}, {"bracketleft", 0x005B}, {"backslash", 0x005C},
        {"bracketright", 0x005D}, {"asciicircum", 0x005E}, {"underscore", 0x005F},
        {"grave", 0x0060}, {"a", 0x0061}, {"b", 0x0062}, {"c", 0x0063}, {"d", 0x0064},
        {"e", 0x0065}, {"f", 0x0066}, {"g", 0x0067}, {"h", 0x0068}, {"i", 0x0069},
        {"j", 0x006A}, {"k", 0x006B}, {"l", 0x006C}, {"m", 0x006D}, {"n", 0x006E},
        {"o", 0x006F}, {"p", 0x0070}, {"q", 0x0071}, {"r", 0x0072}, {"s", 0x0073},
        {"t", 0x0074}, {"u", 0x0075}, {"v", 0x0076}, {"w", 0x0077}, {"x", 0x0078},
        {"y", 0x0079}, {"z", 0x007A}, {"braceleft", 0x007B}, {"bar", 0x007C},
        {"braceright", 0x007D}, {"asciitilde", 0x007E},
        
        // Latin-1 Supplement
        {"exclamdown", 0x00A1}, {"cent", 0x00A2}, {"sterling", 0x00A3}, {"currency", 0x00A4},
        {"yen", 0x00A5}, {"brokenbar", 0x00A6}, {"section", 0x00A7}, {"dieresis", 0x00A8},
        {"copyright", 0x00A9}, {"ordfeminine", 0x00AA}, {"guillemotleft", 0x00AB},
        {"logicalnot", 0x00AC}, {"registered", 0x00AE}, {"macron", 0x00AF},
        {"degree", 0x00B0}, {"plusminus", 0x00B1}, {"twosuperior", 0x00B2},
        {"threesuperior", 0x00B3}, {"acute", 0x00B4}, {"mu", 0x00B5}, {"paragraph", 0x00B6},
        {"periodcentered", 0x00B7}, {"cedilla", 0x00B8}, {"onesuperior", 0x00B9},
        {"ordmasculine", 0x00BA}, {"guillemotright", 0x00BB}, {"onequarter", 0x00BC},
        {"onehalf", 0x00BD}, {"threequarters", 0x00BE}, {"questiondown", 0x00BF},
        
        // Extended Latin characters
        {"Agrave", 0x00C0}, {"Aacute", 0x00C1}, {"Acircumflex", 0x00C2}, {"Atilde", 0x00C3},
        {"Adieresis", 0x00C4}, {"Aring", 0x00C5}, {"AE", 0x00C6}, {"Ccedilla", 0x00C7},
        {"Egrave", 0x00C8}, {"Eacute", 0x00C9}, {"Ecircumflex", 0x00CA}, {"Edieresis", 0x00CB},
        {"Igrave", 0x00CC}, {"Iacute", 0x00CD}, {"Icircumflex", 0x00CE}, {"Idieresis", 0x00CF},
        {"Eth", 0x00D0}, {"Ntilde", 0x00D1}, {"Ograve", 0x00D2}, {"Oacute", 0x00D3},
        {"Ocircumflex", 0x00D4}, {"Otilde", 0x00D5}, {"Odieresis", 0x00D6}, {"multiply", 0x00D7},
        {"Oslash", 0x00D8}, {"Ugrave", 0x00D9}, {"Uacute", 0x00DA}, {"Ucircumflex", 0x00DB},
        {"Udieresis", 0x00DC}, {"Yacute", 0x00DD}, {"Thorn", 0x00DE}, {"germandbls", 0x00DF},
        {"agrave", 0x00E0}, {"aacute", 0x00E1}, {"acircumflex", 0x00E2}, {"atilde", 0x00E3},
        {"adieresis", 0x00E4}, {"aring", 0x00E5}, {"ae", 0x00E6}, {"ccedilla", 0x00E7},
        {"egrave", 0x00E8}, {"eacute", 0x00E9}, {"ecircumflex", 0x00EA}, {"edieresis", 0x00EB},
        {"igrave", 0x00EC}, {"iacute", 0x00ED}, {"icircumflex", 0x00EE}, {"idieresis", 0x00EF},
        {"eth", 0x00F0}, {"ntilde", 0x00F1}, {"ograve", 0x00F2}, {"oacute", 0x00F3},
        {"ocircumflex", 0x00F4}, {"otilde", 0x00F5}, {"odieresis", 0x00F6}, {"divide", 0x00F7},
        {"oslash", 0x00F8}, {"ugrave", 0x00F9}, {"uacute", 0x00FA}, {"ucircumflex", 0x00FB},
        {"udieresis", 0x00FC}, {"yacute", 0x00FD}, {"thorn", 0x00FE}, {"ydieresis", 0x00FF},
        
        // Additional common characters
        {"Euro", 0x20AC}, {"florin", 0x0192}, {"quoteleft", 0x2018}, {"quoteright", 0x2019},
        {"quotedblleft", 0x201C}, {"quotedblright", 0x201D}, {"bullet", 0x2022},
        {"endash", 0x2013}, {"emdash", 0x2014}, {"tilde", 0x02DC}, {"trademark", 0x2122},
        
        // Czech characters for the example
        {"ccaron", 0x010D}, {"tcaron", 0x0165}, {"lcaron", 0x013E}, {"ncaron", 0x0148},
        {"zcaron", 0x017E}, {"scaron", 0x0161}
    };
    
    s_glyph_mappings_initialized = true;
}

// StandardEncoding implementation (Adobe Standard Encoding)
void StandardEncoding::InitializeBaseEncoding() {
    // Initialize all entries to 0/empty first
    for (int i = 0; i < 256; i++) {
        SetEncodingEntry(static_cast<uint8_t>(i), 0, "");
    }
    
    // Standard Encoding table (codes 32-126 are standard ASCII)
    SetEncodingEntry(32, 0x0020, "space");
    SetEncodingEntry(33, 0x0021, "exclam");
    SetEncodingEntry(34, 0x0022, "quotedbl");
    SetEncodingEntry(35, 0x0023, "numbersign");
    SetEncodingEntry(36, 0x0024, "dollar");
    SetEncodingEntry(37, 0x0025, "percent");
    SetEncodingEntry(38, 0x0026, "ampersand");
    SetEncodingEntry(39, 0x2019, "quoteright");
    SetEncodingEntry(40, 0x0028, "parenleft");
    SetEncodingEntry(41, 0x0029, "parenright");
    SetEncodingEntry(42, 0x002A, "asterisk");
    SetEncodingEntry(43, 0x002B, "plus");
    SetEncodingEntry(44, 0x002C, "comma");
    SetEncodingEntry(45, 0x002D, "hyphen");
    SetEncodingEntry(46, 0x002E, "period");
    SetEncodingEntry(47, 0x002F, "slash");
    
    // Numbers 0-9
    for (int i = 0; i < 10; i++) {
        SetEncodingEntry(48 + i, 0x0030 + i, std::string(1, '0' + i));
    }
    
    SetEncodingEntry(58, 0x003A, "colon");
    SetEncodingEntry(59, 0x003B, "semicolon");
    SetEncodingEntry(60, 0x003C, "less");
    SetEncodingEntry(61, 0x003D, "equal");
    SetEncodingEntry(62, 0x003E, "greater");
    SetEncodingEntry(63, 0x003F, "question");
    SetEncodingEntry(64, 0x0040, "at");
    
    // Uppercase A-Z
    for (int i = 0; i < 26; i++) {
        SetEncodingEntry(65 + i, 0x0041 + i, std::string(1, 'A' + i));
    }
    
    SetEncodingEntry(91, 0x005B, "bracketleft");
    SetEncodingEntry(92, 0x005C, "backslash");
    SetEncodingEntry(93, 0x005D, "bracketright");
    SetEncodingEntry(94, 0x005E, "asciicircum");
    SetEncodingEntry(95, 0x005F, "underscore");
    SetEncodingEntry(96, 0x2018, "quoteleft");
    
    // Lowercase a-z
    for (int i = 0; i < 26; i++) {
        SetEncodingEntry(97 + i, 0x0061 + i, std::string(1, 'a' + i));
    }
    
    SetEncodingEntry(123, 0x007B, "braceleft");
    SetEncodingEntry(124, 0x007C, "bar");
    SetEncodingEntry(125, 0x007D, "braceright");
    SetEncodingEntry(126, 0x007E, "asciitilde");
    
    // Extended characters in Standard Encoding
    SetEncodingEntry(161, 0x00A1, "exclamdown");
    SetEncodingEntry(162, 0x00A2, "cent");
    SetEncodingEntry(163, 0x00A3, "sterling");
    SetEncodingEntry(164, 0x2044, "fraction");
    SetEncodingEntry(165, 0x00A5, "yen");
    SetEncodingEntry(166, 0x0192, "florin");
    SetEncodingEntry(167, 0x00A7, "section");
    SetEncodingEntry(168, 0x00A4, "currency");
    SetEncodingEntry(169, 0x0027, "quotesingle");
    SetEncodingEntry(170, 0x201C, "quotedblleft");
    SetEncodingEntry(171, 0x00AB, "guillemotleft");
    SetEncodingEntry(172, 0x2039, "guilsinglleft");
    SetEncodingEntry(173, 0x203A, "guilsinglright");
    SetEncodingEntry(174, 0xFB01, "fi");
    SetEncodingEntry(175, 0xFB02, "fl");
    SetEncodingEntry(177, 0x2013, "endash");
    SetEncodingEntry(178, 0x2020, "dagger");
    SetEncodingEntry(179, 0x2021, "daggerdbl");
    SetEncodingEntry(180, 0x00B7, "periodcentered");
    SetEncodingEntry(182, 0x00B6, "paragraph");
    SetEncodingEntry(183, 0x2022, "bullet");
    SetEncodingEntry(184, 0x201A, "quotesinglbase");
    SetEncodingEntry(185, 0x201E, "quotedblbase");
    SetEncodingEntry(186, 0x201D, "quotedblright");
    SetEncodingEntry(187, 0x00BB, "guillemotright");
    SetEncodingEntry(188, 0x2026, "ellipsis");
    SetEncodingEntry(189, 0x2030, "perthousand");
    SetEncodingEntry(191, 0x00BF, "questiondown");
    SetEncodingEntry(193, 0x0060, "grave");
    SetEncodingEntry(194, 0x00B4, "acute");
    SetEncodingEntry(195, 0x02C6, "circumflex");
    SetEncodingEntry(196, 0x02DC, "tilde");
    SetEncodingEntry(197, 0x00AF, "macron");
    SetEncodingEntry(198, 0x02D8, "breve");
    SetEncodingEntry(199, 0x02D9, "dotaccent");
    SetEncodingEntry(200, 0x00A8, "dieresis");
    SetEncodingEntry(202, 0x02DA, "ring");
    SetEncodingEntry(203, 0x00B8, "cedilla");
    SetEncodingEntry(205, 0x02DD, "hungarumlaut");
    SetEncodingEntry(206, 0x02DB, "ogonek");
    SetEncodingEntry(207, 0x02C7, "caron");
    SetEncodingEntry(208, 0x2014, "emdash");
    SetEncodingEntry(225, 0x00C6, "AE");
    SetEncodingEntry(227, 0x00AA, "ordfeminine");
    SetEncodingEntry(232, 0x0141, "Lslash");
    SetEncodingEntry(233, 0x00D8, "Oslash");
    SetEncodingEntry(234, 0x0152, "OE");
    SetEncodingEntry(235, 0x00BA, "ordmasculine");
    SetEncodingEntry(241, 0x00E6, "ae");
    SetEncodingEntry(245, 0x0131, "dotlessi");
    SetEncodingEntry(248, 0x0142, "lslash");
    SetEncodingEntry(249, 0x00F8, "oslash");
    SetEncodingEntry(250, 0x0153, "oe");
    SetEncodingEntry(251, 0x00DF, "germandbls");
}

// MacRomanEncoding implementation
void MacRomanEncoding::InitializeBaseEncoding() {
    // Initialize all entries to 0/empty first
    for (int i = 0; i < 256; i++) {
        SetEncodingEntry(static_cast<uint8_t>(i), 0, "");
    }
    
    // Mac Roman encoding - first 128 characters are same as ASCII
    for (int i = 0; i < 128; i++) {
        if (i >= 32 && i <= 126) {
            // Printable ASCII characters
            SetEncodingEntry(i, i, "");  // Will be filled with proper glyph names
        }
    }
    
    // Standard ASCII printable characters
    SetEncodingEntry(32, 0x0020, "space");
    SetEncodingEntry(33, 0x0021, "exclam");
    // ... (same as StandardEncoding for 32-126)
    
    // Mac Roman specific mappings for 128-255
    SetEncodingEntry(128, 0x00C4, "Adieresis");
    SetEncodingEntry(129, 0x00C5, "Aring");
    SetEncodingEntry(130, 0x00C7, "Ccedilla");
    SetEncodingEntry(131, 0x00C9, "Eacute");
    SetEncodingEntry(132, 0x00D1, "Ntilde");
    SetEncodingEntry(133, 0x00D6, "Odieresis");
    SetEncodingEntry(134, 0x00DC, "Udieresis");
    SetEncodingEntry(135, 0x00E1, "aacute");
    SetEncodingEntry(136, 0x00E0, "agrave");
    SetEncodingEntry(137, 0x00E2, "acircumflex");
    SetEncodingEntry(138, 0x00E4, "adieresis");
    SetEncodingEntry(139, 0x00E3, "atilde");
    SetEncodingEntry(140, 0x00E5, "aring");
    SetEncodingEntry(141, 0x00E7, "ccedilla");
    SetEncodingEntry(142, 0x00E9, "eacute");
    SetEncodingEntry(143, 0x00E8, "egrave");
    SetEncodingEntry(144, 0x00EA, "ecircumflex");
    SetEncodingEntry(145, 0x00EB, "edieresis");
    SetEncodingEntry(146, 0x00ED, "iacute");
    SetEncodingEntry(147, 0x00EC, "igrave");
    SetEncodingEntry(148, 0x00EE, "icircumflex");
    SetEncodingEntry(149, 0x00EF, "idieresis");
    SetEncodingEntry(150, 0x00F1, "ntilde");
    SetEncodingEntry(151, 0x00F3, "oacute");
    SetEncodingEntry(152, 0x00F2, "ograve");
    SetEncodingEntry(153, 0x00F4, "ocircumflex");
    SetEncodingEntry(154, 0x00F6, "odieresis");
    SetEncodingEntry(155, 0x00F5, "otilde");
    SetEncodingEntry(156, 0x00FA, "uacute");
    SetEncodingEntry(157, 0x00F9, "ugrave");
    SetEncodingEntry(158, 0x00FB, "ucircumflex");
    SetEncodingEntry(159, 0x00FC, "udieresis");
    SetEncodingEntry(160, 0x2020, "dagger");
    SetEncodingEntry(161, 0x00B0, "degree");
    SetEncodingEntry(162, 0x00A2, "cent");
    SetEncodingEntry(163, 0x00A3, "sterling");
    SetEncodingEntry(164, 0x00A7, "section");
    SetEncodingEntry(165, 0x2022, "bullet");
    SetEncodingEntry(166, 0x00B6, "paragraph");
    SetEncodingEntry(167, 0x00DF, "germandbls");
    SetEncodingEntry(168, 0x00AE, "registered");
    SetEncodingEntry(169, 0x00A9, "copyright");
    SetEncodingEntry(170, 0x2122, "trademark");
    SetEncodingEntry(171, 0x00B4, "acute");
    SetEncodingEntry(172, 0x00A8, "dieresis");
    SetEncodingEntry(173, 0x2260, "notequal");
    SetEncodingEntry(174, 0x00C6, "AE");
    SetEncodingEntry(175, 0x00D8, "Oslash");
    // ... continue with remaining Mac Roman characters
}

} // semantics
} // vanillapdf