#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "fwd.h"

namespace gotchangpdf
{
	class Character
	{
		friend basic::BaseStream& operator>> (basic::BaseStream& s, Character& o);
		friend lexical::Stream& operator>> (lexical::Stream& s, Character& o);

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
		bool isNewline(void) const;
		bool isNumeric(void) const;

		Character& operator= (ValueType value);

		operator ValueType() const;
		ValueType Value(void) const;

		bool Equals(const ValueType c) const;
		bool Equals(const Character& c) const;
		bool Equals(const WhiteSpace c) const;
		bool Equals(const Delimiter c) const;
		bool Equals(const Numeric c) const;

	private:
		ValueType _value = 0;
	};

	inline bool Character::Equals(const ValueType c) const { return _value == c; }
	inline bool Character::Equals(const Character& c) const { return Equals(c._value); }
	inline bool Character::Equals(const WhiteSpace c) const { return Equals(static_cast<Character::ValueType>(c)); }
	inline bool Character::Equals(const Delimiter c) const { return Equals(static_cast<Character::ValueType>(c)); }
	inline bool Character::Equals(const Numeric c) const { return Equals(static_cast<Character::ValueType>(c)); }

	inline Character::ValueType Character::Value(void) const { return _value; }
	inline Character::operator ValueType() const { return _value; }

	inline bool Character::isRegular(void) const { return !isWhiteSpace() && !isDelimiter(); }
	inline bool Character::isSpace(void) const { return Equals(WhiteSpace::SPACE); }
	inline bool Character::isNewline(void) const { return Equals(WhiteSpace::LINE_FEED); }
}

#endif /* _CHARACTER_H */
