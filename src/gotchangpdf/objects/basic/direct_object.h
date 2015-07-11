#ifndef _DIRECT_OBJECT_H
#define _DIRECT_OBJECT_H

#include "fwd.h"

#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	typedef boost::variant <
		// first is null object
		NullObjectPtr,
		MixedArrayObjectPtr,
		NameObjectPtr,
		DictionaryObjectPtr,
		FunctionObjectPtr,
		BooleanObjectPtr,
		IndirectObjectReferencePtr,
		IntegerObjectPtr,
		RealObjectPtr,
		StreamObjectPtr,
		LiteralStringObjectPtr,
		HexadecimalStringObjectPtr
	> DirectObject;
}

#endif /* _DIRECT_OBJECT_H */
