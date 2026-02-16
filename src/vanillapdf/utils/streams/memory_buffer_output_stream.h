#ifndef _MEMORY_BUFFER_OUTPUT_STREAM_H
#define _MEMORY_BUFFER_OUTPUT_STREAM_H

#include "utils/streams/output_stream_interface.h"

#include <memory>

namespace vanillapdf {

class MemoryBufferOutputStream : public virtual IOutputStream {
public:
    MemoryBufferOutputStream();
    explicit MemoryBufferOutputStream(std::shared_ptr<fmt::memory_buffer> buffer);

    virtual void Write(const Buffer& data) override;
    virtual void Write(const Buffer& data, types::stream_size size) override;
    virtual void Write(std::string_view data) override;
    virtual void Write(char value) override;
    virtual void Write(unsigned char value) override;
    virtual void Write(WhiteSpace value) override;
    virtual void Write(Delimiter value) override;
    virtual void Write(int32_t value) override;
    virtual void Write(uint32_t value) override;
    virtual void Write(int64_t value) override;
    virtual void Write(uint64_t value) override;

    virtual void Flush(void) override;

    virtual types::stream_size GetOutputPosition(void) override;
    virtual void SetOutputPosition(types::stream_size pos) override;
    virtual void SetOutputPosition(types::stream_size pos, SeekDirection way) override;

    virtual void ExclusiveOutputLock() override;
    virtual void ExclusiveOutputUnlock() override;

    std::string ToString() const;
    std::shared_ptr<fmt::memory_buffer> GetBuffer() const { return m_buffer; }

protected:
    static std::shared_ptr<fmt::memory_buffer> CreateBuffer();

    std::shared_ptr<fmt::memory_buffer> m_buffer;
    types::stream_size m_position = 0;

private:
    void WriteData(const char* data, size_t len);
    void WriteByte(char value);
};

} // vanillapdf

#endif /* _MEMORY_BUFFER_OUTPUT_STREAM_H*/
