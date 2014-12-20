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
	class IndirectObjectReference : public Containable
	{
	public:
		explicit IndirectObjectReference(files::File * file);
		IndirectObjectReference(files::File * file,
			IntegerObjectPtr obj_number,
			IntegerObjectPtr gen_number);

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

		inline bool Equals(const IndirectObjectReference& other) const { return _obj_number->Equals(*other._obj_number) && _gen_number->Equals(*other._gen_number); }

		inline bool IsEmpty() const { return !_reference.HasContents(); }

		inline bool operator==(const IndirectObjectReference& other) const { return Equals(other); }
		inline bool operator!=(const IndirectObjectReference& other) const { return !Equals(other); }
		bool operator<(const IndirectObjectReference& other) const;

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

namespace std
{
	template <> struct hash<gotchangpdf::IndirectObjectReference>
	{
		size_t operator()(const gotchangpdf::IndirectObjectReference& ref) const;
	};
}

#endif /* _INDIRECT_OBJECT_REFERENCE_H */
