#include "precompiled.h"
#include "raw_base_stream.h"

#include "exception.h"

namespace gotchangpdf
{
	namespace syntax
	{
		BaseStream::~BaseStream() {}

		void BaseStream::read_exact(BufferPtr buf)
		{
			auto cmp = read(buf->size());
			if (!buf->Equals(*cmp))
				throw Exception("Stream bytes is not equal to input");
		}
	}
}
