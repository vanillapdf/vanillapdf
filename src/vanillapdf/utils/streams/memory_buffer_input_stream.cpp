#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/memory_buffer_input_stream.h"

#include "utils/constants.h"

#include <cstring>

namespace vanillapdf {

MemoryBufferInputStream::MemoryBufferInputStream(std::shared_ptr<fmt::memory_buffer> buffer)
    : m_buffer(std::move(buffer)) {
    if (m_buffer == nullptr) {
        throw GeneralException("Could not create memory buffer input stream with null buffer");
    }

    m_input_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

BufferPtr MemoryBufferInputStream::Read(types::stream_size len) {
    BufferPtr result = make_deferred_container<Buffer>(len);

    auto available = static_cast<types::stream_size>(m_buffer->size()) - m_position;
    if (available <= 0) {
        m_eof = true;
        result->resize(0);
        return result;
    }

    auto to_read = (len < available) ? len : available;
    std::memcpy(result->data(), m_buffer->data() + m_position, static_cast<size_t>(to_read));
    m_position += to_read;

    if (to_read < len) {
        auto to_read_converted = ValueConvertUtils::SafeConvert<types::size_type>(to_read);
        result->resize(to_read_converted);
    }

    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        m_eof = true;
    }

    return result;
}

types::stream_size MemoryBufferInputStream::Read(char* result, types::stream_size len) {
    auto available = static_cast<types::stream_size>(m_buffer->size()) - m_position;
    if (available <= 0) {
        m_eof = true;
        return 0;
    }

    auto to_read = (len < available) ? len : available;
    std::memcpy(result, m_buffer->data() + m_position, static_cast<size_t>(to_read));
    m_position += to_read;

    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        m_eof = true;
    }

    return to_read;
}

types::stream_size MemoryBufferInputStream::Read(Buffer& result, types::stream_size len) {
    auto length_converted = ValueConvertUtils::SafeConvert<types::size_type>(len);

    assert(result.size() >= length_converted);
    if (result.size() < length_converted) {
        result.resize(length_converted);
    }

    return Read(result.data(), length_converted);
}

types::stream_size MemoryBufferInputStream::GetInputPosition() {
    assert(!m_fail);

    if (m_eof) {
        return constant::BAD_OFFSET;
    }

    return m_position;
}

void MemoryBufferInputStream::SetInputPosition(types::stream_size pos, SeekDirection way) {
    // if badoff is specified, set eof flag
    if (pos == constant::BAD_OFFSET) {
        m_eof = true;
        return;
    }

    // clear eof and fail
    if (m_eof || m_fail) {
        m_eof = false;
        m_fail = false;
    }

    auto buffer_size = static_cast<types::stream_size>(m_buffer->size());

    switch (way) {
        case SeekDirection::Beginning:
            m_position = pos;
            break;
        case SeekDirection::Current:
            m_position += pos;
            break;
        case SeekDirection::End:
            m_position = buffer_size + pos;
            break;
    }

    if (m_position < 0) {
        m_position = 0;
        m_fail = true;
    }
}

void MemoryBufferInputStream::SetInputPosition(types::stream_size pos) {
    SetInputPosition(pos, SeekDirection::Beginning);
}

BufferPtr MemoryBufferInputStream::Readline(void) {
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

    auto buffer_size = static_cast<types::stream_size>(m_buffer->size());

    for (;;) {
        if (m_position >= buffer_size) {
            m_eof = true;
            break;
        }

        auto ch = static_cast<unsigned char>(m_buffer->data()[m_position]);
        m_position++;

        if (ch == '\r') {
            if (m_position < buffer_size && m_buffer->data()[m_position] == '\n') {
                m_position++;
            }

            break;
        }

        if (ch == '\n') {
            break;
        }

        result->push_back(ch);
    }

    return result;
}

void MemoryBufferInputStream::ExclusiveInputLock() {
    m_input_lock->lock();
}

void MemoryBufferInputStream::ExclusiveInputUnlock() {
    m_input_lock->unlock();
}

bool MemoryBufferInputStream::Eof(void) const {
    assert(!m_fail);
    return m_eof || m_position >= static_cast<types::stream_size>(m_buffer->size());
}

bool MemoryBufferInputStream::Ignore(void) {
    assert(!m_eof);

    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        m_eof = true;
        return false;
    }

    m_position++;
    assert(!m_fail);
    return true;
}

int MemoryBufferInputStream::Get(void) {
    assert(!m_eof);

    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        m_eof = true;
        return EOF;
    }

    auto result = static_cast<unsigned char>(m_buffer->data()[m_position]);
    m_position++;
    assert(!m_fail);
    return result;
}

int MemoryBufferInputStream::Peek(void) {
    assert(!m_eof);

    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        m_eof = true;
        return EOF;
    }

    assert(!m_fail);
    return static_cast<unsigned char>(m_buffer->data()[m_position]);
}

bool MemoryBufferInputStream::IsFail(void) const {
    return m_fail;
}

MemoryBufferInputStream::operator bool(void) const {
    assert(!m_fail);
    return !m_eof && !m_fail;
}

} // vanillapdf
