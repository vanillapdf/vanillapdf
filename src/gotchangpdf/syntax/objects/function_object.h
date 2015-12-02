#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class FunctionObject : public Object
		{
		public:
			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Function; }
		};
	}
}

#endif /* _FUNCTION_OBJECT_H */
