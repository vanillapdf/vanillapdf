#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "fwd.h"
#include "object.h"
#include "integer_object.h"
#include "direct_object.h"
#include "constants.h"
#include "containable.h"
#include "objects.h"

namespace gotchangpdf
{
	class IndirectObjectReference : public Containable, public Object
	{
	public:
		IndirectObjectReference() = default;
		explicit IndirectObjectReference(DirectObject obj);

		DirectObject GetReferencedObject() const;
		inline DirectObject operator->() const { return GetReferencedObject(); }

		template <typename T>
		inline const T GetReferencedObjectAs() const
		{
			ObjectVisitor<T> visitor;
			auto direct = GetReferencedObject();
			return direct.apply_visitor(visitor);
		}

		virtual inline Object::Type GetType(void) const override { return Object::Type::IndirectReference; }
		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

		bool Equals(const IndirectObjectReference& other) const;

		inline bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
		inline bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
		bool operator<(const IndirectObjectReference& other) const;

		inline IntegerObjectPtr GetObjectNumber() const { return _ref_obj; }
		inline IntegerObjectPtr GetGenerationNumber() const { return _ref_gen; }

	public:
		IntegerObjectPtr _ref_obj = 0;
		IntegerObjectPtr _ref_gen = 0;

	private:
		mutable DirectObject _object;
		mutable bool _initialized = false;
		ContainerPtr _container;
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
