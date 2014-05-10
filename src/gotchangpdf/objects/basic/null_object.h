#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	class NullObject : public Object
	{
		template <typename T>
		friend void ::boost::intrusive_ptr_add_ref(T*);

		template <typename T>
		friend void ::boost::intrusive_ptr_release(T*);
	};
}

#endif /* _NULL_OBJECT_H */
