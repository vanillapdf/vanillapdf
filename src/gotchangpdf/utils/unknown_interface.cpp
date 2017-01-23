#include "precompiled.h"

#include "utils/unknown_interface.h"

#include <cassert>

namespace gotchangpdf {

WeakReferenceCounter::WeakReferenceCounter(IUnknown* ptr) noexcept
	: m_active(true), m_reference(ptr) {
	assert(nullptr != ptr);
}

bool WeakReferenceCounter::IsActive() const noexcept {
	return m_active.load();
}

void WeakReferenceCounter::Deactivate() noexcept {
	m_active = false;
}

IUnknown* WeakReferenceCounter::GetReference() const noexcept {
	return m_reference;
}

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

void IUnknown::Release() noexcept {
	if (--m_ref_counter == 0) {
		if (m_weak_ref) {
			m_weak_ref->Deactivate();
		}

		delete this;
	}
}

} // gotchangpdf
