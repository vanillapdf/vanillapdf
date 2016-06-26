#include "precompiled.h"
#include "numeric_object.h"

#include <sstream>
#include <iomanip>

namespace gotchangpdf
{
	namespace syntax
	{
		std::string NumericObjectBackend::IntegerString(void) const
		{
			return std::to_string(m_int);
		}

		std::string NumericObjectBackend::RealString(void) const
		{
			std::stringstream ss;
			ss << std::fixed << std::setprecision(m_precision) << m_real;
			return ss.str();
		}

		std::string NumericObjectBackend::ToString(void) const
		{
			switch (m_type)
			{
			case Type::Real:
				return RealString();
			case Type::Integer:
			default:
				return IntegerString();
			}
		}
	}
}