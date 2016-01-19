#ifndef _CONTAINABLE_H
#define _CONTAINABLE_H

#include "syntax_fwd.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class ContainableObject : public Object
		{
		public:
			virtual ~ContainableObject() = 0;
		};

		ContainableObject::~ContainableObject() {}
	}
}

#endif /* _CONTAINABLE_H */
