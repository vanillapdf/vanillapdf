 #ifndef _CHARACTER_H
 #define _CHARACTER_H

namespace gotchangpdf
{
	enum WhiteSpace
	{
		NUL = 0x00,
		HORIZONTAL_TAB = 0x09,
		LINE_FEED = 0x0A,
		FORM_FEED = 0x0C,
		CARRIAGE_RETURN = 0x0D,
		SPACE = 0x20
	};

	enum Delimiter
	{
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

	inline bool IsWhiteSpace(const int c)
	{
		switch (c)
		{
		case WhiteSpace::NUL:
		case WhiteSpace::HORIZONTAL_TAB:
		case WhiteSpace::LINE_FEED:
		case WhiteSpace::FORM_FEED:
		case WhiteSpace::CARRIAGE_RETURN:
		case WhiteSpace::SPACE:
			return true;
		}

		return false;
	}

	inline bool IsDelimiter(const int c)
	{
		switch (c)
		{
		case Delimiter::LEFT_PARENTHESIS:
		case Delimiter::RIGHT_PARENTHESIS:
		case Delimiter::LESS_THAN_SIGN:
		case Delimiter::GREATER_THAN_SIGN:
		case Delimiter::LEFT_SQUARE_BRACKET:
		case Delimiter::RIGHT_SQUARE_BRACKET:
		case Delimiter::LEFT_CURLY_BRACKET:
		case Delimiter::RIGHT_CURLY_BRACKET:
		case Delimiter::SOLIDUS:
		case Delimiter::PERCENT_SIGN:
			return true;
		}

		return false;
	}

	inline bool IsEof(const int c) { return c == std::char_traits<char>::eof(); }
	inline bool IsNumeric(const int c) { return (c >= '0' && c <= '9'); }
	inline bool IsAlpha(const int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
	inline bool IsRegular(const int c) { return !IsWhiteSpace(c) && !IsDelimiter(c) && !IsEof(c); }
	inline bool IsSpace(const int c) { return c == WhiteSpace::SPACE; }
	inline bool IsNewline(const int c) { return c == WhiteSpace::LINE_FEED; }
}

#endif /* _CHARACTER_H */
