#ifndef _RAW_STREAM_H
#define _RAW_STREAM_H

#include "raw_base_stream.h"

namespace gotchangpdf
{
	namespace syntax
	{
		class Stream : public BaseStream::CharacterSource, public BaseStream
		{
		public:
			explicit Stream(CharacterSource & stream);
			Stream(const Stream & other);
			virtual ~Stream();

			virtual BufferPtr read(types::uinteger len) override;
			virtual void read(Buffer& result, types::uinteger len) override;
			virtual BufferPtr readline(void) override;
		};
	}
}

#endif /* _RAW_STREAM_H*/
