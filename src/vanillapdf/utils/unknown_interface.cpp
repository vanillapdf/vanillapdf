#include "precompiled.h"

#include "utils/unknown_interface.h"

#include <algorithm>
#include <cassert>

namespace vanillapdf {

std::atomic<int64_t> IUnknown::s_active_object_count = 0;
std::atomic<int64_t> IUnknown::s_peak_object_count = 0;
std::atomic<int64_t> IUnknown::s_total_objects_created = 0;

WeakReferenceCounter::WeakReferenceCounter() noexcept : m_active(true) {
}

bool WeakReferenceCounter::IsActive() const noexcept {
    return m_active.load();
}

void WeakReferenceCounter::Deactivate() noexcept {
    m_active = false;
}

IUnknown::~IUnknown() {
    s_active_object_count -= 1;
}

IUnknown::IUnknown() noexcept : m_ref_counter(0) {
    auto current = ++s_active_object_count;
    s_total_objects_created += 1;

    // Update peak using compare-exchange loop
    auto peak = s_peak_object_count.load();
    while (current > peak && !s_peak_object_count.compare_exchange_weak(peak, current)) {
    }
}

IUnknown::IUnknown(const IUnknown&) noexcept : m_ref_counter(0) {
    auto current = ++s_active_object_count;
    s_total_objects_created += 1;

    auto peak = s_peak_object_count.load();
    while (current > peak && !s_peak_object_count.compare_exchange_weak(peak, current)) {
    }
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

void IUnknown::Release() noexcept {
    if (--m_ref_counter == 0) {
        delete this;
    }
}

int64_t IUnknown::ActiveObjectCount() noexcept {
    return s_active_object_count.load();
}

int64_t IUnknown::PeakObjectCount() noexcept {
    return s_peak_object_count.load();
}

int64_t IUnknown::TotalObjectsCreated() noexcept {
    return s_total_objects_created.load();
}

void IUnknown::ResetDiagnosticCounters() noexcept {
    auto current = s_active_object_count.load();
    s_peak_object_count.store(current);
    s_total_objects_created.store(current);
}

} // vanillapdf
