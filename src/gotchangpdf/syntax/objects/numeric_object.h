#ifndef _NUMERIC_OBJECT_H
#define _NUMERIC_OBJECT_H

#include "syntax_fwd.h"
#include "constants.h"
#include "containable.h"

#include <string>

namespace gotchangpdf
{
	namespace syntax
	{
		class NumericObjectBackend : public IUnknown, public IModifyObservable
		{
		public:
			NumericObjectBackend() = default;
			NumericObjectBackend(types::native_int value) : m_type(Type::Integer), m_int(value) {}
			NumericObjectBackend(types::native_uint value) : m_type(Type::Integer), m_int(value) {}
			NumericObjectBackend(int64_t value) : m_type(Type::Integer), m_int(value) {}
			NumericObjectBackend(types::real value) : m_type(Type::Real), m_real(value) {}
			NumericObjectBackend(types::real value, uint32_t precision) : m_type(Type::Real), m_real(value), m_precision(precision) {}
			NumericObjectBackend& operator= (int64_t value) { m_type = Type::Integer; m_int = value; return *this; }
			NumericObjectBackend& operator= (types::real value) { m_type = Type::Real; m_real = value; return *this; }

			int64_t GetIntegerValue(void) const noexcept
			{
				if (m_type == Type::Integer)
					return m_int;

				return ValueConvertUtils::SafeConvert<int64_t>(m_real);
			}

			types::real GetRealValue(void) const noexcept
			{
				if (m_type == Type::Real)
					return m_real;

				return ValueConvertUtils::SafeConvert<types::real>(m_int);
			}

			void SetIntegerValue(int64_t value) noexcept { m_type = Type::Integer; m_int = value; OnChanged(); }
			void SetRealValue(types::real value) noexcept { m_type = Type::Real; m_real = value; OnChanged(); }

			std::string ToString(void) const;

		private:
			std::string IntegerString(void) const;
			std::string RealString(void) const;

		private:
			enum class Type
			{
				Integer,
				Real
			};

		private:
			Type m_type = Type::Integer;
			int64_t m_int = 0;
			types::real m_real = 0;
			uint32_t m_precision = 0;
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
