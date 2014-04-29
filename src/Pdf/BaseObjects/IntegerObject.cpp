#include "IntegerObject.h"
#include "lexical/LexicalReverseStream.h"
#include "lexical/LexicalStream.h"
#include "Export.h"

#include <cassert>
#include <vector>
#include <string>

namespace Pdf
{
	using namespace std;

	using Pdf::Lexical::Token;
	using namespace Streams::Lexical;

	IntegerObject::IntegerObject(ValueType value) : NumericObject(Type::IntegerObject), _value(value) {}
	IntegerObject::IntegerObject() : NumericObject(Type::IntegerObject), _value(0) {}
	/*
	IntegerObject::IntegerObject(const CharacterSet& value) : NumericObject(Type::IntegerObject)
	{
		string str(value.begin(), value.end());
		_value = stoi(str);
	}
	*/

	IntegerObject::IntegerObject(const Pdf::Lexical::Token& value ) : NumericObject(Type::IntegerObject)
	{
		assert(value.type() == Token::Type::INTEGER_OBJECT);

		auto data = value.value();

		string str(data.begin(), data.end());
		_value = stoi(str);
	}

	IntegerObject& IntegerObject::operator=(const Token& value)
	{
		return *this = value.value();
	}

	IntegerObject& IntegerObject::operator=(const CharacterSet& value)
	{
		string str(value.begin(), value.end());
		_value = stoi(str);

		return *this;
	}

	IntegerObject& IntegerObject::operator=(ValueType value)
	{
		_value = value;
		return *this;
	}

	IntegerObject::ValueType IntegerObject::value( void ) const
	{
		return _value;
	}

	bool operator== (const IntegerObject& i1, const IntegerObject& i2)
	{
		return i1._value == i2._value;
	}

	bool operator!= (const IntegerObject& i1, const IntegerObject& i2)
	{
		return i1._value != i2._value;
	}

	ReverseStream& operator>> (ReverseStream& s, IntegerObject& o)
	{		
		Token t;
		s >> t;

		// TODO
		//assert(t == Token::Type::INTEGER_OBJECT);

		CharacterSet set = t.value();
		string resultStr(set.begin(), set.end());
		o._value = stoi(resultStr);

		return s;
	}

	Stream& operator>>(Stream& s, IntegerObject& o)
	{
		Token t;
		s >> t;

		assert(t.type() == Token::Type::INTEGER_OBJECT);

		CharacterSet set = t.value();
		string resultStr(set.begin(), set.end());
		o._value = stoi(resultStr);

		return s;
	}

	bool operator<( const IntegerObject& i1, const IntegerObject& i2 )
	{
		return i1._value < i2._value;
	}
}

GOTCHANG_PDF_API long long CALLING_CONVENTION IntegerObject_Value(IntegerObjectHandle handle)
{
	Pdf::IntegerObject* obj = reinterpret_cast<Pdf::IntegerObject*>(handle);
	return obj->value();
}

GOTCHANG_PDF_API void CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle handle)
{
	Pdf::IntegerObject* obj = reinterpret_cast<Pdf::IntegerObject*>(handle);
	boost::intrusive_ptr_release(obj);
}
