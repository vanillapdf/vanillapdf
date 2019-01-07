#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

namespace vanillapdf {
namespace syntax {

class IndirectObjectReference : public ContainableObject {
public:
	IndirectObjectReference() = default;
	explicit IndirectObjectReference(ObjectPtr obj);
	IndirectObjectReference(types::big_uint obj, types::ushort gen);

	void SetReferencedObject(ObjectPtr obj) const;
	ObjectPtr GetReferencedObject() const;
	ObjectPtr operator->() const { return GetReferencedObject(); }

	template <typename T>
	T GetReferencedObjectAs() const {
		return InternalGetReferencedObjectAs<T>();
	}

	virtual Object::Type GetType(void) const noexcept override { return Object::Type::IndirectReference; }
	virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

	bool Equals(const IndirectObjectReference& other) const;
	virtual bool Equals(ObjectPtr other) const override;

	bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
	bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
	bool operator<(const IndirectObjectReference& other) const;

	types::big_uint GetReferencedObjectNumber() const;
	types::ushort GetReferencedGenerationNumber() const;

	void SetReferencedObjectNumber(types::big_uint value);
	void SetReferencedGenerationNumber(types::ushort value);

	bool IsReferenceInitialized(void) const;

	virtual size_t Hash() const override;
	virtual void OnChanged() const override;

	virtual IndirectObjectReference* Clone(void) const override;

private:
	mutable types::big_uint m_reference_object_number = 0;
	mutable types::ushort m_reference_generation_number = 0;
	mutable WeakReference<Object> m_reference;

	mutable size_t m_hash_cache = 0;

	//bool IsCyclicReference(ObjectPtr object, std::map<ObjectPtr, bool>& visited) const;

	template <
		typename T,

		// Lets talk about templates
		// The original code looked like this:

		// return ObjectUtils::ConvertTo<T>(direct);

		// This unfortunately does not work on gcc.
		// Problem is that class ObjectUtils requires
		// definition of IndirectObjectReference,
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

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
