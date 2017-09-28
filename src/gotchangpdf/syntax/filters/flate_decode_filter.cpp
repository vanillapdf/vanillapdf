#include "precompiled.h"

#include "syntax/filters/flate_decode_filter.h"

#include "utils/constants.h"
#include "utils/zlib_wrapper.h"
#include "utils/math_utils.h"

namespace gotchangpdf {
namespace syntax {

// https://www.w3.org/TR/PNG-Filters.html
enum PNGFilterTypes {
	None = 0,
	Sub = 1,
	Up = 2,
	Average = 3,
	Paeth = 4
};

BufferPtr FlateDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const {
	return ZlibWrapper::Deflate(src);
}

BufferPtr FlateDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto dest = ZlibWrapper::Inflate(src);
	return ApplyPredictor(dest, parameters);
}

BufferPtr FlateDecodeFilter::Encode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters) const {
	return ZlibWrapper::Deflate(src, length);
}

BufferPtr FlateDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters) const {
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
		assert(*bits == 1 || *bits == 2 || *bits == 4 || *bits == 8);
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

	// Division should be safe?
	uint32_t bytes_per_pixel = SafeMultiply<uint32_t>(colors_int, bits_int) / 8;

	uint32_t colors_columns = SafeMultiply<uint32_t>(colors_int, columns_int);
	uint32_t colors_columns_bits = SafeMultiply<uint32_t>(colors_columns, bits_int);
	uint32_t bytes_per_row = SafeAddition<uint32_t>(colors_columns_bits, 7) / 8;

	BufferPtr result;
	Buffer current(bytes_per_row);
	Buffer prior(bytes_per_row);

	while (src->Peek() != std::char_traits<char>::eof()) {
		auto filter = src->Get();
		auto read = src->Read(current, bytes_per_row);

		assert(read == bytes_per_row);
		if (read != bytes_per_row) {
			throw GeneralException("Corrupted deflate compressed data");
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
					char current_byte = current[i] & 0xFF;
					char prior_byte = prior[bytes_per_pixel + i] & 0xFF;
					char value = SafeAddition<char>(current_byte, prior_byte) / 2;
					current[i] += value;
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

					current[i] += value;
				}

				break;
			default:
				LOG_ERROR_GLOBAL << "Unknown filter type: " << filter;
				break;
		}

		result->insert(result.end(), current.begin(), current.end());
		prior.assign(current.begin(), current.end());
	}

	return result;
}

} // syntax
} // gotchangpdf
