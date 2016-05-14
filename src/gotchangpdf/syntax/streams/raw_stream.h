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

			virtual BufferPtr read(size_t len) override;
			virtual void read(Buffer& result, size_t len) override;
			virtual BufferPtr readline(void) override;
			virtual types::stream_size GetPosition() override;
			virtual void SetPosition(types::stream_size pos) override;
		};
	}
}

#endif /* _RAW_STREAM_H*/
