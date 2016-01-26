#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "syntax_fwd.h"
#include "constants.h"
#include "containable.h"
#include "util.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class IntegerObject : public ContainableObject
		{
		public:
			typedef types::integer value_type;
			typedef types::double_integer value_type_doubled;

			IntegerObject() = default;
			explicit IntegerObject(value_type value);
			explicit IntegerObject(const RealObject& value);
			IntegerObject& operator= (value_type value);

			template <typename T>
			T SafeConvert(void) const { return gotchangpdf::SafeConvert<T>(_value); }

			operator value_type() const _NOEXCEPT { return _value; }
			value_type Value(void) const _NOEXCEPT { return _value; }

			virtual Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Integer; }

			bool Equals(const IntegerObject& other) const _NOEXCEPT { return _value == other._value; }
			virtual std::string ToPdf(void) const override { return std::to_string(_value); }

		private:
			value_type _value = 0;
		};

		IntegerObject SafeAddition(IntegerObject::value_type number, IntegerObject::value_type addend);
		IntegerObject SafeAddition(const IntegerObject& number, const IntegerObject& addend);
	}

	inline bool operator==(const syntax::IntegerObject left, syntax::IntegerObject::value_type right)
	{
		return left.Value() == right;
	}

	inline bool operator!=(const syntax::IntegerObject& left, syntax::IntegerObject::value_type right)
	{
		return left.Value() != right;
	}

	inline bool operator<(const syntax::IntegerObject& left, syntax::IntegerObject::value_type right)
	{
		return left.Value() < right;
	}

	inline bool operator==(syntax::IntegerObject::value_type left, const syntax::IntegerObject& right)
	{
		return left == right.Value();
	}

	inline bool operator!=(syntax::IntegerObject::value_type left, const syntax::IntegerObject& right)
	{
		return left != right.Value();
	}

	inline bool operator<(syntax::IntegerObject::value_type left, const syntax::IntegerObject& right)
	{
		return left < right.Value();
	}
}

#endif /* _INTEGER_OBJECT_H */
