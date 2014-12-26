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

	class Containable
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
