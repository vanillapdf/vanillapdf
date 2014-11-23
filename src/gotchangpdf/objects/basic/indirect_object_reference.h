#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "fwd.h"
#include "constants.h"
#include "indirect_object.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObjectReference : public Object
	{
	public:
		explicit IndirectObjectReference(files::File * file);
		IndirectObjectReference(files::File * file,
			types::integer obj_number,
			types::ushort gen_number);

		SmartPtr<IndirectObject> GetReferencedObject() const;
		SmartPtr<IndirectObject> operator->() const;

		template <typename T>
		const T GetReferencedObjectAs() const;

		virtual Object::Type GetType(void) const override;

		void SetObjectNumber(types::integer number);
		types::integer GetObjectNumber() const;

		void SetGenerationNumber(types::ushort number);
		types::ushort GetGenerationNumber() const;

	public:
		types::integer _obj_number = -1;
		types::ushort _gen_number = -1;

	private:
		files::File * _file;
		mutable SmartPtr<IndirectObject> _reference;
	};

	typedef SmartPtr<IndirectObjectReference> IndirectObjectReferencePtr;

	template <typename T>
	const T IndirectObjectReference::GetReferencedObjectAs() const
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

	inline void IndirectObjectReference::SetObjectNumber(types::integer number)
	{
		_obj_number = number;
	}

	inline types::integer IndirectObjectReference::GetObjectNumber() const
	{
		return _obj_number;
	}

	inline void IndirectObjectReference::SetGenerationNumber(types::ushort number)
	{
		_gen_number = number;
	}

	inline types::ushort IndirectObjectReference::GetGenerationNumber() const
	{
		return _gen_number;
	}
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
