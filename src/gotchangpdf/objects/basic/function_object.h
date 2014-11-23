#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	class FunctionObject : public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Function; }

	private:
	};

	typedef SmartPtr<FunctionObject> FunctionObjectPtr;
}

#endif /* _FUNCTION_OBJECT_H */
