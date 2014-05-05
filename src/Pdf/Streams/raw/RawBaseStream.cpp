#include "streams/raw/RawBaseStream.h"
#include "Exceptions/Exception.h"

namespace Pdf
{
	namespace Streams
	{		
		namespace Raw
		{
			BaseStream::~BaseStream() {}

			void BaseStream::ReadExact(const char* bytes, unsigned int len)
			{
				auto buf = Read(len);

				for (size_t i = 0; i < len; ++i)
					if (bytes[i] != buf.get()[i])
						throw Exception("Stream bytes is not equal to input");
			}
		}
	}
}
