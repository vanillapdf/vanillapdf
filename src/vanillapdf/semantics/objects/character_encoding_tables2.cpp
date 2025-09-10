#include "precompiled.h"
#include "semantics/objects/character_encoding.h"

namespace vanillapdf {
namespace semantics {

// WinAnsiEncoding implementation (Windows Code Page 1252)
void WinAnsiEncoding::InitializeBaseEncoding() {
    // Initialize all entries to 0/empty first
    for (int i = 0; i < 256; i++) {
        SetEncodingEntry(static_cast<uint8_t>(i), 0, "");
    }
    
    // WinAnsi encoding - first 128 characters are same as ASCII
    for (int i = 32; i <= 126; i++) {
        SetEncodingEntry(i, i, ""); // Will be filled with proper glyph names
    }
    
    // Standard ASCII printable characters (same as StandardEncoding)
    SetEncodingEntry(32, 0x0020, "space");
    SetEncodingEntry(33, 0x0021, "exclam");
    SetEncodingEntry(34, 0x0022, "quotedbl");
    SetEncodingEntry(35, 0x0023, "numbersign");
    SetEncodingEntry(36, 0x0024, "dollar");
    SetEncodingEntry(37, 0x0025, "percent");
    SetEncodingEntry(38, 0x0026, "ampersand");
    SetEncodingEntry(39, 0x0027, "quotesingle");
    SetEncodingEntry(40, 0x0028, "parenleft");
    SetEncodingEntry(41, 0x0029, "parenright");
    SetEncodingEntry(42, 0x002A, "asterisk");
    SetEncodingEntry(43, 0x002B, "plus");
    SetEncodingEntry(44, 0x002C, "comma");
    SetEncodingEntry(45, 0x002D, "hyphen");
    SetEncodingEntry(46, 0x002E, "period");
    SetEncodingEntry(47, 0x002F, "slash");
    
    // Numbers and letters (same as ASCII)
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
    
    for (int i = 0; i < 26; i++) {
        SetEncodingEntry(65 + i, 0x0041 + i, std::string(1, 'A' + i));
        SetEncodingEntry(97 + i, 0x0061 + i, std::string(1, 'a' + i));
    }
    
    SetEncodingEntry(91, 0x005B, "bracketleft");
    SetEncodingEntry(92, 0x005C, "backslash");
    SetEncodingEntry(93, 0x005D, "bracketright");
    SetEncodingEntry(94, 0x005E, "asciicircum");
    SetEncodingEntry(95, 0x005F, "underscore");
    SetEncodingEntry(96, 0x0060, "grave");
    SetEncodingEntry(123, 0x007B, "braceleft");
    SetEncodingEntry(124, 0x007C, "bar");
    SetEncodingEntry(125, 0x007D, "braceright");
    SetEncodingEntry(126, 0x007E, "asciitilde");
    
    // WinAnsi specific mappings for 128-255 (Windows Code Page 1252)
    SetEncodingEntry(128, 0x20AC, "Euro");
    // 129 is undefined in CP1252
    SetEncodingEntry(130, 0x201A, "quotesinglbase");
    SetEncodingEntry(131, 0x0192, "florin");
    SetEncodingEntry(132, 0x201E, "quotedblbase");
    SetEncodingEntry(133, 0x2026, "ellipsis");
    SetEncodingEntry(134, 0x2020, "dagger");
    SetEncodingEntry(135, 0x2021, "daggerdbl");
    SetEncodingEntry(136, 0x02C6, "circumflex");
    SetEncodingEntry(137, 0x2030, "perthousand");
    SetEncodingEntry(138, 0x0160, "Scaron");
    SetEncodingEntry(139, 0x2039, "guilsinglleft");
    SetEncodingEntry(140, 0x0152, "OE");
    // 141 is undefined in CP1252
    SetEncodingEntry(142, 0x017D, "Zcaron");
    // 143 is undefined in CP1252
    // 144 is undefined in CP1252
    SetEncodingEntry(145, 0x2018, "quoteleft");
    SetEncodingEntry(146, 0x2019, "quoteright");
    SetEncodingEntry(147, 0x201C, "quotedblleft");
    SetEncodingEntry(148, 0x201D, "quotedblright");
    SetEncodingEntry(149, 0x2022, "bullet");
    SetEncodingEntry(150, 0x2013, "endash");
    SetEncodingEntry(151, 0x2014, "emdash");
    SetEncodingEntry(152, 0x02DC, "tilde");
    SetEncodingEntry(153, 0x2122, "trademark");
    SetEncodingEntry(154, 0x0161, "scaron");
    SetEncodingEntry(155, 0x203A, "guilsinglright");
    SetEncodingEntry(156, 0x0153, "oe");
    // 157 is undefined in CP1252
    SetEncodingEntry(158, 0x017E, "zcaron");
    SetEncodingEntry(159, 0x0178, "Ydieresis");
    
    // Latin-1 Supplement (160-255)
    SetEncodingEntry(160, 0x00A0, "space"); // Non-breaking space
    SetEncodingEntry(161, 0x00A1, "exclamdown");
    SetEncodingEntry(162, 0x00A2, "cent");
    SetEncodingEntry(163, 0x00A3, "sterling");
    SetEncodingEntry(164, 0x00A4, "currency");
    SetEncodingEntry(165, 0x00A5, "yen");
    SetEncodingEntry(166, 0x00A6, "brokenbar");
    SetEncodingEntry(167, 0x00A7, "section");
    SetEncodingEntry(168, 0x00A8, "dieresis");
    SetEncodingEntry(169, 0x00A9, "copyright");
    SetEncodingEntry(170, 0x00AA, "ordfeminine");
    SetEncodingEntry(171, 0x00AB, "guillemotleft");
    SetEncodingEntry(172, 0x00AC, "logicalnot");
    SetEncodingEntry(173, 0x00AD, "hyphen"); // Soft hyphen
    SetEncodingEntry(174, 0x00AE, "registered");
    SetEncodingEntry(175, 0x00AF, "macron");
    SetEncodingEntry(176, 0x00B0, "degree");
    SetEncodingEntry(177, 0x00B1, "plusminus");
    SetEncodingEntry(178, 0x00B2, "twosuperior");
    SetEncodingEntry(179, 0x00B3, "threesuperior");
    SetEncodingEntry(180, 0x00B4, "acute");
    SetEncodingEntry(181, 0x00B5, "mu");
    SetEncodingEntry(182, 0x00B6, "paragraph");
    SetEncodingEntry(183, 0x00B7, "periodcentered");
    SetEncodingEntry(184, 0x00B8, "cedilla");
    SetEncodingEntry(185, 0x00B9, "onesuperior");
    SetEncodingEntry(186, 0x00BA, "ordmasculine");
    SetEncodingEntry(187, 0x00BB, "guillemotright");
    SetEncodingEntry(188, 0x00BC, "onequarter");
    SetEncodingEntry(189, 0x00BD, "onehalf");
    SetEncodingEntry(190, 0x00BE, "threequarters");
    SetEncodingEntry(191, 0x00BF, "questiondown");
    
    // Extended Latin characters (192-255)
    SetEncodingEntry(192, 0x00C0, "Agrave");
    SetEncodingEntry(193, 0x00C1, "Aacute");
    SetEncodingEntry(194, 0x00C2, "Acircumflex");
    SetEncodingEntry(195, 0x00C3, "Atilde");
    SetEncodingEntry(196, 0x00C4, "Adieresis");
    SetEncodingEntry(197, 0x00C5, "Aring");
    SetEncodingEntry(198, 0x00C6, "AE");
    SetEncodingEntry(199, 0x00C7, "Ccedilla");
    SetEncodingEntry(200, 0x00C8, "Egrave");
    SetEncodingEntry(201, 0x00C9, "Eacute");
    SetEncodingEntry(202, 0x00CA, "Ecircumflex");
    SetEncodingEntry(203, 0x00CB, "Edieresis");
    SetEncodingEntry(204, 0x00CC, "Igrave");
    SetEncodingEntry(205, 0x00CD, "Iacute");
    SetEncodingEntry(206, 0x00CE, "Icircumflex");
    SetEncodingEntry(207, 0x00CF, "Idieresis");
    SetEncodingEntry(208, 0x00D0, "Eth");
    SetEncodingEntry(209, 0x00D1, "Ntilde");
    SetEncodingEntry(210, 0x00D2, "Ograve");
    SetEncodingEntry(211, 0x00D3, "Oacute");
    SetEncodingEntry(212, 0x00D4, "Ocircumflex");
    SetEncodingEntry(213, 0x00D5, "Otilde");
    SetEncodingEntry(214, 0x00D6, "Odieresis");
    SetEncodingEntry(215, 0x00D7, "multiply");
    SetEncodingEntry(216, 0x00D8, "Oslash");
    SetEncodingEntry(217, 0x00D9, "Ugrave");
    SetEncodingEntry(218, 0x00DA, "Uacute");
    SetEncodingEntry(219, 0x00DB, "Ucircumflex");
    SetEncodingEntry(220, 0x00DC, "Udieresis");
    SetEncodingEntry(221, 0x00DD, "Yacute");
    SetEncodingEntry(222, 0x00DE, "Thorn");
    SetEncodingEntry(223, 0x00DF, "germandbls");
    SetEncodingEntry(224, 0x00E0, "agrave");
    SetEncodingEntry(225, 0x00E1, "aacute");
    SetEncodingEntry(226, 0x00E2, "acircumflex");
    SetEncodingEntry(227, 0x00E3, "atilde");
    SetEncodingEntry(228, 0x00E4, "adieresis");
    SetEncodingEntry(229, 0x00E5, "aring");
    SetEncodingEntry(230, 0x00E6, "ae");
    SetEncodingEntry(231, 0x00E7, "ccedilla");
    SetEncodingEntry(232, 0x00E8, "egrave");
    SetEncodingEntry(233, 0x00E9, "eacute");
    SetEncodingEntry(234, 0x00EA, "ecircumflex");
    SetEncodingEntry(235, 0x00EB, "edieresis");
    SetEncodingEntry(236, 0x00EC, "igrave");
    SetEncodingEntry(237, 0x00ED, "iacute");
    SetEncodingEntry(238, 0x00EE, "icircumflex");
    SetEncodingEntry(239, 0x00EF, "idieresis");
    SetEncodingEntry(240, 0x00F0, "eth");
    SetEncodingEntry(241, 0x00F1, "ntilde");
    SetEncodingEntry(242, 0x00F2, "ograve");
    SetEncodingEntry(243, 0x00F3, "oacute");
    SetEncodingEntry(244, 0x00F4, "ocircumflex");
    SetEncodingEntry(245, 0x00F5, "otilde");
    SetEncodingEntry(246, 0x00F6, "odieresis");
    SetEncodingEntry(247, 0x00F7, "divide");
    SetEncodingEntry(248, 0x00F8, "oslash");
    SetEncodingEntry(249, 0x00F9, "ugrave");
    SetEncodingEntry(250, 0x00FA, "uacute");
    SetEncodingEntry(251, 0x00FB, "ucircumflex");
    SetEncodingEntry(252, 0x00FC, "udieresis");
    SetEncodingEntry(253, 0x00FD, "yacute");
    SetEncodingEntry(254, 0x00FE, "thorn");
    SetEncodingEntry(255, 0x00FF, "ydieresis");
}

// PDFDocEncoding implementation
void PDFDocEncoding::InitializeBaseEncoding() {
    // PDFDocEncoding is identical to WinAnsiEncoding for most characters
    // Initialize with WinAnsi first
    WinAnsiEncoding win_ansi;
    for (int i = 0; i < 256; i++) {
        uint32_t unicode = win_ansi.CodeToUnicode(static_cast<uint8_t>(i));
        std::string glyph_name = win_ansi.CodeToGlyphName(static_cast<uint8_t>(i));
        SetEncodingEntry(static_cast<uint8_t>(i), unicode, glyph_name);
    }
    
    // PDFDocEncoding specific differences for codes 128-160
    SetEncodingEntry(128, 0x2022, "bullet");
    SetEncodingEntry(129, 0x2020, "dagger");
    SetEncodingEntry(130, 0x2021, "daggerdbl");
    SetEncodingEntry(131, 0x2026, "ellipsis");
    SetEncodingEntry(132, 0x2014, "emdash");
    SetEncodingEntry(133, 0x2013, "endash");
    SetEncodingEntry(134, 0x0192, "florin");
    SetEncodingEntry(135, 0x2044, "fraction");
    SetEncodingEntry(136, 0x2039, "guilsinglleft");
    SetEncodingEntry(137, 0x203A, "guilsinglright");
    SetEncodingEntry(138, 0x2212, "minus");
    SetEncodingEntry(139, 0x2030, "perthousand");
    SetEncodingEntry(140, 0x201E, "quotedblbase");
    SetEncodingEntry(141, 0x201C, "quotedblleft");
    SetEncodingEntry(142, 0x201D, "quotedblright");
    SetEncodingEntry(143, 0x2018, "quoteleft");
    SetEncodingEntry(144, 0x2019, "quoteright");
    SetEncodingEntry(145, 0x201A, "quotesinglbase");
    SetEncodingEntry(146, 0x2122, "trademark");
    SetEncodingEntry(147, 0xFB01, "fi");
    SetEncodingEntry(148, 0xFB02, "fl");
    SetEncodingEntry(149, 0x0141, "Lslash");
    SetEncodingEntry(150, 0x0152, "OE");
    SetEncodingEntry(151, 0x0160, "Scaron");
    SetEncodingEntry(152, 0x0178, "Ydieresis");
    SetEncodingEntry(153, 0x017D, "Zcaron");
    SetEncodingEntry(154, 0x0131, "dotlessi");
    SetEncodingEntry(155, 0x0142, "lslash");
    SetEncodingEntry(156, 0x0153, "oe");
    SetEncodingEntry(157, 0x0161, "scaron");
    SetEncodingEntry(158, 0x017E, "zcaron");
    // 159 is undefined
    // 160 and above remain as in WinAnsi
}

// MacExpertEncoding implementation
void MacExpertEncoding::InitializeBaseEncoding() {
    // Initialize all entries to 0/empty first
    for (int i = 0; i < 256; i++) {
        SetEncodingEntry(static_cast<uint8_t>(i), 0, "");
    }
    
    // MacExpertEncoding contains expert characters for typography
    // This is a simplified version - full implementation would include all expert characters
    SetEncodingEntry(32, 0x0020, "space");
    SetEncodingEntry(33, 0x0021, "exclamsmall");
    SetEncodingEntry(34, 0x0048, "Hungarumlautsmall");
    SetEncodingEntry(35, 0x0024, "dollaroldstyle");
    SetEncodingEntry(36, 0x0024, "dollarsuperior");
    SetEncodingEntry(37, 0x0041, "ampersandsmall");
    SetEncodingEntry(38, 0x0046, "Acutesmall");
    SetEncodingEntry(39, 0x0027, "parenleftsuperior");
    SetEncodingEntry(40, 0x0028, "parenrightsuperior");
    SetEncodingEntry(41, 0x0032, "twodotenleader");
    SetEncodingEntry(42, 0x006F, "onedotenleader");
    SetEncodingEntry(43, 0x002C, "comma");
    SetEncodingEntry(44, 0x002D, "hyphen");
    SetEncodingEntry(45, 0x002E, "period");
    SetEncodingEntry(46, 0x002F, "fraction");
    SetEncodingEntry(47, 0x007A, "zerooldstyle");
    SetEncodingEntry(48, 0x006F, "oneoldstyle");
    SetEncodingEntry(49, 0x0074, "twooldstyle");
    SetEncodingEntry(50, 0x0074, "threeoldstyle");
    SetEncodingEntry(51, 0x0066, "fouroldstyle");
    SetEncodingEntry(52, 0x0066, "fiveoldstyle");
    SetEncodingEntry(53, 0x0073, "sixoldstyle");
    SetEncodingEntry(54, 0x0073, "sevenoldstyle");
    SetEncodingEntry(55, 0x0065, "eightoldstyle");
    SetEncodingEntry(56, 0x006E, "nineoldstyle");
    SetEncodingEntry(57, 0x003A, "colon");
    SetEncodingEntry(58, 0x003B, "semicolon");
    SetEncodingEntry(59, 0x0063, "commasuperior");
    SetEncodingEntry(60, 0x0074, "threequartersemdash");
    SetEncodingEntry(61, 0x0070, "periodsuperior");
    SetEncodingEntry(62, 0x0071, "questionsmall");
    // ... continue with remaining MacExpert characters
}

// SymbolEncoding implementation
void SymbolEncoding::InitializeBaseEncoding() {
    // Initialize all entries to 0/empty first
    for (int i = 0; i < 256; i++) {
        SetEncodingEntry(static_cast<uint8_t>(i), 0, "");
    }
    
    // Symbol encoding contains mathematical and Greek characters
    SetEncodingEntry(32, 0x0020, "space");
    SetEncodingEntry(33, 0x0021, "exclam");
    SetEncodingEntry(34, 0x2200, "universal");
    SetEncodingEntry(35, 0x0023, "numbersign");
    SetEncodingEntry(36, 0x2203, "existential");
    SetEncodingEntry(37, 0x0025, "percent");
    SetEncodingEntry(38, 0x0026, "ampersand");
    SetEncodingEntry(39, 0x220B, "suchthat");
    SetEncodingEntry(40, 0x0028, "parenleft");
    SetEncodingEntry(41, 0x0029, "parenright");
    SetEncodingEntry(42, 0x2217, "asteriskmath");
    SetEncodingEntry(43, 0x002B, "plus");
    SetEncodingEntry(44, 0x002C, "comma");
    SetEncodingEntry(45, 0x2212, "minus");
    SetEncodingEntry(46, 0x002E, "period");
    SetEncodingEntry(47, 0x002F, "slash");
    SetEncodingEntry(48, 0x0030, "zero");
    SetEncodingEntry(49, 0x0031, "one");
    SetEncodingEntry(50, 0x0032, "two");
    SetEncodingEntry(51, 0x0033, "three");
    SetEncodingEntry(52, 0x0034, "four");
    SetEncodingEntry(53, 0x0035, "five");
    SetEncodingEntry(54, 0x0036, "six");
    SetEncodingEntry(55, 0x0037, "seven");
    SetEncodingEntry(56, 0x0038, "eight");
    SetEncodingEntry(57, 0x0039, "nine");
    SetEncodingEntry(58, 0x003A, "colon");
    SetEncodingEntry(59, 0x003B, "semicolon");
    SetEncodingEntry(60, 0x003C, "less");
    SetEncodingEntry(61, 0x003D, "equal");
    SetEncodingEntry(62, 0x003E, "greater");
    SetEncodingEntry(63, 0x003F, "question");
    SetEncodingEntry(64, 0x2245, "congruent");
    SetEncodingEntry(65, 0x0391, "Alpha");
    SetEncodingEntry(66, 0x0392, "Beta");
    SetEncodingEntry(67, 0x03A7, "Chi");
    SetEncodingEntry(68, 0x0394, "Delta");
    SetEncodingEntry(69, 0x0395, "Epsilon");
    SetEncodingEntry(70, 0x03A6, "Phi");
    SetEncodingEntry(71, 0x0393, "Gamma");
    SetEncodingEntry(72, 0x0397, "Eta");
    SetEncodingEntry(73, 0x0399, "Iota");
    SetEncodingEntry(74, 0x03D1, "theta1");
    SetEncodingEntry(75, 0x039A, "Kappa");
    SetEncodingEntry(76, 0x039B, "Lambda");
    SetEncodingEntry(77, 0x039C, "Mu");
    SetEncodingEntry(78, 0x039D, "Nu");
    SetEncodingEntry(79, 0x039F, "Omicron");
    SetEncodingEntry(80, 0x03A0, "Pi");
    SetEncodingEntry(81, 0x0398, "Theta");
    SetEncodingEntry(82, 0x03A1, "Rho");
    SetEncodingEntry(83, 0x03A3, "Sigma");
    SetEncodingEntry(84, 0x03A4, "Tau");
    SetEncodingEntry(85, 0x03A5, "Upsilon");
    SetEncodingEntry(86, 0x03C2, "sigma1");
    SetEncodingEntry(87, 0x03A9, "Omega");
    SetEncodingEntry(88, 0x039E, "Xi");
    SetEncodingEntry(89, 0x03A8, "Psi");
    SetEncodingEntry(90, 0x0396, "Zeta");
    // ... continue with remaining Symbol characters
}

// ZapfDingbatsEncoding implementation
void ZapfDingbatsEncoding::InitializeBaseEncoding() {
    // Initialize all entries to 0/empty first
    for (int i = 0; i < 256; i++) {
        SetEncodingEntry(static_cast<uint8_t>(i), 0, "");
    }
    
    // ZapfDingbats encoding contains decorative symbols
    SetEncodingEntry(32, 0x0020, "space");
    SetEncodingEntry(33, 0x2701, "a1");
    SetEncodingEntry(34, 0x2702, "a2");
    SetEncodingEntry(35, 0x2703, "a202");
    SetEncodingEntry(36, 0x2704, "a3");
    SetEncodingEntry(37, 0x260E, "a4");
    SetEncodingEntry(38, 0x2706, "a5");
    SetEncodingEntry(39, 0x2707, "a119");
    SetEncodingEntry(40, 0x2708, "a118");
    SetEncodingEntry(41, 0x2709, "a117");
    SetEncodingEntry(42, 0x261B, "a11");
    SetEncodingEntry(43, 0x261E, "a12");
    SetEncodingEntry(44, 0x270C, "a13");
    SetEncodingEntry(45, 0x270D, "a14");
    SetEncodingEntry(46, 0x270E, "a15");
    SetEncodingEntry(47, 0x270F, "a16");
    SetEncodingEntry(48, 0x2710, "a105");
    SetEncodingEntry(49, 0x2711, "a17");
    SetEncodingEntry(50, 0x2712, "a18");
    SetEncodingEntry(51, 0x2713, "a19");
    SetEncodingEntry(52, 0x2714, "a20");
    SetEncodingEntry(53, 0x2715, "a21");
    SetEncodingEntry(54, 0x2716, "a22");
    SetEncodingEntry(55, 0x2717, "a23");
    SetEncodingEntry(56, 0x2718, "a24");
    SetEncodingEntry(57, 0x2719, "a25");
    SetEncodingEntry(58, 0x271A, "a26");
    SetEncodingEntry(59, 0x271B, "a27");
    SetEncodingEntry(60, 0x271C, "a28");
    SetEncodingEntry(61, 0x271D, "a6");
    SetEncodingEntry(62, 0x271E, "a7");
    SetEncodingEntry(63, 0x271F, "a8");
    SetEncodingEntry(64, 0x2720, "a9");
    SetEncodingEntry(65, 0x2721, "a10");
    SetEncodingEntry(66, 0x2722, "a29");
    SetEncodingEntry(67, 0x2723, "a30");
    SetEncodingEntry(68, 0x2724, "a31");
    SetEncodingEntry(69, 0x2725, "a32");
    SetEncodingEntry(70, 0x2726, "a33");
    SetEncodingEntry(71, 0x2727, "a34");
    SetEncodingEntry(72, 0x2605, "a35");
    SetEncodingEntry(73, 0x2729, "a36");
    SetEncodingEntry(74, 0x272A, "a37");
    SetEncodingEntry(75, 0x272B, "a38");
    SetEncodingEntry(76, 0x272C, "a39");
    SetEncodingEntry(77, 0x272D, "a40");
    SetEncodingEntry(78, 0x272E, "a41");
    SetEncodingEntry(79, 0x272F, "a42");
    SetEncodingEntry(80, 0x2730, "a43");
    SetEncodingEntry(81, 0x2731, "a44");
    SetEncodingEntry(82, 0x2732, "a45");
    SetEncodingEntry(83, 0x2733, "a46");
    SetEncodingEntry(84, 0x2734, "a47");
    SetEncodingEntry(85, 0x2735, "a48");
    SetEncodingEntry(86, 0x2736, "a49");
    SetEncodingEntry(87, 0x2737, "a50");
    SetEncodingEntry(88, 0x2738, "a51");
    SetEncodingEntry(89, 0x2739, "a52");
    SetEncodingEntry(90, 0x273A, "a53");
    // ... continue with remaining ZapfDingbats characters
}

} // semantics
} // vanillapdf