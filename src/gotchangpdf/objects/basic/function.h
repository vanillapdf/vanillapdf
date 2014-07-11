#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "object.h"

namespace gotchangpdf
{
	class Function : public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Function; }

	private:
	};
}

#endif /* _FUNCTION_H */
