#ifndef _INDIRECT_OBJECT_REFERENCE_H
#define _INDIRECT_OBJECT_REFERENCE_H

#include "fwd.h"
#include "object.h"
#include "constants.h"
#include "indirect_object.h"
#include "integer_object.h"
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

		IndirectObjectReference(files::File * file, IndirectObjectPtr obj);

		IndirectObjectPtr GetReferencedObject() const;
		inline IndirectObjectPtr operator->() const { return GetReferencedObject(); }

		template <typename T>
		inline const T GetReferencedObjectAs() const { return GetReferencedObject()->GetObjectAs<T>(); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::IndirectReference; }

		void SetObject(files::File * file,
			IntegerObjectPtr obj_number,
			IntegerObjectPtr gen_number);

		void SetObject(files::File * file, IndirectObjectPtr obj);

		inline IntegerObjectPtr GetObjectNumber() const { return _obj_number; }
		inline IntegerObjectPtr GetGenerationNumber() const { return _gen_number; }
		inline files::File* GetFile() const { return _file; }
		inline void SetFile(files::File* file) { _file = file; }

		inline bool Equals(const IndirectObjectReference& other) const { return _obj_number->Equals(*other._obj_number) && _gen_number->Equals(*other._gen_number); }

		inline bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
		inline bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
		bool operator<(const IndirectObjectReference& other) const;

	public:
		IntegerObjectPtr _obj_number = 0;
		IntegerObjectPtr _gen_number = 0;

	private:
		files::File * _file = nullptr;
		mutable IndirectObjectPtr _object;
		mutable bool _initialized = false;

		explicit IndirectObjectReference() = default;
		friend Deferred<IndirectObjectReference>;

		template <typename T>
		friend T* Allocate();
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
