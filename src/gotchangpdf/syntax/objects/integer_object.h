#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "syntax_fwd.h"
#include "constants.h"
#include "object.h"
#include "containable.h"
#include "util.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class IntegerObject : public Object
		{
		public:
			typedef types::integer value_type;
			typedef types::double_integer value_type_doubled;

			IntegerObject() = default;
			explicit IntegerObject(value_type value);

			inline value_type Value(void) const _NOEXCEPT { return _value; }

			template <typename T>
			inline T SafeConvert(void) const { return gotchangpdf::SafeConvert<T>(_value); }

			operator value_type() const { return _value; }

			IntegerObject& operator= (value_type value);
			IntegerObject& operator= (BufferPtr value);

			virtual Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Integer; }

			inline bool Equals(const IntegerObject& other) const _NOEXCEPT { return _value == other._value; }

			inline bool operator==(const IntegerObject& other) const { return Equals(other); }
			inline bool operator!=(const IntegerObject& other) const { return !Equals(other); }
			inline bool operator<(const IntegerObject& other) const { return _value < other._value; }

		private:
			value_type _value = 0;
		};

		inline IntegerObject SafeAddition(IntegerObject::value_type number, IntegerObject::value_type addend) {

			IntegerObject::value_type_doubled result = number + addend;
			auto converted = gotchangpdf::SafeConvert<IntegerObject::value_type>(result);
			return IntegerObject(converted);
		}

		inline IntegerObject SafeAddition(const IntegerObject& number, const IntegerObject& addend) {
			return SafeAddition(number.Value(), addend.Value());
		}
	}
}

#endif /* _INTEGER_OBJECT_H */
