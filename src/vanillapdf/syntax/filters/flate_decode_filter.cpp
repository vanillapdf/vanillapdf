#include "precompiled.h"

#include "syntax/filters/flate_decode_filter.h"

#include "syntax/utils/name_constants.h"

#include "utils/constants.h"
#include "utils/zlib_wrapper.h"
#include "utils/math_utils.h"

#include <cmath>

namespace vanillapdf {
namespace syntax {

// https://www.w3.org/TR/PNG-Filters.html
enum PNGFilterTypes {
    None = 0,
    Sub = 1,
    Up = 2,
    Average = 3,
    Paeth = 4
};

BufferPtr FlateDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr /* = AttributeListPtr() */) const {
    return ZlibWrapper::Deflate(src);
}

BufferPtr FlateDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters, AttributeListPtr /* = AttributeListPtr() */) const {
    auto dest = ZlibWrapper::Inflate(src);
    return ApplyPredictor(dest, parameters);
}

BufferPtr FlateDecodeFilter::Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters, AttributeListPtr /* = AttributeListPtr() */) const {
    return ZlibWrapper::Deflate(src, length);
}

BufferPtr FlateDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters, AttributeListPtr /*= AttributeListPtr() */) const {
    auto dest = ZlibWrapper::Inflate(src, length);
    return ApplyPredictor(dest, parameters);
}

BufferPtr FlateDecodeFilter::ApplyPredictor(BufferPtr src, DictionaryObjectPtr parameters) const {
    auto stream = src->ToInputStream();
    return ApplyPredictor(stream, src->size(), parameters);
}

BufferPtr FlateDecodeFilter::ApplyPredictor(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters) const {
    IntegerObjectPtr predictor = make_deferred<IntegerObject>(1);
    if (parameters->Contains(constant::Name::Predictor)) {
        predictor = parameters->FindAs<IntegerObjectPtr>(constant::Name::Predictor);
        assert(*predictor == 1 || *predictor == 2 || (*predictor >= 10 && *predictor <= 15));
    }

    // No prediction was used
    if (predictor == 1) {
        auto length_converted = ValueConvertUtils::SafeConvert<types::size_type>(length);
        return src->Read(length_converted);
    }

    IntegerObjectPtr colors = make_deferred<IntegerObject>(1);
    if (parameters->Contains(constant::Name::Colors)) {
        colors = parameters->FindAs<IntegerObjectPtr>(constant::Name::Colors);
        assert(*colors >= 1);
    }

    syntax::IntegerObjectPtr bits = make_deferred<IntegerObject>(8);
    if (parameters->Contains(constant::Name::BitsPerComponent)) {
        bits = parameters->FindAs<IntegerObjectPtr>(constant::Name::BitsPerComponent);
        // PDF 32000-1 Table 8: valid values are 1, 2, 4, 8 and (PDF 1.5) 16
        assert(*bits == 1 || *bits == 2 || *bits == 4 || *bits == 8 || *bits == 16);
    }

    IntegerObjectPtr columns = make_deferred<IntegerObject>(1);
    if (parameters->Contains(constant::Name::Columns)) {
        columns = parameters->FindAs<IntegerObjectPtr>(constant::Name::Columns);
    }

    IntegerObjectPtr change = make_deferred<IntegerObject>(1);
    if (parameters->Contains(constant::Name::EarlyChange)) {
        change = parameters->FindAs<IntegerObjectPtr>(constant::Name::EarlyChange);
    }

    if (*predictor == 2) {
        throw NotSupportedException("TIFF predictor is currently not supported");
    } else if (*predictor < 10) {
        throw GeneralException("Unknown predictor type");
    }

    uint32_t colors_int = colors->SafeConvert<uint32_t>();
    uint32_t columns_int = columns->SafeConvert<uint32_t>();
    uint32_t bits_int = bits->SafeConvert<uint32_t>();

    // PNG bytes-per-pixel is the pixel bit depth rounded UP to whole bytes
    // (min 1); flooring yields 0 for sub-byte depths and corrupts output.
    // Division by 8 cannot overflow, as in bytes_per_row below.
    // https://github.com/vanillapdf/vanillapdf/issues/443
    uint32_t pixel_bits = SafeMultiply<uint32_t>(colors_int, bits_int);
    uint32_t bytes_per_pixel = SafeAddition<uint32_t>(pixel_bits, 7) / 8;

    uint32_t colors_columns = SafeMultiply<uint32_t>(colors_int, columns_int);
    uint32_t colors_columns_bits = SafeMultiply<uint32_t>(colors_columns, bits_int);
    uint32_t bytes_per_row = SafeAddition<uint32_t>(colors_columns_bits, 7) / 8;

    BufferPtr result;
    std::vector<uint8_t> current(bytes_per_row);
    std::vector<uint8_t> prior(bytes_per_row);

    while (src->Peek() != std::char_traits<char>::eof()) {
        auto filter = src->Get();
        auto read = src->Read(reinterpret_cast<char*>(current.data()), bytes_per_row);

        // A short read means a truncated final scanline, which is reachable
        // corrupt input (not a programming bug), so throw rather than assert.
        // https://github.com/vanillapdf/vanillapdf/issues/442
        if (read != bytes_per_row) {
            LOG_ERROR_AND_THROW_GENERAL("Corrupted deflate compressed data: read {} of {} scanline bytes", read, bytes_per_row);
        }

        switch (filter) {
            case PNGFilterTypes::None:
                break;
            case PNGFilterTypes::Sub:
                assert(bytes_per_row <= current.size());
                for (uint32_t i = 0; (bytes_per_pixel + i) < bytes_per_row; i++) {
                    current[bytes_per_pixel + i] += current[i];
                }

                break;
            case PNGFilterTypes::Up:
                assert(bytes_per_row <= prior.size());
                assert(bytes_per_row <= current.size());
                for (uint32_t i = 0; i < bytes_per_row; i++) {
                    current[i] += prior[i];
                }

                break;
            case PNGFilterTypes::Average:
                assert(bytes_per_pixel <= prior.size());
                assert(bytes_per_pixel <= current.size());
                for (uint32_t i = 0; i < bytes_per_pixel; i++) {
                    current[i] += (prior[i] / 2);
                }

                assert(bytes_per_row <= prior.size());
                assert(bytes_per_row <= current.size());
                for (uint32_t i = 0; (bytes_per_pixel + i) < bytes_per_row; i++) {
                    uint8_t current_byte = current[i] & 0xFF;
                    uint8_t prior_byte = prior[bytes_per_pixel + i] & 0xFF;

                    // Note:
                    // There used to be safe addition, however this addition is allowed to overflow
                    uint32_t added_value = (current_byte + prior_byte);
                    auto divided_value = (added_value / 2.0f);
                    auto rounded_value = std::floor(divided_value);
                    current[bytes_per_pixel + i] += static_cast<uint8_t>(rounded_value);
                }

                break;
            case PNGFilterTypes::Paeth:
                assert(bytes_per_pixel <= prior.size());
                assert(bytes_per_pixel <= current.size());
                for (uint32_t i = 0; i < bytes_per_pixel; i++) {
                    current[i] += prior[i];
                }

                assert(bytes_per_row <= prior.size());
                assert(bytes_per_row <= current.size());
                for (uint32_t i = 0; (bytes_per_pixel + i) < bytes_per_row; i++) {
                    uint8_t a = current[i] & 0xFF;
                    uint8_t b = prior[bytes_per_pixel + i] & 0xFF;
                    uint8_t c = prior[i] & 0xFF;

                    int32_t p = a + b - c;
                    int32_t pa = std::abs(p - a);
                    int32_t pb = std::abs(p - b);
                    int32_t pc = std::abs(p - c);

                    uint8_t value;
                    if ((pa <= pb) && (pa <= pc)) {
                        value = a;
                    } else if (pb <= pc) {
                        value = b;
                    } else {
                        value = c;
                    }

                    current[bytes_per_pixel + i] += value;
                }

                break;
            default:
                // A valid PDF never carries a PNG filter type outside 0-4; fail
                // closed instead of appending the raw row and returning corrupt data.
                // https://github.com/vanillapdf/vanillapdf/issues/442
                LOG_ERROR_AND_THROW_GENERAL("Unknown PNG predictor filter type: {}", filter);
        }

        result->insert(result.end(), current.begin(), current.end());
        prior.assign(current.begin(), current.end());
    }

    return result;
}

} // syntax
} // vanillapdf
