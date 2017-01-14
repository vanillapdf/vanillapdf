#ifndef _CONTAINABLE_H
#define _CONTAINABLE_H

#include "syntax/objects/object.h"

namespace gotchangpdf {
namespace syntax {

class ContainableObject : public Object {
public:
	virtual ContainableObject* Clone(void) const = 0;
};

} // syntax
} // gotchangpdf

#endif /* _CONTAINABLE_H */
