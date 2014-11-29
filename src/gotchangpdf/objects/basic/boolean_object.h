#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "object.h"
#include "containerable.h"

namespace gotchangpdf
{
	class BooleanObject : public Object, public ParentContainer<ContainerPtr>
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Boolean; }

	//private:
	public:
		bool _value = false;
	};

	typedef Deferred<BooleanObject> BooleanObjectPtr;
}

#endif /* _BOOLEAN_OBJECT_H */
