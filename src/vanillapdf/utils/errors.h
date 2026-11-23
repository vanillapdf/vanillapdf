#ifndef _ERRORS_H
#define _ERRORS_H

#include <string_view>

namespace vanillapdf {

class Errors {
public:
    static uint32_t GetLastError() noexcept;

    // Stores the code and the message as the last error of the thread.
    // Returns the code, so that a C API function can return it in one statement.
    static uint32_t SetLastError(uint32_t value, std::string_view message) noexcept;

    static size_type GetLastErrorMessageLength() noexcept;
    static const char* GetLastErrorMessage() noexcept;

private:
    static thread_local uint32_t m_error;
    static thread_local size_type m_message_length;
    static thread_local char m_message[constant::MAX_MESSAGE_SIZE];
};

} // vanillapdf

#endif /* _ERRORS_H */
