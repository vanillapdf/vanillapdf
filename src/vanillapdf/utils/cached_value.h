#ifndef _CACHED_VALUE_H
#define _CACHED_VALUE_H

#include <atomic>
#include <mutex>
#include <functional>

namespace vanillapdf {

// Thread-safe, compute-once lazy cache for a single value.
//
// Replaces the hand-rolled lazy-init pattern
//     if (!m_x.empty()) { result = m_x; return true; }
//     ... compute ...
//     m_x = make_deferred<...>(); result = m_x; return true;
// which is racy under concurrent first access: two threads can both observe the
// empty cache, both construct, and race on the cached pointer (torn read /
// double release / use-after-free).
//
// Mirrors the established lazy-init idiom in the codebase (see
// UnicodeCharacterMap::Initialize): a lock-free acquire fast path, a
// double-checked init under a recursive mutex, and a release store so other
// threads observe the fully constructed value before they see the flag set.
//
// The std::atomic and std::recursive_mutex members make CachedValue
// non-copyable and non-movable; embed it as a member of a non-copyable owner.
template <typename T>
class CachedValue {
public:
    // A resolver is a const member function of the owner that produces the
    // value: bool Owner::Resolve(T& out) const
    //   returns true  -> value produced; cached and returned via result
    //   returns false -> entry absent; NOT cached, retried on the next call
    //
    // Passing it as a member function pointer (rather than the minimal state)
    // gives the resolver implicit access to the whole owner — every member and
    // private helper — which is what most resolvers need. std::invoke performs
    // the member dispatch so the call reads like an ordinary call. Only the
    // cold path touches owner and resolver; the already initialized fast path
    // returns the cached value directly.
    template <typename Owner>
    bool GetOrCreate(T& result, const Owner* owner, bool (Owner::*resolver)(T&) const) const {
        if (m_initialized.load(std::memory_order_acquire)) {
            result = m_value;
            return true;
        }

        std::lock_guard<std::recursive_mutex> lock(m_access_lock);

        // Double-check after acquiring the lock
        if (m_initialized.load(std::memory_order_relaxed)) {
            result = m_value;
            return true;
        }

        T produced;
        if (!std::invoke(resolver, owner, produced)) {
            return false;
        }

        m_value = produced;
        m_initialized.store(true, std::memory_order_release);

        result = m_value;
        return true;
    }

private:
    mutable std::atomic<bool> m_initialized = false;
    mutable std::recursive_mutex m_access_lock;
    mutable T m_value;
};

} // vanillapdf

#endif /* _CACHED_VALUE_H */
