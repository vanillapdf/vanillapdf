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
			IntegerObject();
			explicit IntegerObject(types::native_int value);
			explicit IntegerObject(types::native_uint value);
			explicit IntegerObject(types::big_int value);
			explicit IntegerObject(types::big_uint value);
			explicit IntegerObject(const NumericObject& value);
			explicit IntegerObject(NumericObjectBackendPtr value);

			IntegerObject& operator= (types::native_int value);
			IntegerObject& operator= (types::native_uint value);
			IntegerObject& operator= (types::big_int value);
			IntegerObject& operator= (types::big_uint value);

			template <typename T>
			T SafeConvert(void) const { return ValueConvertUtils::SafeConvert<T>(m_value->GetIntegerValue()); }

			operator types::big_int() const { return m_value->GetIntegerValue(); }
			types::big_int GetIntegerValue(void) const { return m_value->GetIntegerValue(); }
			types::big_uint GetUnsignedIntegerValue(void) const { return m_value->GetUnsignedIntegerValue(); }
			types::real GetRealValue(void) const { return m_value->GetRealValue(); }

			void SetValue(types::native_int value) { m_value->SetIntegerValue(value); OnChanged(); }
			void SetValue(types::native_uint value) { m_value->SetUnsignedIntegerValue(value); OnChanged(); }
			void SetValue(types::big_int value) { m_value->SetIntegerValue(value); OnChanged(); }
			void SetValue(types::big_uint value) { m_value->SetUnsignedIntegerValue(value); OnChanged(); }
			void SetValue(types::real value) { m_value->SetRealValue(value); OnChanged(); }

			virtual Object::Type GetType(void) const noexcept override { return Object::Type::Integer; }

			bool Equals(const IntegerObject& other) const noexcept { return m_value == other.m_value; }
			virtual std::string ToPdf(void) const override { return m_value->ToString(); }

			virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

			virtual IntegerObject* Clone(void) const override { return new IntegerObject(m_value->Clone()); }
		};
	}

	inline bool operator==(const syntax::IntegerObject left, types::native_int right)
	{
		return left.GetIntegerValue() == right;
	}

	inline bool operator!=(const syntax::IntegerObject& left, types::native_int right)
	{
		return left.GetIntegerValue() != right;
	}

	inline bool operator<(const syntax::IntegerObject& left, types::native_int right)
	{
		return left.GetIntegerValue() < right;
	}

	inline bool operator>(const syntax::IntegerObject& left, types::native_int right)
	{
		return left.GetIntegerValue() > right;
	}

	inline bool operator==(types::native_int left, const syntax::IntegerObject& right)
	{
		return left == right.GetIntegerValue();
	}

	inline bool operator!=(types::native_int left, const syntax::IntegerObject& right)
	{
		return left != right.GetIntegerValue();
	}

	inline bool operator<(types::native_int left, const syntax::IntegerObject& right)
	{
		return left < right.GetIntegerValue();
	}

	inline bool operator>(types::native_int left, const syntax::IntegerObject& right)
	{
		return left > right.GetIntegerValue();
	}
}

#endif /* _INTEGER_OBJECT_H */
