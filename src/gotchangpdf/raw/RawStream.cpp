#include "RawStream.h"

namespace gotchangpdf
{
	namespace raw
	{
		using namespace std;

		Stream::Stream(std::istream& stream) : std::istream(stream.rdbuf()) {}
		Stream::Stream(const raw::Stream &other) : raw::BaseStream(), std::istream(other.rdbuf()) {}

		Stream::~Stream() {}

		void Stream::Read(char* bytes, unsigned int len)
		{
			std::istream::read(bytes, len);
		}

		unique_ptr<char> Stream::Read(unsigned int len)
		{
			unique_ptr<char> result(new char[len]);

			/* unique_ptr handles memory in case of exception,
			otherwise try-catch would needed to be involved */
			char *hack = result.get();

			Read(hack, len);

			return result;
		}
	}
}
