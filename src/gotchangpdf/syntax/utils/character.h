 #ifndef _CHARACTER_H
 #define _CHARACTER_H

#include "syntax_fwd.h"

namespace gotchangpdf
{
	namespace syntax
	{
		enum class WhiteSpace : char
		{
			NUL = 0x00,
			HORIZONTAL_TAB = 0x09,
			LINE_FEED = 0x0A,
			FORM_FEED = 0x0C,
			CARRIAGE_RETURN = 0x0D,
			SPACE = 0x20
		};

		enum class Delimiter : char
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

		inline bool Equals(const char c1, const int c2) { return c1 == c2; }
		inline bool Equals(const char c1, const long c2) { return c1 == c2; }
		inline bool Equals(const char c1, const unsigned char c2) { return c1 == c2; }
		inline bool Equals(const char c1, const signed char c2) { return c1 == c2; }
		inline bool Equals(const char c1, const char c2) { return c1 == c2; }
		inline bool Equals(const char c1, const WhiteSpace c2) { return Equals(c1, static_cast<char>(c2)); }
		inline bool Equals(const char c1, const Delimiter c2) { return Equals(c1, static_cast<char>(c2)); }

		bool IsWhiteSpace(const char c)
		{
			switch (c)
			{
			case static_cast<const char>(WhiteSpace::NUL):
			case static_cast<const char>(WhiteSpace::HORIZONTAL_TAB):
			case static_cast<const char>(WhiteSpace::LINE_FEED):
			case static_cast<const char>(WhiteSpace::FORM_FEED):
			case static_cast<const char>(WhiteSpace::CARRIAGE_RETURN):
			case static_cast<const char>(WhiteSpace::SPACE):
				return true;
			}

			return false;
		}

		bool IsDelimiter(const char c)
		{
			switch (c)
			{
			case static_cast<const char>(Delimiter::LEFT_PARENTHESIS):
			case static_cast<const char>(Delimiter::RIGHT_PARENTHESIS):
			case static_cast<const char>(Delimiter::LESS_THAN_SIGN):
			case static_cast<const char>(Delimiter::GREATER_THAN_SIGN):
			case static_cast<const char>(Delimiter::LEFT_SQUARE_BRACKET):
			case static_cast<const char>(Delimiter::RIGHT_SQUARE_BRACKET):
			case static_cast<const char>(Delimiter::LEFT_CURLY_BRACKET):
			case static_cast<const char>(Delimiter::RIGHT_CURLY_BRACKET):
			case static_cast<const char>(Delimiter::SOLIDUS):
			case static_cast<const char>(Delimiter::PERCENT_SIGN):
				return true;
			}

			return false;
		}

		inline bool IsNumeric(const char c) { return (c >= '0' && c <= '9'); }

		inline bool IsRegular(const char c) { return !IsWhiteSpace(c) && !IsDelimiter(c); }
		inline bool IsSpace(const char c) { return Equals(c, WhiteSpace::SPACE); }
		inline bool IsNewline(const char c) { return Equals(c, WhiteSpace::LINE_FEED); }
	}
}

#endif /* _CHARACTER_H */
