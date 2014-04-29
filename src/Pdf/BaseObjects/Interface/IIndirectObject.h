#ifndef _I_INDIRECT_OBJECT_H
#define _I_INDIRECT_OBJECT_H

#include "Constants.h"
#include "Interface/IObject.h"

namespace Pdf
{
	class IIndirectObject
	{
	public:
		virtual IObject* GetIObject() const = 0;
		virtual streamOffsetValueType GetOffset() const = 0;
	};
}

#endif /* _I_INDIRECT_OBJECT_H */
