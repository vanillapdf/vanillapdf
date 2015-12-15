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
}
