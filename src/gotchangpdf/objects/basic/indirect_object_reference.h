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
		explicit IndirectObjectReference(files::File * file);
		IndirectObjectReference(files::File * file, unsigned int obj_number,
			unsigned int gen_number);

		SmartPtr<IndirectObject> GetReferencedObject() const;
		SmartPtr<IndirectObject> operator->() const;

		template <typename T>
		SmartPtr<T> GetReferencedObjectAs() const;

		virtual Object::Type GetType(void) const override;

	private:
		files::File * _file;
		mutable SmartPtr<IndirectObject> _reference;

		unsigned int _obj_number, _gen_number;
	};

	template <typename T>
	SmartPtr<T> IndirectObjectReference::GetReferencedObjectAs() const
	{
		return GetReferencedObject()->GetObjectAs<T>();
	}

	inline Object::Type IndirectObjectReference::GetType(void) const
	{
		return Object::Type::IndirectObjectReference;
	}

	inline SmartPtr<IndirectObject> IndirectObjectReference::operator->() const
	{
		return GetReferencedObject();
	}
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
