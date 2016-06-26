#include "precompiled.h"
#include "integer_object.h"
#include "real_object.h"

#include <cassert>
#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		IntegerObject::IntegerObject() { m_value->Subscribe(this); }
		IntegerObject::IntegerObject(types::native_int value) { m_value->SetIntegerValue(value); m_value->Subscribe(this); }
		IntegerObject::IntegerObject(types::native_uint value) { m_value->SetIntegerValue(value); m_value->Subscribe(this); }
		IntegerObject::IntegerObject(value_type value) { m_value->SetIntegerValue(value); m_value->Subscribe(this); }
		IntegerObject::IntegerObject(const NumericObject& value)
		{
			m_value = value.GetNumericBackend();
			m_value->Subscribe(this);
		}

		IntegerObject& IntegerObject::operator=(value_type value)
		{
			m_value->SetIntegerValue(value);
			return *this;
		}
	}
}
