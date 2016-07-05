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

			virtual Object::Type GetType(void) const noexcept override { return Object::Type::Boolean; }
			virtual std::string ToPdf(void) const override { return _value ? "true" : "false"; }

			bool GetValue(void) const noexcept { return _value; }
			void SetValue(bool value) noexcept { _value = value; OnChanged(); }
			operator bool() const noexcept { return _value; }

			virtual Object* Clone(void) const override { return new BooleanObject(*this); }

		private:
			bool _value = false;
		};
	}
}

#endif /* _BOOLEAN_OBJECT_H */
