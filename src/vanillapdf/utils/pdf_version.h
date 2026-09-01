#ifndef _PDF_VERSION_H
#define _PDF_VERSION_H

#include "utils/exceptions.h"
#include "utils/log.h"

namespace vanillapdf {

enum class Version {
    Undefined = 0,
    PDF10,
    PDF11,
    PDF12,
    PDF13,
    PDF14,
    PDF15,
    PDF16,
    PDF17,
    PDF20
};

inline const char* VersionName(Version version) {
    switch (version) {
        case Version::PDF10:
            return "1.0";
        case Version::PDF11:
            return "1.1";
        case Version::PDF12:
            return "1.2";
        case Version::PDF13:
            return "1.3";
        case Version::PDF14:
            return "1.4";
        case Version::PDF15:
            return "1.5";
        case Version::PDF16:
            return "1.6";
        case Version::PDF17:
            return "1.7";
        case Version::PDF20:
            return "2.0";
        default:

            // The name is used in log and exception messages, so it has to stay
            // a printable string even for values outside of the enumeration.

            return "Undefined";
    }
}

template <Version ver>
class RequireVersion {
public:
    RequireVersion() = default;

    void OnWriteCheck(Version file_version) {
        if (static_cast<unsigned char>(ver) > static_cast<unsigned char>(file_version)) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "The entry requires PDF version {}, but the file declares {}",
                VersionName(ver), VersionName(file_version));
        }
    }
};

} // vanillapdf

#endif /* _PDF_VERSION_H */
