#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "object.h"

namespace gotchangpdf
{
	class Function : public Object
	{
	private:
		template <typename T>
		friend void ::boost::intrusive_ptr_add_ref(T*);

		template <typename T>
		friend void ::boost::intrusive_ptr_release(T*);
	};
}

#endif /* _FUNCTION_H */
