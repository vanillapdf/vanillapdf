#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

namespace gotchangpdf {
namespace syntax {

class RealObject : public NumericObject, public IModifyObserver {
public:
	typedef types::real value_type;

public:
	RealObject();
	explicit RealObject(types::real value);
	explicit RealObject(types::real value, uint32_t precision);
	explicit RealObject(const NumericObject& value);
	explicit RealObject(NumericObjectBackendPtr value);

	virtual Object::Type GetType(void) const noexcept override;
	virtual std::string ToPdf(void) const override;

	operator value_type() const;
	value_type GetValue(void) const;
	void SetValue(value_type value);

	virtual void ObserveeChanged(IModifyObservable*) override;
	virtual RealObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;

private:
	NumericObjectBackendPtr m_value;
};

inline Object::Type RealObject::GetType(void) const noexcept {
	return Object::Type::Real;
}

inline std::string RealObject::ToPdf(void) const {
	return m_value->ToString();
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
} // gotchangpdf

#endif /* _REAL_OBJECT_H */
