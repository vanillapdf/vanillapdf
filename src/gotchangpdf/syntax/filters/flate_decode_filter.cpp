#include "precompiled.h"
#include "flate_decode_filter.h"

#include "utils/constants.h"
#include "utils/zlib_wrapper.h"
#include "syntax/streams/raw_stream.h"

namespace gotchangpdf {
namespace syntax {

BufferPtr FlateDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const {
	return ZlibWrapper::Deflate(src);
}

BufferPtr FlateDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto dest = ZlibWrapper::Inflate(src);
	return ApplyPredictor(dest, parameters);
}

BufferPtr FlateDecodeFilter::Encode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters) const {
	return ZlibWrapper::Deflate(src, length);
}

BufferPtr FlateDecodeFilter::Decode(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters) const {
	auto dest = ZlibWrapper::Inflate(src, length);
	return ApplyPredictor(dest, parameters);
}

BufferPtr FlateDecodeFilter::ApplyPredictor(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToStringStream();
	return ApplyPredictor(stream, src->size(), parameters);
}

BufferPtr FlateDecodeFilter::ApplyPredictor(std::istream& src, types::stream_size length, DictionaryObjectPtr parameters) const {
	IntegerObjectPtr predictor = 1;
	if (parameters->Contains(constant::Name::Predictor)) {
		predictor = parameters->FindAs<IntegerObjectPtr>(constant::Name::Predictor);
		assert(*predictor == 1 || *predictor == 2 || (*predictor >= 10 && *predictor <= 15));
	}

	// No prediction was used
	if (predictor == 1) {
		auto length_converted = ValueConvertUtils::SafeConvert<size_t>(length);
		return Stream(src).read(length_converted);
	}

	IntegerObjectPtr colors = 1;
	if (parameters->Contains(constant::Name::Colors)) {
		colors = parameters->FindAs<IntegerObjectPtr>(constant::Name::Colors);
		assert(*colors >= 1);
	}

	syntax::IntegerObjectPtr bits = 8;
	if (parameters->Contains(constant::Name::BitsPerComponent)) {
		bits = parameters->FindAs<IntegerObjectPtr>(constant::Name::BitsPerComponent);
		assert(*bits == 1 || *bits == 2 || *bits == 4 || *bits == 8);
	}

	IntegerObjectPtr columns = 1;
	if (parameters->Contains(constant::Name::Columns))
		columns = parameters->FindAs<IntegerObjectPtr>(constant::Name::Columns);

	IntegerObjectPtr change = 1;
	if (parameters->Contains(constant::Name::EarlyChange))
		change = parameters->FindAs<IntegerObjectPtr>(constant::Name::EarlyChange);

	if (*predictor == 2) {
		throw NotSupportedException("TIFF predictor is currently not supported");
	} else if (*predictor < 10) {
		throw GeneralException("Unknown predictor type");
	}

	types::native_int colors_int = colors->SafeConvert<types::native_int>();
	types::native_int columns_int = columns->SafeConvert<types::native_int>();
	types::native_int bits_int = bits->SafeConvert<types::native_int>();

	types::native_int bytesPerPixel = colors_int * bits_int / 8;
	types::native_int bytesPerRow = (colors_int * columns_int * bits_int + 7) / 8;

	BufferPtr result;
	Stream strm(src);
	Buffer curr(bytesPerRow, '\0');
	Buffer prior(bytesPerRow, '\0');

	while (strm.peek() != std::char_traits<char>::eof()) {
		auto filter = strm.get();
		strm.read(curr, bytesPerRow);

		switch (filter) {
			case 0: //PNG_FILTER_NONE
				break;
			case 1: //PNG_FILTER_SUB
				for (types::native_int i = bytesPerPixel; i < bytesPerRow; i++) {
					curr[i] += curr[i - bytesPerPixel];
				}
				break;
			case 2: //PNG_FILTER_UP
				for (types::native_int i = 0; i < bytesPerRow; i++) {
					curr[i] += prior[i];
				}
				break;
			case 3: //PNG_FILTER_AVERAGE
				for (types::native_int i = 0; i < bytesPerPixel; i++) {
					curr[i] += (prior[i] / 2);
				}
				for (types::native_int i = bytesPerPixel; i < bytesPerRow; i++) {
					curr[i] += (((curr[i - bytesPerPixel] & 0xff) + (prior[i] & 0xff)) / 2);
				}
				break;
			case 4: //PNG_FILTER_PAETH
				for (types::native_int i = 0; i < bytesPerPixel; i++) {
					curr[i] += prior[i];
				}

				for (types::native_int i = bytesPerPixel; i < bytesPerRow; i++) {
					uint8_t a = curr[i - bytesPerPixel] & 0xff;
					uint8_t b = prior[i] & 0xff;
					uint8_t c = prior[i - bytesPerPixel] & 0xff;

					int p = a + b - c;
					int pa = std::abs(p - a);
					int pb = std::abs(p - b);
					int pc = std::abs(p - c);

					uint8_t ret;
					if ((pa <= pb) && (pa <= pc)) {
						ret = a;
					} else if (pb <= pc) {
						ret = b;
					} else {
						ret = c;
					}
					curr[i] += ret;
				}
				break;
			default:
				LOG_ERROR_GLOBAL << "Unknown filter type: " << filter;
				break;
		}

		result->insert(result.end(), curr.begin(), curr.end());
		std::swap(curr, prior);
	}

	return result;
}

} // syntax
} // gotchangpdf
