#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class BooleanObject : public Object
		{
		public:
			BooleanObject() = default;
			explicit BooleanObject(bool value) : _value(value) {}

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Boolean; }

		private:
			bool _value = false;
		};
	}
}

#endif /* _BOOLEAN_OBJECT_H */
