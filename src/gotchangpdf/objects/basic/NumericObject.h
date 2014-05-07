#ifndef _NUMERIC_OBJECT_H
#define _NUMERIC_OBJECT_H

#include "Object.h"

namespace Pdf
{
	class NumericObject : public Object
	{
	protected:
		explicit NumericObject(Type type);

	public:
		virtual ~NumericObject() = 0;
	};
}

#endif /* _NUMERIC_OBJECT_H */
