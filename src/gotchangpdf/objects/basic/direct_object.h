#ifndef _DIRECT_OBJECT_H
#define _DIRECT_OBJECT_H

#include "fwd.h"
#include "deferred.h"

#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	//typedef boost::variant <
	//	Deferred<LiteralString>,
	//	Deferred<HexadecimalString>
	//> StringObjectPtr;

	typedef boost::variant <
		// first is null object
		Deferred<NullObject>,
		Deferred<MixedArrayObject>,
		Deferred<NameObject>,
		Deferred<DictionaryObject>,
		Deferred<FunctionObject>,
		Deferred<BooleanObject>,
		Deferred<IndirectObjectReference>,
		Deferred<IntegerObject>,
		Deferred<RealObject>,
		Deferred<StreamObject>,
		Deferred<LiteralString>,
		Deferred<HexadecimalString>
	> DirectObject;
}
//
//#include "array_object.h"
//#include "name_object.h"
//#include "dictionary_object.h"
//#include "function_object.h"
//#include "null_object.h"
//#include "boolean_object.h"
//#include "indirect_object_reference.h"
//#include "integer_object.h"
//#include "real_object.h"
//#include "stream_object.h"
//#include "string_object.h"
//
//namespace gotchangpdf
//{
//	bool operator!=(const DirectObject& lhs, const DirectObject& rhs)
//	{
//		return !(lhs.operator==(rhs));
//	}
//}


#endif /* _DIRECT_OBJECT_H */
