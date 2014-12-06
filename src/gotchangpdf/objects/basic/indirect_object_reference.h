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
		explicit IndirectObjectReference(files::File * file);
		IndirectObjectReference(files::File * file,
			types::integer obj_number,
			types::ushort gen_number);

		IndirectObjectPtr GetReferencedObject() const;
		inline IndirectObjectPtr operator->() const { return GetReferencedObject(); }

		template <typename T>
		inline const T GetReferencedObjectAs() const { return GetReferencedObject()->GetObjectAs<T>(); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::IndirectObjectReference; }

		inline void SetObjectNumber(IntegerObjectPtr number){ _obj_number = number; }
		inline IntegerObjectPtr GetObjectNumber() const { return _obj_number; }

		inline void SetGenerationNumber(IntegerObjectPtr number){ _gen_number = number; }
		inline IntegerObjectPtr GetGenerationNumber() const { return _gen_number; }

		inline void SetFile(files::File *file) { _file = file; }
		inline files::File* GetFile() const { return _file; }

	public:
		IntegerObjectPtr _obj_number = 0;
		IntegerObjectPtr _gen_number = 0;

	private:
		files::File * _file;
		mutable IndirectObjectPtr _reference;

		explicit IndirectObjectReference() = default;
		friend Deferred<IndirectObjectReference>;

		template <typename T>
		friend T* Allocate();
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
