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
			explicit RealObject(types::real value, uint32_t precision) : _value(value), _precision(precision) {}
			explicit RealObject(const IntegerObject& value) { _value = SafeConvert<types::real>(value.Value()); }

			virtual Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Real; }
			virtual std::string ToPdf(void) const override;

			value_type Value(void) const _NOEXCEPT { return _value; }
			operator value_type() const _NOEXCEPT { return _value; }

		private:
			types::real _value = 0;
			uint32_t _precision = 0;
		};
	}
}

#endif /* _REAL_OBJECT_H */
