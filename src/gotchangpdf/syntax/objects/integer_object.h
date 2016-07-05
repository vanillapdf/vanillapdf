#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "syntax_fwd.h"
#include "util.h"

#include "numeric_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class IntegerObject : public NumericObject, public IModifyObserver
		{
		public:
			typedef int64_t value_type;

			IntegerObject();
			explicit IntegerObject(types::native_int value);
			explicit IntegerObject(types::native_uint value);
			explicit IntegerObject(value_type value);
			explicit IntegerObject(const NumericObject& value);
			IntegerObject& operator= (value_type value);

			template <typename T>
			T SafeConvert(void) const { return ValueConvertUtils::SafeConvert<T>(m_value->GetIntegerValue()); }

			operator value_type() const noexcept { return m_value->GetIntegerValue(); }
			value_type GetValue(void) const noexcept { return m_value->GetIntegerValue(); }
			void SetValue(value_type value) noexcept { m_value->SetIntegerValue(value); OnChanged(); }

			virtual Object::Type GetType(void) const noexcept override { return Object::Type::Integer; }

			bool Equals(const IntegerObject& other) const noexcept { return m_value == other.m_value; }
			virtual std::string ToPdf(void) const override { return m_value->ToString(); }

			virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

			virtual Object* Clone(void) const override { return new IntegerObject(*this); }
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
