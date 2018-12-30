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
		throw GeneralException("Could not open file");
	}

	return make_deferred<OutputStream>(output);
}

IInputStreamPtr StreamUtils::InputStreamFromFile(const std::string& filename) {

	auto input = std::make_shared<std::fstream>();
	input->open(filename, std::ios::in |std::ios::binary);

	if (!input || !input->good()) {
		throw GeneralException("Could not open file");
	}

	return make_deferred<InputStream>(input);
}

IInputOutputStreamPtr StreamUtils::InputOutputStreamFromFile(const std::string& filename) {

	auto input = std::make_shared<std::fstream>();
	input->open(filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!input || !input->good()) {
		throw GeneralException("Could not open file");
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

} // vanillapdf
