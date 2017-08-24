#include "precompiled.h"

#include "syntax/filters/dct_decode_filter.h"

#include "utils/math_utils.h"

#include <cstring>

#if defined(GOTCHANG_PDF_HAVE_JPEG)
#include <jpeglib.h>
#endif

namespace gotchangpdf {
namespace syntax {

#if defined(GOTCHANG_PDF_HAVE_JPEG)

void error_exit(j_common_ptr ptr) {
	char buffer[JMSG_LENGTH_MAX];
	(*ptr->err->format_message) (ptr, buffer);

	LOG_ERROR_GLOBAL << "JPEG decompression exited with error: " << buffer;
}

void output_message(j_common_ptr ptr) {
	char buffer[JMSG_LENGTH_MAX];
	(*ptr->err->format_message) (ptr, buffer);

	LOG_WARNING_GLOBAL << "JPEG decompression reported message: " << buffer;
}

void init_source(j_decompress_ptr ptr) {
	UNUSED(ptr);
}

boolean fill_input_buffer(j_decompress_ptr ptr) {
	UNUSED(ptr);

	assert(!"This method is not handled");
	LOG_ERROR_GLOBAL << "JPEG decompression failure. Please report this issue";

	return TRUE;
}

void skip_input_data(j_decompress_ptr ptr, long num_bytes) {
	UNUSED(ptr); UNUSED(num_bytes);

	assert(!"This method is not handled");
	LOG_ERROR_GLOBAL << "JPEG decompression failure. Please report this issue";
}

void term_source(j_decompress_ptr ptr) {
	UNUSED(ptr);
}

#endif

BufferPtr DCTDecodeFilter::Encode(std::istream&, types::stream_size, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */) const {
	throw NotSupportedException("DCTDecodeFilter encoding is not supported");
}

BufferPtr DCTDecodeFilter::Decode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */) const {

#if defined(GOTCHANG_PDF_HAVE_JPEG)

	jpeg_decompress_struct jpeg = {0};
	jpeg_error_mgr err = {0};

	jpeg.err = jpeg_std_error(&err);
	err.error_exit = &error_exit;
	err.output_message = &output_message;

	jpeg_create_decompress(&jpeg);

	// We want to capture input by value, because it might be out of scope
	// In order to call non-const method we have to tag the lambda mutable
	auto cleanup_lambda = [jpeg]() mutable {
		jpeg_destroy_decompress(&jpeg);
	};

	SCOPE_GUARD(cleanup_lambda);

	jpeg.src = static_cast<jpeg_source_mgr*>(
		(jpeg.mem->alloc_small)
		(reinterpret_cast<j_common_ptr>(&jpeg), JPOOL_PERMANENT, sizeof(jpeg_source_mgr))
		);

	jpeg.src->init_source = init_source;
	jpeg.src->fill_input_buffer = fill_input_buffer;
	jpeg.src->skip_input_data = skip_input_data;
	jpeg.src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
	jpeg.src->term_source = term_source;

	size_t length_converted = ValueConvertUtils::SafeConvert<size_t>(length);

	BufferPtr input = make_deferred<Buffer>(length_converted);
	src.read(input->data(), length_converted);

	jpeg.src->next_input_byte = reinterpret_cast<unsigned char*>(input->data());
	jpeg.src->bytes_in_buffer = length_converted;

	int header = jpeg_read_header(&jpeg, TRUE);
	if (header != JPEG_HEADER_OK) {
		throw GeneralException("Could not read jpeg header");
	}

	boolean started = jpeg_start_decompress(&jpeg);
	if (started != TRUE) {
		throw GeneralException("Could not start jpeg decompression");
	}

	JDIMENSION row_bytes = SafeMultiply<JDIMENSION, JDIMENSION>(jpeg.output_width, jpeg.output_components);
	JSAMPARRAY jpeg_buffer = (*jpeg.mem->alloc_sarray)(reinterpret_cast<j_common_ptr>(&jpeg), JPOOL_IMAGE, row_bytes, 1);

	BufferPtr result;
	Buffer buffer(row_bytes);

	while (jpeg.output_scanline < jpeg.output_height) {
		JDIMENSION lines = jpeg_read_scanlines(&jpeg, jpeg_buffer, 1);
		assert(1 == lines); UNUSED(lines);

		if (jpeg.output_components == 4) {
			for (uint32_t i = 0; i < jpeg.output_width; i++) {
				uint32_t offset = SafeMultiply<uint32_t, uint32_t>(i, 4);
				assert(offset < buffer.size());

				buffer[offset] = jpeg_buffer[0][offset];
				buffer[offset + 1] = jpeg_buffer[0][offset + 1];
				buffer[offset + 2] = jpeg_buffer[0][offset + 2];
				buffer[offset + 3] = jpeg_buffer[0][offset + 3];
			}
		} else if (jpeg.output_components == 3) {
			for (uint32_t i = 0; i < jpeg.output_width; i++) {
				uint32_t offset = SafeMultiply<uint32_t, uint32_t>(i, 3);
				assert(offset < buffer.size());
				buffer[offset] = jpeg_buffer[0][offset];
				buffer[offset + 1] = jpeg_buffer[0][offset + 1];
				buffer[offset + 2] = jpeg_buffer[0][offset + 2];
			}
		} else if (jpeg.output_components == 1) {
			std::memcpy(buffer.data(), jpeg_buffer[0], jpeg.output_width);
		} else {
			assert(!"Unknown JPEG components");
			LOG_ERROR_GLOBAL << "Unknown JPEG components";
		}

		result->insert(result->end(), buffer.begin(), buffer.end());
	}

	boolean finished = jpeg_finish_decompress(&jpeg);
	if (finished != TRUE) {
		throw GeneralException("Could not finish jpeg decompression");
	}

	return result;

#else
	(void) src; (void) length;
	throw NotSupportedException("This library was compiled without JPEG support");
#endif

}

BufferPtr DCTDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToStringStream();
	return Encode(*stream, src->size());
}

BufferPtr DCTDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToStringStream();
	return Decode(*stream, src->size());
}

} // syntax
} // gotchangpdf
