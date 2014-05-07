#ifndef _FLATE_DECODE_FILTER_H
#define _FLATE_DECODE_FILTER_H

#include "Filter.h"
#include "Buffer.h"
#include "constants.h"

#include "zlib.h"

#include <algorithm>

namespace gotchangpdf
{
	namespace filters
	{
		using namespace exceptions;

		class FlateDecodeFilter : Filter
		{
		public:
			FlateDecodeFilter() : Filter(Type::FlateDecode) {}

			virtual Buffer Encode(Buffer& src) const override
			{
				// TODO
				throw Exception("TODO");
			}

			virtual Buffer Decode(Buffer& src) const override
			{
				int ret;
				int size = src.Size();
				int count = size / Constant::BUFFER_SIZE;
				z_stream strm;
				unsigned char out[Constant::BUFFER_SIZE];
				Buffer dest;

				/* allocate inflate state */
				strm.zalloc = Z_NULL;
				strm.zfree = Z_NULL;
				strm.opaque = Z_NULL;
				strm.avail_in = 0;
				strm.next_in = Z_NULL;
				ret = inflateInit(&strm);
				if (ret != Z_OK)
					//return ret;
					throw Exception("TODO");

				for (int i = 0; i < count; ++i)
				{
					strm.avail_in = Constant::BUFFER_SIZE;
					if (strm.avail_in == 0)
						break;

					strm.next_in = reinterpret_cast<unsigned char*>(src.Data(i * Constant::BUFFER_SIZE));

					/* run inflate() on input until output buffer not full */
					do {
						strm.avail_out = Constant::BUFFER_SIZE;
						strm.next_out = out;
						ret = inflate(&strm, Z_NO_FLUSH);
						assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
						switch (ret) {
						case Z_NEED_DICT:
							ret = Z_DATA_ERROR;     /* and fall through */
						case Z_DATA_ERROR:
						case Z_MEM_ERROR:
							(void)inflateEnd(&strm);
							//return ret;
							throw Exception("TODO");
						}
						unsigned have = Constant::BUFFER_SIZE - strm.avail_out;
						dest.Append(Buffer(reinterpret_cast<const char*>(out), have));
					} while (strm.avail_out == 0);
				}

				strm.avail_in = size - count * Constant::BUFFER_SIZE;
				strm.next_in = reinterpret_cast<unsigned char*>(src.Data(count * Constant::BUFFER_SIZE));

				/* run inflate() on input until output buffer not full */
				do {
					strm.avail_out = Constant::BUFFER_SIZE;
					strm.next_out = out;
					ret = inflate(&strm, Z_NO_FLUSH);
					assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
					switch (ret) {
					case Z_NEED_DICT:
						ret = Z_DATA_ERROR;     /* and fall through */
					case Z_DATA_ERROR:
					case Z_MEM_ERROR:
						(void)inflateEnd(&strm);
						//return ret;
						throw Exception("TODO");
					}
					unsigned have = Constant::BUFFER_SIZE - strm.avail_out;
					dest.Append(Buffer(reinterpret_cast<const char*>(out), have));
				} while (strm.avail_out == 0);

				/* clean up and return */
				(void)inflateEnd(&strm);
				//return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;

				return dest;
			}
		};
	}
}

#endif /* _FLATE_DECODE_FILTER_H */
