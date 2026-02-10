#ifndef _FILE_STREAM_OUTPUT_STREAM_H
#define _FILE_STREAM_OUTPUT_STREAM_H

#include "utils/streams/output_stream_interface.h"

#include <cstdio>
#include <memory>
#include <mutex>

namespace vanillapdf {

class FileStreamOutputStream : public virtual IOutputStream {
public:
    explicit FileStreamOutputStream(std::shared_ptr<FILE> file);

    virtual void Write(const Buffer& data) override;
    virtual void Write(const Buffer& data, types::stream_size size) override;
    virtual void Write(std::string_view data) override;
    virtual void Write(const char* str) override;
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

protected:
    std::shared_ptr<FILE> m_file;
    std::shared_ptr<std::recursive_mutex> m_output_lock;
};

} // vanillapdf

#endif /* _FILE_STREAM_OUTPUT_STREAM_H */
