#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

namespace vanillapdf {
namespace syntax {

class RealObject : public NumericObject {
public:
	typedef types::real value_type;

public:
	RealObject();
	explicit RealObject(types::real value);
	explicit RealObject(types::real value, uint32_t precision);
	explicit RealObject(NumericObjectBackendPtr value);

	virtual Object::Type GetType(void) const noexcept override;
	virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

	operator value_type() const;
	value_type GetValue(void) const;
	void SetValue(value_type value);

	virtual RealObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;
};

inline Object::Type RealObject::GetType(void) const noexcept {
	return Object::Type::Real;
}

inline RealObject::operator RealObject::value_type() const {
	return m_value->GetRealValue();
}

inline RealObject::value_type RealObject::GetValue(void) const {
	return m_value->GetRealValue();
}

inline void RealObject::SetValue(value_type value) {
	m_value->SetRealValue(value);
}

} // syntax
} // vanillapdf

#endif /* _REAL_OBJECT_H */
