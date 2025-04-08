#include "precompiled.h"

#include "syntax/objects/boolean_object.h"
#include "utils/streams/output_stream_interface.h"

namespace vanillapdf {
namespace syntax {

BooleanObject::BooleanObject(bool value) : m_value(value) {
}

bool BooleanObject::GetValue(void) const noexcept {
    return m_value;
}

void BooleanObject::SetValue(bool value) {
    m_value = value;
    OnChanged();
}

BooleanObject::operator bool() const noexcept {
    return m_value;
}

size_t BooleanObject::Hash() const {
    std::hash<decltype(m_value)> hasher;
    return hasher(m_value);
}

bool BooleanObject::Equals(ObjectPtr other) const {
    if (!ObjectUtils::IsType<BooleanObjectPtr>(other)) {
        return false;
    }

    auto other_obj = ObjectUtils::ConvertTo<BooleanObjectPtr>(other);
    return (GetValue() == other_obj->GetValue());
}

void BooleanObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
    output->Write(m_value ? "true" : "false");
}

BooleanObject* BooleanObject::Clone(void) const {
    BooleanObjectPtr result(pdf_new BooleanObject(), false);
    result->SetValue(m_value);

    CloneBaseProperties(result);

    return result.detach();
}

} // syntax
} // vanillapdf
