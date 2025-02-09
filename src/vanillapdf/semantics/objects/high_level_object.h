#ifndef _HIGH_LEVEL_OBJECT_H
#define _HIGH_LEVEL_OBJECT_H

#include "utils/unknown_interface.h"

namespace vanillapdf {
namespace semantics {

template <typename T>
class HighLevelObject : public virtual IUnknown {
public:
    template <typename = typename std::enable_if<std::is_default_constructible<T>::value>::type>
    HighLevelObject() {}

    explicit HighLevelObject(const T& obj) : _obj(obj) {}
    T GetObject(void) const { return _obj; }

protected:
    T _obj;

    void SetObject(const T& obj) { _obj = obj; }
};

} // semantics
} // vanillapdf

#endif /* _HIGH_LEVEL_OBJECT_H */
