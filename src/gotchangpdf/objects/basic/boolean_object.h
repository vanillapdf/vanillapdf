#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class BooleanObject : public Containable, public Object
	{
	public:
		BooleanObject() = default;
		explicit BooleanObject(bool value) : _value(value) {}

		virtual inline Object::Type GetType(void) const _NOEXCEPT override{ return Object::Type::Boolean; }

		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

	private:
		bool _value = false;
		ContainerPtr _container;
	};
}

#endif /* _BOOLEAN_OBJECT_H */
