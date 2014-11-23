#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"

namespace gotchangpdf
{
	class NullObject : public Object
	{
	public:
		virtual inline Object::Type GetType(void) const override { return Object::Type::NullObject; }

		static NullObject& GetInstance(void)
		{
			static NullObject instance;
			return instance;
		}

	private:
		NullObject() = default;
		NullObject(const NullObject&) = delete;
		NullObject& operator=(const NullObject&) = delete;
	};

	typedef SmartPtr<NullObject> NullObjectPtr;
}

#endif /* _NULL_OBJECT_H */
