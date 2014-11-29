#ifndef _HASHER_H
#define _HASHER_H

#include "fwd.h"

namespace std
{
	template <> struct hash<gotchangpdf::NameObjectPtr>
	{
		size_t operator()(const gotchangpdf::NameObjectPtr& name) const;
	};
}

#endif /* _HASHER_H */
