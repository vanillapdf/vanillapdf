#ifndef _CONTAINERABLE_H
#define _CONTAINERABLE_H

#include "fwd.h"
#include "deferred.h"

//#include "array_object.h"
//#include "dictionary_object.h"
//#include "null_object.h"

#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	typedef boost::variant <
		Deferred<NullObject>,
		Deferred<MixedArrayObject>,
		Deferred<DictionaryObject>
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
	template <typename T>
	class ParentContainer
	{
	public:
		inline void SetContainer(T obj) { _container = obj; }
		inline T GetContainer() const { return _container; }

	protected:
		T _container;
	};
}

#include "array_object.h"
#include "dictionary_object.h"
#include "null_object.h"

#endif /* _CONTAINERABLE_H */
