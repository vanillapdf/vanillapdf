#ifndef _FLATE_DECODE_FILTER_H
#define _FLATE_DECODE_FILTER_H

#include "BaseObjects/Filter.h"
#include "Buffer.h"

#include "zlib.h"

namespace Pdf
{
	namespace Filters
	{
		class FlateDecodeFilter : Filter
		{
		public:
			FlateDecodeFilter() : Filter(Type::FlateDecode) {}

			virtual Buffer Apply(Buffer& src) const override
			{
				int ret;
				unsigned have;
				z_stream strm;
				unsigned char in[1024];
				unsigned char out[1024];
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

				/* decompress until deflate stream ends or end of file */
				do {
					strm.avail_in = src.Size();
					if (strm.avail_in == 0)
						break;

					//TODO
					strm.next_in = src.Data();

					/* run inflate() on input until output buffer not full */
					do {
						strm.avail_out = 1024;
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
						have = 1024 - strm.avail_out;
						dest.Append(Buffer(reinterpret_cast<const char*>(out), have));
					} while (strm.avail_out == 0);

					/* done when inflate() says it's done */
				} while (ret != Z_STREAM_END);

				/* clean up and return */
				(void)inflateEnd(&strm);
				//return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
			}
		};
	}
}

#endif /* _FLATE_DECODE_FILTER_H */
