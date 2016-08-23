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
			explicit Rectangle(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> list);

			syntax::IntegerObjectPtr LowerLeftX() const { return m_llx; }
			syntax::IntegerObjectPtr LowerLeftY() const { return m_lly; }
			syntax::IntegerObjectPtr UpperRightX() const { return m_urx; }
			syntax::IntegerObjectPtr UpperRightY() const { return m_ury; }

		private:
			syntax::IntegerObjectPtr m_llx;
			syntax::IntegerObjectPtr m_lly;
			syntax::IntegerObjectPtr m_urx;
			syntax::IntegerObjectPtr m_ury;
		};
	}
}

#endif /* _RECTANGLE_H */
