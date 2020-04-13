#include "precompiled.h"

#include "syntax/objects/null_object.h"
#include "utils/streams/output_stream_interface.h"

namespace vanillapdf {
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

size_t NullObject::Hash() const {
	return 0;
}

NullObjectPtr NullObject::GetInstance(void) {
	static NullObjectPtr instance = NullObjectPtr(pdf_new NullObject());
	return instance;
}

void NullObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
	output->Write("null");
}

NullObject* NullObject::Clone(void) const {

	// TODO:
	// In the document "(EN) Samsung UE75NU7172 Manual.pdf" there are so many null objects
	// The cloning of these objects takes up 2 GB RAM

	NullObjectPtr result(pdf_new NullObject(), false);
	CloneBaseProperties(result);
	return result.detach();
}

} // syntax
} // vanillapdf
