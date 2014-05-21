#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "object_reference_wrapper.h"
#include "token.h"

#include <vector>

namespace gotchangpdf
{
	class ArrayObject : public Object
	{
	public:
		ArrayObject() : Object(Object::Type::ArrayObject) {};

		inline int Size(void) const { return _list.size(); }
		inline ObjectReferenceWrapper<Object> operator[](unsigned int i) const { return _list[i]; }
		inline ObjectReferenceWrapper<Object> At(unsigned int at) const { return _list.at(at); }

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject& o);

		//virtual ~ArrayObject() {};

	public:
		std::vector<ObjectReferenceWrapper<Object>> _list;

		ArrayObject(std::vector<ObjectReferenceWrapper<Object>>&& list) : _list(std::move(list)) {}
		//friend class ArrayObjectAST;
	};
}

#endif /* _ARRAY_OBJECT_H */
