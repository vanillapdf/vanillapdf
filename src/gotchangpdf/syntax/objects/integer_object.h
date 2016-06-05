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
			typedef int64_t value_type;

			IntegerObject() = default;
			explicit IntegerObject(types::native_int value);
			explicit IntegerObject(types::native_uint value);
			explicit IntegerObject(value_type value);
			explicit IntegerObject(const RealObject& value);
			IntegerObject& operator= (value_type value);

			template <typename T>
			T SafeConvert(void) const { return ValueConvertUtils::SafeConvert<T>(_value); }

			operator value_type() const noexcept { return _value; }
			value_type GetValue(void) const noexcept { return _value; }
			void SetValue(value_type value) noexcept { _value = value; OnChanged(); }

			virtual Object::Type GetType(void) const noexcept override { return Object::Type::Integer; }

			bool Equals(const IntegerObject& other) const noexcept { return _value == other._value; }
			virtual std::string ToPdf(void) const override { return std::to_string(_value); }

		private:
			value_type _value = 0;
		};
	}

	inline bool operator==(const syntax::IntegerObject left, types::native_int right)
	{
		return left.GetValue() == right;
	}

	inline bool operator!=(const syntax::IntegerObject& left, types::native_int right)
	{
		return left.GetValue() != right;
	}

	inline bool operator<(const syntax::IntegerObject& left, types::native_int right)
	{
		return left.GetValue() < right;
	}

	inline bool operator>(const syntax::IntegerObject& left, types::native_int right)
	{
		return left.GetValue() > right;
	}

	inline bool operator==(types::native_int left, const syntax::IntegerObject& right)
	{
		return left == right.GetValue();
	}

	inline bool operator!=(types::native_int left, const syntax::IntegerObject& right)
	{
		return left != right.GetValue();
	}

	inline bool operator<(types::native_int left, const syntax::IntegerObject& right)
	{
		return left < right.GetValue();
	}

	inline bool operator>(types::native_int left, const syntax::IntegerObject& right)
	{
		return left > right.GetValue();
	}
}

#endif /* _INTEGER_OBJECT_H */
