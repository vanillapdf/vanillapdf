#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	class NullObject : public Containable, public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::Null; }

		static Deferred<NullObject> GetInstance(void)
		{
			static NullObject instance;
			return instance;
		}

		inline bool Equals(const NullObject& other) const { return true; }

		inline bool operator==(const NullObject& other) const { return Equals(other); }
		inline bool operator!=(const NullObject& other) const { return !Equals(other); }
		inline bool operator<(const NullObject& other) const { return false; }

	public:
		NullObject() = default;
		NullObject(const NullObject&) = default;
		NullObject& operator=(const NullObject&) = default;
	};
}

#endif /* _NULL_OBJECT_H */
