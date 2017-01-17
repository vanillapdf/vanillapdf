#include "precompiled.h"

#include "syntax/objects/boolean_object.h"

namespace gotchangpdf {
namespace syntax {

bool BooleanObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<BooleanObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<BooleanObjectPtr>(other);
	return (GetValue() == other_obj->GetValue());
}

} // syntax
} // gotchangpdf
