#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/file_stream_input_stream.h"

#include "utils/constants.h"

namespace vanillapdf {

FileStreamInputStream::FileStreamInputStream(std::shared_ptr<FILE> file) : m_file(file) {
    if (m_file == nullptr) {
        throw GeneralException("Could not create input stream");
    }

    m_input_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

BufferPtr FileStreamInputStream::Read(types::stream_size len) {
    BufferPtr result = make_deferred_container<Buffer>(len);
    auto bytes_read = std::fread(result->data(), 1, static_cast<size_t>(len), m_file.get());

    assert(static_cast<types::stream_size>(bytes_read) <= len);

    if (bytes_read == 0 && std::feof(m_file.get())) {
        m_eof = true;
    }

    if (std::ferror(m_file.get())) {
        m_fail = true;
    }

    // Trim the buffer in case there is not enough data
    if (static_cast<types::stream_size>(bytes_read) < len) {
        auto bytes_read_converted = ValueConvertUtils::SafeConvert<types::size_type>(bytes_read);
        result->resize(bytes_read_converted);
    }

    return result;
}

types::stream_size FileStreamInputStream::Read(char* result, types::stream_size len) {
    auto bytes_read = std::fread(result, 1, static_cast<size_t>(len), m_file.get());

    if (bytes_read == 0 && std::feof(m_file.get())) {
        m_eof = true;
    }

    if (std::ferror(m_file.get())) {
        m_fail = true;
    }

    return static_cast<types::stream_size>(bytes_read);
}

types::stream_size FileStreamInputStream::Read(Buffer& result, types::stream_size len) {
    auto length_converted = ValueConvertUtils::SafeConvert<types::size_type>(len);

    assert(result.size() >= length_converted);
    if (result.size() < length_converted) {
        result.resize(length_converted);
    }

    return Read(result.data(), length_converted);
}

types::stream_size FileStreamInputStream::GetInputPosition() {
    assert(!m_fail);

    if (m_eof) {
        return constant::BAD_OFFSET;
    }

#if _WIN32
    auto pos = _ftelli64(m_file.get());
#else
    auto pos = ftello(m_file.get());
#endif

    if (pos < 0) {
        m_fail = true;
        return constant::BAD_OFFSET;
    }

    return static_cast<types::stream_size>(pos);
}

void FileStreamInputStream::SetInputPosition(types::stream_size pos, SeekDirection way) {
    auto initial_offset = GetInputPosition();

    // if badoff is specified, set eof flag
    if (pos == constant::BAD_OFFSET) {
        m_eof = true;
        return;
    }

    // clear eof and fail
    if (m_eof || m_fail) {
        m_eof = false;
        m_fail = false;
        std::clearerr(m_file.get());
    }

    int origin = SEEK_SET;
    switch (way) {
        case SeekDirection::Beginning: origin = SEEK_SET; break;
        case SeekDirection::Current: origin = SEEK_CUR; break;
        case SeekDirection::End: origin = SEEK_END; break;
    }

    // seek to the actual position
#if _WIN32
    int result = _fseeki64(m_file.get(), static_cast<int64_t>(pos), origin);
#else
    int result = fseeko(m_file.get(), static_cast<off_t>(pos), origin);
#endif

    if (result != 0) {
        m_fail = true;
    } else {
        auto verify_offset = GetInputPosition();
        if (way == SeekDirection::Beginning) {
            // verify if the position is correct
            assert(pos == verify_offset);
        }

        if (way == SeekDirection::Current) {
            // verify if the position is correct
            assert(initial_offset + pos == verify_offset);
        }

        UNUSED(verify_offset);
        UNUSED(initial_offset);
    }
}

void FileStreamInputStream::SetInputPosition(types::stream_size pos) {
    SetInputPosition(pos, SeekDirection::Beginning);
}

BufferPtr FileStreamInputStream::Readline(void) {
    BufferPtr result;

    bool stream_failed = m_fail;
    assert(!stream_failed && "Stream is in failed state");
    if (stream_failed) {
        throw GeneralException("Stream is in failed state");
    }

    bool stream_eof = m_eof;
    assert(!stream_eof && "Stream reached eof");
    if (stream_eof) {
        throw GeneralException("Stream reached eof");
    }

    for (;;) {
        int eof_test = std::fgetc(m_file.get());
        if (eof_test == EOF) {
            m_eof = true;
            break;
        }

        if (eof_test == '\r') {
            int line_feed = std::fgetc(m_file.get());
            if (line_feed != '\n' && line_feed != EOF) {
                std::ungetc(line_feed, m_file.get());
            }

            break;
        }

        if (eof_test == '\n') {
            break;
        }

        auto converted = ValueConvertUtils::SafeConvert<unsigned char>(eof_test);
        result->push_back(converted);
    }

    return result;
}

void FileStreamInputStream::ExclusiveInputLock() {
    m_input_lock->lock();
}

void FileStreamInputStream::ExclusiveInputUnlock() {
    m_input_lock->unlock();
}

bool FileStreamInputStream::Eof(void) const {
    assert(!m_fail);
    return m_eof || std::feof(m_file.get()) != 0;
}

bool FileStreamInputStream::Ignore(void) {
    assert(!m_eof);
    int ch = std::fgetc(m_file.get());
    if (ch == EOF) {
        m_eof = true;
        return false;
    }
    assert(!m_fail);
    return true;
}

int FileStreamInputStream::Get(void) {
    assert(!m_eof);
    int result = std::fgetc(m_file.get());
    if (result == EOF) {
        m_eof = true;
    }
    assert(!m_fail);
    return result;
}

int FileStreamInputStream::Peek(void) {
    assert(!m_eof);
    int result = std::fgetc(m_file.get());
    if (result == EOF) {
        m_eof = true;
        return EOF;
    }
    std::ungetc(result, m_file.get());
    assert(!m_fail);
    return result;
}

bool FileStreamInputStream::IsFail(void) const {
    return m_fail || std::ferror(m_file.get()) != 0;
}

FileStreamInputStream::operator bool(void) const {
    assert(!m_fail);
    return !m_eof && !m_fail && std::ferror(m_file.get()) == 0;
}

} // vanillapdf
