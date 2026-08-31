#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/memory_buffer_input_stream.h"

#include "utils/constants.h"

#include <cstring>

namespace vanillapdf {

MemoryBufferInputStream::MemoryBufferInputStream(std::shared_ptr<fmt::memory_buffer> buffer)
    : m_buffer(std::move(buffer)) {
    if (m_buffer == nullptr) {
        LOG_ERROR_AND_THROW_GENERAL("Could not create memory buffer input stream with null buffer");
    }

    m_input_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

BufferPtr MemoryBufferInputStream::Read(types::stream_size len) {
    BufferPtr result = make_deferred_container<Buffer>(len);

    if (len <= 0) {
        return result;
    }

    auto available = static_cast<types::stream_size>(m_buffer->size()) - m_position;
    if (available <= 0) {
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

    return result;
}

types::stream_size MemoryBufferInputStream::Read(char* result, types::stream_size len) {
    auto available = static_cast<types::stream_size>(m_buffer->size()) - m_position;
    if (available <= 0) {
        return 0;
    }

    auto to_read = (len < available) ? len : available;
    std::memcpy(result, m_buffer->data() + m_position, static_cast<size_t>(to_read));
    m_position += to_read;

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
    // Return BAD_OFFSET only when the position is strictly past the buffer end
    // (an invalid state from a corrupt seek). When m_position == size, return
    // the actual position — fstream's tellg() does the same because eof() is
    // only set after a failed read, not when merely positioned at the end.
    // The old >= check returned BAD_OFFSET at m_position == size, which
    // corrupted the tokenizer's offset cache (all end-of-buffer tokens
    // collapsed to the same BAD_OFFSET key).
    if (m_position > static_cast<types::stream_size>(m_buffer->size())) {
        return constant::BAD_OFFSET;
    }

    return m_position;
}

void MemoryBufferInputStream::SetInputPosition(types::stream_size pos, SeekDirection way) {
    // if badoff is specified, move to end (makes Eof() true)
    if (pos == constant::BAD_OFFSET) {
        m_position = static_cast<types::stream_size>(m_buffer->size());
        return;
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
        default:
            LOG_ERROR_AND_THROW_GENERAL("Unknown seek direction: {}", static_cast<int>(way));
    }

    if (m_position < 0) {
        LOG_ERROR_AND_THROW(IOErrorException, "Could not seek memory buffer to {}/{}, resulting position is negative",
            pos, static_cast<int>(way));
    }
}

void MemoryBufferInputStream::SetInputPosition(types::stream_size pos) {
    SetInputPosition(pos, SeekDirection::Beginning);
}

BufferPtr MemoryBufferInputStream::Readline(void) {
    BufferPtr result;

    auto buffer_size = static_cast<types::stream_size>(m_buffer->size());

    if (m_position >= buffer_size) {
        LOG_ERROR_AND_THROW_GENERAL("Stream reached eof");
    }

    for (;;) {
        if (m_position >= buffer_size) {
            break;
        }

        auto ch = static_cast<unsigned char>(m_buffer->data()[m_position]);
        m_position += 1;

        if (ch == '\r') {
            if (m_position < buffer_size && m_buffer->data()[m_position] == '\n') {
                m_position += 1;
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
    return m_position >= static_cast<types::stream_size>(m_buffer->size());
}

bool MemoryBufferInputStream::Ignore(void) {
    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        return false;
    }

    m_position += 1;
    return true;
}

int MemoryBufferInputStream::Get(void) {
    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        return EOF;
    }

    auto result = static_cast<unsigned char>(m_buffer->data()[m_position]);
    m_position += 1;
    return result;
}

int MemoryBufferInputStream::Peek(void) {
    if (m_position >= static_cast<types::stream_size>(m_buffer->size())) {
        return EOF;
    }

    return static_cast<unsigned char>(m_buffer->data()[m_position]);
}

bool MemoryBufferInputStream::IsFail(void) const {
    return false;
}

MemoryBufferInputStream::operator bool(void) const {
    return m_position < static_cast<types::stream_size>(m_buffer->size());
}

} // vanillapdf
