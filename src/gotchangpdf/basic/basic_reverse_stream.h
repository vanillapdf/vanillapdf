#ifndef _BASIC_REVERSE_STREAM_H
#define _BASIC_REVERSE_STREAM_H

#include "basic_base_stream.h"
#include "raw_reverse_stream.h"

namespace gotchangpdf
{
	namespace basic
	{
		class ReverseStream : public BaseStream, public raw::ReverseStream
		{
		public:
			explicit ReverseStream(std::istream& stream);

			virtual Character Peek() override;
			virtual Character Get() override;
			virtual Character GetHex() override;
			virtual void Unget() override;
			virtual std::shared_ptr<Buffer> Readline(void) override;
		};
	}
}

#endif /* _BASIC_REVERSE_STREAM_H */
