#ifndef _PRECOMPILED_H
#define _PRECOMPILED_H

/*--------------Global warning disables--------------*/

// declaration of 'end' hides class member
#pragma warning (disable: 4458)

// declaration of 'attr' hides global declaration
#pragma warning (disable: 4459)

// decorated name length exceeded, name was truncated
#pragma warning (disable: 4503)

// forceinline not inlined
#pragma warning (disable: 4714)

// Do while false
#pragma warning (disable: 4127)

/*---------------------------------------------------*/

// Memory leak tracking
#if defined(DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>

	#ifndef pdf_new
		#define pdf_new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	#endif
#elif defined(RELEASE)
	#ifndef pdf_new
		#define pdf_new new
	#endif
#else
	#error Unknown project configuration
#endif

// Solution
#include "deferred.h"
#include "output_pointer.h"
#include "objects.h"
#include "object_utils.h"
#include "exception.h"
#include "constants.h"
#include "log.h"

#endif /* _PRECOMPILED_H */
