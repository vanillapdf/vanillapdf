#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "syntax_fwd.h"
#include "containable.h"
#include "objects.h"
#include "direct_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class IndirectObjectReference : public Containable, public Object
		{
		public:
			IndirectObjectReference() = default;
			explicit IndirectObjectReference(DirectObject obj);
			IndirectObjectReference(types::integer obj, types::ushort gen);

			DirectObject GetReferencedObject() const;
			inline DirectObject operator->() const { return GetReferencedObject(); }

			template <typename T>
			inline const T GetReferencedObjectAs() const
			{
				ObjectVisitor<T> visitor;
				auto direct = GetReferencedObject();
				return direct.apply_visitor(visitor);
			}

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::IndirectReference; }
			virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
			virtual inline ContainerPtr GetContainer() const override { return _container; }

			bool Equals(const IndirectObjectReference& other) const;

			inline bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
			inline bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
			bool operator<(const IndirectObjectReference& other) const;

			inline types::integer GetReferencedObjectNumber() const _NOEXCEPT { return _ref_obj; }
			inline types::ushort GetReferencedGenerationNumber() const _NOEXCEPT { return _ref_gen; }

		private:
			mutable DirectObject _object;
			mutable bool _initialized = false;
			ContainerPtr _container;

			types::integer _ref_obj = 0;
			types::ushort _ref_gen = 0;
		};
	}
}

#include "object_visitors.h"

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
