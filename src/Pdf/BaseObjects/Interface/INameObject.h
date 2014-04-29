#ifndef _I_NAME_OBJECT_H
#define _I_NAME_OBJECT_H

#include "Interface/IObject.h"
#include "Interface/ICharacterSet.h"

#include "boost/intrusive_ptr.hpp"

namespace Pdf
{
	class INameObject
	{
	public:
		virtual const ICharacterSet& Value() const = 0;
	};
}

#endif /* _I_NAME_OBJECT_H */
