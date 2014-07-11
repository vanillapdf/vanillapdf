#ifndef _NUMERIC_OBJECT_H
#define _NUMERIC_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	class NumericObject : public Object
	{
	public:
		virtual ~NumericObject() = 0;
	};
}

#endif /* _NUMERIC_OBJECT_H */
