#include "precompiled.h"

#include "utils/streams/input_stream.h"
#include "utils/streams/output_stream.h"
#include "utils/streams/input_output_stream.h"
#include "utils/streams/memory_buffer_input_output_stream.h"

#include "utils/streams/stream_utils.h"
#include "utils/windows_utils.h"

#include <fstream>
#include <filesystem>

namespace vanillapdf {

std::shared_ptr<std::fstream> StreamUtils::OpenFileStream(const std::string& path, std::ios_base::openmode mode) {
    auto fs_path = std::filesystem::path(path);

#if _WIN32
    fs_path = WindowsUtils::MultiByteToWideChar(path);
#endif /* _WIN32 */

    auto fstream = std::make_shared<std::fstream>();
    fstream->open(fs_path, mode);

    if (!fstream || !fstream->good()) {
        LOG_ERROR_AND_THROW_GENERAL("Could not open file: {}", path);
    }

    return fstream;
}

IOutputStreamPtr StreamUtils::OutputStreamFromFile(const std::string& filename) {
    auto fstream = OpenFileStream(filename, std::ios::out | std::ios::binary);
    return make_deferred<OutputStream>(fstream);
}

IInputStreamPtr StreamUtils::InputStreamFromFile(const std::string& filename) {
    auto fstream = OpenFileStream(filename, std::ios::in | std::ios::binary);
    return make_deferred<InputStream>(fstream);
}

IInputOutputStreamPtr StreamUtils::InputOutputStreamFromFile(const std::string& filename) {
    auto fstream = OpenFileStream(filename, std::ios::in | std::ios::out | std::ios::binary);
    return make_deferred<InputOutputStream>(fstream);
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

IInputOutputStreamPtr StreamUtils::CreateFileStream(const std::string& path, std::ios_base::openmode mode) {
    auto fstream = OpenFileStream(path, mode);
    return make_deferred<InputOutputStream>(fstream);
}

IInputOutputStreamPtr StreamUtils::CreateMemoryBufferStream(const std::string& path, std::ios_base::openmode mode) {
    bool has_in = (mode & std::ios_base::in) != 0;
    bool has_trunc = (mode & std::ios_base::trunc) != 0;

    // Create mode (trunc): return empty buffer
    if (has_trunc) {
        return make_deferred<MemoryBufferInputOutputStream>();
    }

    // Open mode: read entire file into buffer
    if (has_in) {
        auto fstream = OpenFileStream(path, std::ios::binary | std::ios::in | std::ios::ate);

        auto file_size = fstream->tellg();
        fstream->seekg(0, std::ios::beg);

        auto buffer = std::make_shared<fmt::memory_buffer>();
        if (file_size <= 0) {
            return make_deferred<MemoryBufferInputOutputStream>(buffer);
        }

        auto size = static_cast<size_t>(file_size);
        buffer->resize(size);
        fstream->read(buffer->data(), static_cast<std::streamsize>(size));

        if (!fstream->good() && !fstream->eof()) {
            LOG_ERROR_AND_THROW_GENERAL("Could not read file: {}", path);
        }

        auto result = make_deferred<MemoryBufferInputOutputStream>(buffer);

        // ate (at-end): position at end after loading
        if (mode & std::ios_base::ate) {
            result->SetInputPosition(0, SeekDirection::End);
        }

        return result;
    }

    LOG_ERROR_AND_THROW_GENERAL("Unsupported memory buffer stream mode: {}", static_cast<int>(mode));
}

} // vanillapdf
