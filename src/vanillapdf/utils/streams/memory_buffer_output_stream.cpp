#include "precompiled.h"

#include "utils/math_utils.h"
#include "utils/streams/memory_buffer_output_stream.h"

#include <cstring>

namespace vanillapdf {

MemoryBufferOutputStream::MemoryBufferOutputStream()
    : m_buffer(CreateBuffer()) {
}

MemoryBufferOutputStream::MemoryBufferOutputStream(std::shared_ptr<fmt::memory_buffer> buffer)
    : m_buffer(std::move(buffer)) {
    if (m_buffer == nullptr) {
        throw GeneralException("Could not create memory buffer output stream with null buffer");
    }
}

std::shared_ptr<fmt::memory_buffer> MemoryBufferOutputStream::CreateBuffer() {
    return std::make_shared<fmt::memory_buffer>();
}

void MemoryBufferOutputStream::Write(const Buffer& data) {
    auto str = data.ToStringView();
    WriteData(str.data(), str.size());
}

void MemoryBufferOutputStream::Write(const Buffer& data, types::stream_size size) {
    auto size_converted = ValueConvertUtils::SafeConvert<size_t>(size);

    auto str = data.ToStringView();
    auto substring = str.substr(0, size_converted);

    WriteData(substring.data(), substring.size());
}

void MemoryBufferOutputStream::Write(std::string_view data) {
    WriteData(data.data(), data.size());
}

void MemoryBufferOutputStream::Write(char value) {
    WriteByte(value);
}

void MemoryBufferOutputStream::Write(unsigned char value) {
    WriteByte(static_cast<char>(value));
}

void MemoryBufferOutputStream::Write(WhiteSpace value) {
    WriteByte(static_cast<char>(value));
}

void MemoryBufferOutputStream::Write(Delimiter value) {
    WriteByte(static_cast<char>(value));
}

void MemoryBufferOutputStream::Write(int32_t value) {
    fmt::memory_buffer temp;
    fmt::format_to(std::back_inserter(temp), "{}", value);

    WriteData(temp.data(), temp.size());
}

void MemoryBufferOutputStream::Write(uint32_t value) {
    fmt::memory_buffer temp;
    fmt::format_to(std::back_inserter(temp), "{}", value);

    WriteData(temp.data(), temp.size());
}

void MemoryBufferOutputStream::Write(int64_t value) {
    fmt::memory_buffer temp;
    fmt::format_to(std::back_inserter(temp), "{}", value);

    WriteData(temp.data(), temp.size());
}

void MemoryBufferOutputStream::Write(uint64_t value) {
    fmt::memory_buffer temp;
    fmt::format_to(std::back_inserter(temp), "{}", value);

    WriteData(temp.data(), temp.size());
}

void MemoryBufferOutputStream::Flush(void) {
    // No flush needed
}

types::stream_size MemoryBufferOutputStream::GetOutputPosition(void) {
    return m_position;
}

void MemoryBufferOutputStream::SetOutputPosition(types::stream_size pos) {
    m_position = pos;
}

void MemoryBufferOutputStream::SetOutputPosition(types::stream_size pos, SeekDirection way) {
    if (way == SeekDirection::Beginning) {
        m_position = pos;
        return;
    }

    if (way == SeekDirection::Current) {
        auto desired = SafeAddition<types::stream_size>(m_position, pos);
        auto desired_unsigned = ValueConvertUtils::SafeConvert<size_t>(desired);
        if (m_buffer->size() < desired_unsigned) {
            LOG_ERROR_AND_THROW(IOErrorException, "Could not seek memory buffer to {}/{}, buffer size {}",
                pos, static_cast<int>(way), m_buffer->size());
        }

        m_position += pos;
        return;
    }

    if (way == SeekDirection::End) {
        auto destination_converted = ValueConvertUtils::SafeConvert<size_t>(pos);
        if (m_buffer->size() < destination_converted) {
            LOG_ERROR_AND_THROW(IOErrorException, "Could not seek memory buffer to {}/{}, buffer size {}",
                pos, static_cast<int>(way), m_buffer->size());
        }

        m_position = (m_buffer->size() - pos);
        return;
    }

    LOG_ERROR_AND_THROW(IOErrorException, "Unknown seek direction: {}", static_cast<int>(way));
}

void MemoryBufferOutputStream::ExclusiveOutputLock() {
    // This implementation is not thread-safe
}

void MemoryBufferOutputStream::ExclusiveOutputUnlock() {
    // This implementation is not thread-safe
}

std::string MemoryBufferOutputStream::ToString() const {
    return fmt::to_string(*m_buffer);
}

void MemoryBufferOutputStream::WriteData(const char* data, size_t len) {
    auto pos = static_cast<size_t>(m_position);

    if (pos == m_buffer->size()) {
        // Common case: sequential append
        m_buffer->append(data, data + len);
    } else {
        // Random-access overwrite (e.g. DocumentSigner patching ByteRange)
        std::memcpy(m_buffer->data() + pos, data, len);
    }

    m_position += len;
}

void MemoryBufferOutputStream::WriteByte(char value) {
    auto pos = static_cast<size_t>(m_position);

    if (pos == m_buffer->size()) {
        // Common case: sequential append
        m_buffer->push_back(value);
    } else {
        // Random-access overwrite
        m_buffer->data()[pos] = value;
    }

    m_position += 1;
}

} // vanillapdf
