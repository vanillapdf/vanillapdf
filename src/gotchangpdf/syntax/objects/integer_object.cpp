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
			_value = gotchangpdf::SafeConvert<value_type>(value.Value());
		}

		IntegerObject& IntegerObject::operator=(value_type value)
		{
			_value = value;
			return *this;
		}

		IntegerObject SafeAddition(IntegerObject::value_type number, IntegerObject::value_type addend)
		{
			IntegerObject::value_type result = number + addend;

			if (number < 0) {
				if (addend < 0) {
					if (result > number) {
						std::stringstream ss;
						ss << "Could not compute sum of " << number << " and " << addend;
						throw std::out_of_range(ss.str());
					}
				}

				return IntegerObject(result);
			}

			if (result < number) {
				std::stringstream ss;
				ss << "Could not compute sum of " << number << " and " << addend;
				throw std::out_of_range(ss.str());
			}

			return IntegerObject(result);
		}

		IntegerObject SafeAddition(const IntegerObject& number, const IntegerObject& addend)
		{
			return SafeAddition(number.Value(), addend.Value());
		}
	}
}
