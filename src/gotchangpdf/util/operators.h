#ifndef _OPERATORS_H
#define _OPERATORS_H

#include "direct_object.h"
#include "objects.h"

namespace gotchangpdf
{
	inline bool operator==(const DirectObject& lhs, const DirectObject& rhs)
	{
		return lhs.operator==(rhs);
	}

	inline bool operator!=(const DirectObject& lhs, const DirectObject& rhs)
	{
		return !(lhs.operator==(rhs));
	}
}

#endif /* _OPERATORS_H */
