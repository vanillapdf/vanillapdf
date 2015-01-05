#ifndef _CONTAINABLE_H
#define _CONTAINABLE_H

#include "fwd.h"

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
		Containable() = default;
		virtual ~Containable() = 0;

	public:
		inline void SetContainer(ContainerPtr obj) { _container = obj; }
		inline ContainerPtr GetContainer() const { return _container; }

	private:
		ContainerPtr _container;
	};

	inline Containable::~Containable() {}
}

#include "array_object.h"
#include "dictionary_object.h"
#include "null_object.h"

#endif /* _CONTAINABLE_H */
