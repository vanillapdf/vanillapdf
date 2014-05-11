#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "fwd.h"
#include "interface.h"

namespace gotchangpdf
{
	class Character : public ICharacter
	{
	public:
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
