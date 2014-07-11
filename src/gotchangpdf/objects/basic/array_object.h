#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "object_reference_wrapper.h"

#include <vector>

namespace gotchangpdf
{
	class ArrayObject : public Object
	{
	public:
		inline int Size(void) const { return _list.size(); }
		inline ObjectReferenceWrapper<Object> operator[](unsigned int i) const { return _list[i]; }
		inline ObjectReferenceWrapper<Object> At(unsigned int at) const { return _list.at(at); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::ArrayObject; }

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject& o);

	private:
		std::vector<ObjectReferenceWrapper<Object>> _list;
	};
}

#endif /* _ARRAY_OBJECT_H */
