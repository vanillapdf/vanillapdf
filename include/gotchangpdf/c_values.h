#ifndef _VALUES_H
#define _VALUES_H

#include "c_export.h"

#include <stdint.h>

typedef int32_t error_type;

typedef int8_t boolean_type;
typedef int8_t* out_boolean_type;

typedef double real_type;
typedef double* out_real_type;

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

#ifdef __cplusplus
extern "C"
{
#endif

	// boolean values
	GOTCHANG_PDF_API extern const boolean_type GOTCHANG_PDF_RV_FALSE;
	GOTCHANG_PDF_API extern const boolean_type GOTCHANG_PDF_RV_TRUE;

	// global error states
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_SUCCES;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_PARAMETER_VALUE;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_NOT_SUPPORTED;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_GENERAL;

	// syntax errors
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_FILE_DISPOSED;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_OBJECT_MISSING;

	// semantic errors
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;

#ifdef __cplusplus
};
#endif

#endif /* _VALUES_H */
