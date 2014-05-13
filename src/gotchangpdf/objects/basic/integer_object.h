#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "fwd.h"
#include "numeric_object.h"
#include "token.h"

namespace gotchangpdf
{
	class IntegerObject : public NumericObject
	{
	public:
		typedef long long ValueType;

		IntegerObject();
		explicit IntegerObject(ValueType value);
		//explicit IntegerObject(const CharacterSet& value);
		explicit IntegerObject(const lexical::Token& value);

		friend bool operator== (const IntegerObject& i1, const IntegerObject& i2);
		friend bool operator!= (const IntegerObject& i1, const IntegerObject& i2);
		friend bool operator< (const IntegerObject& i1, const IntegerObject& i2);

		ValueType Value(void) const;

		operator ValueType() const { return _value; }
		//virtual Object* Clone(void) const  override { return new IntegerObject(static_cast<IntegerObject const&>(*this)); };

		IntegerObject& operator= (ValueType value);
		IntegerObject& operator= (const Buffer& value);
		IntegerObject& operator= (const lexical::Token& value);

		friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, IntegerObject& o);
		friend lexical::Stream& operator>> (lexical::Stream& s, IntegerObject& o);

	private:
		ValueType _value;
	};
}

#endif /* _INTEGER_OBJECT_H */
