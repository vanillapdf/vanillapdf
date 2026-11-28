#include "precompiled.h"

#include "utils/errors.h"

#include <algorithm>

namespace vanillapdf {

thread_local uint32_t Errors::m_error = 0;
thread_local size_type Errors::m_message_length = 0;
thread_local char Errors::m_message[] = {0};

uint32_t Errors::GetLastError() noexcept {
    return m_error;
}

uint32_t Errors::SetLastError(uint32_t value, std::string_view message) noexcept {
    auto message_length = std::min<size_type>(message.size() + 1, constant::MAX_MESSAGE_SIZE - 1);
    auto copied_length = std::min<size_type>(message.size(), message_length);

    std::copy_n(message.data(), copied_length, m_message);
    m_message[copied_length] = '\0';

    m_error = value;
    m_message_length = message_length;

    return value;
}

size_type Errors::GetLastErrorMessageLength() noexcept {
    return m_message_length;
}

const char * Errors::GetLastErrorMessage() noexcept {
    return m_message;
}

} // vanillapdf
