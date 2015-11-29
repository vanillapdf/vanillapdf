#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class FunctionObject : public Containable, public Object
		{
		public:
			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Function; }
			virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
			virtual inline ContainerPtr GetContainer() const override { return _container; }

		private:
			ContainerPtr _container;
		};
	}
}

#endif /* _FUNCTION_OBJECT_H */
