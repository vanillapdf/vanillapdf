#ifndef _ZLIB_WRAPPER_H
#define _ZLIB_WRAPPER_H

#include "syntax_fwd.h"

namespace gotchangpdf
{
	class ZlibWrapper
	{
	public:
		static BufferPtr Deflate(const Buffer& input);
		static BufferPtr Inflate(const Buffer& input);

		static BufferPtr Deflate(std::istream& input, types::stream_size length);
		static BufferPtr Inflate(std::istream& input, types::stream_size length);

	private:
		ZlibWrapper() = default;
	};
}

#endif /* _ZLIB_WRAPPER_H */
