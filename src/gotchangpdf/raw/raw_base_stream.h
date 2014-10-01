#ifndef _RAW_BASE_STREAM_H
#define _RAW_BASE_STREAM_H

#include "character.h"
#include "buffer.h"

#include <istream>
#include <boost/iostreams/filtering_stream.hpp>

namespace gotchangpdf
{
	namespace raw
	{
		class BaseStream
		{
		public:
			typedef std::istream CharacterSource;
			typedef std::basic_streambuf<char> CharacterSourceBuffer;
			typedef boost::iostreams::filtering_stream<boost::iostreams::input_seekable, char> CharacterFilteringSource;
			typedef boost::iostreams::filtering_streambuf<boost::iostreams::input_seekable, char> CharacterFilteringSourceBuffer;

		public:
			virtual Buffer read(unsigned int len) = 0;
			virtual void read_exact(const Buffer & buf);

			virtual char get_hex(void) = 0;
			virtual Buffer readline(void) = 0;

			virtual ~BaseStream() = 0;
		};
	}
}

#endif /* _RAW_BASE_STREAM_H */
