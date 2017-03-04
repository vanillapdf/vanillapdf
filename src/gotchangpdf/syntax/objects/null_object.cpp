#include "precompiled.h"

#include "syntax/objects/null_object.h"

namespace gotchangpdf {
namespace syntax {

bool NullObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<NullObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<NullObjectPtr>(other);
	return Equals(*other_obj);
}

bool NullObject::Equals(const NullObject&) const {
	// In future there will be separate null object for every file
	//return GetFile() == other.GetFile();

	return true;
}

NullObjectPtr NullObject::GetInstance(void) {
	static NullObjectPtr instance = NullObjectPtr(pdf_new NullObject());
	return instance;
}

std::string NullObject::ToPdf(void) const {
	return "null";
}

NullObject* NullObject::Clone(void) const {
	NullObjectPtr result(pdf_new NullObject(), false);

	result->SetFile(m_file);

	return result.detach();
}

} // syntax
} // gotchangpdf
