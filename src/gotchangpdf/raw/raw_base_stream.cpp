#include "raw_base_stream.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace raw
	{
		using namespace exceptions;

		BaseStream::~BaseStream() {}

		void BaseStream::read_exact(const Buffer & buf)
		{
			auto cmp = read(buf.size());
			if (buf != cmp)
				throw Exception("Stream bytes is not equal to input");
		}
	}
}
