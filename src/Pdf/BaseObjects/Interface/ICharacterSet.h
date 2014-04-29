#ifndef _I_CHARACTER_SET_H
#define _I_CHARACTER_SET_H

#include "Interface/IObject.h"
#include "Character.h"

namespace Pdf
{
	class ICharacterSet
	{
	public:
		virtual Character At(long at) const = 0;
	};
}

#endif /* _I_CHARACTER_SET_H */
