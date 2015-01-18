#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "fwd.h"
#include "object.h"
#include "objects.h"
#include "constants.h"
#include "containable.h"

#include <memory>

namespace gotchangpdf
{
	class IndirectObjectReference : public Containable, public Object
	{
	public:
		IndirectObjectReference(files::File * file,
			IntegerObjectPtr obj_number,
			IntegerObjectPtr gen_number);

		IndirectObjectReference(files::File * file, DirectObject obj);

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

		void SetObject(files::File * file,
			IntegerObjectPtr obj_number,
			IntegerObjectPtr gen_number);

		void SetObject(files::File * file, DirectObject obj);

		inline IntegerObjectPtr GetObjectNumber() const { return _obj_number; }
		inline IntegerObjectPtr GetGenerationNumber() const { return _gen_number; }

		bool Equals(const IndirectObjectReference& other) const;

		inline bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
		inline bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
		bool operator<(const IndirectObjectReference& other) const;

	public:
		IntegerObjectPtr _obj_number = 0;
		IntegerObjectPtr _gen_number = 0;

	private:
		mutable DirectObject _object;
		mutable bool _initialized = false;

		explicit IndirectObjectReference() = default;
		friend IndirectObjectReferencePtr;

		template <typename T>
		friend T* Allocate();
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
