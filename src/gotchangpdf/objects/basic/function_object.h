#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "object.h"
#include "containerable.h"

namespace gotchangpdf
{
	class FunctionObject : public Object, public ParentContainer<ContainerPtr>
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Function; }

	private:
	};
}

#endif /* _FUNCTION_OBJECT_H */
