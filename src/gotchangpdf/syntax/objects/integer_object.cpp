#include "precompiled.h"
#include "syntax/objects/integer_object.h"
#include "real_object.h"

#include <cassert>
#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		IntegerObject::IntegerObject() { m_value->Subscribe(this); }
		IntegerObject::IntegerObject(types::native_int value) { m_value->SetIntegerValue(value); m_value->Subscribe(this); }
		IntegerObject::IntegerObject(types::native_uint value) { m_value->SetUnsignedIntegerValue(value); m_value->Subscribe(this); }
		IntegerObject::IntegerObject(types::big_int value) { m_value->SetIntegerValue(value); m_value->Subscribe(this); }
		IntegerObject::IntegerObject(types::big_uint value) { m_value->SetUnsignedIntegerValue(value); m_value->Subscribe(this); }

		IntegerObject::IntegerObject(const NumericObject& value)
		{
			m_value = value.GetNumericBackend();
			m_value->Subscribe(this);
		}

		IntegerObject::IntegerObject(NumericObjectBackendPtr value)
		{
			m_value = value;
			m_value->Subscribe(this);
		}

		IntegerObject& IntegerObject::operator=(types::native_int value)
		{
			m_value->SetIntegerValue(value);
			return *this;
		}

		IntegerObject& IntegerObject::operator=(types::native_uint value)
		{
			m_value->SetUnsignedIntegerValue(value);
			return *this;
		}

		IntegerObject& IntegerObject::operator=(types::big_int value)
		{
			m_value->SetIntegerValue(value);
			return *this;
		}

		IntegerObject& IntegerObject::operator=(types::big_uint value)
		{
			m_value->SetUnsignedIntegerValue(value);
			return *this;
		}
	}
}
