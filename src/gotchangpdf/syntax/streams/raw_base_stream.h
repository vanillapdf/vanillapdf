#ifndef _RAW_BASE_STREAM_H
#define _RAW_BASE_STREAM_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/buffer.h"

#include <istream>

namespace gotchangpdf
{
	namespace syntax
	{
		class BaseStream
		{
		public:
			typedef std::istream CharacterSource;
			typedef std::streambuf CharacterSourceBuffer;

		public:
			virtual void read(Buffer& result, size_t len) = 0;
			virtual BufferPtr read(size_t len) = 0;
			virtual BufferPtr readline(void) = 0;
			virtual types::stream_size GetPosition() = 0;
			virtual void SetPosition(types::stream_size pos) = 0;

			virtual ~BaseStream() {};
		};
	}
}

#endif /* _RAW_BASE_STREAM_H */
