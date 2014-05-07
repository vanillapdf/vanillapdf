#ifndef _I_NAME_OBJECT_H
#define _I_NAME_OBJECT_H

#include "i_object.h"
#include "i_buffer.h"

namespace gotchangpdf
{
	class INameObject
	{
	public:
		virtual const IBuffer& Value() const = 0;
	};
}

#endif /* _I_NAME_OBJECT_H */
