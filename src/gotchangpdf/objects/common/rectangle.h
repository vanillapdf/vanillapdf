#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "high_level_object.h"
#include "integer_object.h"
#include "array_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class Rectangle : public HighLevelObject<ArrayObject<IntegerObject>>
		{
		public:
			explicit Rectangle(ObjectReferenceWrapper<ArrayObject<IntegerObject>> list) : HighLevelObject(list) {}

			virtual inline HighLevelObject::Type GetType(void) const { return HighLevelObject::Type::Rectangle; }

		private:
		};
	}
}

#endif /* _RECTANGLE_H */
