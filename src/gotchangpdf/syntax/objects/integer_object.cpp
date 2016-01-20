#include "precompiled.h"
#include "integer_object.h"
#include "real_object.h"

#include <cassert>
#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		IntegerObject::IntegerObject(value_type value) : _value(value) {}
		IntegerObject::IntegerObject(const RealObject& value)
		{
			_value = gotchangpdf::SafeConvert<value_type>(value.Value());
		}

		IntegerObject& IntegerObject::operator=(value_type value)
		{
			_value = value;
			return *this;
		}

		IntegerObject SafeAddition(IntegerObject::value_type number, IntegerObject::value_type addend)
		{
			IntegerObject::value_type_doubled result = number + addend;
			auto converted = gotchangpdf::SafeConvert<IntegerObject::value_type>(result);
			return IntegerObject(converted);
		}

		IntegerObject SafeAddition(const IntegerObject& number, const IntegerObject& addend)
		{
			return SafeAddition(number.Value(), addend.Value());
		}
	}
}
