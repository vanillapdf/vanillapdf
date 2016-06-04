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
			explicit RealObject(const IntegerObject& value) { _value = ValueConvertUtils::SafeConvert<types::real>(value.GetValue()); }

			virtual Object::Type GetType(void) const noexcept override { return Object::Type::Real; }
			virtual std::string ToPdf(void) const override;

			operator value_type() const noexcept { return _value; }
			value_type GetValue(void) const noexcept { return _value; }
			void SetValue(value_type value) noexcept { _value = value; SetDirty(true); }

		private:
			types::real _value = 0;
			uint32_t _precision = 0;
		};
	}
}

#endif /* _REAL_OBJECT_H */
