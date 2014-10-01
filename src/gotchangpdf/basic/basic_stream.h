#ifndef _BASIC_STREAM_H
#define _BASIC_STREAM_H

#include "basic_base_stream.h"
#include "raw_stream.h"
#include "character_source.h"

namespace gotchangpdf
{
	namespace basic
	{
		class Stream : public basic::BaseStream, public raw::Stream
		{
		public:
			explicit Stream(CharacterSource & stream);
			Stream(const Stream & other);

			virtual Character Peek() override;
			virtual Character Get() override;
			virtual Character GetHex() override;
			virtual void Unget() override;
			virtual Buffer Readline(void) override;

		};
	}
}

#endif /* _BASIC_STREAM_H */
