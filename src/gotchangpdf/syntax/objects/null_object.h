#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "containable.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class NullObject : public ContainableObject
		{
		public:
			virtual Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Null; }
			virtual std::string ToPdf(void) const override { return "null"; }

			static NullObjectPtr GetInstance(void)
			{
				static NullObjectPtr instance = NullObjectPtr(pdf_new NullObject());
				return instance;
			}

			bool Equals(const NullObject&) const { return true; }
			virtual Object* Clone(void) const override { return new NullObject(*this); }

			bool operator==(const NullObject& other) const { return Equals(other); }
			bool operator!=(const NullObject& other) const { return !Equals(other); }
			bool operator<(const NullObject&) const { return false; }
		};
	}
}

#endif /* _NULL_OBJECT_H */
