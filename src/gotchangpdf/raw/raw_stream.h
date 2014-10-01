#ifndef _RAW_STREAM_H
#define _RAW_STREAM_H

#include "raw_base_stream.h"

namespace gotchangpdf
{
	namespace raw
	{
		class Stream : public raw::BaseStream::CharacterSource, public raw::BaseStream
		{
		public:
			explicit Stream(CharacterSource & stream);
			Stream(const Stream & other);
			virtual ~Stream();

			virtual Buffer read(unsigned int len) override;
			virtual char get_hex() override;
			virtual Buffer readline(void) override;

		private:

#ifdef USE_BOOST_FILTERING_STREAMS
			class FilteringBuffer : public CharacterFilteringSourceBuffer
			{
			public:
				FilteringBuffer(CharacterSource & stream) : CharacterFilteringSourceBuffer(stream) {}
				FilteringBuffer(CharacterSourceBuffer & buf) : CharacterFilteringSourceBuffer(buf) {}
			};
#endif
		};
	}
}

#endif /* _RAW_STREAM_H*/
