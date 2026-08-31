#ifndef _MATRIX_H
#define _MATRIX_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/real_object.h"

namespace vanillapdf {
namespace semantics {

// Transformation matrix [a b c d e f] mapping one coordinate space into
// another, see 8.3.3 - Common Transformations. The same six-element form is
// used by the /Matrix entry of a form XObject, the cm operator and Tm.
class Matrix : public HighLevelObject<syntax::ArrayObjectPtr<syntax::RealObjectPtr>> {
public:
    // Creates the identity matrix [1 0 0 1 0 0]
    Matrix();
    explicit Matrix(syntax::ArrayObjectPtr<syntax::RealObjectPtr> list);

    types::real GetA() const { return m_a->GetValue(); }
    types::real GetB() const { return m_b->GetValue(); }
    types::real GetC() const { return m_c->GetValue(); }
    types::real GetD() const { return m_d->GetValue(); }
    types::real GetE() const { return m_e->GetValue(); }
    types::real GetF() const { return m_f->GetValue(); }

    void SetA(types::real value) { m_a->SetValue(value); }
    void SetB(types::real value) { m_b->SetValue(value); }
    void SetC(types::real value) { m_c->SetValue(value); }
    void SetD(types::real value) { m_d->SetValue(value); }
    void SetE(types::real value) { m_e->SetValue(value); }
    void SetF(types::real value) { m_f->SetValue(value); }

private:
    syntax::RealObjectPtr m_a;
    syntax::RealObjectPtr m_b;
    syntax::RealObjectPtr m_c;
    syntax::RealObjectPtr m_d;
    syntax::RealObjectPtr m_e;
    syntax::RealObjectPtr m_f;
};

} // semantics
} // vanillapdf

#endif /* _MATRIX_H */
