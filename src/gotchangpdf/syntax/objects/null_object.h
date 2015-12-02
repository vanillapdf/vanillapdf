#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "object.h"
#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class NullObject : public Object
		{
		public:
			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Null; }

			static NullObjectPtr GetInstance(void)
			{
				static NullObjectPtr instance = NullObjectPtr(pdf_new NullObject());
				return instance;
			}

			inline bool Equals(const NullObject&) const { return true; }

			inline bool operator==(const NullObject& other) const { return Equals(other); }
			inline bool operator!=(const NullObject& other) const { return !Equals(other); }
			inline bool operator<(const NullObject&) const { return false; }
		};
	}
}

#endif /* _NULL_OBJECT_H */
