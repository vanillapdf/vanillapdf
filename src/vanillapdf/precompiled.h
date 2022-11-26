#ifndef _PRECOMPILED_H
#define _PRECOMPILED_H

#include "vanillapdf/c_platform.h"

/*-----------------------TO DO-----------------------*/

/*--------------------Known issues--------------------*/
// Issue: 1
// FIX: vc141 (works on v140) triggers error:
// error C2600: 'vanillapdf::syntax::ObjectPtr::ObjectPtr': cannot define a compiler-generated special member function (must be declared in the class first)

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

#if defined(COMPILER_GCC)

	// On Linux arm GCC the entire log is filled with notes:

	// parameter passing for argument of type 'std::_Rb_tree<vanillapdf::syntax::IndirectReferenceObject,
	// std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool>,
	// std::_Select1st<std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool> >,
	// std::less<vanillapdf::syntax::IndirectReferenceObject>,
	// std::allocator<std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool> > >::const_iterator' changed in GCC 7.1

	// That warning is telling you that there was a subtle ABI change
	// (actually a conformance fix) between 6 and 7.1,
	// such that libraries built with 6.x or earlier may not work properly when called
	// from code built with 7.x (and vice-versa).
	// As long as all your C++ code is built with GCC 7.1 or later, you can safely ignore this warning.
	// To disable it, pass -Wno-psabi to the compiler.
	// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=77728
	#pragma GCC diagnostic ignored "-Wno-psabi"

#endif /* COMPILER_GCC */

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
