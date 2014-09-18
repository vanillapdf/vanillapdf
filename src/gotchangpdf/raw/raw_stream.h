#ifndef _RAW_STREAM_H
#define _RAW_STREAM_H

#include "raw_base_stream.h"

#include <istream>

namespace gotchangpdf
{
	namespace raw
	{
		class Stream : public raw::BaseStream, public std::istream
		{
		public:
			explicit Stream(std::istream& stream);
			Stream(const Stream &);

			virtual void Read(char* bytes, unsigned int len) override;
			virtual char* Read(unsigned int len) override;

			virtual ~Stream();
		};
	}
}

#endif /* _RAW_STREAM_H*/
