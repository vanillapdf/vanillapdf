#ifndef _MEMORY_BUFFER_INPUT_STREAM_H
#define _MEMORY_BUFFER_INPUT_STREAM_H

#include "utils/streams/input_stream_interface.h"

#include <memory>
#include <mutex>

namespace vanillapdf {

class MemoryBufferInputStream : public virtual IInputStream {
public:
    explicit MemoryBufferInputStream(std::shared_ptr<fmt::memory_buffer> buffer);

    virtual BufferPtr Read(types::stream_size len) override;
    virtual types::stream_size Read(char* result, types::stream_size len) override;
    virtual types::stream_size Read(Buffer& result, types::stream_size len) override;
    virtual BufferPtr Readline(void) override;
    virtual types::stream_size GetInputPosition() override;
    virtual void SetInputPosition(types::stream_size pos) override;
    virtual void SetInputPosition(types::stream_size pos, SeekDirection way) override;

    virtual void ExclusiveInputLock() override;
    virtual void ExclusiveInputUnlock() override;

    virtual bool Eof(void) const override;
    virtual bool Ignore(void) override;
    virtual int Get(void) override;
    virtual int Peek(void) override;

    virtual bool IsFail(void) const override;

    virtual operator bool(void) const override;

protected:
    std::shared_ptr<fmt::memory_buffer> m_buffer;
    types::stream_size m_position = 0;
    std::shared_ptr<std::recursive_mutex> m_input_lock;
};

} // vanillapdf

#endif /* _MEMORY_BUFFER_INPUT_STREAM_H */
