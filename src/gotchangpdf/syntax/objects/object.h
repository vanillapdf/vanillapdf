#ifndef _OBJECT_H
#define _OBJECT_H

#include "syntax/utils/syntax_fwd.h"

#include "utils/constants.h"
#include "utils/unknown_interface.h"
#include "utils/attribute_list.h"
#include "utils/modify_observer_interface.h"

#include <memory>
#include <string>

namespace gotchangpdf {
namespace syntax {

class Object : public virtual IUnknown, public IWeakReferenceable<Object>, public IModifyObservable {
public:
	enum class Type {
		Null = 0,
		Array,
		Boolean,
		Dictionary,
		Integer,
		Name,
		Real,
		Stream,
		String,
		IndirectReference
	};

	static const char* TypeName(Type type);

public:
	virtual Type GetType(void) const noexcept = 0;
	virtual std::string ToString(void) const { return ToPdf(); }
	virtual std::string ToPdf(void) const = 0;

public:
	bool IsIndirect(void) const noexcept;
	void SetXrefEntry(XrefUsedEntryBasePtr entry);
	WeakReference<XrefUsedEntryBase> GetXrefEntry() const;
	void ClearXrefEntry(bool check_xref_reference);

	bool IsDirty(void) const noexcept { return m_dirty; }
	void SetDirty(bool dirty = true) noexcept { m_dirty = dirty; }

	bool IsEncryptionExempted() const noexcept;
	void SetEncryptionExempted(bool exempted = true) noexcept { m_encryption_exempted = exempted; }

	void SetOffset(types::stream_offset offset) noexcept { m_offset = offset; }
	types::stream_offset GetOffset() const noexcept { return m_offset; }

	types::big_uint GetObjectNumber() const;
	types::ushort GetGenerationNumber() const;

	void SetOwner(WeakReference<Object> owner) noexcept { m_owner = owner; }
	WeakReference<Object> GetOwner() const noexcept { return m_owner; }
	void ClearOwner() noexcept { m_owner.Reset(); }
	bool HasOwner() const noexcept;

	virtual void SetFile(WeakReference<File> file) noexcept { m_file = file; }
	WeakReference<File> GetFile() const noexcept { return m_file; }

	void AddAttribute(IAttributePtr attribute);
	bool RemoveAttribute(IAttributePtr attribute);
	bool ContainsAttribute(IAttribute::Type type) const;
	IAttributePtr GetAttribute(IAttribute::Type type);

	template <typename T>
	T GetAttributeAs(IAttribute::Type type) {
		return m_attributes.GetAs<T>(type);
	}

	bool Identity(ObjectPtr other) const;

	virtual size_t Hash() const = 0;
	virtual bool Equals(ObjectPtr other) const = 0;
	virtual Object* Clone(void) const = 0;

	virtual void OnChanged() override;

protected:
	WeakReference<File> m_file;
	bool m_dirty = false;
	types::stream_offset m_offset = constant::BAD_OFFSET;
	bool m_encryption_exempted = false;
	WeakReference<XrefUsedEntryBase> m_entry;
	WeakReference<Object> m_owner;

	AttributeList m_attributes;
};

class ObjectPtr : public Deferred<Object> {
	using Deferred<Object>::Deferred;

public:
	ObjectPtr();
};

} // syntax

bool operator==(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right);
bool operator!=(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right);
bool operator<(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right);

} // gotchangpdf

namespace std {

template <> struct hash<gotchangpdf::syntax::ObjectPtr> {
	size_t operator()(const gotchangpdf::syntax::ObjectPtr& x) const {
		return x->Hash();
	}
};

} // std

#endif /* _OBJECT_H */
