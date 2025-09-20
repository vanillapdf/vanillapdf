#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <utility>

namespace fetchcontent_example {

template <typename FunctionType>
class ScopeGuard {
public:
    explicit ScopeGuard(FunctionType deleter) : _deleter(deleter) {}

    ~ScopeGuard() {
        if (!_released) {
            _deleter();
        }
    }

    void Release() {
        _released = true;
    }

    // Non-copyable but movable
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard&& other) : _deleter(other._deleter) {
        other.Release();
    }

    ScopeGuard& operator=(ScopeGuard&& other) {
        _deleter = other._deleter;
        other.Release();
        return *this;
    }

private:
    bool _released = false;
    FunctionType _deleter;
};

template <typename FunctionType>
ScopeGuard<FunctionType> make_scope_guard(FunctionType&& f) {
    return ScopeGuard<FunctionType>(std::forward<FunctionType>(f));
}

} // namespace fetchcontent_example

#define TOKENPASTE(x, y, z) x ## y ## z
#define TOKENPASTE2(x, y, z) TOKENPASTE(x, y, z)
#define SCOPE_GUARD(deleter_function) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = fetchcontent_example::make_scope_guard(deleter_function)

#endif // SCOPE_GUARD_H