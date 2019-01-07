#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "vanillapdf/c_types.h"

namespace vanillapdf {
namespace types {

// Forward type definitions from C API
typedef ::real_type real;
typedef ::ushort_type ushort;
typedef ::size_type size_type;
typedef ::bigint_type big_int;
typedef ::biguint_type big_uint;

// Custom internal types
typedef ::offset_type stream_offset;
typedef ::offset_type stream_size;

} // types

namespace constant {

// General constants
extern const int BUFFER_SIZE;
extern const uint32_t TOKENIZER_CACHE_SIZE;
extern const types::stream_offset BAD_OFFSET;
extern const types::ushort MAX_GENERATION_NUMBER;

// Compiler time required
const uint32_t MAX_MESSAGE_SIZE = 256;

} // constant
} // vanillapdf

#endif /* _CONSTANTS_H */
