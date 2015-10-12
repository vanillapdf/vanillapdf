#ifndef _CONTAINABLE_H
#define _CONTAINABLE_H

#include "fwd.h"

#include <boost/variant/variant.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>

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
		LiteralStringObjectPtr,
		HexadecimalStringObjectPtr
	> ContainableObject;

	typedef boost::intrusive_ptr<Object> ContainerPtr;

	class Containable
	{
	public:
		virtual void SetContainer(ContainerPtr obj) = 0;
		virtual ContainerPtr GetContainer() const = 0;
	};
}

#endif /* _CONTAINABLE_H */
