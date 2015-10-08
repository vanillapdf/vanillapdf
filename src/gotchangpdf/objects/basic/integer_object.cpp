#include "precompiled.h"
#include "integer_object.h"

#include <cassert>
#include <vector>

namespace gotchangpdf
{
	using namespace std;

	IntegerObject::IntegerObject(value_type value) : _value(value) {}

	#pragma endregion

	#pragma region Operators

	IntegerObject& IntegerObject::operator=(BufferPtr value)
	{
		_value = stoi(value->ToString());
		return *this;
	}

	IntegerObject& IntegerObject::operator=(value_type value)
	{
		_value = value;
		return *this;
	}
}
