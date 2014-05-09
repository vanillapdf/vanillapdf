#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "i_array_object.h"
#include "object_reference_wrapper.h"

#include <vector>

namespace gotchangpdf
{
	class ArrayObject : public Object, public IArrayObject
	{
	public:
		ArrayObject();

		virtual long Size(void) const override;
		virtual IObject* At(long at) const override;

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject& o);

	private:
		std::vector<ObjectReferenceWrapper<Object>> _list;

		friend void ::boost::intrusive_ptr_add_ref(ArrayObject*);
		friend void ::boost::intrusive_ptr_release(ArrayObject*);
	};
}

#endif /* _ARRAY_OBJECT_H */
