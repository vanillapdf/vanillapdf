#ifndef _BASIC_STREAM_H
#define _BASIC_STREAM_H

#include "BasicBaseStream.h"
#include "RawStream.h"

namespace gotchangpdf
{
	namespace basic
	{
		class Stream : public basic::BaseStream, public raw::Stream
		{
		public:
			explicit Stream(std::istream& stream);

			Stream(const Stream &);

			virtual Character Peek() override;
			virtual Character Get() override;
			virtual Character GetHex() override;
			virtual void Unget() override;
			virtual std::shared_ptr<Buffer> Readline(void) override;

		};
	}
}

#endif /* _BASIC_STREAM_H */
