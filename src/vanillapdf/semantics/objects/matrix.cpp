#include "precompiled.h"
#include "semantics/objects/matrix.h"

namespace vanillapdf {
namespace semantics {

Matrix::Matrix() {
    m_a->SetValue(1);
    m_d->SetValue(1);

    _obj->Append(m_a);
    _obj->Append(m_b);
    _obj->Append(m_c);
    _obj->Append(m_d);
    _obj->Append(m_e);
    _obj->Append(m_f);
}

Matrix::Matrix(syntax::ArrayObjectPtr<syntax::RealObjectPtr> list) : HighLevelObject(list) {
    if (list->GetSize() != 6) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Invalid matrix size: {}", list->GetSize());
    }

    m_a = list->GetValue(0);
    m_b = list->GetValue(1);
    m_c = list->GetValue(2);
    m_d = list->GetValue(3);
    m_e = list->GetValue(4);
    m_f = list->GetValue(5);
}

} // semantics
} // vanillapdf
