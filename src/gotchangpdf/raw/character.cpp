#include "character.h"
//#include "exception.h"
//#include "basic_base_stream.h"
//#include "lexical_stream.h"

namespace gotchangpdf
{
	namespace character
	{
		//Character::Character(MyValueType hinib, MyValueType lonib) : _value((hinib << 4) + lonib) {}
		//Character::Character(Character hinib, Character lonib) : Character(hinib.Value(), lonib.Value()) {}

		/*
		bool Character::isMetacharacter(void) const
		{
		return
		(std::numeric_limits<CharacterValueType>::min() < _value &&
		std::numeric_limits<CharacterValueType>::max() > _value);
		}
		*/
		bool IsWhiteSpace(const char c)
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

		bool IsDelimiter(const char c)
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

		bool IsNumeric(const char c)
		{
			switch (c)
			{
			case Numeric::ZERO:
			case Numeric::ONE:
			case Numeric::TWO:
			case Numeric::THREE:
			case Numeric::FOUR:
			case Numeric::FIVE:
			case Numeric::SIX:
			case Numeric::SEVEN:
			case Numeric::EIGHT:
			case Numeric::NINE:
				return true;
			}

			return false;
		}
	}
}
