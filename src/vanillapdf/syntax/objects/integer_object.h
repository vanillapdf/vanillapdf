#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/numeric_object.h"

#include "utils/conversion_utils.h"

namespace vanillapdf {
namespace syntax {

class IntegerObject : public NumericObject {
public:
	IntegerObject();
	IntegerObject(const IntegerObject&) = delete;

	explicit IntegerObject(NumericObjectBackendPtr value);

	template <
		typename T,
		typename std::enable_if<
			std::is_integral<T>::value &&
			!std::is_same<T, bool>::value &&
			std::is_signed<T>::value,
			T
		>::type = true
	>
	explicit IntegerObject(T value) {
		m_value->SetIntegerValue(value);
	}

	template <
		typename T,
		typename std::enable_if<
			std::is_integral<T>::value &&
			!std::is_same<T, bool>::value &&
			!std::is_signed<T>::value,
			T
		>::type = true
	>
	explicit IntegerObject(T value) {
		m_value->SetUnsignedIntegerValue(value);
	}

	template <
		typename T,
		typename std::enable_if<
			std::is_integral<T>::value &&
			!std::is_same<T, bool>::value &&
			std::is_signed<T>::value,
			T
		>::type = true
	>
	IntegerObject& operator= (T value) {
		m_value->SetIntegerValue(value);
		return *this;
	}

	template <
		typename T,
		typename std::enable_if<
			std::is_integral<T>::value &&
			!std::is_same<T, bool>::value &&
			!std::is_signed<T>::value,
			T
		>::type = true
	>
	IntegerObject& operator= (T value) {
		m_value->SetUnsignedIntegerValue(value);
		return *this;
	}

	template <typename T>
	T SafeConvert(void) const { return ValueConvertUtils::SafeConvert<T>(m_value->GetIntegerValue()); }

	operator types::big_int() const { return m_value->GetIntegerValue(); }
	types::big_int GetIntegerValue(void) const { return m_value->GetIntegerValue(); }
	types::big_uint GetUnsignedIntegerValue(void) const { return m_value->GetUnsignedIntegerValue(); }
	types::real GetRealValue(void) const { return m_value->GetRealValue(); }

	template <
		typename T,
		typename std::enable_if<
			std::is_integral<T>::value &&
			!std::is_same<T, bool>::value &&
			std::is_signed<T>::value,
			T
		>::type = true
	>
	void SetValue(T value) {
		m_value->SetIntegerValue(value);
	}

	template <
		typename T,
		typename std::enable_if<
			std::is_integral<T>::value &&
			!std::is_same<T, bool>::value &&
			!std::is_signed<T>::value,
			T
		>::type = true
	>
	void SetValue(T value) {
		m_value->SetUnsignedIntegerValue(value);
	}

	void SetValue(types::real value) { m_value->SetRealValue(value); }

	void ToggleBit(int pos, bool value) { m_value->ToggleBit(pos, value); }
	void SetBit(int pos) { m_value->SetBit(pos); }
	void ClearBit(int pos) { m_value->ClearBit(pos); }
	bool IsBitSet(int pos) const { return m_value->IsBitSet(pos); }

	virtual Object::Type GetObjectType(void) const noexcept override { return Object::Type::Integer; }

	bool Equals(const IntegerObject& other) const { return GetIntegerValue() == other.GetIntegerValue(); }
	virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

	virtual IntegerObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;
};

} // syntax

inline bool operator==(const syntax::IntegerObject& left, int32_t right) {
	return left.GetIntegerValue() == right;
}

inline bool operator!=(const syntax::IntegerObject& left, int32_t right) {
	return left.GetIntegerValue() != right;
}

inline bool operator<(const syntax::IntegerObject& left, int32_t right) {
	return left.GetIntegerValue() < right;
}

inline bool operator>(const syntax::IntegerObject& left, int32_t right) {
	return left.GetIntegerValue() > right;
}

inline bool operator==(int32_t left, const syntax::IntegerObject& right) {
	return left == right.GetIntegerValue();
}

inline bool operator!=(int32_t left, const syntax::IntegerObject& right) {
	return left != right.GetIntegerValue();
}

inline bool operator<(int32_t left, const syntax::IntegerObject& right) {
	return left < right.GetIntegerValue();
}

inline bool operator>(int32_t left, const syntax::IntegerObject& right) {
	return left > right.GetIntegerValue();
}

} // vanillapdf

#endif /* _INTEGER_OBJECT_H */
