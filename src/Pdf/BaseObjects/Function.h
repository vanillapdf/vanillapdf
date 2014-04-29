#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "BaseObjects/Object.h"

namespace Pdf
{
	class Function : public Object
	{
	private:
		friend void ::boost::intrusive_ptr_add_ref(Function*);
		friend void ::boost::intrusive_ptr_release(Function*);
	};
}

#endif /* _FUNCTION_H */
