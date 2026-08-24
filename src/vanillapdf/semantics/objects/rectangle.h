#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/real_object.h"

namespace vanillapdf {
namespace semantics {

class Rectangle : public HighLevelObject<syntax::ArrayObjectPtr<syntax::RealObjectPtr>> {
public:
    Rectangle();
    explicit Rectangle(syntax::ArrayObjectPtr<syntax::RealObjectPtr> list);

    types::real GetLowerLeftX() const { return m_llx->GetValue(); }
    types::real GetLowerLeftY() const { return m_lly->GetValue(); }
    types::real GetUpperRightX() const { return m_urx->GetValue(); }
    types::real GetUpperRightY() const { return m_ury->GetValue(); }

    void SetLowerLeftX(types::real value) { m_llx->SetValue(value); }
    void SetLowerLeftY(types::real value) { m_lly->SetValue(value); }
    void SetUpperRightX(types::real value) { m_urx->SetValue(value); }
    void SetUpperRightY(types::real value) { m_ury->SetValue(value); }

private:
    syntax::RealObjectPtr m_llx;
    syntax::RealObjectPtr m_lly;
    syntax::RealObjectPtr m_urx;
    syntax::RealObjectPtr m_ury;
};

} // semantics
} // vanillapdf

#endif /* _RECTANGLE_H */
