#include "precompiled.h"
#include "flate_decode_filter.h"

#include "constants.h"
#include "exception.h"
#include "character_sink.h"

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace gotchangpdf
{
	namespace filters
	{
		namespace io = boost::iostreams;

		BufferPtr FlateDecodeFilter::Encode(BufferPtr src) const
		{
			Buffer dest;
			{
				CharacterFilteringSink sink;

				sink.push(io::zlib_compressor());
				sink.push(io::back_inserter(dest));

				io::write(sink, src->data(), src->size());

				// using block instead of flush
			}

			return dest;
		}

		BufferPtr FlateDecodeFilter::Decode(BufferPtr src) const
		{
			Buffer dest;
			{
				CharacterFilteringSink sink;

				sink.push(io::zlib_decompressor());
				sink.push(io::back_inserter(dest));

				io::write(sink, src->data(), src->size());

				// using block instead of flush
			}

			return dest;
		}
	}
}
