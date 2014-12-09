#ifndef _CONTAINERABLE_H
#define _CONTAINERABLE_H

#include "fwd.h"
#include "object.h"

#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	typedef boost::variant <
		NullObjectPtr,
		MixedArrayObjectPtr,
		DictionaryObjectPtr
	> ContainerPtr;
	/*
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
		Deferred<LiteralString>,
		Deferred<HexadecimalString>
	> ContainerableObject;
	*/

	class Containable : public Object
	{
	public:
		inline void SetContainer(ContainerPtr obj) { _container = obj; }
		inline ContainerPtr GetContainer() const { return _container; }

	private:
		ContainerPtr _container;
	};
}

#include "array_object.h"
#include "dictionary_object.h"
#include "null_object.h"

#endif /* _CONTAINERABLE_H */
