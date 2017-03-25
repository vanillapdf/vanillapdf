#include "precompiled.h"
#include "syntax/objects/object.h"
#include "syntax/files/xref.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include <cassert>

namespace gotchangpdf {
namespace syntax {

ObjectPtr::ObjectPtr() : Deferred<Object>(NullObject::GetInstance()) {
}

bool Object::IsIndirect(void) const noexcept {
	bool is_indirect = (!m_entry.IsEmpty() && m_entry.IsActive());
	if (is_indirect) {

		// Verify that the entry refers to this object
		auto entry = m_entry.GetReference();
		if (entry->InUse()) {
			bool identity = Identity(entry->GetReference());
			assert(identity && "Indirect entry does not reference to this object");
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
	return !m_owner.IsEmpty() && m_owner.IsActive();
}

types::big_uint Object::GetObjectNumber() const {
	assert(IsIndirect() || HasOwner());
	assert(!(IsIndirect() && HasOwner()));

	if (IsIndirect()) {
		XrefEntryBasePtr entry = m_entry.GetReference();
		return entry->GetObjectNumber();
	}

	if (HasOwner()) {
		ObjectPtr owner = m_owner.GetReference();
		return owner->GetObjectNumber();
	}

	throw GeneralException("Object does not have assigned object number");
}

types::ushort Object::GetGenerationNumber() const {
	assert(IsIndirect() || HasOwner());
	assert(!(IsIndirect() && HasOwner()));

	if (IsIndirect()) {
		XrefEntryBasePtr entry = m_entry.GetReference();
		return entry->GetGenerationNumber();
	}

	if (HasOwner()) {
		ObjectPtr owner = m_owner.GetReference();
		return owner->GetGenerationNumber();
	}

	throw GeneralException("Object does not have assigned generation number");
}

bool Object::IsEncryptionExempted() const noexcept {
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

	SetDirty();
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

bool Object::Identity(ObjectPtr other) const {
	auto other_ptr = other.get();
	return (this == other_ptr);
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

} // gotchangpdf
