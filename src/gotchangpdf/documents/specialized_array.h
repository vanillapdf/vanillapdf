#ifndef _SPECIALIZED_ARRAY_H
#define _SPECIALIZED_ARRAY_H

#include "array_object.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		template <typename T>
		class SpecializedArrayObject : public HighLevelObject<ArrayObject>
		{
		public:
			SpecializedArrayObject<T>() {}
			SpecializedArrayObject<T>(ObjectReferenceWrapper<ArrayObject> obj) : HighLevelObject(obj) {/* TODO validation of array */}

			virtual inline Type GetType(void) const { return HighLevelObject::Type::SpecializedArrayObject; }

			inline int Size(void) const { return _obj->Size(); }
			inline ObjectReferenceWrapper<T> operator[](unsigned int i) const { return dynamic_wrapper_cast<T>((*_obj)[i]); }
			inline ObjectReferenceWrapper<T> At(unsigned int at) const { return dynamic_wrapper_cast<T>(_obj->At(at)); }
		};
	}
}

#endif /* _SPECIALIZED_ARRAY_H */
