#include "character.h"
#include "exception.h"
#include "basic_base_stream.h"
#include "lexical_stream.h"

namespace gotchangpdf
{
	#pragma region Constructors

	Character::Character() {}
	Character::Character(ValueType value) : _value(value) {}
	Character::Character(WhiteSpace value) : _value(static_cast<ValueType>(value)) {}
	Character::Character(Delimiter value) : _value(static_cast<ValueType>(value)) {}
	Character::Character(Numeric value) : _value(static_cast<ValueType>(value)) {}
	Character::Character(ValueType hinib, ValueType lonib) : _value((hinib << 4) + lonib) {}
	Character::Character(Character hinib, Character lonib) : Character(hinib.Value(), lonib.Value()) {}

	#pragma endregion

	#pragma region Operators

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

	bool Character::isWhiteSpace(void) const
	{
		switch (_value)
		{
			case static_cast<ValueType>(WhiteSpace::NUL) :
			case static_cast<ValueType>(WhiteSpace::HORIZONTAL_TAB) :
			case static_cast<ValueType>(WhiteSpace::LINE_FEED) :
			case static_cast<ValueType>(WhiteSpace::FORM_FEED) :
			case static_cast<ValueType>(WhiteSpace::CARRIAGE_RETURN) :
			case static_cast<ValueType>(WhiteSpace::SPACE) :
				return true;
		}

		return false;
	}

	bool Character::isDelimiter(void) const
	{
		switch (_value)
		{
			case static_cast<ValueType>(Delimiter::LEFT_PARENTHESIS) :
			case static_cast<ValueType>(Delimiter::RIGHT_PARENTHESIS) :
			case static_cast<ValueType>(Delimiter::LESS_THAN_SIGN) :
			case static_cast<ValueType>(Delimiter::GREATER_THAN_SIGN) :
			case static_cast<ValueType>(Delimiter::LEFT_SQUARE_BRACKET) :
			case static_cast<ValueType>(Delimiter::RIGHT_SQUARE_BRACKET) :
			case static_cast<ValueType>(Delimiter::LEFT_CURLY_BRACKET) :
			case static_cast<ValueType>(Delimiter::RIGHT_CURLY_BRACKET) :
			case static_cast<ValueType>(Delimiter::SOLIDUS) :
			case static_cast<ValueType>(Delimiter::PERCENT_SIGN) :
				return true;
		}

		return false;
	}

	bool Character::isNumeric(void) const
	{
		switch (_value)
		{
			case static_cast<ValueType>(Numeric::ZERO) :
			case static_cast<ValueType>(Numeric::ONE) :
			case static_cast<ValueType>(Numeric::TWO) :
			case static_cast<ValueType>(Numeric::THREE) :
			case static_cast<ValueType>(Numeric::FOUR) :
			case static_cast<ValueType>(Numeric::FIVE) :
			case static_cast<ValueType>(Numeric::SIX) :
			case static_cast<ValueType>(Numeric::SEVEN) :
			case static_cast<ValueType>(Numeric::EIGHT) :
			case static_cast<ValueType>(Numeric::NINE) :
				return true;
		}

		return false;
	}
}
