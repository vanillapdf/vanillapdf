#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class BooleanObject : public Containable
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Boolean; }

	//private:
	public:
		bool _value = false;
	};
}

#endif /* _BOOLEAN_OBJECT_H */
