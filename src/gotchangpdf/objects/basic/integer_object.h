#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "token.h"
#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class IntegerObject : public Containable, public Object
	{
	public:
		typedef types::integer value_type;

		IntegerObject() = default;
		explicit IntegerObject(value_type value);
		//explicit IntegerObject(const CharacterSet& value);
		explicit IntegerObject(const lexical::Token& value);

		inline value_type IntegerObject::Value(void) const { return _value; }

		operator value_type() const { return _value; }
		//virtual Object* Clone(void) const  override { return new IntegerObject(static_cast<IntegerObject const&>(*this)); };

		IntegerObject& operator= (value_type value);
		IntegerObject& operator= (BufferPtr value);
		IntegerObject& operator= (const lexical::Token& value);

		virtual Object::Type GetType(void) const override;

		friend lexical::ReverseStream& operator>> (lexical::ReverseStream& s, IntegerObject& o);
		friend lexical::Stream& operator>> (lexical::Stream& s, IntegerObject& o);

		inline bool Equals(const IntegerObject& other) const { return _value == other._value; }

		inline bool operator==(const IntegerObject& other) const { return Equals(other); }
		inline bool operator!=(const IntegerObject& other) const { return !Equals(other); }
		inline bool operator<(const IntegerObject& other) const { return _value < other._value; }

	//private:
	public:
		value_type _value = 0;
	};

	inline  Object::Type IntegerObject::GetType(void) const { return Object::Type::Integer; }
}

#endif /* _INTEGER_OBJECT_H */
