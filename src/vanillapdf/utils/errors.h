#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>

namespace vanillapdf {

class Errors {
public:
	static uint32_t GetLastError() noexcept;
	static void SetLastError(uint32_t value) noexcept;

	static std::string GetLastErrorMessage();
	static void SetLastErrorMessage(const std::string& value);

private:
	static thread_local uint32_t m_error;
	static thread_local std::string m_message;
};

} // vanillapdf

#endif /* _ERRORS_H */
