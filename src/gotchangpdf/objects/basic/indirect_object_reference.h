#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "fwd.h"
#include "object.h"
#include "constants.h"
#include "indirect_object.h"
#include "containerable.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObjectReference : public Object, public ParentContainer<ContainerPtr>
	{
	public:
		IndirectObjectReference() = default;
		explicit IndirectObjectReference(files::File * file);
		IndirectObjectReference(files::File * file,
			types::integer obj_number,
			types::ushort gen_number);

		Deferred<IndirectObject> GetReferencedObject() const;
		Deferred<IndirectObject> operator->() const;

		template <typename T>
		const T GetReferencedObjectAs() const;

		virtual Object::Type GetType(void) const override;

		void SetObjectNumber(IntegerObjectPtr number);
		IntegerObjectPtr GetObjectNumber() const;

		void SetGenerationNumber(IntegerObjectPtr number);
		IntegerObjectPtr GetGenerationNumber() const;

	public:
		IntegerObjectPtr _obj_number = 0;
		IntegerObjectPtr _gen_number = 0;

	private:
		files::File * _file;
		mutable Deferred<IndirectObject> _reference;
	};

	template <typename T>
	const T IndirectObjectReference::GetReferencedObjectAs() const
	{
		return GetReferencedObject()->GetObjectAs<T>();
	}

	inline Object::Type IndirectObjectReference::GetType(void) const
	{
		return Object::Type::IndirectObjectReference;
	}

	inline Deferred<IndirectObject> IndirectObjectReference::operator->() const
	{
		return GetReferencedObject();
	}

	inline void IndirectObjectReference::SetObjectNumber(IntegerObjectPtr number)
	{
		_obj_number = number;
	}

	inline IntegerObjectPtr IndirectObjectReference::GetObjectNumber() const
	{
		return _obj_number;
	}

	inline void IndirectObjectReference::SetGenerationNumber(IntegerObjectPtr number)
	{
		_gen_number = number;
	}

	inline IntegerObjectPtr IndirectObjectReference::GetGenerationNumber() const
	{
		return _gen_number;
	}
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
