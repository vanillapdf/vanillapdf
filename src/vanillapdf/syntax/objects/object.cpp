#include "precompiled.h"
#include "syntax/objects/object.h"
#include "syntax/files/xref.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "utils/streams/output_stream.h"
#include "utils/streams/output_stream_interface.h"

#include "syntax/utils/base_object_attribute.h"
#include "syntax/utils/serialization_override_object_attribute.h"

#include <cassert>

namespace vanillapdf {
namespace syntax {

ObjectPtr::ObjectPtr() : Deferred<Object>(NullObject::GetInstance()) {
}

ObjectPtr::ObjectPtr(Deferred<Object> value) : Deferred<Object>(value) {

}

bool Object::HasOverrideAttribute() const {
	return ContainsAttribute(BaseAttribute::Type::SerializationOverride);
}

std::string Object::GetOverrideAttribute() const {
	// This was first introduced with the digital signatures support
	// Whole file first has to be written, and at the end the byte range
	// and signature value will be updated. Since we don't know the values
	// we can either write the file multiple times OR we leave some space
	// for the the values, when we later update them.

	auto base_attribute = GetAttribute(BaseAttribute::Type::SerializationOverride);
	auto override_attribute = ConvertUtils<decltype(base_attribute)>::ConvertTo<SerializationOverrideAttributePtr>(base_attribute);
	return override_attribute->GetValue();
}

std::string Object::ToPdf(void) const {

	// If the object contains attribute, that controls it's serialization
	if (HasOverrideAttribute()) {
		return GetOverrideAttribute();
	}

	auto ss = std::make_shared<std::stringstream>();
	OutputStreamPtr output_stream = make_deferred<OutputStream>(ss);
	ToPdfStream(output_stream);
	return ss->str();
}

void Object::ToPdfStream(IOutputStreamPtr output) const {

	// If the object contains attribute, that controls it's serialization
	if (HasOverrideAttribute()) {
		auto override_attribute = GetOverrideAttribute();
		output->Write(override_attribute);
		return;
	}

	ToPdfStreamInternal(output);
}

void Object::ToPdfStreamUpdateOffset(IOutputStreamPtr output) {
	UpdateOffset(output);
	ToPdfStream(output);
}

void Object::UpdateOffset(IOutputStreamPtr output) {
	bool is_indirect = IsIndirect();
	if (!is_indirect) {
		auto new_offset = output->GetOutputPosition();
		SetOffset(new_offset);
	}
}

bool Object::IsIndirect(void) const {
	bool is_indirect = (!m_entry.IsEmpty() && m_entry.IsActive());
	if (is_indirect) {

		// Verify that the entry refers to this object
		auto entry = m_entry.GetReference();
		if (entry->InUse()) {
			bool identity = Identity(entry->GetReference());
			assert(identity && "Indirect entry does not reference to this object"); UNUSED(identity);
		}
	}

	return is_indirect;
}

void Object::SetXrefEntry(XrefUsedEntryBasePtr entry) {
	// I have call explicit conversion to weak reference,
	// because it could be either XrefEntryBase or XrefUsedEntryBase
	m_entry = entry->IWeakReferenceable<XrefUsedEntryBase>::GetWeakReference();
}

WeakReference<XrefUsedEntryBase> Object::GetXrefEntry() const {
	return m_entry;
}

void Object::ClearXrefEntry(bool check_xref_reference) {
	if (check_xref_reference && !m_entry.IsEmpty() && m_entry.IsActive()) {

		// Verify that the entry refers to this object
		auto entry = m_entry.GetReference();
		if (entry->InUse()) {
			assert(entry->GetReference()->Identity(this) && "Reference entry has changed");
			entry->ReleaseReference(false);
		}
	}

	m_entry.Reset();
}

bool Object::HasOwner() const noexcept {
	return (!m_owner.IsEmpty() && m_owner.IsActive());
}

types::big_uint Object::GetObjectNumber() const {

	bool is_indirect = IsIndirect();
	bool has_owner = HasOwner();

	assert(is_indirect || has_owner);
	assert(!(is_indirect && has_owner));

	if (is_indirect) {
		XrefEntryBasePtr entry = m_entry.GetReference();
		return entry->GetObjectNumber();
	}

	if (has_owner) {
		throw GeneralException("Object does not an indirect object, but is a composite of another object");
	}

	throw GeneralException("Object does not have assigned object number");
}

types::ushort Object::GetGenerationNumber() const {

	bool is_indirect = IsIndirect();
	bool has_owner = HasOwner();

	assert(is_indirect || has_owner);
	assert(!(is_indirect && has_owner));

	if (is_indirect) {
		XrefEntryBasePtr entry = m_entry.GetReference();
		return entry->GetGenerationNumber();
	}

	if (has_owner) {
		throw GeneralException("Object does not an indirect object, but a composite of another object");
	}

	throw GeneralException("Object does not have assigned generation number");
}

types::big_uint Object::GetRootObjectNumber() const {

	bool is_indirect = IsIndirect();
	bool has_owner = HasOwner();

	assert(is_indirect || has_owner);
	assert(!(is_indirect && has_owner));

	if (is_indirect) {
		XrefEntryBasePtr entry = m_entry.GetReference();
		return entry->GetObjectNumber();
	}

	if (has_owner) {
		ObjectPtr owner = m_owner.GetReference();
		return owner->GetRootObjectNumber();
	}

	throw GeneralException("Object does not have assigned object number");
}

types::ushort Object::GetRootGenerationNumber() const {

	bool is_indirect = IsIndirect();
	bool has_owner = HasOwner();

	assert(is_indirect || has_owner);
	assert(!(is_indirect && has_owner));

	if (is_indirect) {
		XrefEntryBasePtr entry = m_entry.GetReference();
		return entry->GetGenerationNumber();
	}

	if (has_owner) {
		ObjectPtr owner = m_owner.GetReference();
		return owner->GetRootGenerationNumber();
	}

	throw GeneralException("Object does not have assigned generation number");
}

bool Object::IsEncryptionExempted() const {
	if (m_encryption_exempted) {
		return true;
	}

	if (HasOwner()) {
		ObjectPtr owner = m_owner.GetReference();
		return owner->IsEncryptionExempted();
	}

	return false;
}

void Object::OnChanged() {
	if (!m_initialized) {
		return;
	}

	m_dirty = true;
	IModifyObservable::OnChanged();
}

const char* Object::TypeName(Type type) {
	// TODO this should be a single line solution using preprocessor

	switch (type) {
		case Type::Array:
			return "Array";
		case Type::Boolean:
			return "Boolean";
		case Type::Dictionary:
			return "Dictionary";
		case Type::Integer:
			return "Integer";
		case Type::Name:
			return "Name";
		case Type::Null:
			return "Null";
		case Type::Real:
			return "Real";
		case Type::Stream:
			return "Stream";
		case Type::String:
			return "String";
		case Type::IndirectReference:
			return "IndirectReference";
		default:
			return nullptr;
	}
}

void Object::CloneBaseProperties(Object* other) const {
	assert(other != nullptr && "Invalid clone object parameter");
	if (other == nullptr) {
		throw GeneralException("Invalid clone object parameter");
	}

	other->SetFile(m_file);
	other->SetDirty(m_dirty);
	other->SetEncryptionExempted(m_encryption_exempted);

	for (auto attribute : m_attributes) {
		auto new_attribute = attribute.second->Clone();
		other->AddAttribute(new_attribute);
	}
}

bool Object::Identity(ObjectPtr other) const {
	auto other_ptr = other.get();
	return (this == other_ptr);
}

void Object::AddAttribute(BaseAttributePtr attribute) {
	m_attributes->Add(attribute);
}
bool Object::RemoveAttribute(BaseAttribute::Type type) {
	return m_attributes->Remove(type);
}

bool Object::ContainsAttribute(BaseAttribute::Type type) const {
	return m_attributes->Contains(type);
}

BaseAttributePtr Object::GetAttribute(BaseAttribute::Type type) const {
	return m_attributes->Get(type);
}

AttributeListPtr Object::GetAttributeList() const {
	return m_attributes;
}

} // syntax

bool operator==(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right) {
	return left->Equals(right);
}

bool operator!=(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right) {
	return !left->Equals(right);
}

bool operator<(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right) {
	return (left.get() < right.get());
}

} // vanillapdf
