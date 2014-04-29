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

			std::shared_ptr<Filter> BaseStream::GetFilter(void) const { return _filter; }
			void BaseStream::SetFilter(std::unique_ptr<Filter> filter) { _filter = std::move(filter); }
		}
	}
}