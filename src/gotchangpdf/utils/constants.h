#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <cstdint>

namespace gotchangpdf {
namespace types {

typedef int native_int;
typedef unsigned int native_uint;

typedef uint16_t ushort;
typedef int32_t integer;
typedef uint32_t uinteger;

typedef int64_t big_int;
typedef uint64_t big_uint;

typedef int64_t stream_offset;
typedef int64_t stream_size;

typedef double real;

} // types

namespace constant {

const int BUFFER_SIZE = 16384;
const int BAD_OFFSET = -1;
const types::ushort MAX_GENERATION_NUMBER = 65535;

} // constant
} // gotchangpdf

#endif /* _CONSTANTS_H */
