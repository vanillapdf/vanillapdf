#include "precompiled.h"

#include "utils/errors.h"

#include <cstring>

namespace vanillapdf {

thread_local uint32_t Errors::m_error = 0;
thread_local size_type Errors::m_message_length = 0;
thread_local char Errors::m_message[] = {0};

uint32_t Errors::GetLastError() noexcept {
    return m_error;
}

void Errors::SetLastError(uint32_t value) noexcept {
    m_error = value;
}

size_type Errors::GetLastErrorMessageLength() noexcept {
    return m_message_length;
}

const char * Errors::GetLastErrorMessage() noexcept {
    return m_message;
}

void Errors::SetLastErrorMessage(const std::string& value) noexcept {
    SetLastErrorMessage(value.c_str(), value.size() + 1);
}

void Errors::SetLastErrorMessage(const char * value) noexcept {
    auto length = std::strlen(value);
    SetLastErrorMessage(value, length + 1);
}

void Errors::SetLastErrorMessage(const char * value, size_type length) noexcept {
    auto limited_length = std::min<size_type>(length, constant::MAX_MESSAGE_SIZE - 1);

    std::memcpy(m_message, value, limited_length);
    m_message[limited_length] = '\0';

    m_message_length = limited_length;
}

} // vanillapdf
