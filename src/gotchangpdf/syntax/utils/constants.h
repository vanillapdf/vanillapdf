#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <cstdint>

namespace gotchangpdf
{
	namespace constant
	{
		const int BUFFER_SIZE = 16384;
	}

	namespace types
	{
		typedef uint16_t ushort;
		typedef int32_t integer;
		typedef uint32_t uinteger;

		typedef int64_t stream_offset;
		typedef int64_t stream_size;

		/* This is required to be double of the size of the integer */
		typedef int64_t double_integer;
		static_assert(sizeof(double_integer) == 2 * sizeof(integer), "Adjust type of result to be double size of the integer type");

		typedef float real;
	}
}

#endif /* _CONSTANTS_H */
