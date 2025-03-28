#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/utils/name_constants.h"
#include "syntax/filters/dct_decode_filter.h"
#include "syntax/utils/image_metadata_object_attribute.h"

#include "utils/math_utils.h"
#include "utils/streams/input_output_stream.h"
#include "utils/streams/stream_utils.h"

#include <cstring>

#if defined(VANILLAPDF_HAVE_JPEG)
#include <jpeglib.h>
#endif

namespace vanillapdf {
namespace syntax {

#if defined(VANILLAPDF_HAVE_JPEG)

struct CustomDestinationManager {
public:
    explicit CustomDestinationManager(InputOutputStreamPtr input_stream)
        : stream(input_stream) {

    }

    jpeg_destination_mgr jpeg_manager = {0};

    BufferPtr buffer;
    InputOutputStreamPtr stream;
};

void error_exit(j_common_ptr cinfo) {
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);

    spdlog::error("JPEG decompression exited with error: {}", buffer);

    throw GeneralException(buffer);
}

void output_message(j_common_ptr cinfo) {
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);

    spdlog::warn("JPEG decompression reported message: {}", buffer);
}

void init_source(j_decompress_ptr cinfo) {
    UNUSED(cinfo);
}

boolean fill_input_buffer(j_decompress_ptr cinfo) {
    UNUSED(cinfo);

    assert(!"This method is not handled");
    spdlog::error("JPEG decompression failure. Please report this issue");

    return TRUE;
}

void skip_input_data(j_decompress_ptr cinfo, long num_bytes) {

    if (cinfo == nullptr) {
        throw GeneralException("Invalid jpeg pointer");
    }

    if (cinfo->src == nullptr) {
        throw GeneralException("Missing jpeg source manager");
    }

    cinfo->src->next_input_byte += num_bytes;
    cinfo->src->bytes_in_buffer -= num_bytes;
}

void term_source(j_decompress_ptr cinfo) {
    UNUSED(cinfo);
}

boolean resync_to_restart(j_decompress_ptr cinfo, int desired) {
    spdlog::info("JPEG decompression resync restart, desired: {}", desired);
    return jpeg_resync_to_restart(cinfo, desired);
}

void init_destination(j_compress_ptr cinfo) {

    if (cinfo == nullptr) {
        throw GeneralException("Invalid jpeg pointer");
    }

    if (cinfo->dest == nullptr) {
        throw GeneralException("Missing jpeg dest manager");
    }

    CustomDestinationManager* destination = reinterpret_cast<CustomDestinationManager*>(cinfo->dest);
    destination->buffer->resize(constant::BUFFER_SIZE);

    cinfo->dest->next_output_byte = reinterpret_cast<JOCTET*>(destination->buffer->data());
    cinfo->dest->free_in_buffer = destination->buffer->size();
}

boolean empty_output_buffer(j_compress_ptr cinfo) {

    if (cinfo == nullptr) {
        throw GeneralException("Invalid jpeg pointer");
    }

    if (cinfo->dest == nullptr) {
        throw GeneralException("Missing jpeg dest manager");
    }

    CustomDestinationManager* destination = reinterpret_cast<CustomDestinationManager*>(cinfo->dest);
    destination->stream->Write(destination->buffer);

    cinfo->dest->next_output_byte = reinterpret_cast<JOCTET*>(destination->buffer->data());
    cinfo->dest->free_in_buffer = destination->buffer->size();

    return true;
}

void term_destination(j_compress_ptr cinfo) {

    if (cinfo == nullptr) {
        throw GeneralException("Invalid jpeg pointer");
    }

    if (cinfo->dest == nullptr) {
        throw GeneralException("Missing jpeg dest manager");
    }

    CustomDestinationManager* destination = reinterpret_cast<CustomDestinationManager*>(cinfo->dest);

    auto remaining_bytes = destination->buffer->size() - cinfo->dest->free_in_buffer;
    destination->stream->Write(destination->buffer, remaining_bytes);
    destination->stream->Flush();
}

J_COLOR_SPACE GetColorSpace(const NameObject& name) {
    if (name.Equals(constant::Name::RGB) || name.Equals(constant::Name::DeviceRGB)) {
        return JCS_RGB;
    }

    if (name.Equals(constant::Name::G) || name.Equals(constant::Name::DeviceGray)) {
        return JCS_GRAYSCALE;
    }

    if (name.Equals(constant::Name::CMYK) || name.Equals(constant::Name::DeviceCMYK)) {
        return JCS_CMYK;
    }

    return JCS_UNKNOWN;
}

#endif

BufferPtr DCTDecodeFilter::Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {

#if defined(VANILLAPDF_HAVE_JPEG)

    IntegerObjectPtr width;
    IntegerObjectPtr height;
    NameObjectPtr color_space;

    // Check the parameter dictionary for input
    if (parameters->Contains(constant::Name::Width)) {
        width = parameters->FindAs<IntegerObjectPtr>(constant::Name::Width);
    }

    if (parameters->Contains(constant::Name::Height)) {
        height = parameters->FindAs<IntegerObjectPtr>(constant::Name::Height);
    }

    if (parameters->Contains(constant::Name::ColorSpace)) {
        auto color_space_object = parameters->Find(constant::Name::ColorSpace);

        // if name
        if (ObjectUtils::IsType<NameObjectPtr>(color_space_object)) {
            color_space = ObjectUtils::ConvertTo<NameObjectPtr>(color_space_object);
        }

        // if array
        if (ObjectUtils::IsType<MixedArrayObjectPtr>(color_space_object)) {

            auto color_space_array = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(color_space_object);

            spdlog::error("Non-standard color spaces {} are not supported", color_space_array->ToString());

            // TODO: ICCBased, Indexed, Lab, Separation, DeviceN
            throw NotSupportedException("Non-standard colorspaces are not supported");
        }
    }

    // Check additional object attributes as well
    if (object_attributes->Contains(BaseAttribute::Type::ImageMetadata)) {
        auto image_metadata = object_attributes->GetAs<ImageMetadataObjectAttributePtr>(BaseAttribute::Type::ImageMetadata);

        *width = image_metadata->GetWidth();
        *height = image_metadata->GetHeight();

        if (image_metadata->GetColorSpace() == ImageMetadataObjectAttribute::ColorSpaceType::GRAY) {
            *color_space = constant::Name::DeviceGray;
        }

        if (image_metadata->GetColorSpace() == ImageMetadataObjectAttribute::ColorSpaceType::RGB) {
            *color_space = constant::Name::DeviceRGB;
        }

        if (image_metadata->GetColorSpace() == ImageMetadataObjectAttribute::ColorSpaceType::CMYK) {
            *color_space = constant::Name::DeviceCMYK;
        }
    }

    // In case the input parameters are missing, terminate immediately
    if (width.empty()) {
        throw GeneralException("Missing parameter Width");
    }

    if (height.empty()) {
        throw GeneralException("Missing parameter Height");
    }

    if (color_space.empty()) {
        throw GeneralException("Missing parameter ColorSpace");
    }

    jpeg_compress_struct jpeg = { 0 };
    jpeg_error_mgr err = { 0 };

    jpeg.err = jpeg_std_error(&err);
    err.error_exit = &error_exit;
    err.output_message = &output_message;

    jpeg_create_compress(&jpeg);

    SCOPE_GUARD([&jpeg]() { jpeg_destroy_compress(&jpeg); });

    auto output_stream = make_deferred<InputOutputStream>(std::make_shared<std::stringstream>());
    auto destination_manager = std::make_shared<CustomDestinationManager>(output_stream);

    destination_manager->jpeg_manager.init_destination = init_destination;
    destination_manager->jpeg_manager.empty_output_buffer = empty_output_buffer;
    destination_manager->jpeg_manager.term_destination = term_destination;

    jpeg.dest = reinterpret_cast<jpeg_destination_mgr*>(destination_manager.get());

    jpeg.image_width = width->SafeConvert<JDIMENSION>();
    jpeg.image_height = height->SafeConvert<JDIMENSION>();
    jpeg.in_color_space = GetColorSpace(color_space);

    // Number of components can be evaluated from the color space
    // I don't know, how to obtain the number, but calling this
    // function sets the corresponding num_components field.

    jpeg_set_defaults(&jpeg);
    jpeg.input_components = jpeg.num_components;

    jpeg_start_compress(&jpeg, TRUE);

    auto row_size = SafeMultiply<decltype(length), JDIMENSION>(jpeg.image_width, jpeg.input_components);
    auto row_size_converted = ValueConvertUtils::SafeConvert<Buffer::size_type>(row_size);
    BufferPtr buffer = make_deferred_container<Buffer>(row_size_converted);

    decltype(length) read_total = 0;
    while (jpeg.next_scanline < jpeg.image_height) {

        auto read_plus_row = SafeAddition<decltype(length)>(read_total, row_size);
        if (read_plus_row > length) {
            throw GeneralException(
                "Insufficient source data, read_plus_row: " +
                std::to_string(read_plus_row) +
                ", length: " +
                std::to_string(length)
            );
        }

        auto read = src->Read(buffer, row_size);
        if (read != row_size) {
            throw GeneralException(
                "Insufficient source data, read: " +
                std::to_string(read) +
                ", row_size: " +
                std::to_string(row_size)
            );
        }

        JSAMPROW row_pointer[1];
        row_pointer[0] = reinterpret_cast<JSAMPROW>(buffer->data());
        jpeg_write_scanlines(&jpeg, row_pointer, 1);

        read_total = SafeAddition<decltype(length)>(read_total, read);
    }

    jpeg_finish_compress(&jpeg);

    return StreamUtils::InputStreamToBuffer(destination_manager->stream);

#else
    (void) src; (void) length;
    throw NotSupportedException("This library was compiled without JPEG support");
#endif
}

BufferPtr DCTDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {

#if defined(VANILLAPDF_HAVE_JPEG)

    jpeg_decompress_struct jpeg = {0};
    jpeg_error_mgr err = {0};

    jpeg.err = jpeg_std_error(&err);
    err.error_exit = &error_exit;
    err.output_message = &output_message;

    jpeg_create_decompress(&jpeg);

    SCOPE_GUARD([&jpeg]() { jpeg_destroy_decompress(&jpeg); });

    jpeg.src = static_cast<jpeg_source_mgr*>(
        (jpeg.mem->alloc_small)
        (reinterpret_cast<j_common_ptr>(&jpeg), JPOOL_PERMANENT, sizeof(jpeg_source_mgr))
        );

    jpeg.src->init_source = init_source;
    jpeg.src->fill_input_buffer = fill_input_buffer;
    jpeg.src->skip_input_data = skip_input_data;
    jpeg.src->resync_to_restart = resync_to_restart;
    jpeg.src->term_source = term_source;

    size_t length_converted = ValueConvertUtils::SafeConvert<size_t>(length);
    BufferPtr input = src->Read(length_converted);

    jpeg.src->next_input_byte = reinterpret_cast<uint8_t *>(input->data());
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

    InputOutputStreamPtr result = make_deferred<InputOutputStream>(std::make_shared<std::stringstream>());
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
            spdlog::error("Unknown JPEG components");
        }

        result->Write(buffer);
    }

    boolean finished = jpeg_finish_decompress(&jpeg);
    if (finished != TRUE) {
        throw GeneralException("Could not finish jpeg decompression");
    }

    ImageMetadataObjectAttribute::ColorSpaceType attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::Undefined;

    if (jpeg.out_color_space == JCS_GRAYSCALE) {
        attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::GRAY;
    }

    if (jpeg.out_color_space == JCS_RGB) {
        attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::RGB;
    }

    if (jpeg.out_color_space == JCS_CMYK) {
        attribute_color_space = ImageMetadataObjectAttribute::ColorSpaceType::CMYK;
    }

    // Create the attribute to augument the stream object
    auto metadata_attribute = make_deferred<ImageMetadataObjectAttribute>();
    metadata_attribute->SetWidth(jpeg.output_width);
    metadata_attribute->SetHeight(jpeg.output_height);
    metadata_attribute->SetColorSpace(attribute_color_space);
    metadata_attribute->SetColorComponents(jpeg.out_color_components);

    // Associate the attribute with the object
    object_attributes->Add(metadata_attribute);

    return StreamUtils::InputStreamToBuffer(result);

#else
    (void) src; (void) length;
    throw NotSupportedException("This library was compiled without JPEG support");
#endif

}

BufferPtr DCTDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {
    auto stream = src->ToInputStream();
    return Encode(stream, src->size(), parameters, object_attributes);
}

BufferPtr DCTDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {
    auto stream = src->ToInputStream();
    return Decode(stream, src->size(), parameters, object_attributes);
}

} // syntax
} // vanillapdf
