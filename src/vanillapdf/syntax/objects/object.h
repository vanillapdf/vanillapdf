#ifndef _OBJECT_H
#define _OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/utils/object_attribute_list.h"

#include "utils/constants.h"
#include "utils/unknown_interface.h"
#include "utils/modify_observer_interface.h"

#include <memory>
#include <string>

namespace vanillapdf {
namespace syntax {

class Object : public virtual IUnknown, public IWeakReferenceable<Object>, public IModifyObservable {
public:
    enum class Type {
        Undefined = 0,
        Null,
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
    virtual Type GetObjectType(void) const noexcept = 0;
    virtual std::string ToString(void) const { return ToPdf(); }
    virtual std::string ToPdf(void) const;
    virtual void ToPdfStream(IOutputStreamPtr output) const;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const = 0;
    virtual void ToPdfStreamUpdateOffset(IOutputStreamPtr output);
    virtual void UpdateOffset(IOutputStreamPtr output);

protected:
    bool HasOverrideAttribute() const;
    std::string GetOverrideAttribute() const;

public:
    bool IsIndirect(void) const;
    void SetXrefEntry(XrefUsedEntryBasePtr entry);
    WeakReference<XrefUsedEntryBase> GetXrefEntry() const;
    void ClearXrefEntry(bool check_xref_reference);

    bool IsDirty(void) const noexcept { return m_dirty; }
    void SetDirty(bool dirty = true) noexcept { m_dirty = dirty; }

    bool IsEncryptionExempted() const;
    void SetEncryptionExempted(bool exempted = true) noexcept { m_encryption_exempted = exempted; }

    void SetOffset(types::stream_offset offset) noexcept { m_offset = offset; }
    types::stream_offset GetOffset() const noexcept { return m_offset; }

    types::big_uint GetObjectNumber() const;
    types::ushort GetGenerationNumber() const;

    types::big_uint GetRootObjectNumber() const;
    types::ushort GetRootGenerationNumber() const;

    void SetOwner(WeakReference<Object> owner) noexcept { m_owner = owner; }
    WeakReference<Object> GetOwner() const noexcept { return m_owner; }
    void ClearOwner() noexcept { m_owner.Reset(); }
    bool HasOwner() const noexcept;

    virtual void SetFile(WeakReference<File> file) { m_file = file; }
    WeakReference<File> GetFile() const noexcept { return m_file; }

    void AddAttribute(BaseAttributePtr attribute);
    bool RemoveAttribute(BaseAttribute::Type type);
    bool ContainsAttribute(BaseAttribute::Type type) const;
    BaseAttributePtr GetAttribute(BaseAttribute::Type type) const;
    AttributeListPtr GetAttributeList() const;

    template <typename T>
    T GetAttributeAs(BaseAttribute::Type type) {
        return m_attributes->GetAs<T>(type);
    }

    bool Identity(ObjectPtr other) const;

    virtual size_t Hash() const = 0;
    virtual bool Equals(ObjectPtr other) const = 0;
    virtual Object* Clone(void) const = 0;

    // Raw pointer, because at this point the smart pointer may not be constructed
    // When cloning an object, it's reference counter must be zero
    void CloneBaseProperties(Object* other) const;

    virtual void OnChanged() override;

protected:
    WeakReference<File> m_file;
    types::stream_offset m_offset = constant::BAD_OFFSET;
    WeakReference<XrefUsedEntryBase> m_entry;
    WeakReference<Object> m_owner;

    bool m_encryption_exempted = false;
    bool m_dirty = false;

    AttributeListPtr m_attributes;
};

class ObjectPtr : public Deferred<Object> {
    using Deferred<Object>::Deferred;

public:
    ObjectPtr();
    ObjectPtr(Deferred<Object> value);
};

} // syntax

bool operator==(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right);
bool operator!=(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right);
bool operator<(const syntax::ObjectPtr& left, const syntax::ObjectPtr& right);

} // vanillapdf

namespace std {

template <> struct hash<vanillapdf::syntax::ObjectPtr> {
    size_t operator()(const vanillapdf::syntax::ObjectPtr& x) const {
        return x->Hash();
    }
};

} // std

#endif /* _OBJECT_H */
