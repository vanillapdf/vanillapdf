#ifndef _INDIRECT_REFERENCE_OBJECT_H
#define _INDIRECT_REFERENCE_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

namespace vanillapdf {
namespace syntax {

class IndirectReferenceObject : public ContainableObject {
public:
    IndirectReferenceObject() = default;
    //IndirectReferenceObject(const IndirectReferenceObject&) = delete;

    explicit IndirectReferenceObject(ObjectPtr obj);
    IndirectReferenceObject(types::big_uint obj, types::ushort gen);

    void SetReferencedObject(ObjectPtr obj);
    ObjectPtr GetReferencedObject() const;
    ObjectPtr operator->() const { return GetReferencedObject(); }

    template <typename T>
    T GetReferencedObjectAs() const {
        return InternalGetReferencedObjectAs<T>();
    }

    virtual Object::Type GetObjectType(void) const noexcept override { return Object::Type::IndirectReference; }
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

    bool Equals(const IndirectReferenceObject& other) const;
    virtual bool Equals(ObjectPtr other) const override;

    bool operator==(const IndirectReferenceObject& other) const { return Equals(other); }
    bool operator!=(const IndirectReferenceObject& other) const { return !Equals(other); }
    bool operator<(const IndirectReferenceObject& other) const;

    types::big_uint GetReferencedObjectNumber() const;
    types::ushort GetReferencedGenerationNumber() const;

    void SetReferencedObjectNumber(types::big_uint value);
    void SetReferencedGenerationNumber(types::ushort value);

    bool IsReferenceInitialized(void) const;

    virtual size_t Hash() const override;
    virtual void OnChanged() override;

    virtual IndirectReferenceObject* Clone(void) const override;

private:
    mutable types::big_uint m_reference_object_number = 0;
    mutable types::ushort m_reference_generation_number = 0;
    mutable WeakReference<Object> m_reference;

    mutable size_t m_hash_cache = 0;

    // The library interface wants to be thread-safe as much as possible
    // Even though the are currently no cases for multi-thread access
    // to the dictonary, let's try to be visionary and prepare for this
    std::shared_ptr<std::recursive_mutex> m_access_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());

    //bool IsCyclicReference(ObjectPtr object, std::map<ObjectPtr, bool>& visited) const;

    template <
        typename T,

        // Lets talk about templates
        // The original code looked like this:

        // return ObjectUtils::ConvertTo<T>(direct);

        // This unfortunately does not work on gcc.
        // Problem is that class ObjectUtils requires
        // definition of IndirectReferenceObject,
        // therefore this class is not yet defined.
        // To work around this problem I've passed
        // another defauled template parameter U
        // to defer resolution of class ObjectUtils.
        // That even was not enough, because the
        // compiler does not know, that this type
        // contains static templated method ConvertTo.
        // I had to explictitly specify this using
        // U::template syntax.
        typename U = ObjectUtils
    >
        T InternalGetReferencedObjectAs() const {
        auto direct = GetReferencedObject();
        return U::template ConvertTo<T>(direct);
    }
};

} // syntax
} // vanillapdf

#endif /* _INDIRECT_REFERENCE_OBJECT_H */
