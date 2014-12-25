#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "high_level_object.h"
#include "integer_object.h"
#include "array_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class Rectangle : public HighLevelObject<ArrayObjectPtr<IntegerObjectPtr>>
		{
		public:
			explicit Rectangle(ArrayObjectPtr<IntegerObjectPtr> list) : HighLevelObject(list) {}

			virtual inline HighLevelObject::Type GetType(void) const { return HighLevelObject::Type::Rectangle; }

		private:
		};

		typedef SmartPtr<Rectangle> RectanglePtr;
	}
}

#endif /* _RECTANGLE_H */
