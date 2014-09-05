#ifndef _FLATE_DECODE_FILTER_H
#define _FLATE_DECODE_FILTER_H

#include "Filter.h"
#include "Buffer.h"
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
		using namespace exceptions;
		namespace io = boost::iostreams;

		class FlateDecodeFilter : Filter
		{
		public:
			FlateDecodeFilter() : Filter(Type::FlateDecode) {}

			virtual Buffer Encode(const Buffer& src) const override
			{
				std::vector<char> dest;
				io::filtering_ostream os;

				os.push(io::zlib_compressor());
				os.push(io::back_inserter(dest));

				io::write(os, src.Data(), src.Size());

				return Buffer(dest);
			}

			virtual Buffer Decode(const Buffer& src) const override
			{
				std::vector<char> dest;
				io::filtering_ostream os;

				os.push(io::zlib_decompressor());
				os.push(io::back_inserter(dest));

				io::write(os, src.Data(), src.Size());

				return Buffer(dest);
			}
		};
	}
}

#endif /* _FLATE_DECODE_FILTER_H */
