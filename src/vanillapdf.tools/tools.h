#ifndef _VANILLAPDF_TOOLS_H
#define _VANILLAPDF_TOOLS_H

#include "vanillapdf/c_vanillapdf_api.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

#if defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

extern const int VANILLAPDF_TOOLS_ERROR_SUCCESS;
extern const int VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
extern const int VANILLAPDF_TOOLS_ERROR_FAILURE;

int process_merge(int argc, char *argv[]);
int process_filter(int argc, char *argv[]);
int process_extract(int argc, char *argv[]);
int process_sign(int argc, char *argv[]);

#define RETURN_ERROR_IF_NOT_SUCCESS(fn) \
do { \
	error_type __result__ = (fn); \
	if (VANILLAPDF_ERROR_SUCCESS != __result__) \
	{ \
		printf("Function call \"%s\" has failed with result %u { %s:%d }\n", \
		#fn, __result__, __FILE__, __LINE__); \
		assert(!"Operation failed"); \
		return VANILLAPDF_TOOLS_ERROR_FAILURE; \
	} \
} while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, release) \
do { \
	error_type __result__ = (eval); \
	if (VANILLAPDF_ERROR_SUCCESS == __result__) \
	{ \
		RETURN_ERROR_IF_NOT_SUCCESS(call); \
		RETURN_ERROR_IF_NOT_SUCCESS(release); \
	} \
	else if (VANILLAPDF_ERROR_OBJECT_MISSING == __result__) \
	{ \
		/* Do nothing */ \
	} \
	else \
	{ \
		printf("Function call \"%s\" has failed with result %u { %s:%d }\n", \
		#eval, __result__, __FILE__, __LINE__); \
		assert(!"Operation failed"); \
		return VANILLAPDF_TOOLS_ERROR_FAILURE; \
	} \
} while(0)

#define RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(eval, call) \
RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(eval, call, VANILLAPDF_ERROR_SUCCESS)

#endif /* _VANILLAPDF_TOOLS_H */
