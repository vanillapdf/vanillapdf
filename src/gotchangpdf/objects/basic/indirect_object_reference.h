#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "fwd.h"
#include "indirect_object.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObjectReference : public Object
	{
	public:
		explicit IndirectObjectReference(std::shared_ptr<files::File> file);
		IndirectObjectReference(std::shared_ptr<files::File> file, unsigned int obj_number, unsigned int gen_number);

		SmartPtr<IndirectObject> GetReferencedObject() const;

		inline SmartPtr<IndirectObject> operator->() const { return GetReferencedObject(); }

		template <typename T>
		SmartPtr<T> GetReferencedObjectAs() const { return GetReferencedObject()->GetObjectAs<T>(); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::IndirectObjectReference; }

	private:
		std::shared_ptr<files::File> _file;
		mutable SmartPtr<IndirectObject> _reference;

		unsigned int _obj_number, _gen_number;
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
