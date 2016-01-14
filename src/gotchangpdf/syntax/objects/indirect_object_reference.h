#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "syntax_fwd.h"
#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class IndirectObjectReference : public ContainableObject
		{
		public:
			IndirectObjectReference() = default;
			explicit IndirectObjectReference(ObjectPtr obj);
			IndirectObjectReference(types::integer obj, types::ushort gen);

			ObjectPtr GetReferencedObject() const;
			inline ObjectPtr operator->() const { return GetReferencedObject(); }

			template <typename T>
			inline const T GetReferencedObjectAs() const
			{
				auto direct = GetReferencedObject();
				return ObjectUtils::ConvertTo<T>(direct);
			}

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::IndirectReference; }
			virtual std::string ToPdf(void) const override;

			bool Equals(const IndirectObjectReference& other) const;

			inline bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
			inline bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
			bool operator<(const IndirectObjectReference& other) const;

			inline types::uinteger GetReferencedObjectNumber() const _NOEXCEPT { return _ref_obj; }
			inline types::ushort GetReferencedGenerationNumber() const _NOEXCEPT { return _ref_gen; }

		private:
			types::uinteger _ref_obj = 0;
			types::ushort _ref_gen = 0;
		};
	}
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
