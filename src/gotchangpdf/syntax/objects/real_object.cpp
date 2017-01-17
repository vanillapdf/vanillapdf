#include "precompiled.h"

#include "syntax/objects/real_object.h"

namespace gotchangpdf {
namespace syntax {

RealObject::RealObject() {
	m_value->Subscribe(this);
}

RealObject::RealObject(types::real value) : m_value(value) {
	m_value->Subscribe(this);
}

RealObject::RealObject(types::real value, uint32_t precision) : m_value(value, precision) {
	m_value->Subscribe(this);
}
RealObject::RealObject(const NumericObject& value) {
	m_value = value.GetNumericBackend();
	m_value->Subscribe(this);
}

RealObject::RealObject(NumericObjectBackendPtr value) {
	m_value = value;
	m_value->Subscribe(this);
}

void RealObject::ObserveeChanged(IModifyObservable*) {
	OnChanged();
}

RealObject* RealObject::Clone(void) const {
	return new RealObject(m_value->Clone());
}

bool RealObject::Equals(const ObjectPtr other) const {
	if (!ObjectUtils::IsType<RealObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<RealObjectPtr>(other);
	return (GetValue() == other_obj->GetValue());
}

} // syntax
} // gotchangpdf
