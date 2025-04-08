#include "precompiled.h"

#include "syntax/objects/real_object.h"
#include "syntax/objects/integer_object.h"
#include "utils/streams/output_stream_interface.h"

#include <cassert>
#include <vector>

namespace vanillapdf {
namespace syntax {

IntegerObject::IntegerObject() {
}

IntegerObject::IntegerObject(NumericObjectBackendPtr value) {
    m_value = value;
    m_value->Subscribe(this);
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

void IntegerObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
    output->Write(m_value->ToString());
}

} // syntax
} // vanillapdf
