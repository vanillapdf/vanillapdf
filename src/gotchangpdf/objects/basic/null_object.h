#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class NullObject : public Containable
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Null; }

		static Deferred<NullObject> GetInstance(void)
		{
			static NullObject instance;
			return instance;
		}

	public:
		NullObject() = default;
		NullObject(const NullObject&) = default;
		NullObject& operator=(const NullObject&) = default;
	};
}

#endif /* _NULL_OBJECT_H */
