#ifndef _NUMERIC_OBJECT_H
#define _NUMERIC_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

#include <string>

namespace gotchangpdf
{
	namespace syntax
	{
		class NumericObjectBackend : public IUnknown, public IModifyObservable
		{
		public:
			NumericObjectBackend() : m_type(Type::SignedInteger), m_int(0) {}
			NumericObjectBackend(types::native_int value) : m_type(Type::SignedInteger), m_int(value) {}
			NumericObjectBackend(types::native_uint value) : m_type(Type::UnsignedInteger), m_uint(value) {}
			NumericObjectBackend(types::big_int value) : m_type(Type::SignedInteger), m_int(value) {}
			NumericObjectBackend(types::real value) : m_type(Type::Real), m_real(value) {}
			NumericObjectBackend(types::real value, uint32_t precision) : m_type(Type::Real), m_real(value), m_precision(precision) {}
			NumericObjectBackend& operator= (types::big_int value) { m_type = Type::SignedInteger; m_int = value; return *this; }
			NumericObjectBackend& operator= (types::real value) { m_type = Type::Real; m_real = value; return *this; }

			types::big_int GetIntegerValue(void) const;
			types::big_uint GetUnsignedIntegerValue(void) const;
			types::real GetRealValue(void) const;

			void SetIntegerValue(types::big_int value);
			void SetUnsignedIntegerValue(types::big_uint value);
			void SetRealValue(types::real value);

			NumericObjectBackend* Clone(void) const { return new NumericObjectBackend(*this); }

			std::string ToString(void) const;

		private:
			std::string IntegerString(void) const;
			std::string UnsignedIntegerString(void) const;
			std::string RealString(void) const;

		private:
			enum class Type
			{
				SignedInteger,
				UnsignedInteger,
				Real
			};

		private:
			Type m_type = Type::SignedInteger;
			uint32_t m_precision = 0;

			union {
				types::big_int m_int;
				types::big_uint m_uint;
				types::real m_real;
			};
		};

		class NumericObject : public ContainableObject
		{
		public:
			NumericObjectBackendPtr GetNumericBackend(void) const { return m_value; }

		protected:
			NumericObjectBackendPtr m_value;
		};
	}
}

#endif /* _NUMERIC_OBJECT_H */
