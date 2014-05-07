#ifndef _BOOLEAN_H
#define _BOOLEAN_H

#include "Object.h"

namespace gotchangpdf
{
	class Boolean : public Object
	{
	private:
		bool _value;

		friend void ::boost::intrusive_ptr_add_ref(Boolean*);
		friend void ::boost::intrusive_ptr_release(Boolean*);
	};
}

#endif /* _BOOLEAN_H */
