#ifndef _CONTAINABLE_H
#define _CONTAINABLE_H

#include "syntax/objects/object.h"

namespace vanillapdf {
namespace syntax {

class ContainableObject : public Object {
public:
    virtual ContainableObject* Clone(void) const override = 0;
};

} // syntax
} // vanillapdf

#endif /* _CONTAINABLE_H */
