#include "precompiled.h"

#include "utils/unknown_interface.h"

#include <cassert>

namespace vanillapdf {

WeakReferenceCounter::WeakReferenceCounter() noexcept : m_active(true) {
}

bool WeakReferenceCounter::IsActive() const noexcept {
    return m_active.load();
}

void WeakReferenceCounter::Deactivate() noexcept {
    m_active = false;
}

IUnknown::~IUnknown() {}

IUnknown::IUnknown() noexcept : m_ref_counter(0) {
}

IUnknown::IUnknown(const IUnknown&) noexcept : m_ref_counter(0) {
}

IUnknown& IUnknown::operator= (const IUnknown&) noexcept {
    return *this;
}

uint32_t IUnknown::UseCount() const noexcept {
    return m_ref_counter.load();
}

void IUnknown::AddRef() noexcept {
    m_ref_counter++;
}

bool IUnknown::TryAddRef() noexcept {

    // "Increment, but only if not zero" is a conditional read-modify-write, and
    // compare_exchange is the only one the hardware offers - there is no atomic
    // increment-if-not-zero. The exchange fails when another thread modified the
    // counter in between, writing the value it observed back into current, so
    // the attempt is repeated against that fresh value. This is not a spin wait:
    // it re-runs only while the counter is actively being modified, and it stops
    // as soon as the object reaches zero and is therefore dead.
    auto current = m_ref_counter.load();

    while (current != 0) {
        if (m_ref_counter.compare_exchange_weak(current, current + 1)) {
            return true;
        }
    }

    return false;
}

void IUnknown::Release() noexcept {
    if (--m_ref_counter == 0) {
        delete this;
    }
}

} // vanillapdf
