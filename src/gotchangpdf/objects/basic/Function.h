#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "Object.h"

namespace gotchangpdf
{
	class Function : public Object
	{
	private:
		friend void ::boost::intrusive_ptr_add_ref(Function*);
		friend void ::boost::intrusive_ptr_release(Function*);
	};
}

#endif /* _FUNCTION_H */
