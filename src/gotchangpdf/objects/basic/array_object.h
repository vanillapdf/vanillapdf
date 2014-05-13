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

		long Size(void) const;
		Object* At(long at) const;

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject& o);

	private:
		std::vector<ObjectReferenceWrapper<Object>> _list;

		template <typename T>
		friend void ::boost::intrusive_ptr_add_ref(T*);

		template <typename T>
		friend void ::boost::intrusive_ptr_release(T*);
	};
}

#endif /* _ARRAY_OBJECT_H */
