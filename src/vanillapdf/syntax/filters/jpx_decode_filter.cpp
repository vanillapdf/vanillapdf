#include "precompiled.h"

#include "syntax/filters/jpx_decode_filter.h"

#include "syntax/utils/image_metadata_object_attribute.h"

#include <openjpeg.h>

namespace vanillapdf {
namespace syntax {

using namespace std;

static void error_callback(const char* msg, void*) {
    spdlog::error("OpenJPEG error: {}", msg);
}

static void warning_callback(const char* msg, void*) {
    spdlog::warn("OpenJPEG error: {}", msg);
}

static void info_callback(const char*, void*) {
    //LOG_INFO_GLOBAL << "OpenJPEG error: " << msg;
}

// Custom stream to read from memory buffer
static OPJ_SIZE_T memory_stream_read(void* p_buffer, OPJ_SIZE_T p_nb_bytes, void* p_user_data) {
    IInputStream* input_stream = static_cast<IInputStream*>(p_user_data);
    auto result = input_stream->Read(static_cast<char*>(p_buffer), p_nb_bytes);

    // We should return (OPJ_SIZE_T) -1 instead of exception according to examples.
    // https://github.com/uclouvain/openjpeg/blob/master/src/lib/openjp2/openjpeg.c
    // Unfortunately it's not mentioned anywhere and I do not have a sample to test,
    // so let's keep the exception behavior as default for SafeConvert.

    return ValueConvertUtils::SafeConvert<OPJ_SIZE_T>(result);
}

static OPJ_OFF_T memory_stream_skip(OPJ_OFF_T p_nb_bytes, void* p_user_data) {
    IInputStream* input_stream = static_cast<IInputStream*>(p_user_data);
    input_stream->SetInputPosition(p_nb_bytes, SeekDirection::Current);
    return input_stream->GetInputPosition();
}

static OPJ_BOOL memory_stream_seek(OPJ_OFF_T p_nb_bytes, void* p_user_data) {
    IInputStream* input_stream = static_cast<IInputStream*>(p_user_data);
    input_stream->SetInputPosition(p_nb_bytes, SeekDirection::Beginning);

    // If we have reached eof return false
    if (input_stream->Eof()) {
        return OPJ_FALSE;
    }

    return OPJ_TRUE;
}

BufferPtr JPXDecodeFilter::Encode(IInputStreamPtr src, types::stream_size, DictionaryObjectPtr /* = DictionaryObjectPtr() */, AttributeListPtr /* = AttributeListPtr() */) const {
    throw NotSupportedException("JPXDecodeFilter encoding is not supported");
}

BufferPtr JPXDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr /* = DictionaryObjectPtr() */, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {

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

    auto length_converted = ValueConvertUtils::SafeConvert<OPJ_SIZE_T>(length);

    // Set up the stream
    opj_stream_t* stream = opj_stream_create(length_converted, OPJ_TRUE);
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

    if (image->color_space == OPJ_CLRSPC_SRGB ||
        image->color_space == OPJ_CLRSPC_GRAY ||
        image->color_space == OPJ_CLRSPC_CMYK) {

        // Use 8 bits per components in the result

        // Calculate the size of the image data
        size_t image_size = image->x1 * image->y1 * image->numcomps;

        std::vector<uint8_t> result;
        result.resize(image_size);

        for (OPJ_UINT32 y = 0; y < image->y1; ++y) {
            for (OPJ_UINT32 x = 0; x < image->x1; ++x) {
                for (OPJ_UINT32 comp = 0; comp < image->numcomps; ++comp) {
                    auto data_offset = (y * image->x1) + x;
                    auto result_offset = (y * image->x1 + x) * image->numcomps + comp;

                    auto current_value = image->comps[comp].data[data_offset];
                    result[result_offset] = ValueConvertUtils::SafeConvert<uint8_t>(current_value);
                }
            }
        }

        ImageMetadataObjectAttribute::ColorSpaceType attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::Undefined;

        if (image->color_space == OPJ_CLRSPC_SRGB) {
            attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::RGB;
        }

        if (image->color_space == OPJ_CLRSPC_GRAY) {
            attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::GRAY;
        }

        if (image->color_space == OPJ_CLRSPC_CMYK) {
            attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::CMYK;
        }

        // Create the attribute to augument the stream object
        auto metadata_attribute = make_deferred<ImageMetadataObjectAttribute>();
        metadata_attribute->SetWidth(image->x1);
        metadata_attribute->SetHeight(image->y1);
        metadata_attribute->SetColorSpace(attribute_color_space);
        metadata_attribute->SetColorComponents(image->numcomps);

        // Associate the attribute with the object
        object_attributes->Add(metadata_attribute);

        // Copy the image data to a new buffer
        // The reason I have not used the Buffer is that it is currently based on signed char
        return make_deferred_container<Buffer>(result.begin(), result.end());
    }

    throw GeneralException("Unknown JPEG2000 color space: " + std::to_string(image->color_space));
}

BufferPtr JPXDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {
    auto stream = src->ToInputStream();
    return Encode(stream, src->size(), parameters, object_attributes);
}

BufferPtr JPXDecodeFilter::Decode(BufferPtr src,  DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {
    auto stream = src->ToInputStream();
    return Decode(stream, src->size(), parameters, object_attributes);
}

} // syntax
} // vanillapdf
