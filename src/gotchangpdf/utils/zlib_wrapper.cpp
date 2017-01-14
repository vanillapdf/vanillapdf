#include "precompiled.h"

#include "utils/zlib_wrapper.h"
#include "utils/exceptions.h"
#include "utils/buffer.h"
#include "utils/util.h"

#if defined(GOTCHANG_PDF_HAVE_ZLIB)
#include <zlib.h>
#endif

#include <cassert>
#include <istream>
#include <algorithm>

namespace gotchangpdf {

static BufferPtr Inflate(std::istream& input, types::stream_size length, types::stream_size errors_after) {

#if defined(GOTCHANG_PDF_HAVE_ZLIB)

	int rv = 0;
	z_stream strm = { 0 };
	BufferPtr result;
	types::stream_size read_total = 0;
	bool expect_errors = (length != errors_after);
	std::vector<uint8_t> in_buffer(constant::BUFFER_SIZE);
	std::vector<uint8_t> out_buffer(constant::BUFFER_SIZE);

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	rv = inflateInit(&strm);
	if (rv != Z_OK) {
		throw GeneralException("Zlib initialization failed");
	}

	/* clean up */
	SCOPE_GUARD_CAPTURE_REFERENCES(inflateEnd(&strm));

	/* decompress until deflate stream ends or end of file */
	do {
		types::stream_size read_size;
		if (expect_errors && read_total >= errors_after) {
			read_size = std::min(static_cast<types::stream_size>(1), length - read_total);
		} else {
			read_size = std::min(length - read_total, static_cast<types::stream_size>(in_buffer.size()));
		}

		input.read(reinterpret_cast<char *>(in_buffer.data()), read_size);
		auto read = input.gcount();
		if (read == 0)
			break;

		read_total += read;
		strm.avail_in = static_cast<uInt>(read);
		strm.next_in = in_buffer.data();

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = constant::BUFFER_SIZE;
			strm.next_out = out_buffer.data();
			rv = inflate(&strm, Z_NO_FLUSH);
			assert(rv != Z_STREAM_ERROR);
			switch (rv) {
				case Z_DATA_ERROR:
					if (expect_errors) return result;
					if (nullptr == strm.msg)
						throw ZlibDataErrorException(result->size());
					else
						throw ZlibDataErrorException(result->size(), std::string(strm.msg));
				case Z_NEED_DICT:
				case Z_MEM_ERROR:
					if (nullptr == strm.msg)
						throw GeneralException("Could not decompress data");
					else
						throw GeneralException("Could not decompress data: " + std::string(strm.msg));
			}

			unsigned int have = constant::BUFFER_SIZE - strm.avail_out;
			result->insert(result.end(), out_buffer.begin(), out_buffer.begin() + have);
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (rv != Z_STREAM_END);

	return result;

#else
	(void) input; (void) length; (void) errors_after;
	throw NotSupportedException("This library is compiled without zlib support");
#endif

}

static BufferPtr Inflate(std::istream& input, types::stream_size length) {
	return Inflate(input, length, length);
}

static BufferPtr Deflate(std::istream& input, types::stream_size length) {

#if defined(GOTCHANG_PDF_HAVE_ZLIB)

	int rv = 0;
	int flush = Z_NO_FLUSH;
	z_stream strm = { 0 };
	BufferPtr result;
	std::vector<uint8_t> in_buffer(constant::BUFFER_SIZE);
	std::vector<uint8_t> out_buffer(constant::BUFFER_SIZE);

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	rv = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if (rv != Z_OK) {
		throw GeneralException("Zlib initialization failed");
	}

	/* clean up */
	SCOPE_GUARD_CAPTURE_REFERENCES(deflateEnd(&strm));

	do {
		input.read(reinterpret_cast<char *>(in_buffer.data()), std::min(length, static_cast<types::stream_size>(constant::BUFFER_SIZE)));
		auto read = input.gcount();
		length -= read;
		strm.avail_in = static_cast<uInt>(read);
		strm.next_in = in_buffer.data();
		flush = (strm.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH;

		/* run deflate() on input until output buffer not full, finish
		compression if all of source has been read in */
		do {
			strm.avail_out = constant::BUFFER_SIZE;
			strm.next_out = out_buffer.data();
			rv = deflate(&strm, flush);
			assert(rv != Z_STREAM_ERROR);
			unsigned int have = constant::BUFFER_SIZE - strm.avail_out;
			result->insert(result.end(), out_buffer.begin(), out_buffer.begin() + have);
		} while (strm.avail_out == 0);

		/* done when last data in file processed */
	} while (rv != Z_STREAM_END);

	return result;

#else
	(void) input; (void) length;
	throw NotSupportedException("This library is compiled without zlib support");
#endif

}

BufferPtr ZlibWrapper::Deflate(std::istream& input, types::stream_size length) {
	return gotchangpdf::Deflate(input, length);
}

BufferPtr ZlibWrapper::Inflate(std::istream& input, types::stream_size length) {
	try {
		return gotchangpdf::Inflate(input, length);
	} catch (ZlibDataErrorException& ex) {
		auto size = ex.Size();
		input.seekg(0, std::ios::beg);
		return gotchangpdf::Inflate(input, length, size);
	}
}

BufferPtr ZlibWrapper::Deflate(const Buffer& input) {
	auto stream = input.ToStringStream();
	return Deflate(stream, input.size());
}

BufferPtr ZlibWrapper::Inflate(const Buffer& input) {
	auto stream = input.ToStringStream();
	return Inflate(stream, input.size());
}

} // gotchangpdf
