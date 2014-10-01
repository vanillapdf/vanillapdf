#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "fwd.h"

#include <cassert>
#include <iosfwd>
#include <locale>

namespace gotchangpdf
{
	namespace character
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

		enum class Numeric : char
		{
			ZERO = '0',
			ONE = '1',
			TWO = '2',
			THREE = '3',
			FOUR = '4',
			FIVE = '5',
			SIX = '6',
			SEVEN = '7',
			EIGHT = '8',
			NINE = '9'
		};

		//Character(MyValueType hinib, MyValueType lonib);
		//Character(Character hinib, Character lonib);


		//inline bool Character::operator== (const Character& other) const { return Equals(other); }
		//inline bool Character::operator!= (const Character& other) const { return !Equals(other); }

		inline bool Equals(const char c1, const int c2) { return c1 == c2; }
		inline bool Equals(const char c1, const long c2) { return c1 == c2; }
		inline bool Equals(const char c1, const unsigned char c2) { return c1 == c2; }
		inline bool Equals(const char c1, const signed char c2) { return c1 == c2; }
		inline bool Equals(const char c1, const char c2) { return c1 == c2; }
		inline bool Equals(const char c1, const WhiteSpace c2) { return Equals(c1, static_cast<char>(c2)); }
		inline bool Equals(const char c1, const Delimiter c2) { return Equals(c1, static_cast<char>(c2)); }
		inline bool Equals(const char c1, const Numeric c2) { return Equals(c1, static_cast<char>(c2)); }

		bool IsWhiteSpace(const char c);
		bool IsDelimiter(const char c);
		bool IsNumeric(const char c);

		inline bool IsRegular(const char c) { return !IsWhiteSpace(c) && !IsDelimiter(c); }
		inline bool IsSpace(const char c) { return Equals(c, WhiteSpace::SPACE); }
		inline bool IsNewline(const char c) { return Equals(c, WhiteSpace::LINE_FEED); }
	}
}

#endif /* _CHARACTER_H */
