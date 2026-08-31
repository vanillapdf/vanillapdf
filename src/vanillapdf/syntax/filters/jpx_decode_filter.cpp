#include "precompiled.h"

#include "syntax/filters/jpx_decode_filter.h"
#include "syntax/utils/image_metadata_object_attribute.h"

#include "utils/math_utils.h"

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

    // https://github.com/uclouvain/openjpeg/blob/master/src/lib/openjp2/openjpeg.c
    // opj_stream_read_data calls this function until it has the requested amount of
    // data or the source reports the end through (OPJ_SIZE_T) -1. A zero is not that
    // report, it only says this attempt made no progress, so returning it at the end
    // of the data spins the loop forever. A truncated JP2 carrying nothing beyond its
    // twelve byte signature box was enough to hang the decoder.
    if (result <= 0) {
        return static_cast<OPJ_SIZE_T>(-1);
    }

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
    opj_dparameters_t parameters = { };
    opj_set_default_decoder_parameters(&parameters);

    // Initialize the JPEG2000 decoder
    auto codec = opj_create_decompress(OPJ_CODEC_JP2);
    if (codec == nullptr) {
        throw ImageCodecErrorException("Failed to create JPEG2000 decoder");
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
        throw ImageCodecErrorException("Failed to create JPEG2000 stream");
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
        throw ImageCodecErrorException("Failed to setup JPEG2000 decoder");
    }

    opj_image_t* image = nullptr;
    auto read_header_result = opj_read_header(stream, codec, &image);

    // The image belongs to the caller from the moment opj_read_header returns, and
    // every step below it can still fail on a malformed codestream. The guard is
    // therefore installed on the returned pointer rather than on a decoded image,
    // otherwise each rejected input leaks one image. opj_read_header leaves a null
    // pointer behind when it fails, which opj_image_destroy accepts.
    SCOPE_GUARD([image]() { opj_image_destroy(image); });

    if (!read_header_result) {
        throw ImageCodecErrorException("Failed to read JPEG2000 header");
    }

    // The header states the grid before anything is decoded. Checking it here rather
    // than next to the allocation below matters, because opj_decode fills a sample
    // buffer per component first, and those are four bytes per sample. By the time the
    // decoded image is copied out, the memory a rejected size was meant to save has
    // already been taken.
    auto declared_row_size = SafeMultiply<size_t, OPJ_UINT32>(image->x1, image->numcomps);
    auto declared_size = SafeMultiply<size_t, size_t>(declared_row_size, image->y1);

    if (declared_size > constant::MAX_IMAGE_SIZE) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Declared JPEG2000 image of {} bytes ({}x{}, {} components) exceeds the {} byte limit",
            declared_size, image->x1, image->y1, image->numcomps, constant::MAX_IMAGE_SIZE);
    }

    if (!opj_set_decode_area(codec, image, parameters.DA_x0, parameters.DA_y0, parameters.DA_x1, parameters.DA_y1)) {
        throw ImageCodecErrorException("Failed to decode JPEG2000 area");
    }

    // Decode the image
    if (!opj_decode(codec, stream, image)) {
        throw ImageCodecErrorException("Failed to decode JPEG2000 image");
    }

    // TODO: This code hangs the application in the unit tests
    //if (!opj_end_decompress(codec, stream)) {
    //    throw GeneralException("Failed to end JPEG2000 decompression");
    //}

    // JPEG decompression returned empty results
    if (image->comps == nullptr) {
        throw ImageCodecErrorException("Received empty image components in JPEG2000 decompression");
    }

    if (image->color_space == OPJ_CLRSPC_SRGB ||
        image->color_space == OPJ_CLRSPC_GRAY ||
        image->color_space == OPJ_CLRSPC_CMYK) {

        // Use 8 bits per components in the result

        // ISO/IEC 15444-1, B.2 Image and tile size
        // The image sits on a reference grid between two corners, so its dimensions
        // are the distance between them rather than the far corner on its own.
        if (image->x1 <= image->x0 || image->y1 <= image->y0) {
            LOG_ERROR_AND_THROW(ImageCodecErrorException, "JPEG2000 image covers an empty region [{}, {}] - [{}, {}]",
                image->x0, image->y0, image->x1, image->y1);
        }

        OPJ_UINT32 width = image->x1 - image->x0;
        OPJ_UINT32 height = image->y1 - image->y0;

        // ISO/IEC 15444-1, Table A.11
        // Every component declares its own subsampling, so its sample grid can be
        // smaller than the image grid. Addressing such a component through the image
        // dimensions reads past the end of its samples, therefore only components
        // covering the whole grid are decoded.
        for (OPJ_UINT32 comp = 0; comp < image->numcomps; ++comp) {
            const opj_image_comp_t& component = image->comps[comp];

            if (component.data == nullptr) {
                LOG_ERROR_AND_THROW(ImageCodecErrorException, "JPEG2000 component {} carries no decoded samples", comp);
            }

            if (component.w != width || component.h != height) {
                LOG_ERROR_AND_THROW(NotSupportedException, "Subsampled JPEG2000 component {} of size {}x{} in a {}x{} image is not supported",
                    comp, component.w, component.h, width, height);
            }
        }

        // Safely multiply rowsize first, to avoid any overflows
        auto row_size = SafeMultiply<size_t, OPJ_UINT32>(width, image->numcomps);

        // Rowsize multiplied number of rows is the size of the image data
        auto image_size = SafeMultiply<size_t, size_t>(row_size, height);

        std::vector<uint8_t> result;
        result.resize(image_size);

        // Both offsets are products of the image dimensions, which no longer fit the
        // type those dimensions are declared in, so the arithmetic is checked rather
        // than silently truncated. The row and the pixel are computed once per turn
        // of their own loop, the innermost step only adds the component index.
        for (OPJ_UINT32 y = 0; y < height; ++y) {
            auto row_offset = SafeMultiply<size_t, OPJ_UINT32>(y, width);

            for (OPJ_UINT32 x = 0; x < width; ++x) {
                auto data_offset = SafeAddition<size_t, size_t, OPJ_UINT32>(row_offset, x);
                auto component_base = SafeMultiply<size_t, size_t>(data_offset, image->numcomps);

                for (OPJ_UINT32 comp = 0; comp < image->numcomps; ++comp) {
                    auto result_offset = SafeAddition<size_t, size_t, OPJ_UINT32>(component_base, comp);

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

    throw ImageCodecErrorException("Unknown JPEG2000 color space: " + std::to_string(image->color_space));
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
