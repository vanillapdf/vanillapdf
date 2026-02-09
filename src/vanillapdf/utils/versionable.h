#ifndef _VERSIONABLE_H
#define _VERSIONABLE_H

#include "utils/unknown_interface.h"

#include <atomic>

namespace vanillapdf {

// Poll-based dirty tracking via atomic version counter.
// Replaces the previous observer pattern (IModifyObservable/IModifyObserver)
// which required heap-allocated subscriber lists and virtual dispatch per mutation.
//
// Benchmark comparison (Release, MSVC 17, x64):
//
//   Benchmark                              Before (ns)  After (ns)  Speedup
//   CreateFromEncodedString_Hex/empty              963         240    +301%
//   CreateFromEncodedString_Hex/values            1025         314    +226%
//   CreateFromEncodedString_Literal/empty          963         267    +261%
//   CreateFromEncodedString_Literal/basic         1005         308    +226%
//   CreateFromEncodedString_Name/empty             544         188    +189%
//   CreateFromEncodedString_Name/basic             715         392     +82%
//   StringGetValue_Hex/empty                      1074         408    +163%
//   StringGetValue_Hex/unpaired                   1228         547    +124%
//   StringGetValue_Literal/empty                  1604         855     +88%
//   StringGetValue_Literal/basic                  3115        2197     +42%
//   HexStringObjectToPdf/empty                    1475         750     +97%
//   NameObjectToPdf/empty                         1350         628    +115%
//   NameObjectToPdf/basic                         2455        1709     +44%
//   LiteralStringObjectToPdf/empty                2250        1535     +47%
//   RealObjectToPdf/no_precision                   609         531     +15%
//
class Versionable : public virtual IUnknown {
public:
    Versionable() = default;

    // Copy/move: intentionally do not copy version state.
    // A copied object starts as clean with version 0.
    Versionable(const Versionable&) noexcept : m_version(0), m_initialized(false) {}
    Versionable& operator=(const Versionable&) noexcept { return *this; }
    Versionable(Versionable&&) noexcept : m_version(0), m_initialized(false) {}
    Versionable& operator=(Versionable&&) noexcept { return *this; }

    uint32_t GetVersion() const noexcept {
        return m_version.load(std::memory_order_relaxed);
    }

    void IncrementVersion() noexcept {
        if (m_initialized.load(std::memory_order_relaxed)) {
            m_version.fetch_add(1, std::memory_order_relaxed);
        }
    }

    bool IsInitialized() const noexcept {
        return m_initialized.load(std::memory_order_relaxed);
    }

    virtual void SetInitialized(bool initialized = true) {
        m_initialized.store(initialized, std::memory_order_relaxed);
    }

    virtual ~Versionable() = 0;

protected:
    std::atomic<uint32_t> m_version = 0;
    std::atomic<bool> m_initialized = false;
};

} // vanillapdf

#endif /* _VERSIONABLE_H */
