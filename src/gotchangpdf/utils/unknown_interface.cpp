#include "precompiled.h"

#include "utils/unknown_interface.h"

#include <cassert>

namespace gotchangpdf {

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

void IUnknown::Release() noexcept {
	if (--m_ref_counter == 0) {
		delete this;
	}
}

} // gotchangpdf
