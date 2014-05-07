#include "Character.h"
#include "exception.h"
#include "basic_base_stream.h"
#include "lexical_stream.h"

#include <climits>

namespace gotchangpdf
{
	#pragma region Constructors

	Character::Character() : _value(0) {}
	Character::Character(ValueType value) : _value(value) {}
	Character::Character(WhiteSpace value) : _value(static_cast<ValueType>(value)) {}
	Character::Character(Delimiter value) : _value(static_cast<ValueType>(value)) {}
	Character::Character(Numeric value) : _value(static_cast<ValueType>(value)) {}
	Character::Character(ValueType hinib, ValueType lonib) : _value((hinib << 4) + lonib) {}
	Character::Character(Character hinib, Character lonib) : Character(hinib.Value(), lonib.Value()) {}

	#pragma endregion

	#pragma region Operators

	Character::ValueType Character::Value(void) const { return _value; }

	bool operator==(const Character& c1, const Character& c2) { return c1._value == c2._value; }
	bool operator!=(const Character& c1, const Character& c2) { return c1._value != c2._value; }

	bool operator==(const Character& c1, const Character::WhiteSpace c2) { return c1 == static_cast<Character::ValueType>(c2); }
	bool operator==(const Character& c1, const Character::Delimiter c2) { return c1 == static_cast<Character::ValueType>(c2); }
	bool operator==(const Character& c1, const Character::Numeric c2) { return c1 == static_cast<Character::ValueType>(c2); }
	bool operator!=(const Character& c1, const Character::WhiteSpace c2) { return c1 != static_cast<Character::ValueType>(c2); }
	bool operator!=(const Character& c1, const Character::Delimiter c2) { return c1 != static_cast<Character::ValueType>(c2); }
	bool operator!=(const Character& c1, const Character::Numeric c2) { return c1 != static_cast<Character::ValueType>(c2); }

	Character& Character::operator=(ValueType value)
	{
		this->_value = value;
		return *this;
	}

	basic::BaseStream& operator>>(basic::BaseStream& s, Character& o)
	{
		o = s.Get();
		return s;
	}

	lexical::Stream& operator>>(lexical::Stream& s, Character& o)
	{
		o = s.Get();
		return s;
	}

	#pragma endregion

	bool Character::isRegular(void) const { return !isWhiteSpace() && !isDelimiter(); }
	bool Character::isSpace(void) const { return *this == WhiteSpace::SPACE; }
	bool Character::isEndOfLine(void) const { return *this == WhiteSpace::LINE_FEED; }

	bool Character::isWhiteSpace(void) const
	{
		switch (_value)
		{
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

	bool Character::isDelimiter(void) const
	{
		switch (_value)
		{
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

	bool Character::isNumeric(void) const
	{
		switch (_value)
		{
			case static_cast<int>(Numeric::ZERO):
			case static_cast<int>(Numeric::ONE):
			case static_cast<int>(Numeric::TWO):
			case static_cast<int>(Numeric::THREE):
			case static_cast<int>(Numeric::FOUR):
			case static_cast<int>(Numeric::FIVE):
			case static_cast<int>(Numeric::SIX):
			case static_cast<int>(Numeric::SEVEN):
			case static_cast<int>(Numeric::EIGHT):
			case static_cast<int>(Numeric::NINE):
				return true;
		}

		return false;
	}
}
