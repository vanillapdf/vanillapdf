#ifndef _DIRECT_OBJECT_H
#define _DIRECT_OBJECT_H

#include "fwd.h"
#include "deferred.h"

#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
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

#endif /* _DIRECT_OBJECT_H */
