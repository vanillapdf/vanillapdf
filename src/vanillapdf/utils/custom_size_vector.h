#ifndef _CUSTOM_SIZE_VECTOR_H
#define _CUSTOM_SIZE_VECTOR_H

#include "utils/constants.h"
#include "utils/custom_size_allocator.h"

#include <vector>

namespace vanillapdf {

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO) && defined(ENVIRONMENT_32_BIT)

	// Visual studio (only in 32-bit) triggers warning about possible loss of data:
	// "warning C4244: 'argument': conversion from 'const unsigned __int64' to 'int', possible loss of data"

	// This whole masquerade is because type size_t is implementation defined.
	// Most of my API is dependent on this type, but I need to guarantee the API regardless of the compiler.
	// I am trying to force the size_type of vector to be possibly different than size_t.
	// On GCC it works without any error or warning, but on VS it does not.

	// The problem is rooted in VC standard library, where in method assign it uses ptrdiff_t
	// defined in standard instead of iterator trait difference_type.
	// However, even if the compiler says it is possible loss of data, I believe such case should not happen.

	// On 32-bit architecture where size_t is 32-bit unsigned integer

	static_assert(sizeof(size_t) == 4, "32-bit Visual studio has changed size_t type");

	// does not allow to allocate more than std::numeric_limits<size_t>::max().
	// There seem to be a rule, that allows implicit conversion from unsigned int to signed int
	// and it does not trigger any error nor warning.
	// Assuming this case is valid, then any other unsigned integer types greater or equal
	// than unsigned int should behave in the same fashion.

	//#pragma warning (push)
	//#pragma warning (disable : 4244)

	// Note: this warning disable does not work!
	// I am forced to use different size types for 32-bit and 64-bit platforms.

#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO && ENVIRONMENT_32_BIT */

template <typename T, typename size_type_template = types::size_type>
class CustomSizeVector : public std::vector<T, CustomSizeAllocator<T, size_type_template>> {
public:
	using std::vector<T, CustomSizeAllocator<T, size_type_template>>::vector;
};

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO) && defined(ENVIRONMENT_32_BIT)
	//#pragma warning (pop)
#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO && ENVIRONMENT_32_BIT */

} // vanillapdf

#endif /* _CUSTOM_SIZE_VECTOR_H */
