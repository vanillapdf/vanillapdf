#ifndef _VALUES_H
#define _VALUES_H

#include <stdint.h>

typedef int32_t error_type;

typedef int32_t boolean_type;
typedef int32_t* out_boolean_type;

typedef int32_t integer_type;
typedef int32_t* out_integer_type;

typedef uint16_t ushort_type;
typedef uint16_t* out_ushort_type;

typedef int64_t offset_type;
typedef int64_t* out_offset_type;

typedef int64_t size_type;
typedef int64_t* out_size_type;

typedef const char* string_type;
typedef const char** out_string_type;

#define GOTCHANG_PDF_RV_FALSE              0
#define GOTCHANG_PDF_RV_TRUE               1

#define GOTCHANG_PDF_ERROR_SUCCES          0
#define GOTCHANG_PDF_ERROR_GENERAL         1
#define GOTCHANG_PDF_ERROR_PARAMETER_VALUE 2
#define GOTCHANG_PDF_ERROR_FILE_DISPOSED   3

#endif /* _VALUES_H */
