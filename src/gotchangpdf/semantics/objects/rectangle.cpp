#include "precompiled.h"
#include "rectangle.h"

namespace gotchangpdf
{
	namespace semantics
	{
		Rectangle::Rectangle(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> list) : HighLevelObject(list)
		{
			assert(list->Size() == 4 && "Only fully specified rectangles are yet supported");

			m_llx = list->At(0);
			m_lly = list->At(1);
			m_urx = list->At(2);
			m_ury = list->At(3);
		}
	}
}
