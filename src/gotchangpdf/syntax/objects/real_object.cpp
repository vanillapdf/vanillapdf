#include "precompiled.h"
#include "real_object.h"

#include <sstream>
#include <iomanip>

namespace gotchangpdf
{
	namespace syntax
	{
		std::string RealObject::ToPdf(void) const
		{
			std::stringstream ss;
			ss << std::fixed << std::setprecision(_precision) << _value;
			return ss.str();
		}
	}
}