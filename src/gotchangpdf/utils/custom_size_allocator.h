#ifndef _CUSTOM_SIZE_ALLOCATOR_H
#define _CUSTOM_SIZE_ALLOCATOR_H

#include "utils/conversion_utils.h"

#include <memory>

namespace gotchangpdf {

template <typename T, typename size_type_template>
class CustomSizeAllocator : public std::allocator<T> {
public:
	static_assert(std::is_unsigned<size_type_template>::value, "Size allocator type should be unsigned");

	// There are already multiple cases, when smaller size type can cause harm.
	// All types of conversions done in the standard library forced me to protect
	// size of this template parameter.
	static_assert(sizeof(size_t) <= sizeof(size_type_template), "Size allocator type should be greater or equal to size_t");

public:
	typedef T value_type;
	typedef T* pointer;
	typedef size_type_template size_type;
	typedef typename std::make_signed<size_type_template>::type difference_type;

	template <typename U>
	struct rebind {
		typedef CustomSizeAllocator<U, size_type> other;
	};

	CustomSizeAllocator() = default;
	CustomSizeAllocator(const CustomSizeAllocator& other) = default;
	CustomSizeAllocator(CustomSizeAllocator&& other) = default;

	CustomSizeAllocator& operator=(const CustomSizeAllocator& other) = default;
	CustomSizeAllocator& operator=(CustomSizeAllocator&& other) = default;

	template <typename U>
	CustomSizeAllocator(const CustomSizeAllocator<U, size_type>&) {

	}

	pointer allocate(size_type n) {
		size_t n_converted = ValueConvertUtils::SafeConvert<size_t>(n);
		return std::allocator<T>::allocate(n_converted);
	}

	void deallocate(pointer p, size_type n) {
		size_t n_converted = ValueConvertUtils::SafeConvert<size_t>(n);
		return std::allocator<T>::deallocate(p, n_converted);
	}

	size_type max_size() const {
		return (std::numeric_limits<size_type>::max() / sizeof(value_type));
	}
};

} // gotchangpdf

#endif /* _CUSTOM_SIZE_ALLOCATOR_H */
