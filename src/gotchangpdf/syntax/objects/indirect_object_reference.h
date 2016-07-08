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
			IndirectObjectReference(types::big_uint obj, types::ushort gen);

			ObjectPtr GetReferencedObject() const;
			ObjectPtr operator->() const { return GetReferencedObject(); }

			template <typename T>
			const T GetReferencedObjectAs() const
			{
				auto direct = GetReferencedObject();
				return ObjectUtils::ConvertTo<T>(direct);
			}

			virtual Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::IndirectReference; }
			virtual std::string ToPdf(void) const override;

			bool Equals(const IndirectObjectReference& other) const;

			bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
			bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
			bool operator<(const IndirectObjectReference& other) const;

			types::big_uint GetReferencedObjectNumber() const _NOEXCEPT { return _ref_obj; }
			types::ushort GetReferencedGenerationNumber() const _NOEXCEPT { return _ref_gen; }

			virtual IndirectObjectReference* Clone(void) const override { return new IndirectObjectReference(*this); }

		private:
			types::big_uint _ref_obj = 0;
			types::ushort _ref_gen = 0;
		};
	}
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
