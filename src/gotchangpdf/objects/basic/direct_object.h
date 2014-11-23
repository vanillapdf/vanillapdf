#ifndef _DIRECT_OBJECT_H
#define _DIRECT_OBJECT_H

#include "fwd.h"
#include "smart_ptr.h"

#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	typedef boost::variant <
		SmartPtr<LiteralString>,
		SmartPtr<HexadecimalString>
	> StringObjectPtr;

	typedef boost::variant <
		SmartPtr<MixedArrayObject>,
		SmartPtr<NameObject>,
		SmartPtr<DictionaryObject>,
		SmartPtr<FunctionObject>,
		SmartPtr<NullObject>,
		SmartPtr<BooleanObject>,
		SmartPtr<IndirectObjectReference>,
		SmartPtr<IntegerObject>,
		SmartPtr<RealObject>,
		SmartPtr<StreamObject>,
		StringObjectPtr
	> DirectObject;
}

#include "array_object.h"
#include "name_object.h"
#include "dictionary_object.h"
#include "function_object.h"
#include "null_object.h"
#include "boolean_object.h"
#include "indirect_object_reference.h"
#include "integer_object.h"
#include "real_object.h"
#include "stream_object.h"
#include "string_object.h"

#endif /* _DIRECT_OBJECT_H */
