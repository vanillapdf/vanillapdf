#ifndef VANILLAPDF_EXAMPLES_SCOPE_GUARD_H
#define VANILLAPDF_EXAMPLES_SCOPE_GUARD_H

#include <utility>

namespace vanillapdf_examples {

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

} // namespace vanillapdf_examples

#define TOKENPASTE(x, y, z) x ## y ## z
#define TOKENPASTE2(x, y, z) TOKENPASTE(x, y, z)
#define SCOPE_GUARD(deleter_function) auto TOKENPASTE2(__scope_guard_, __LINE__, __) = vanillapdf_examples::make_scope_guard(deleter_function)

#endif // VANILLAPDF_EXAMPLES_SCOPE_GUARD_H
