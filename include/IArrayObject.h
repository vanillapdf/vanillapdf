#ifndef _I_ARRAY_OBJECT_H
#define _I_ARRAY_OBJECT_H

#include "IObject.h"

#include "boost/intrusive_ptr.hpp"

namespace Pdf
{
	class IArrayObject
	{
	public:
		virtual long Size(void) const = 0;
		virtual IObject* At(long at) const = 0;
	};
}

#endif /* _I_ARRAY_OBJECT_H */
