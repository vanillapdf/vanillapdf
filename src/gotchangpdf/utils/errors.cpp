#include "precompiled.h"

#include "utils/errors.h"

namespace gotchangpdf {

thread_local uint32_t Errors::m_error = 0;
thread_local std::string Errors::m_message;

uint32_t Errors::GetLastError() noexcept {
	return m_error;
}

void Errors::SetLastError(uint32_t value) noexcept {
	m_error = value;
}

std::string Errors::GetLastErrorMessage() {
	return m_message;
}

void Errors::SetLastErrorMessage(std::string value) {
	m_message = value;
}

} // gotchangpdf
