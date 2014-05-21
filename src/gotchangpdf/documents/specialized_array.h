#ifndef _SPECIALIZED_ARRAY_H
#define _SPECIALIZED_ARRAY_H

#include "array_object.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		template <typename T>
		class SpecializedArrayObject : public HighLevelObject
		{
		public:
			SpecializedArrayObject<T>() {}
			SpecializedArrayObject<T>(const ArrayObject& obj) : _array(new ArrayObject(obj)) {}

			virtual inline Type GetType(void) const { return HighLevelObject::Type::SpecializedArrayObject; }

			inline int Size(void) const { return _array->Size(); }
			inline ObjectReferenceWrapper<T> operator[](unsigned int i) const { return dynamic_wrapper_cast<T>((*_array)[i]); }
			inline ObjectReferenceWrapper<T> At(unsigned int at) const { return dynamic_wrapper_cast<T>(_array->At(at)); }

		public:
			SpecializedArrayObject(std::vector<ObjectReferenceWrapper<T>>&& list) : _array(std::move(list)) {}
			ObjectReferenceWrapper<ArrayObject> _array;
			//friend class ArrayObjectAST;
		};
	}
}

#endif /* _SPECIALIZED_ARRAY_H */
