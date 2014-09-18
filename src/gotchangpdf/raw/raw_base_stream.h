#ifndef _RAW_BASE_STREAM_H
#define _RAW_BASE_STREAM_H

#include "constants.h"

#include <memory>

namespace gotchangpdf
{
	namespace raw
	{
		class BaseStream
		{
		public:

			virtual char* Read(unsigned int len) = 0;
			virtual void Read(char *buf, unsigned int len) = 0;
			virtual void ReadExact(const char* bytes, unsigned int len);
			virtual ~BaseStream() = 0;
		};
	}
}

#endif /* _RAW_BASE_STREAM_H */
