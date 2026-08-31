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

template <Version ver>
class RequireVersion {
public:
    RequireVersion() = default;

    void OnWriteCheck(Version file_version) {
        if (static_cast<unsigned char>(ver) > static_cast<unsigned char>(file_version)) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "The entry requires PDF version {}, but the file declares {}",
                static_cast<int>(ver), static_cast<int>(file_version));
        }
    }
};

} // vanillapdf

#endif /* _PDF_VERSION_H */
