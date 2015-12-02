#ifndef _CONTAINABLE_H
#define _CONTAINABLE_H

#include "syntax_fwd.h"

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace gotchangpdf
{
	namespace syntax
	{
		typedef boost::variant <
			// first is null object
			NullObjectPtr,
			boost::recursive_wrapper<MixedArrayObjectPtr>,
			NameObjectPtr,
			boost::recursive_wrapper<DictionaryObjectPtr>,
			FunctionObjectPtr,
			BooleanObjectPtr,
			boost::recursive_wrapper<IndirectObjectReferencePtr>,
			IntegerObjectPtr,
			RealObjectPtr,
			LiteralStringObjectPtr,
			HexadecimalStringObjectPtr
		> ContainableObject;
	}
}

#endif /* _CONTAINABLE_H */
