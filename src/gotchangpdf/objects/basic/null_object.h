#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	class NullObject : public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::NullObject; }
	};
}

#endif /* _NULL_OBJECT_H */
