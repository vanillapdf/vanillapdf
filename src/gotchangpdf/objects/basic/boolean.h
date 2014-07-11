#ifndef _BOOLEAN_H
#define _BOOLEAN_H

#include "object.h"

namespace gotchangpdf
{
	class Boolean : public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Boolean; }

	private:
		bool _value = false;
	};
}

#endif /* _BOOLEAN_H */
