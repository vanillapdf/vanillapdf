#include "integer_object.h"
#include "lexical_reverse_stream.h"
#include "lexical_stream.h"

#include <cassert>
#include <vector>

namespace gotchangpdf
{
	using namespace std;
	using namespace lexical;

	#pragma region Constructors

	IntegerObject::IntegerObject(value_type value) : _value(value) {}

	IntegerObject::IntegerObject(const Token& value)
	{
		assert(value.GetType() == Token::Type::INTEGER_OBJECT);

		auto buffer = value.Value();
		_value = stoi(buffer.ToString());
	}

	#pragma endregion

	#pragma region Operators

	IntegerObject& IntegerObject::operator=(const Token& value)
	{
		return this->operator=(value.Value());
	}

	IntegerObject& IntegerObject::operator=(const Buffer& value)
	{
		_value = stoi(value.ToString());
		return *this;
	}

	IntegerObject& IntegerObject::operator=(value_type value)
	{
		_value = value;
		return *this;
	}

	#pragma endregion

	IntegerObject::value_type IntegerObject::Value(void) const { return _value; }

	ReverseStream& operator>> (ReverseStream& s, IntegerObject& o)
	{
		auto token = s.ReadToken();

		assert(token.GetType() == Token::Type::INTEGER_OBJECT);

		Buffer buffer = token.Value();
		o._value = stoi(buffer.ToString());

		return s;
	}

	Stream& operator>>(Stream& s, IntegerObject& o)
	{
		auto token = s.ReadToken();

		assert(token.GetType() == Token::Type::INTEGER_OBJECT);

		Buffer buffer = token.Value();
		o._value = stoi(buffer.ToString());

		return s;
	}
}
