#include "precompiled.h"

#include "syntax/objects/real_object.h"

namespace gotchangpdf {
namespace syntax {

RealObject::RealObject() {
	m_value->Subscribe(this);
}

RealObject::~RealObject() {
	m_value->Unsubscribe(this);
}

RealObject::RealObject(types::real value) {
	m_value->SetRealValue(value);
	m_value->Subscribe(this);
}

RealObject::RealObject(types::real value, uint32_t precision) {
	m_value->SetRealValue(value);
	m_value->SetRealPrecision(precision);
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
	NumericObjectBackendPtr new_value(m_value->Clone());
	RealObjectPtr result(new_value);
	return result.detach();
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
