#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "Fwd.h"

namespace gotchangpdf
{
	class Character
	{
	public:
		typedef unsigned char ValueType;

		enum class WhiteSpace : unsigned char
		{
			NUL = 0x00,
			HORIZONTAL_TAB = 0x09,
			LINE_FEED = 0x0A,
			FORM_FEED = 0x0C,
			CARRIAGE_RETURN = 0x0D,
			SPACE = 0x20
		};

		enum class Delimiter : unsigned char
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

		enum class Numeric : unsigned char
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

		Character();
		explicit Character(ValueType value);
		explicit Character(WhiteSpace value);
		explicit Character(Delimiter value);
		explicit Character(Numeric value);

		Character(ValueType hinib, ValueType lonib);
		Character(Character hinib, Character lonib);

		bool isRegular(void) const;
		bool isSpace(void) const;
		bool isWhiteSpace(void) const;
		bool isDelimiter(void) const;
		bool isEndOfLine(void) const;
		bool isNumeric(void) const;

		Character& operator= (ValueType value);

		operator ValueType() const { return _value; }
		ValueType Value(void) const;

		friend basic::BaseStream& operator>> (basic::BaseStream& s, Character& o);
		friend lexical::Stream& operator>> (lexical::Stream& s, Character& o);
		
		friend bool operator== (const Character& c1, const Character& c2);
		friend bool operator!= (const Character& c1, const Character& c2);
		friend bool operator== (const Character& c1, const WhiteSpace c2);
		friend bool operator!= (const Character& c1, const WhiteSpace c2);
		friend bool operator== (const Character& c1, const Delimiter c2);
		friend bool operator!= (const Character& c1, const Delimiter c2);
		friend bool operator== (const Character& c1, const Numeric c2);
		friend bool operator!= (const Character& c1, const Numeric c2);

	private:
		ValueType _value;
	};
}

#endif /* _CHARACTER_H */
