#ifndef _RAW_BASE_STREAM_H
#define _RAW_BASE_STREAM_H

#include "syntax_fwd.h"
#include "buffer.h"

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
			virtual void read(BufferPtr& result, types::uinteger len) = 0;
			virtual BufferPtr read(types::uinteger len) = 0;
			virtual void read_exact(BufferPtr buf);
			virtual BufferPtr readline(void) = 0;

			virtual ~BaseStream() = 0;
		};
	}
}

#endif /* _RAW_BASE_STREAM_H */
