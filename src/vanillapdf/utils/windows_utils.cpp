#include "precompiled.h"

#if _WIN32

// Include header for this file
#include "utils/windows_utils.h"

// Platform specific headers
#include "Windows.h"

namespace vanillapdf {

std::wstring WindowsUtils::MultiByteToWideChar(const std::string& data) {
    return MultiByteToWideChar(data, CP_UTF8);
}

std::wstring WindowsUtils::MultiByteToWideChar(const std::string& data, int code_page) {

    int wide_string_size = ::MultiByteToWideChar(code_page, 0, data.data(), -1, nullptr, 0);

    std::wstring result_wide;
    result_wide.resize(wide_string_size);

    auto conversion_result = ::MultiByteToWideChar(code_page, 0, data.c_str(), -1, result_wide.data(), wide_string_size);
    if (conversion_result == 0) {
        throw GeneralException("Could not convert to wide char: " + data + ", errno: " + std::to_string(GetLastError()));
    }

    return result_wide;
}

} // vanillapdf

#endif /* _WIN32 */
