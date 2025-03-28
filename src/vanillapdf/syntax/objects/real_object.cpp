#include "precompiled.h"

#include "syntax/objects/real_object.h"
#include "utils/streams/output_stream_interface.h"

namespace vanillapdf {
namespace syntax {

RealObject::RealObject() {
}

RealObject::RealObject(types::real value) {
    m_value->SetRealValue(value);
}

RealObject::RealObject(types::real value, uint32_t precision) {
    m_value->SetRealValue(value);
    m_value->SetRealPrecision(precision);
}

RealObject::RealObject(NumericObjectBackendPtr value) {
    m_value = value;
    m_value->Subscribe(this);
}

RealObject* RealObject::Clone(void) const {
    NumericObjectBackendPtr new_value(m_value->Clone());
    RealObjectPtr result(pdf_new RealObject(new_value), false);

    CloneBaseProperties(result);
    return result.detach();
}

bool RealObject::Equals(const ObjectPtr other) const {
    if (!ObjectUtils::IsType<RealObjectPtr>(other)) {
        return false;
    }

    auto other_obj = ObjectUtils::ConvertTo<RealObjectPtr>(other);
    return (GetValue() == other_obj->GetValue());
}

void RealObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
    output << m_value->ToString();
}

} // syntax
} // vanillapdf
