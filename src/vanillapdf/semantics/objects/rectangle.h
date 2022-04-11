#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

namespace vanillapdf {
namespace semantics {

class Rectangle : public HighLevelObject<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>> {
public:
	Rectangle();
	explicit Rectangle(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> list);

	types::big_int GetLowerLeftX() const { return m_llx->GetIntegerValue(); }
	types::big_int GetLowerLeftY() const { return m_lly->GetIntegerValue(); }
	types::big_int GetUpperRightX() const { return m_urx->GetIntegerValue(); }
	types::big_int GetUpperRightY() const { return m_ury->GetIntegerValue(); }

	void SetLowerLeftX(types::big_int value) { m_llx->SetValue(value); }
	void SetLowerLeftY(types::big_int value) { m_lly->SetValue(value); }
	void SetUpperRightX(types::big_int value) { m_urx->SetValue(value); }
	void SetUpperRightY(types::big_int value) { m_ury->SetValue(value); }

private:
	syntax::IntegerObjectPtr m_llx;
	syntax::IntegerObjectPtr m_lly;
	syntax::IntegerObjectPtr m_urx;
	syntax::IntegerObjectPtr m_ury;
};

} // semantics
} // vanillapdf

#endif /* _RECTANGLE_H */
