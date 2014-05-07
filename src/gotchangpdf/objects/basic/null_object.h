#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	class NullObject : public Object
	{
	private:
		friend void ::boost::intrusive_ptr_add_ref(NullObject*);
		friend void ::boost::intrusive_ptr_release(NullObject*);
	};
}

#endif /* _NULL_OBJECT_H */
