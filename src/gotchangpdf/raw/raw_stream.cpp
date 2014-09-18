#include "raw_stream.h"

namespace gotchangpdf
{
	namespace raw
	{
		Stream::Stream(std::istream& stream) : std::istream(stream.rdbuf()) {}
		Stream::Stream(const raw::Stream &other) : raw::BaseStream(), std::istream(other.rdbuf()) {}

		Stream::~Stream() {}

		void Stream::Read(char* bytes, unsigned int len)
		{
			std::istream::read(bytes, len);
		}

		char* Stream::Read(unsigned int len)
		{
			char *result = new char[len];
			Read(result, len);
			return result;
		}
	}
}
