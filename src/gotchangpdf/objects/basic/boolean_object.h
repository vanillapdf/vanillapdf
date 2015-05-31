#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class BooleanObject : public Containable, public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Boolean; }

		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

	//private:
	public:
		bool _value = false;

	private:
		ContainerPtr _container;
	};
}

#endif /* _BOOLEAN_OBJECT_H */
