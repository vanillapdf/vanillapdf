#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/utils/name_constants.h"
#include "syntax/filters/dct_decode_filter.h"
#include "syntax/utils/image_metadata_object_attribute.h"

#include "utils/math_utils.h"
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
    explicit CustomDestinationManager(IInputOutputStreamPtr input_stream)
        : stream(input_stream) {

    }

    jpeg_destination_mgr jpeg_manager = { };

    BufferPtr buffer;
    IInputOutputStreamPtr stream;
};

void error_exit(j_common_ptr cinfo) {
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);

    LOG_ERROR_AND_THROW(ImageCodecErrorException, "JPEG decompression exited with error: {}", buffer);
}

void output_message(j_common_ptr cinfo) {
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);

    spdlog::warn("JPEG decompression reported message: {}", buffer);
}

void init_destination(j_compress_ptr cinfo) {

    if (cinfo == nullptr) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Invalid jpeg pointer");
    }

    if (cinfo->dest == nullptr) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Missing jpeg dest manager");
    }

    CustomDestinationManager* destination = reinterpret_cast<CustomDestinationManager*>(cinfo->dest);
    destination->buffer->resize(constant::BUFFER_SIZE);

    cinfo->dest->next_output_byte = reinterpret_cast<JOCTET*>(destination->buffer->data());
    cinfo->dest->free_in_buffer = destination->buffer->size();
}

boolean empty_output_buffer(j_compress_ptr cinfo) {

    if (cinfo == nullptr) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Invalid jpeg pointer");
    }

    if (cinfo->dest == nullptr) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Missing jpeg dest manager");
    }

    CustomDestinationManager* destination = reinterpret_cast<CustomDestinationManager*>(cinfo->dest);
    destination->stream->Write(destination->buffer);

    cinfo->dest->next_output_byte = reinterpret_cast<JOCTET*>(destination->buffer->data());
    cinfo->dest->free_in_buffer = destination->buffer->size();

    return true;
}

void term_destination(j_compress_ptr cinfo) {

    if (cinfo == nullptr) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Invalid jpeg pointer");
    }

    if (cinfo->dest == nullptr) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Missing jpeg dest manager");
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
            LOG_ERROR_AND_THROW(NotSupportedException, "Non-standard colorspaces are not supported");
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
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Missing parameter Width");
    }

    if (height.empty()) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Missing parameter Height");
    }

    if (color_space.empty()) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Missing parameter ColorSpace");
    }

    jpeg_compress_struct jpeg = { };
    jpeg_error_mgr err = { };

    jpeg.err = jpeg_std_error(&err);
    err.error_exit = &error_exit;
    err.output_message = &output_message;

    jpeg_create_compress(&jpeg);

    SCOPE_GUARD([&jpeg]() { jpeg_destroy_compress(&jpeg); });

    auto output_stream = StreamUtils::InputOutputStreamFromMemory();
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
            LOG_ERROR_AND_THROW(ImageCodecErrorException, "Insufficient source data, read_plus_row: {}, length: {}", read_plus_row, length);
        }

        auto read = src->Read(buffer, row_size);
        if (read != row_size) {
            LOG_ERROR_AND_THROW(ImageCodecErrorException, "Insufficient source data, read: {}, row_size: {}", read, row_size);
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
    LOG_ERROR_AND_THROW(NotSupportedException, "This library was compiled without JPEG support");
#endif
}

// Optimized: pre-allocated output buffer with direct scanline decode (zero-copy).
// Benchmark (Release, MSVC 17, x64):
//
// Before:
// ┌────────────────────────────────┬───────────┬─────────┬────────────┐
// │           Benchmark            │ Mean Time │   CPU   │ Iterations │
// ├────────────────────────────────┼───────────┼─────────┼────────────┤
// │ BM_DCTDecode_Small  (92x144)   │ 105 us    │ 100 us  │ 4978       │
// ├────────────────────────────────┼───────────┼─────────┼────────────┤
// │ BM_DCTDecode_Medium (640x480)  │ 2.40 ms   │ 2.40 ms │ 299        │
// ├────────────────────────────────┼───────────┼─────────┼────────────┤
// │ BM_DCTDecode_Large  (1920x1080)│ 19.2 ms   │ 18.2 ms │ 37         │
// └────────────────────────────────┴───────────┴─────────┴────────────┘
//
// After:
// ┌────────────────────────────────┬───────────┬─────────┬────────────┐
// │           Benchmark            │ Mean Time │   CPU   │ Iterations │
// ├────────────────────────────────┼───────────┼─────────┼────────────┤
// │ BM_DCTDecode_Small  (92x144)   │  32.3 us  │ 32.2 us │ 21816      │
// ├────────────────────────────────┼───────────┼─────────┼────────────┤
// │ BM_DCTDecode_Medium (640x480)  │ 567 us    │ 578 us  │ 1000       │
// ├────────────────────────────────┼───────────┼─────────┼────────────┤
// │ BM_DCTDecode_Large  (1920x1080)│ 4.33 ms   │ 4.33 ms │ 166        │
// └────────────────────────────────┴───────────┴─────────┴────────────┘
BufferPtr DCTDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */, AttributeListPtr object_attributes /* = AttributeListPtr() */) const {

#if defined(VANILLAPDF_HAVE_JPEG)

    jpeg_decompress_struct jpeg = { };
    jpeg_error_mgr err = { };

    jpeg.err = jpeg_std_error(&err);
    err.error_exit = &error_exit;
    err.output_message = &output_message;

    jpeg_create_decompress(&jpeg);

    SCOPE_GUARD([&jpeg]() { jpeg_destroy_decompress(&jpeg); });

    size_t length_converted = ValueConvertUtils::SafeConvert<size_t>(length);
    BufferPtr input = src->Read(length_converted);

    jpeg_mem_src(&jpeg,
        reinterpret_cast<const unsigned char*>(input->data()),
        static_cast<unsigned long>(length_converted));

    int header = jpeg_read_header(&jpeg, TRUE);
    if (header != JPEG_HEADER_OK) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Could not read jpeg header");
    }

    boolean started = jpeg_start_decompress(&jpeg);
    if (started != TRUE) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Could not start jpeg decompression");
    }

    // Pre-allocate output buffer for the entire decoded image
    JDIMENSION row_bytes = SafeMultiply<JDIMENSION, JDIMENSION>(jpeg.output_width, jpeg.output_components);
    auto total_size = SafeMultiply<size_t, JDIMENSION>(row_bytes, jpeg.output_height);
    BufferPtr result = make_deferred_container<Buffer>(total_size);

    while (jpeg.output_scanline < jpeg.output_height) {
        // Decode directly into the output buffer at the correct offset
        auto offset = SafeMultiply<size_t, JDIMENSION>(jpeg.output_scanline, row_bytes);
        JSAMPROW row_pointer = reinterpret_cast<JSAMPROW>(result->data() + offset);
        JDIMENSION lines = jpeg_read_scanlines(&jpeg, &row_pointer, 1);
        assert(1 == lines); UNUSED(lines);
    }

    boolean finished = jpeg_finish_decompress(&jpeg);
    if (finished != TRUE) {
        LOG_ERROR_AND_THROW(ImageCodecErrorException, "Could not finish jpeg decompression");
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

    return result;

#else
    (void) src; (void) length;
    LOG_ERROR_AND_THROW(NotSupportedException, "This library was compiled without JPEG support");
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
