#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class RealObject : public Object
		{
		public:
			typedef types::real value_type;

		public:
			RealObject() = default;
			explicit RealObject(types::real value) : _value(value) {}

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Real; }

		private:
			types::real _value = 0;
		};
	}
}

#endif /* _REAL_OBJECT_H */
