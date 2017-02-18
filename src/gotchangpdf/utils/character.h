#ifndef _CHARACTER_H
#define _CHARACTER_H

namespace gotchangpdf {

enum class WhiteSpace : char {
	NUL = 0x00,
	HORIZONTAL_TAB = 0x09,
	LINE_FEED = 0x0A,
	FORM_FEED = 0x0C,
	CARRIAGE_RETURN = 0x0D,
	SPACE = 0x20
};

enum class Delimiter : char {
	LEFT_PARENTHESIS = 0x28,
	RIGHT_PARENTHESIS = 0x29,
	LESS_THAN_SIGN = 0x3C,
	GREATER_THAN_SIGN = 0x3E,
	LEFT_SQUARE_BRACKET = 0x5B,
	RIGHT_SQUARE_BRACKET = 0x5D,
	LEFT_CURLY_BRACKET = 0x7B,
	RIGHT_CURLY_BRACKET = 0x7D,
	SOLIDUS = 0x2F,
	PERCENT_SIGN = 0x25
};

inline bool IsWhiteSpace(int c) {
	switch (c) {
		case static_cast<int>(WhiteSpace::NUL):
		case static_cast<int>(WhiteSpace::HORIZONTAL_TAB):
		case static_cast<int>(WhiteSpace::LINE_FEED):
		case static_cast<int>(WhiteSpace::FORM_FEED):
		case static_cast<int>(WhiteSpace::CARRIAGE_RETURN):
		case static_cast<int>(WhiteSpace::SPACE):
			return true;
	}

	return false;
}

inline bool IsDelimiter(int c) {
	switch (c) {
		case static_cast<int>(Delimiter::LEFT_PARENTHESIS):
		case static_cast<int>(Delimiter::RIGHT_PARENTHESIS):
		case static_cast<int>(Delimiter::LESS_THAN_SIGN):
		case static_cast<int>(Delimiter::GREATER_THAN_SIGN):
		case static_cast<int>(Delimiter::LEFT_SQUARE_BRACKET):
		case static_cast<int>(Delimiter::RIGHT_SQUARE_BRACKET):
		case static_cast<int>(Delimiter::LEFT_CURLY_BRACKET):
		case static_cast<int>(Delimiter::RIGHT_CURLY_BRACKET):
		case static_cast<int>(Delimiter::SOLIDUS):
		case static_cast<int>(Delimiter::PERCENT_SIGN):
			return true;
	}

	return false;
}

inline bool IsEof(int c) {
	return c == std::char_traits<char>::eof();
}

inline bool IsNumeric(int c) {
	return (c >= '0' && c <= '9');
}

inline bool IsAlpha(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline bool IsRegular(int c) {
	return !IsWhiteSpace(c) && !IsDelimiter(c) && !IsEof(c);
}

inline bool IsSpace(int c) {
	return (c == static_cast<int>(WhiteSpace::SPACE));
}

inline bool IsNewline(int c) {
	return (c == static_cast<int>(WhiteSpace::LINE_FEED));
}

inline bool operator==(WhiteSpace white, int ch) {
	return (ch == static_cast<int>(white));
}

inline bool operator==(int ch, WhiteSpace white) {
	return (ch == static_cast<int>(white));
}

inline bool operator==(Delimiter delim, int ch) {
	return (ch == static_cast<int>(delim));
}

inline bool operator==(int ch, Delimiter delim) {
	return (ch == static_cast<int>(delim));
}

} // gotchangpdf

#endif /* _CHARACTER_H */
