#ifndef _PARSER_INTERFACE_H
#define _PARSER_INTERFACE_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/xref.h"

#include "syntax/parsers/object_stream_entry.h"

#include "utils/constants.h"

#include <vector>

namespace vanillapdf {
namespace syntax {

class IParser {
public:
    virtual XrefBasePtr ReadXref(void) = 0;
    virtual XrefBasePtr ReadXref(types::stream_offset offset) = 0;

    virtual ObjectStreamEntries ReadObjectStreamEntries(types::big_uint first, types::size_type size) = 0;

    virtual ~IParser() {}
};

class IReverseParser {
public:
    virtual types::stream_offset ReadLastXrefOffset() = 0;

    virtual ~IReverseParser() {}
};

} // syntax
} // vanillapdf

#endif /* _PARSER_INTERFACE_H */
