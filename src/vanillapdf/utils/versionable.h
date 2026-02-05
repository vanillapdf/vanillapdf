#ifndef _VERSIONABLE_H
#define _VERSIONABLE_H

#include "utils/unknown_interface.h"

#include <atomic>

namespace vanillapdf {

class Versionable : public virtual IUnknown {
public:
    Versionable() = default;

    // Copy/move: intentionally do not copy version state.
    // A copied object starts as clean with version 0.
    Versionable(const Versionable&) noexcept : m_version(0), m_initialized(false) {}
    Versionable& operator=(const Versionable&) noexcept { return *this; }
    Versionable(Versionable&&) noexcept : m_version(0), m_initialized(false) {}
    Versionable& operator=(Versionable&&) noexcept { return *this; }

    uint32_t GetVersion() const noexcept { return m_version.load(std::memory_order_relaxed); }

    void IncrementVersion() noexcept {
        if (m_initialized) {
            m_version.fetch_add(1, std::memory_order_relaxed);
        }
    }

    bool IsInitialized() const noexcept { return m_initialized; }

    virtual void SetInitialized(bool initialized = true) {
        m_initialized = initialized;
    }

    virtual ~Versionable() = 0;

protected:
    std::atomic<uint32_t> m_version = 0;
    bool m_initialized = false;
};

} // vanillapdf

#endif /* _VERSIONABLE_H */
