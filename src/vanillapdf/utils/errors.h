#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>

namespace vanillapdf {

class Errors {
public:
	static uint32_t GetLastError() noexcept;
	static void SetLastError(uint32_t value) noexcept;

	static size_type GetLastErrorMessageLength() noexcept;
	static const char* GetLastErrorMessage() noexcept;

	static void SetLastErrorMessage(const char * value) noexcept;
	static void SetLastErrorMessage(const char * value, size_type length) noexcept;
	static void SetLastErrorMessage(const std::string& value) noexcept;

private:
	static thread_local uint32_t m_error;
	static thread_local size_type m_message_length;
	static thread_local char m_message[constant::MAX_MESSAGE_SIZE];
};

} // vanillapdf

#endif /* _ERRORS_H */
