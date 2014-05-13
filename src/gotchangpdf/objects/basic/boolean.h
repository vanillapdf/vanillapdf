#ifndef _BOOLEAN_H
#define _BOOLEAN_H

#include "object.h"

namespace gotchangpdf
{
	class Boolean : public Object
	{
	private:
		bool _value;
	};
}

#endif /* _BOOLEAN_H */
