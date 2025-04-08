#ifndef _EMPTY_OBJECT_ATTRIBUTE_H
#define _EMPTY_OBJECT_ATTRIBUTE_H

#include "syntax/utils/base_object_attribute.h"

namespace vanillapdf {
namespace syntax {

class EmptyAttribute : public BaseAttribute {
public:
    virtual Type GetType(void) const noexcept override {
        return Type::Empty;
    }

    virtual EmptyAttribute* Clone(void) const override {
        return pdf_new EmptyAttribute();
    }
};

} // syntax
} // vanillapdf

#endif /* _EMPTY_OBJECT_ATTRIBUTE_H */
