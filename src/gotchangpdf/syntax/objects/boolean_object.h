#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class BooleanObject : public ContainableObject
		{
		public:
			BooleanObject() = default;
			explicit BooleanObject(bool value) : _value(value) {}

			virtual Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Boolean; }
			virtual std::string ToPdf(void) const override { return _value ? "true" : "false"; }

			bool Value(void) const _NOEXCEPT { return _value; }
			operator bool() const _NOEXCEPT { return _value; }

		private:
			bool _value = false;
		};
	}
}

#endif /* _BOOLEAN_OBJECT_H */
