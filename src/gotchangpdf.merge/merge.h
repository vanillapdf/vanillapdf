#ifndef _GOTCHANGPDF_MERGE_H
#define _GOTCHANGPDF_MERGE_H

#include "gotchangpdf/c_gotchangpdf_api.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

#if defined(DEBUG) && defined(_MSC_VER)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

extern const int GOTCHANG_PDF_MERGE_ERROR_SUCCESS;
extern const int GOTCHANG_PDF_MERGE_ERROR_INVALID_PARAMETERS;
extern const int GOTCHANG_PDF_MERGE_ERROR_FAILURE;

void print_help();

#define RETURN_ERROR_IF_NOT_SUCCESS(fn) \
do { \
	error_type __result__ = (fn); \
	if (GOTCHANG_PDF_ERROR_SUCCES != __result__) \
	{ \
		printf("Function call \"%s\" has failed with result %d { %s:%d }\n", \
		#fn, __result__, __FILE__, __LINE__); \
		assert(!"Operation failed"); \
		return GOTCHANG_PDF_MERGE_ERROR_FAILURE; \
	} \
} while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, release) \
do { \
	error_type __result__ = (eval); \
	if (GOTCHANG_PDF_ERROR_SUCCES == __result__) \
	{ \
		RETURN_ERROR_IF_NOT_SUCCESS(call); \
		RETURN_ERROR_IF_NOT_SUCCESS(release); \
	} \
	else if (GOTCHANG_PDF_ERROR_OBJECT_MISSING == __result__) \
	{ \
		/* Do nothing */ \
	} \
	else \
	{ \
		printf("Function call \"%s\" has failed with result %d { %s:%d }\n", \
		#eval, __result__, __FILE__, __LINE__); \
		assert(!"Operation failed"); \
		return GOTCHANG_PDF_MERGE_ERROR_FAILURE; \
	} \
} while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(eval, call) \
RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, GOTCHANG_PDF_ERROR_SUCCES)

#endif /* _GOTCHANGPDF_MERGE_H */
