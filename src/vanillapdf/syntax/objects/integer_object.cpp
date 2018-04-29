#include "precompiled.h"

#include "syntax/objects/real_object.h"
#include "syntax/objects/integer_object.h"
#include "utils/streams/output_stream_interface.h"

#include <cassert>
#include <vector>

namespace vanillapdf {
namespace syntax {

IntegerObject::IntegerObject() {
	m_value->Subscribe(this);
}

IntegerObject::~IntegerObject() {
	m_value->Unsubscribe(this);
}

IntegerObject::IntegerObject(int32_t value) {
	m_value->SetIntegerValue(value);
	m_value->Subscribe(this);
}

IntegerObject::IntegerObject(uint32_t value) {
	m_value->SetUnsignedIntegerValue(value);
	m_value->Subscribe(this);
}

IntegerObject::IntegerObject(types::big_int value) {
	m_value->SetIntegerValue(value);
	m_value->Subscribe(this);
}

IntegerObject::IntegerObject(types::big_uint value) {
	m_value->SetUnsignedIntegerValue(value);
	m_value->Subscribe(this);
}

IntegerObject::IntegerObject(const NumericObject& value) {
	m_value = value.GetNumericBackend();
	m_value->Subscribe(this);
}

IntegerObject::IntegerObject(NumericObjectBackendPtr value) {
	m_value = value;
	m_value->Subscribe(this);
}

IntegerObject& IntegerObject::operator=(int32_t value) {
	m_value->SetIntegerValue(value);
	return *this;
}

IntegerObject& IntegerObject::operator=(uint32_t value) {
	m_value->SetUnsignedIntegerValue(value);
	return *this;
}

IntegerObject& IntegerObject::operator=(types::big_int value) {
	m_value->SetIntegerValue(value);
	return *this;
}

IntegerObject& IntegerObject::operator=(types::big_uint value) {
	m_value->SetUnsignedIntegerValue(value);
	return *this;
}

bool IntegerObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<IntegerObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<IntegerObjectPtr>(other);
	return Equals(*other_obj);
}

IntegerObject* IntegerObject::Clone(void) const {
	NumericObjectBackendPtr new_value(m_value->Clone());
	IntegerObjectPtr result(pdf_new IntegerObject(new_value), false);

	CloneBaseProperties(result);

	return result.detach();
}

void IntegerObject::ToPdfStream(IOutputStreamPtr output) const {
	output->Write(m_value->ToString());
}

} // syntax
} // vanillapdf
