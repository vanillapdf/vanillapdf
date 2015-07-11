#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class RealObject : public Containable, public Object
	{
	public:
		typedef types::real value_type;

	public:
		RealObject();
		explicit RealObject(double value);

		virtual inline Object::Type GetType(void) const _NOEXCEPT override{ return Object::Type::Real; }
		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

	//private:
	public:
		types::real _value = 0;
		ContainerPtr _container;
	};
}

#endif /* _REAL_OBJECT_H */
