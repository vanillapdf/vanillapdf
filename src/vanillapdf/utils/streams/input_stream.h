#ifndef _INPUT_STREAM_H
#define _INPUT_STREAM_H

#include "utils/streams/input_stream_interface.h"

#include <mutex>

namespace vanillapdf {

class InputStream : public virtual IInputStream {
public:
    explicit InputStream(std::shared_ptr<std::istream> stream);

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
    std::shared_ptr<std::istream> m_stream;
    std::shared_ptr<std::recursive_mutex> m_input_lock;
};

} // vanillapdf

#endif /* _INPUT_STREAM_H*/
