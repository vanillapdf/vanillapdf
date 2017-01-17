#include "precompiled.h"

#include "syntax/objects/indirect_object_reference.h"

#include "syntax/files/file.h"

#include "syntax/exceptions/syntax_exceptions.h"

namespace gotchangpdf {
namespace syntax {

IndirectObjectReference::IndirectObjectReference(ObjectPtr obj) {
	SetReferencedObject(obj);
}

IndirectObjectReference::IndirectObjectReference(types::big_uint obj, types::ushort gen)
	: m_reference_object_number(obj), m_reference_generation_number(gen) {
}

void IndirectObjectReference::SetReferencedObject(ObjectPtr obj) const {
	// Reference shall be indirect object or null
	bool indirect_or_null = (obj->IsIndirect() || ObjectUtils::IsType<NullObjectPtr>(obj));
	assert(indirect_or_null && "Referenced object is neither indirect nor null"); indirect_or_null;

	if (obj->IsIndirect()) {
		m_reference_object_number = obj->GetObjectNumber();
		m_reference_generation_number = obj->GetGenerationNumber();
	}

	m_reference = obj;
}

ObjectPtr IndirectObjectReference::GetReferencedObject() const {
	if (IsReferenceInitialized()) {
		return m_reference.GetReference();
	}

	auto locked_file = m_file.lock();
	if (!locked_file)
		throw FileDisposedException();

	auto new_reference = locked_file->GetIndirectObject(
		m_reference_object_number,
		m_reference_generation_number);

	SetReferencedObject(new_reference);
	return new_reference;
}

bool IndirectObjectReference::Equals(const IndirectObjectReference& other) const {
	auto object_number = GetReferencedObjectNumber();
	auto other_object_number = other.GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();
	auto other_generation_number = GetReferencedGenerationNumber();

	bool object_numbers_equals = (object_number == other_object_number);
	bool generation_numbers_equals = (generation_number == other_generation_number);

	return (object_numbers_equals && generation_numbers_equals);
}

bool IndirectObjectReference::operator<(const IndirectObjectReference& other) const {
	auto object_number = GetReferencedObjectNumber();
	auto other_object_number = other.GetReferencedObjectNumber();
	if (object_number != other_object_number)
		return object_number < other_object_number;

	auto generation_number = GetReferencedGenerationNumber();
	auto other_generation_number = GetReferencedGenerationNumber();
	if (generation_number != other_generation_number)
		return generation_number < other_generation_number;

	return false;
}

std::string IndirectObjectReference::ToPdf(void) const {
	auto object_number = GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();
	return std::to_string(object_number) + " " + std::to_string(generation_number) + " R";
}

bool IndirectObjectReference::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<IndirectObjectReferencePtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<IndirectObjectReferencePtr>(other);
	return Equals(*other_obj);
}

} // syntax
} // gotchangpdf
