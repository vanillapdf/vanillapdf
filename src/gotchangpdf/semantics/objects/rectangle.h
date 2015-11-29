#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "high_level_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class Rectangle : public HighLevelObject<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>
		{
		public:
			explicit Rectangle(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> list) : HighLevelObject(list) {}

			virtual inline HighLevelObject::Type GetType(void) const { return HighLevelObject::Type::Rectangle; }

		private:
		};
	}
}

#endif /* _RECTANGLE_H */
