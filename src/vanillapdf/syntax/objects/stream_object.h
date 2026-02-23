#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/object.h"
#include "syntax/objects/dictionary_object.h"

#include "utils/buffer.h"

#include <optional>

namespace vanillapdf {
namespace syntax {

class StreamObject : public Object {
public:
    StreamObject();
    StreamObject(const StreamObject&) = delete;

    explicit StreamObject(bool initialized);
    StreamObject(DictionaryObjectPtr header, types::stream_offset offset);
    virtual Object::Type GetObjectType(void) const noexcept override;
    virtual std::string ToString(void) const override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

    virtual bool IsDirty() const override;

    DictionaryObjectPtr GetHeader() const;
    void SetHeader(DictionaryObjectPtr header);

    types::stream_offset GetDataOffset() const;
    void SetDataOffset(types::stream_offset offset);

    BufferPtr GetBodyRaw() const;
    BufferPtr GetBody() const;
    BufferPtr GetBodyEncoded() const;
    BufferPtr GetBodyDecrypted() const;
    void SetBody(BufferPtr value);

    virtual size_t Hash() const override;
    virtual StreamObject* Clone(void) const override;
    virtual bool Equals(ObjectPtr other) const override;

    virtual void SetFile(WeakReference<File> file) override;
    virtual void SetInitialized(bool initialized = true) override;

    virtual ~StreamObject() = default;

private:
    DictionaryObjectPtr _header;
    types::stream_offset _raw_data_offset = constant::BAD_OFFSET;

    mutable BufferPtr _body_raw;
    mutable BufferPtr _body_decrypted;
    mutable BufferPtr _body_decoded;

    // The library interface wants to be thread-safe as much as possible
    // Even though the are currently no cases for multi-thread access
    // to the dictonary, let's try to be visionary and prepare for this
    std::unique_ptr<std::recursive_mutex> _access_lock;

    // Helpers for GetBodyRaw() that separate lock-holding from file I/O.
    // See GetBodyRaw() for the lock-ordering rationale.
    std::optional<types::stream_offset> CaptureBodyOffset() const;
    void LoadBody(types::stream_offset offset) const;

    BufferPtr EncryptStream(BufferPtr data, types::big_uint obj_number, types::ushort generation_number) const;
    BufferPtr EncryptData(BufferPtr data, types::big_uint obj_number, types::ushort generation_number, NameObjectPtr handler) const;
    BufferPtr DecryptStream(BufferPtr data, types::big_uint obj_number, types::ushort generation_number) const;
    BufferPtr DecryptData(BufferPtr data, types::big_uint obj_number, types::ushort generation_number, NameObjectPtr handler) const;
};

} // syntax
} // vanillapdf

#endif /* _STREAM_OBJECT_H */
