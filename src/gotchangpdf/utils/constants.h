#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "gotchangpdf/c_types.h"

namespace gotchangpdf {
namespace types {

// Forward type definitions from C API
typedef ::real_type real;
typedef ::ushort_type ushort;
typedef ::size_type size_type;
typedef ::bigint_type big_int;
typedef ::biguint_type big_uint;

// Custom internal types
typedef int64_t stream_offset;
typedef int64_t stream_size;

} // types

namespace constant {

const int BUFFER_SIZE = 16384;
const types::stream_offset BAD_OFFSET = -1;
const types::ushort MAX_GENERATION_NUMBER = 65535;

} // constant
} // gotchangpdf

#endif /* _CONSTANTS_H */
