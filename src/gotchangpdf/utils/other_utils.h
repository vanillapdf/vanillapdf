#ifndef _OTHER_UTILS_H
#define _OTHER_UTILS_H

#include <string>

namespace gotchangpdf {

inline std::string ExtractFilename(const std::string& path) {
	auto pos = path.rfind('\\');
	if (pos == std::string::npos) {
		pos = path.rfind('/');
		if (pos == std::string::npos) {
			return path;
		}
	}

	return std::string(path.begin() + (pos + 1), path.end());
}

} // gotchangpdf

#endif /* _OTHER_UTILS_H */
