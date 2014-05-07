#ifndef _I_NAME_OBJECT_H
#define _I_NAME_OBJECT_H

#include "IObject.h"
#include "IBuffer.h"

namespace Pdf
{
	class INameObject
	{
	public:
		virtual const IBuffer& Value() const = 0;
	};
}

#endif /* _I_NAME_OBJECT_H */
