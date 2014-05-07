#include "integer_object.h"
#include "lexical_reverse_stream.h"
#include "lexical_stream.h"
#include "export.h"

#include <cassert>
#include <vector>

namespace gotchangpdf
{
	using namespace std;
	using namespace lexical;

	#pragma region Constructors

	IntegerObject::IntegerObject(ValueType value) : NumericObject(Type::IntegerObject), _value(value) {}
	IntegerObject::IntegerObject() : NumericObject(Type::IntegerObject), _value(0) {}

	IntegerObject::IntegerObject(const Token& value) : NumericObject(Type::IntegerObject)
	{
		assert(value.type() == Token::Type::INTEGER_OBJECT);

		auto buffer = value.value();
		_value = stoi(buffer.ToString());
	}

	#pragma endregion

	#pragma region Operators

	bool operator== (const IntegerObject& i1, const IntegerObject& i2) { return i1._value == i2._value; }
	bool operator!= (const IntegerObject& i1, const IntegerObject& i2) { return i1._value != i2._value; }
	bool operator<(const IntegerObject& i1, const IntegerObject& i2) { return i1._value < i2._value; }

	IntegerObject& IntegerObject::operator=(const Token& value) { return *this = value.value(); }

	IntegerObject& IntegerObject::operator=(const Buffer& value)
	{
		_value = stoi(value.ToString());
		return *this;
	}

	IntegerObject& IntegerObject::operator=(ValueType value)
	{
		_value = value;
		return *this;
	}

	#pragma endregion

	IntegerObject::ValueType IntegerObject::value(void) const { return _value; }

	ReverseStream& operator>> (ReverseStream& s, IntegerObject& o)
	{
		auto token = s.ReadToken();

		assert(token->type() == Token::Type::INTEGER_OBJECT);

		Buffer buffer = token->value();
		o._value = stoi(buffer.ToString());

		return s;
	}

	Stream& operator>>(Stream& s, IntegerObject& o)
	{
		auto token = s.ReadToken();

		assert(token->type() == Token::Type::INTEGER_OBJECT);

		Buffer buffer = token->value();
		o._value = stoi(buffer.ToString());

		return s;
	}
}

GOTCHANG_PDF_API long long CALLING_CONVENTION IntegerObject_Value(IntegerObjectHandle handle)
{
	gotchangpdf::IntegerObject* obj = reinterpret_cast<gotchangpdf::IntegerObject*>(handle);
	return obj->value();
}

GOTCHANG_PDF_API void CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle handle)
{
	gotchangpdf::IntegerObject* obj = reinterpret_cast<gotchangpdf::IntegerObject*>(handle);
	boost::intrusive_ptr_release(obj);
}
