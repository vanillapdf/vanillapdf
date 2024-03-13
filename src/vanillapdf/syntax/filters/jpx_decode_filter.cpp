#include "precompiled.h"

#include "syntax/filters/jpx_decode_filter.h"

#include <openjpeg.h>

namespace vanillapdf {
namespace syntax {

using namespace std;

static void error_callback(const char* msg, void*) {
    LOG_ERROR_GLOBAL << "OpenJPEG error: " << msg;
}

static void warning_callback(const char* msg, void*) {
    LOG_WARNING_GLOBAL << "OpenJPEG error: " << msg;
}

static void info_callback(const char*, void*) {
    //LOG_INFO_GLOBAL << "OpenJPEG error: " << msg;
}

// Custom stream to read from memory buffer
static OPJ_SIZE_T memory_stream_read(void* p_buffer, OPJ_SIZE_T p_nb_bytes, void* p_user_data) {
    IInputStream* input_stream = static_cast<IInputStream*>(p_user_data);
    return input_stream->Read(static_cast<char*>(p_buffer), p_nb_bytes);
}

static OPJ_OFF_T memory_stream_skip(OPJ_OFF_T p_nb_bytes, void* p_user_data) {
    IInputStream* input_stream = static_cast<IInputStream*>(p_user_data);
    input_stream->SetInputPosition(p_nb_bytes, SeekDirection::Current);
    return input_stream->GetInputPosition();
}

static OPJ_BOOL memory_stream_seek(OPJ_OFF_T p_nb_bytes, void* p_user_data) {
    IInputStream* input_stream = static_cast<IInputStream*>(p_user_data);
    input_stream->SetInputPosition(p_nb_bytes, SeekDirection::Beginning);

    return OPJ_TRUE;
}

BufferPtr JPXDecodeFilter::Encode(IInputStreamPtr src, types::stream_size, DictionaryObjectPtr /* = DictionaryObjectPtr() */) const {
	throw NotSupportedException("LZWDecodeFilter encoding is not supported");
}

BufferPtr JPXDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr /* = DictionaryObjectPtr() */) const {

    // Set default decoder parameters
    opj_dparameters_t parameters = { 0 };
    opj_set_default_decoder_parameters(&parameters);

    // Initialize the JPEG2000 decoder
    auto codec = opj_create_decompress(OPJ_CODEC_JP2);
    if (codec == nullptr) {
        throw GeneralException("Failed to create JPEG2000 decoder");
    }

    SCOPE_GUARD([codec]() { opj_destroy_codec(codec); });

    // Set error, warning, and information message handlers
    opj_set_info_handler(codec, info_callback, nullptr);
    opj_set_warning_handler(codec, warning_callback, nullptr);
    opj_set_error_handler(codec, error_callback, nullptr);

    // Set up the stream
    opj_stream_t* stream = opj_stream_create(length, OPJ_TRUE);
    if (stream == nullptr) {
        throw GeneralException("Failed to create JPEG2000 stream");
    }

    SCOPE_GUARD([stream]() { opj_stream_destroy(stream); });

    opj_stream_set_user_data(stream, src.get(), nullptr);
    opj_stream_set_user_data_length(stream, length);
    opj_stream_set_read_function(stream, memory_stream_read);
    opj_stream_set_skip_function(stream, memory_stream_skip);
    opj_stream_set_seek_function(stream, memory_stream_seek);

    // Setup the decoder
    bool decoder_setup_result = opj_setup_decoder(codec, &parameters);
    if (!decoder_setup_result) {
        throw GeneralException("Failed to setup JPEG2000 decoder");
    }

    opj_image_t* image = nullptr;
    if (!opj_read_header(stream, codec, &image)) {
        throw GeneralException("Failed to read JPEG2000 header");
    }

    if (!opj_set_decode_area(codec, image, parameters.DA_x0, parameters.DA_y0, parameters.DA_x1, parameters.DA_y1)) {
        throw GeneralException("Failed to decode JPEG2000 area");
    }

    // Decode the image
    if (!opj_decode(codec, stream, image)) {
        throw GeneralException("Failed to decode JPEG2000 image");
    }

    SCOPE_GUARD([image]() { opj_image_destroy(image); });

    // TODO: This code hangs the application in the unit tests
    //if (!opj_end_decompress(codec, stream)) {
    //    throw GeneralException("Failed to end JPEG2000 decompression");
    //}

    // JPEG decompression returned empty results
    if (image->comps == nullptr) {
        throw GeneralException("Received empty image components in JPEG2000 decompression");
    }

    // Calculate the size of the image data
    size_t image_size = image->x1 * image->y1 * image->numcomps;

    std::vector<uint8_t> result;
    result.resize(image_size);

    for (OPJ_UINT32 comp = 0; comp < image->numcomps; ++comp) {
        for (OPJ_UINT32 y = 0; y < image->comps[comp].h; ++y) {
            for (OPJ_UINT32 x = 0; x < image->comps[comp].w; ++x) {
                auto current_value = image->comps[comp].data[y * image->comps[comp].w + x];
                result[(y * image->comps[comp].w + x) * image->numcomps + comp] = ValueConvertUtils::SafeConvert<uint8_t>(current_value);
            }
        }
    }

    // Copy the image data to a new buffer
    // The reason I have not used the Buffer is that it is currently based on signed char
    return make_deferred_container<Buffer>(result.begin(), result.end());
}

BufferPtr JPXDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToInputStream();
	return Encode(stream, src->size(), parameters);
}

BufferPtr JPXDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToInputStream();
	return Decode(stream, src->size(), parameters);
}

} // syntax
} // vanillapdf
