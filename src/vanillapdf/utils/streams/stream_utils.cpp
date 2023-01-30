#include "precompiled.h"

#include "utils/streams/input_stream.h"
#include "utils/streams/output_stream.h"
#include "utils/streams/input_output_stream.h"

#include "utils/streams/stream_utils.h"

#include <fstream>

namespace vanillapdf {

IOutputStreamPtr StreamUtils::OutputStreamFromFile(const std::string& filename) {

	auto output = std::make_shared<std::fstream>();
	output->open(filename, std::ios::out | std::ios::binary);

	if (!output || !output->good()) {
		throw GeneralException("Could not open file: " + filename);
	}

	return make_deferred<OutputStream>(output);
}

IInputStreamPtr StreamUtils::InputStreamFromFile(const std::string& filename) {

	auto input = std::make_shared<std::fstream>();
	input->open(filename, std::ios::in |std::ios::binary);

	if (!input || !input->good()) {
		throw GeneralException("Could not open file: " + filename);
	}

	return make_deferred<InputStream>(input);
}

IInputOutputStreamPtr StreamUtils::InputOutputStreamFromFile(const std::string& filename) {

	auto input = std::make_shared<std::fstream>();
	input->open(filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!input || !input->good()) {
		throw GeneralException("Could not open file: " + filename);
	}

	return make_deferred<InputOutputStream>(input);
}

IInputOutputStreamPtr StreamUtils::InputOutputStreamFromMemory() {
	auto ss = std::make_shared<std::stringstream>();
	return make_deferred<InputOutputStream>(ss);
}

IInputStreamPtr StreamUtils::InputStreamFromBuffer(BufferPtr data) {
	return data->ToInputStream();
}

BufferPtr StreamUtils::InputStreamToBuffer(IInputStreamPtr source) {

	BufferPtr result;
	Buffer buffer(constant::BUFFER_SIZE);

	for (;;) {
		auto read = source->Read(buffer, buffer.size());
		if (read == 0) {
			break;
		}

		auto read_converted = ValueConvertUtils::SafeConvert<decltype(buffer)::difference_type>(read);
		result->insert(result.end(), buffer.begin(), buffer.begin() + read_converted);
	}

	return result;
}

SeekDirection StreamUtils::ConvertToSeekDirection(std::ios_base::seekdir value) {
	if (value == std::ios_base::beg) {
		return SeekDirection::Beginning;
	}

	if (value == std::ios_base::cur) {
		return SeekDirection::Current;
	}

	if (value == std::ios_base::end) {
		return SeekDirection::End;
	}

	throw GeneralException("Unknown seek direction: " + std::to_string(value));
}

std::ios_base::seekdir StreamUtils::ConvertFromSeekDirection(SeekDirection value) {
	if (value == SeekDirection::Beginning) {
		return std::ios_base::beg;
	}

	if (value == SeekDirection::Current) {
		return std::ios_base::cur;
	}

	if (value == SeekDirection::End) {
		return std::ios_base::end;
	}

	throw GeneralException("Unknown seek direction: " + std::to_string(static_cast<int>(value)));
}

} // vanillapdf
