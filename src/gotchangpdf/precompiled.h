#ifndef _PRECOMPILED_H
#define _PRECOMPILED_H

#include "gotchangpdf/c_platform.h"

/*--------------Global warning disables--------------*/

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)

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

	// MSVC warns using standard library
	// function localtime and it recommends
	// using non-standard localtime_s.
	// That's just SILLY!
	#define _CRT_SECURE_NO_WARNINGS
#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

/*---------------------------------------------------*/

// Memory leak tracking
#if defined(DEBUG)

	#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>

		// This only works with VC++
		#ifndef pdf_new
			#define pdf_new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#endif
	#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

	// TODO create allocation tracking function for other platforms
	#ifndef pdf_new
		#define pdf_new new
	#endif /* pdf_new */

#else
	// Ignore memory tracking in non-debug modes

	#ifndef pdf_new
		#define pdf_new new
	#endif /* pdf_new */
#endif /* DEBUG */

// Solution
#include "utils/deferred.h"
#include "utils/constants.h"
#include "utils/exceptions.h"
#include "utils/log.h"
#include "utils/util.h"

#include "syntax/utils/object_utils.h"

#include "syntax/objects/objects.h"

#endif /* _PRECOMPILED_H */
