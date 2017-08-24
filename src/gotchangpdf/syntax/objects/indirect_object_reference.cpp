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

types::big_uint IndirectObjectReference::GetReferencedObjectNumber() const {
	if (IsReferenceInitialized()) {
		auto referenced_object = GetReferencedObject();
		return referenced_object->GetObjectNumber();
	}

	// Use the backup number for uninitialized references
	return m_reference_object_number;
}

types::ushort IndirectObjectReference::GetReferencedGenerationNumber() const {
	if (IsReferenceInitialized()) {
		auto referenced_object = GetReferencedObject();
		return referenced_object->GetGenerationNumber();
	}

	// Use the backup number for uninitialized references
	return m_reference_generation_number;
}

void IndirectObjectReference::SetReferencedObjectNumber(types::big_uint value) noexcept {
	m_reference_object_number = value;
	m_reference.Reset();
}

void IndirectObjectReference::SetReferencedGenerationNumber(types::ushort value) noexcept {
	m_reference_generation_number = value;
	m_reference.Reset();
}

//bool IndirectObjectReference::IsCyclicReference(ObjectPtr object, std::map<ObjectPtr, bool>& visited) const {
//	// We have found the cycle
//	if (Identity(object)) {
//		return true;
//	}
//
//	bool already_visited = visited[object];
//	if (already_visited) {
//		return false;
//	}
//
//	// Mark current object as visited
//	visited[object] = true;
//
//	//if (ObjectUtils::IsType<IndirectObjectReferencePtr>(object)) {
//	//	return false;
//	//}
//
//	if (ObjectUtils::IsType<DictionaryObjectPtr>(object)) {
//		auto referenced_dictionary = ObjectUtils::ConvertTo<DictionaryObjectPtr>(object);
//		for (auto item : referenced_dictionary) {
//			if (IsCyclicReference(item.second, visited)) {
//				return true;
//			}
//		}
//
//		return false;
//	}
//
//	if (ObjectUtils::IsType<MixedArrayObjectPtr>(object)) {
//		auto referenced_array = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(object);
//		for (auto item : referenced_array) {
//			if (IsCyclicReference(item, visited)) {
//				return true;
//			}
//		}
//
//		return false;
//	}
//
//	return false;
//}

size_t IndirectObjectReference::Hash() const {

	auto object_number = GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();

	std::hash<decltype(object_number)> obj_hasher;
	std::hash<decltype(generation_number)> gen_hasher;
	return obj_hasher(object_number) ^ gen_hasher(generation_number);

	//auto referenced_object = GetReferencedObject();

	// Special case, when this is a reference to object
	// which contains the reference itself
	//std::map<ObjectPtr, bool> visited;
	//if (IsCyclicReference(referenced_object, visited)) {

	//	// Has with value 0 should be ignored
	//	return 0;
	//}

	//return referenced_object->Hash();
}

void IndirectObjectReference::SetReferencedObject(ObjectPtr obj) const {
	// Reference shall be indirect object or null
	bool indirect_or_null = (obj->IsIndirect() || ObjectUtils::IsType<NullObjectPtr>(obj));
	assert(indirect_or_null && "Referenced object is neither indirect nor null"); UNUSED(indirect_or_null);

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

	if (!m_file.IsActive()) {
		throw FileDisposedException();
	}

	auto locked_file = m_file.GetReference();
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
	auto other_generation_number = other.GetReferencedGenerationNumber();

	bool object_numbers_equals = (object_number == other_object_number);
	bool generation_numbers_equals = (generation_number == other_generation_number);

	return (object_numbers_equals && generation_numbers_equals);
}

bool IndirectObjectReference::operator<(const IndirectObjectReference& other) const {
	auto object_number = GetReferencedObjectNumber();
	auto other_object_number = other.GetReferencedObjectNumber();
	if (object_number != other_object_number) {
		return object_number < other_object_number;
	}

	auto generation_number = GetReferencedGenerationNumber();
	auto other_generation_number = other.GetReferencedGenerationNumber();
	if (generation_number != other_generation_number) {
		return generation_number < other_generation_number;
	}

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

IndirectObjectReference* IndirectObjectReference::Clone(void) const {
	IndirectObjectReferencePtr result(pdf_new IndirectObjectReference(), false);

	auto object_number = GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();

	result->SetFile(m_file);
	result->SetReferencedObjectNumber(object_number);
	result->SetReferencedGenerationNumber(generation_number);

	return result.detach();
}

} // syntax
} // gotchangpdf
