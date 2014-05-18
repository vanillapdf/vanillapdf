#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "array_object.h"
#include "integer_object.h"

namespace gotchangpdf
{
	class Rectangle
	{
	public:
		//Rectangle() : ArrayObject() {}
		//using ArrayObject::ArrayObject;

		//virtual ~Rectangle() {}
	private:
		//Rectangle(std::vector<ObjectReferenceWrapper<IntegerObject>> list) : _list(std::move(list)) {}
		//friend class ArrayObjectBase;

		ArrayObject<IntegerObject> _list;
	};
}

#endif /* _RECTANGLE_H */
