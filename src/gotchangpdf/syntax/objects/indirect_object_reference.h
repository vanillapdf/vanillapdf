#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

namespace gotchangpdf {
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
	virtual std::string ToPdf(void) const override;

	bool Equals(const IndirectObjectReference& other) const;
	virtual bool Equals(ObjectPtr other) const override;

	bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
	bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
	bool operator<(const IndirectObjectReference& other) const;

	types::big_uint GetReferencedObjectNumber() const noexcept { return m_reference_object_number; }
	types::ushort GetReferencedGenerationNumber() const noexcept { return m_reference_generation_number; }

	void SetReferencedObjectNumber(types::big_uint value) noexcept {
		m_reference_object_number = value;
		m_reference.Reset();
	}

	void SetReferencedGenerationNumber(types::ushort value) noexcept {
		m_reference_generation_number = value;
		m_reference.Reset();
	}

	bool IsReferenceInitialized(void) const noexcept {
		return !m_reference.IsEmpty() && m_reference.IsActive();
	}

	virtual size_t Hash() const override;
	virtual IndirectObjectReference* Clone(void) const override;

private:
	mutable types::big_uint m_reference_object_number = 0;
	mutable types::ushort m_reference_generation_number = 0;
	mutable WeakReference<Object> m_reference;

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
} // gotchangpdf

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
