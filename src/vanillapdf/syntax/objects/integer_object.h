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

	explicit IntegerObject(int32_t value);
	explicit IntegerObject(uint32_t value);
	explicit IntegerObject(types::big_int value);
	explicit IntegerObject(types::big_uint value);
	explicit IntegerObject(NumericObjectBackendPtr value);

	IntegerObject& operator= (int32_t value);
	IntegerObject& operator= (uint32_t value);
	IntegerObject& operator= (types::big_int value);
	IntegerObject& operator= (types::big_uint value);

	template <typename T>
	T SafeConvert(void) const { return ValueConvertUtils::SafeConvert<T>(m_value->GetIntegerValue()); }

	operator types::big_int() const { return m_value->GetIntegerValue(); }
	types::big_int GetIntegerValue(void) const { return m_value->GetIntegerValue(); }
	types::big_uint GetUnsignedIntegerValue(void) const { return m_value->GetUnsignedIntegerValue(); }
	types::real GetRealValue(void) const { return m_value->GetRealValue(); }

	void SetValue(int32_t value) { m_value->SetIntegerValue(value); }
	void SetValue(uint32_t value) { m_value->SetUnsignedIntegerValue(value); }
	void SetValue(types::big_int value) { m_value->SetIntegerValue(value); }
	void SetValue(types::big_uint value) { m_value->SetUnsignedIntegerValue(value); }
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
