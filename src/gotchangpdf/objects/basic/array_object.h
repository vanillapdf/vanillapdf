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
		ArrayObject();

		int Size(void) const;

		ObjectReferenceWrapper<Object> operator[](unsigned int i) const;
		ObjectReferenceWrapper<Object> At(unsigned int at) const;

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject& o);

	private:
		std::vector<ObjectReferenceWrapper<Object>> _list;
	};
}

#endif /* _ARRAY_OBJECT_H */
