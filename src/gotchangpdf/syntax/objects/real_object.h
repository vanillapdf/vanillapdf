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
	RealObject() { m_value->Subscribe(this); }
	explicit RealObject(types::real value) : m_value(value) { m_value->Subscribe(this); }
	explicit RealObject(types::real value, uint32_t precision) : m_value(value, precision) { m_value->Subscribe(this); }
	explicit RealObject(const NumericObject& value) {
		m_value = value.GetNumericBackend();
		m_value->Subscribe(this);
	}

	explicit RealObject(NumericObjectBackendPtr value) {
		m_value = value;
		m_value->Subscribe(this);
	}

	virtual Object::Type GetType(void) const noexcept override { return Object::Type::Real; }
	virtual std::string ToPdf(void) const override { return m_value->ToString(); }

	operator value_type() const { return m_value->GetRealValue(); }
	value_type GetValue(void) const { return m_value->GetRealValue(); }
	void SetValue(value_type value) { m_value->SetRealValue(value); }

	virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

	virtual RealObject* Clone(void) const override { return new RealObject(m_value->Clone()); }

private:
	NumericObjectBackendPtr m_value;
};

} // syntax
} // gotchangpdf

#endif /* _REAL_OBJECT_H */
