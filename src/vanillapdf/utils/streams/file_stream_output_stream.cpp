#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/file_stream_output_stream.h"

#include "utils/constants.h"

#include <cinttypes>
#include <cstring>

namespace vanillapdf {

FileStreamOutputStream::FileStreamOutputStream(std::shared_ptr<FILE> file) : m_file(file) {
    if (m_file == nullptr) {
        throw GeneralException("Could not create output stream");
    }

    m_output_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

void FileStreamOutputStream::Write(const Buffer& data) {
    std::fwrite(data.data(), 1, data.size(), m_file.get());
}

void FileStreamOutputStream::Write(const Buffer& data, types::stream_size size) {
    std::fwrite(data.data(), 1, static_cast<size_t>(size), m_file.get());
}

void FileStreamOutputStream::Write(std::string_view data) {
    std::fwrite(data.data(), 1, data.size(), m_file.get());
}

void FileStreamOutputStream::Write(const char* str) {
    std::fwrite(str, 1, std::strlen(str), m_file.get());
}

void FileStreamOutputStream::Write(char value) {
    std::fputc(value, m_file.get());
}

void FileStreamOutputStream::Write(unsigned char value) {
    std::fputc(value, m_file.get());
}

void FileStreamOutputStream::Write(WhiteSpace value) {
    std::fputc(static_cast<char>(value), m_file.get());
}

void FileStreamOutputStream::Write(Delimiter value) {
    std::fputc(static_cast<char>(value), m_file.get());
}

void FileStreamOutputStream::Write(int32_t value) {
    std::fprintf(m_file.get(), "%" PRId32, value);
}

void FileStreamOutputStream::Write(uint32_t value) {
    std::fprintf(m_file.get(), "%" PRIu32, value);
}

void FileStreamOutputStream::Write(int64_t value) {
    std::fprintf(m_file.get(), "%" PRId64, value);
}

void FileStreamOutputStream::Write(uint64_t value) {
    std::fprintf(m_file.get(), "%" PRIu64, value);
}

void FileStreamOutputStream::Flush(void) {
    std::fflush(m_file.get());
}

types::stream_size FileStreamOutputStream::GetOutputPosition(void) {
    auto pos = std::ftell(m_file.get());

    return static_cast<types::stream_size>(pos);
}

void FileStreamOutputStream::SetOutputPosition(types::stream_size pos) {
    std::fseek(m_file.get(), static_cast<long>(pos), SEEK_SET);
}

void FileStreamOutputStream::SetOutputPosition(types::stream_size pos, SeekDirection way) {
    int origin = SEEK_SET;
    switch (way) {
        case SeekDirection::Beginning: origin = SEEK_SET; break;
        case SeekDirection::Current: origin = SEEK_CUR; break;
        case SeekDirection::End: origin = SEEK_END; break;
    }

    std::fseek(m_file.get(), static_cast<long>(pos), origin);
}

void FileStreamOutputStream::ExclusiveOutputLock() {
    m_output_lock->lock();
}

void FileStreamOutputStream::ExclusiveOutputUnlock() {
    m_output_lock->unlock();
}

} // vanillapdf
