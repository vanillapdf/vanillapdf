#ifndef _PDF_VERSION_H
#define _PDF_VERSION_H

#include "utils/exceptions.h"

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
        if (static_cast<unsigned char>(ver) > static_cast<unsigned char>(file_version))
            throw GeneralException("Expected file version does not match");
    }
};

} // vanillapdf

#endif /* _PDF_VERSION_H */
