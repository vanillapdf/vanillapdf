#include "precompiled.h"
#include "integer_object.h"
#include "real_object.h"

#include <cassert>
#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		IntegerObject::IntegerObject(types::native_int value) : _value(value) {}
		IntegerObject::IntegerObject(types::native_uint value) : _value(value) {}
		IntegerObject::IntegerObject(value_type value) : _value(value) {}
		IntegerObject::IntegerObject(const RealObject& value)
		{
			_value = ValueConvertUtils::SafeConvert<value_type>(value.Value());
		}

		IntegerObject& IntegerObject::operator=(value_type value)
		{
			_value = value;
			return *this;
		}
	}
}
