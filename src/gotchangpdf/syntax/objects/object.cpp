#include "precompiled.h"
#include "object.h"
#include "xref.h"

#include "exception.h"

#include <cassert>

namespace gotchangpdf
{
	namespace syntax
	{
		ObjectPtr::ObjectPtr() : Deferred<Object>(NullObject::GetInstance())
		{

		}

		bool Object::IsIndirect(void) const noexcept
		{
			return !m_entry.IsEmpty() && m_entry.IsActive();
		}

		void Object::SetXrefEntry(XrefEntryBasePtr entry)
		{
			m_entry = entry->GetWeakReference<XrefEntryBase>();
		}

		void Object::ClearXrefEntry()
		{
			m_entry.Reset();
		}

		bool Object::HasOwner() const noexcept
		{
			return !m_owner.IsEmpty() && m_owner.IsActive();
		}

		types::big_uint Object::GetObjectNumber() const
		{
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

		types::ushort Object::GetGenerationNumber() const
		{
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

		bool Object::IsEncryptionExempted() const noexcept
		{
			if (m_encryption_exempted) {
				return true;
			}

			if (HasOwner()) {
				ObjectPtr owner = m_owner.GetReference();
				return owner->IsEncryptionExempted();
			}

			return false;
		}

		void Object::OnChanged()
		{
			if (!m_initialized) {
				return;
			}

			SetDirty();
			IModifyObservable::OnChanged();
		}

		const char* Object::TypeName(Type type)
		{
			// TODO this should be a single line solution using preprocessor

			switch (type)
			{
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
	}
}
