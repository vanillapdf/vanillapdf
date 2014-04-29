#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "NumericObject.h"
#include "streams/lexical/LexicalStreamFwd.h"
#include "streams/lexical/LexicalReverseStreamFwd.h"
#include "Lexical/Token.h"

namespace Pdf
{
	class CharacterSet;

	class IntegerObject : public NumericObject
	{
	public:
		typedef long long ValueType;

		IntegerObject();
		explicit IntegerObject(ValueType value);
		//explicit IntegerObject(const CharacterSet& value);
		explicit IntegerObject(const Pdf::Lexical::Token& value);

		friend bool operator== (const IntegerObject& i1, const IntegerObject& i2);
		friend bool operator!= (const IntegerObject& i1, const IntegerObject& i2);
		friend bool operator< (const IntegerObject& i1, const IntegerObject& i2);

		ValueType value(void) const;

		operator ValueType() const { return _value; }
		//virtual Object* Clone(void) const  override { return new IntegerObject(static_cast<IntegerObject const&>(*this)); };

		IntegerObject& operator= (ValueType value);
		IntegerObject& operator= (const CharacterSet& value);
		IntegerObject& operator= (const Pdf::Lexical::Token& value);

		friend Streams::Lexical::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, IntegerObject& o);
		friend Streams::Lexical::Stream& operator>> (Streams::Lexical::Stream& s, IntegerObject& o);

	private:
		ValueType _value;

		friend void ::boost::intrusive_ptr_add_ref(IntegerObject*);
		friend void ::boost::intrusive_ptr_release(IntegerObject*);
	};
}

#endif /* _INTEGER_OBJECT_H */
