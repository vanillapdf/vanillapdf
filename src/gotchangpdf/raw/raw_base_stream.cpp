#include "raw_base_stream.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace raw
	{
		using namespace exceptions;

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
