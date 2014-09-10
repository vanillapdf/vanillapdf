#include "flate_decode_filter.h"

#include "constants.h"
#include "exception.h"

#include <vector>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>

namespace gotchangpdf
{
	namespace filters
	{
		namespace io = boost::iostreams;

		Buffer FlateDecodeFilter::Encode(const Buffer& src) const
		{
			std::vector<char> dest;
			io::filtering_ostream os;

			os.push(io::zlib_compressor());
			os.push(io::back_inserter(dest));

			io::write(os, src.Data(), src.Size());

			return Buffer(dest);
		}

		Buffer FlateDecodeFilter::Decode(const Buffer& src) const
		{
			std::vector<char> dest;
			io::filtering_ostream os;

			os.push(io::zlib_decompressor());
			os.push(io::back_inserter(dest));

			io::write(os, src.Data(), src.Size());

			return Buffer(dest);
		}
	}
}

