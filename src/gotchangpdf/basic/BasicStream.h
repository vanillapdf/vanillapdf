#ifndef _BASIC_STREAM_H
#define _BASIC_STREAM_H

#include "BasicBaseStream.h"
#include "RawStream.h"

namespace Pdf
{
	namespace Streams
	{
		namespace Basic
		{
			class Stream : public Basic::BaseStream, public Raw::Stream
			{
			public:
				explicit Stream(std::istream& stream);

				Pdf::Streams::Basic::Stream::Stream(const Pdf::Streams::Basic::Stream &);

				virtual Character Peek() override;
				virtual Character Get() override;
				virtual void Unget() override;
				virtual std::shared_ptr<Buffer> Readline(void) override;
			};
		}
	}
}

#endif /* _BASIC_STREAM_H */
