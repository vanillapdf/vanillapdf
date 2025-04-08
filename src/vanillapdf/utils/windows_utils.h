#ifndef _WINDOWS_UTILS_H
#define _WINDOWS_UTILS_H

#if _WIN32

#include <string>

namespace vanillapdf {

class WindowsUtils {
public:
    static std::wstring MultiByteToWideChar(const std::string& data);
    static std::wstring MultiByteToWideChar(const std::string& data, int code_page);

private:
    WindowsUtils() = default;
};

} // vanillapdf

#endif /* _WIN32 */

#endif /* _WINDOWS_UTILS_H */
