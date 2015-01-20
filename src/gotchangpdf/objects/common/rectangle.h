#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "high_level_object.h"

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
	}
}

#endif /* _RECTANGLE_H */
