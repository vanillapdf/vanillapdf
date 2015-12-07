#ifndef _REAL_OBJECT_H
#define _REAL_OBJECT_H

#include "syntax_fwd.h"
#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class RealObject : public ContainableObject
		{
		public:
			typedef types::real value_type;

		public:
			RealObject() = default;
			explicit RealObject(types::real value) : _value(value) {}

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Real; }

			inline value_type Value(void) const _NOEXCEPT { return _value; }
			operator value_type() const _NOEXCEPT { return _value; }

		private:
			types::real _value = 0;
		};
	}
}

#endif /* _REAL_OBJECT_H */
