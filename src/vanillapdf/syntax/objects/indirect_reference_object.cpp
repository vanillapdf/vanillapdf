#include "precompiled.h"

#include "syntax/objects/indirect_reference_object.h"

#include "syntax/files/file.h"

#include "syntax/exceptions/syntax_exceptions.h"

namespace vanillapdf {
namespace syntax {

IndirectReferenceObject::IndirectReferenceObject(ObjectPtr obj) {
	SetReferencedObject(obj);
}

IndirectReferenceObject::IndirectReferenceObject(types::big_uint obj, types::ushort gen)
	: m_reference_object_number(obj), m_reference_generation_number(gen) {
}

types::big_uint IndirectReferenceObject::GetReferencedObjectNumber() const {
	if (IsReferenceInitialized()) {
		auto referenced_object = GetReferencedObject();
		return referenced_object->GetObjectNumber();
	}

	// Use the backup number for uninitialized references
	return m_reference_object_number;
}

types::ushort IndirectReferenceObject::GetReferencedGenerationNumber() const {
	if (IsReferenceInitialized()) {
		auto referenced_object = GetReferencedObject();
		return referenced_object->GetGenerationNumber();
	}

	// Use the backup number for uninitialized references
	return m_reference_generation_number;
}

void IndirectReferenceObject::SetReferencedObjectNumber(types::big_uint value) {
	m_reference_object_number = value;
	m_reference.Reset();

	OnChanged();
}

void IndirectReferenceObject::SetReferencedGenerationNumber(types::ushort value) {
	m_reference_generation_number = value;
	m_reference.Reset();

	OnChanged();
}

bool IndirectReferenceObject::IsReferenceInitialized(void) const {
	bool active = (!m_reference.IsEmpty() && m_reference.IsActive());
	if (!active) {
		return false;
	}

	// Null reference does not count as active
	return !ObjectUtils::IsType<NullObjectPtr>(m_reference.GetReference());
}

//bool IndirectReferenceObject::IsCyclicReference(ObjectPtr object, std::map<ObjectPtr, bool>& visited) const {
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
//	//if (ObjectUtils::IsType<IndirectReferenceObjectPtr>(object)) {
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

void IndirectReferenceObject::OnChanged() {
	Object::OnChanged();

	m_hash_cache = 0;
}

size_t IndirectReferenceObject::Hash() const {

	if (m_hash_cache != 0) {
		return m_hash_cache;
	}

	auto object_number = GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();

	std::hash<decltype(object_number)> obj_hasher;
	std::hash<decltype(generation_number)> gen_hasher;

	m_hash_cache = obj_hasher(object_number) ^ gen_hasher(generation_number);
	return m_hash_cache;

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

void IndirectReferenceObject::SetReferencedObject(ObjectPtr obj) {

	// Reference shall be indirect object or null
	bool indirect_or_null = (obj->IsIndirect() || ObjectUtils::IsType<NullObjectPtr>(obj));
	assert(indirect_or_null && "Referenced object is neither indirect nor null");

	if (!indirect_or_null) {
		throw GeneralException("Indirect reference must point to indirect object");
	}

	if (obj->IsIndirect()) {
		m_reference_object_number = obj->GetObjectNumber();
		m_reference_generation_number = obj->GetGenerationNumber();
	}

	m_reference = obj;
	OnChanged();
}

ObjectPtr IndirectReferenceObject::GetReferencedObject() const {
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

	m_reference = new_reference;
	return new_reference;
}

bool IndirectReferenceObject::Equals(const IndirectReferenceObject& other) const {
	auto object_number = GetReferencedObjectNumber();
	auto other_object_number = other.GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();
	auto other_generation_number = other.GetReferencedGenerationNumber();

	bool object_numbers_equals = (object_number == other_object_number);
	bool generation_numbers_equals = (generation_number == other_generation_number);

	return (object_numbers_equals && generation_numbers_equals);
}

bool IndirectReferenceObject::operator<(const IndirectReferenceObject& other) const {
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

void IndirectReferenceObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
	auto object_number = GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();
	output << std::to_string(object_number) << " " << std::to_string(generation_number) << " R";
}

bool IndirectReferenceObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<IndirectReferenceObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<IndirectReferenceObjectPtr>(other);
	return Equals(*other_obj);
}

IndirectReferenceObject* IndirectReferenceObject::Clone(void) const {
	IndirectReferenceObjectPtr result(pdf_new IndirectReferenceObject(), false);

	auto object_number = GetReferencedObjectNumber();
	auto generation_number = GetReferencedGenerationNumber();

	result->SetReferencedObjectNumber(object_number);
	result->SetReferencedGenerationNumber(generation_number);

	CloneBaseProperties(result);

	return result.detach();
}

} // syntax
} // vanillapdf
