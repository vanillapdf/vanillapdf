#ifndef _BOOLEAN_H
#define _BOOLEAN_H

#include "object.h"

namespace gotchangpdf
{
	class Boolean : public Object
	{
	private:
		bool _value;

		template <typename T>
		friend void ::boost::intrusive_ptr_add_ref(T*);

		template <typename T>
		friend void ::boost::intrusive_ptr_release(T*);
	};
}

#endif /* _BOOLEAN_H */
