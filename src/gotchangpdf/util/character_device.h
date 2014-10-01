#ifndef _CHARACTER_DEVICE_H
#define _CHARACTER_DEVICE_H

#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>

namespace gotchangpdf
{
	namespace io = boost::iostreams;

	typedef std::iostream CharacterDevice;
	typedef io::filtering_stream<io::seekable, char> CharacterFilteringDevice;
}

#endif /* _CHARACTER_DEVICE_H */
