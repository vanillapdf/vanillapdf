#ifndef _RAW_STREAM_H
#define _RAW_STREAM_H

#include "streams/raw/RawBaseStream.h"

#include <istream>

namespace Pdf
{
	namespace Streams
	{
		namespace Raw
		{
			class Stream : public Raw::BaseStream, public std::istream
			{
			public:
				explicit Stream(std::istream& stream);
				Pdf::Streams::Raw::Stream::Stream(const Pdf::Streams::Raw::Stream &);

				virtual void Read(char* bytes, unsigned int len) override;
				virtual std::unique_ptr<char> Read(unsigned int len) override;

				virtual ~Stream();
			};
		}
	}
}

#endif /* _RAW_STREAM_H*/
