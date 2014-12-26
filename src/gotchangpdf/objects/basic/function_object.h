#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class FunctionObject : public Containable, public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Function; }

	private:
	};
}

#endif /* _FUNCTION_OBJECT_H */
