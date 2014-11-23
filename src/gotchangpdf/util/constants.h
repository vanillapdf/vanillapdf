#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <cstdint>

namespace gotchangpdf
{
	namespace constant
	{
		const int BUFFER_SIZE = 4096;
	}

	namespace types
	{
		typedef uint16_t ushort;
		typedef uint32_t integer;

		typedef int64_t stream_offset;
		typedef int64_t stream_size;

		typedef double real;
	}
}

#endif /* _CONSTANTS_H */
