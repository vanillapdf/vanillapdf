#ifndef _BASIC_REVERSE_STREAM_H
#define _BASIC_REVERSE_STREAM_H

#include "streams/basic/BasicBaseStream.h"
#include "streams/raw/RawReverseStream.h"

namespace Pdf
{
	namespace Streams
	{
		namespace Basic
		{
			class ReverseStream : public BaseStream, public Raw::ReverseStream
			{
			public:
				explicit ReverseStream(std::istream& stream);

				virtual Character Peek() override;
				virtual Character Get() override;
				virtual void Unget() override;
				virtual std::unique_ptr<CharacterSet> Readline(void) override;
			};
		}
	}
}

#endif /* _BASIC_REVERSE_STREAM_H */
