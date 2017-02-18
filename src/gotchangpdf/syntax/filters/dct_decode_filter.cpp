#include "precompiled.h"
#include "dct_decode_filter.h"

#include <cstring>

#if defined(GOTCHANG_PDF_HAVE_JPEG)
#include <jpeglib.h>

namespace gotchangpdf {
namespace syntax {

void JPegErrorExit(j_common_ptr ptr) {
	ptr;
	char buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	(*ptr->err->format_message) (ptr, buffer);

	jpeg_destroy(ptr);
}

void JPegErrorOutput(j_common_ptr ptr, int val) {
	ptr; val;
	char buffer[JMSG_LENGTH_MAX];
	(*ptr->err->format_message) (ptr, buffer);
}

void init_source(j_decompress_ptr ptr) {
	ptr;
}

boolean fill_input_buffer(j_decompress_ptr ptr) {
	ptr;
	return TRUE;
}

void skip_input_data(j_decompress_ptr ptr, long num_bytes) {
	ptr; num_bytes;
}

void term_source(j_decompress_ptr ptr) {
	ptr;
}

#endif

BufferPtr DCTDecodeFilter::Encode(std::istream&, types::stream_size, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */) const {
	throw NotSupportedException("DCTDecodeFilter encoding is not supported");
}

BufferPtr DCTDecodeFilter::Decode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */) const {

#if defined(GOTCHANG_PDF_HAVE_JPEG)

	BufferPtr result;

	jpeg_decompress_struct jpeg;
	jpeg_error_mgr err;

	jpeg.err = jpeg_std_error(&err);
	err.error_exit = &JPegErrorExit;
	err.emit_message = &JPegErrorOutput;

	jpeg_create_decompress(&jpeg);

	jpeg.src = static_cast<jpeg_source_mgr*>(
		(jpeg.mem->alloc_small)
		(reinterpret_cast<j_common_ptr>(&jpeg), JPOOL_PERMANENT, sizeof(jpeg_source_mgr))
		);

	jpeg.src->init_source = init_source;
	jpeg.src->fill_input_buffer = fill_input_buffer;
	jpeg.src->skip_input_data = skip_input_data;
	jpeg.src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
	jpeg.src->term_source = term_source;

	BufferPtr input(static_cast<size_t>(length));
	src.read(input->data(), length);

	jpeg.src->next_input_byte = reinterpret_cast<unsigned char*>(input->data());
	jpeg.src->bytes_in_buffer = static_cast<size_t>(length);

	jpeg_read_header(&jpeg, TRUE);
	jpeg_start_decompress(&jpeg);

	BufferPtr  buffer;
	JSAMPARRAY pBuffer;
	long       lRowBytes = jpeg.output_width * jpeg.output_components;
	const int  iComponents = jpeg.output_components;

	// pBuffer will be deleted by jpeg_destroy_decompress
	pBuffer = (*jpeg.mem->alloc_sarray)(reinterpret_cast<j_common_ptr>(&jpeg), JPOOL_IMAGE, 1500, 1);
	buffer->resize(lRowBytes);

	while (jpeg.output_scanline < jpeg.output_height) {
		int lines = jpeg_read_scanlines(&jpeg, pBuffer, 1);
		assert(1 == lines); lines;
		if (iComponents == 4) {
			for (unsigned int i = 0, c = 0; i < jpeg.output_width; i++, c += 4) {
				buffer[c] = pBuffer[0][i * 4];
				buffer[c + 1] = pBuffer[0][i * 4 + 1];
				buffer[c + 2] = pBuffer[0][i * 4 + 2];
				buffer[c + 3] = pBuffer[0][i * 4 + 3];
			}
		}

		if (iComponents == 3) {
			for (unsigned int i = 0, c = 0; i < jpeg.output_width; i++, c += 3) {
				buffer[c] = pBuffer[0][i * 3];
				buffer[c + 1] = pBuffer[0][i * 3 + 1];
				buffer[c + 2] = pBuffer[0][i * 3 + 2];
			}
		}

		if (iComponents == 1) {
			std::memcpy(buffer->data(), pBuffer[0], jpeg.output_width);
		} else {
		}

		result->insert(result->end(), buffer->begin(), buffer->end());
	}


	jpeg_finish_decompress(&jpeg);
	jpeg_destroy_decompress(&jpeg);

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
