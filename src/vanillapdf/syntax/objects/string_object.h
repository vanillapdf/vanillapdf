#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"
#include "utils/buffer.h"

#include <mutex>

namespace vanillapdf {
namespace syntax {

class StringObjectBase : public ContainableObject {
public:
    enum class StringType {
        Undefined = 0,
        Literal,
        Hexadecimal
    };

    virtual BufferPtr GetValue() const = 0;
    virtual void SetValue(BufferPtr value) = 0;
    virtual void SetValue(std::string_view value) = 0;

    virtual StringType GetStringType(void) const noexcept = 0;
    virtual Object::Type GetObjectType(void) const noexcept override { return Object::Type::String; }

    bool Equals(const StringObjectBase& other) const { return GetValue() == other.GetValue(); }
    bool operator==(const StringObjectBase& other) const { return Equals(other); }
    bool operator<(const StringObjectBase& other) const { return GetValue() < other.GetValue(); }

    virtual bool Equals(ObjectPtr other) const override;
    virtual size_t Hash() const override;

protected:
    // Protects concurrent access to the string value
    std::unique_ptr<std::recursive_mutex> _access_lock;
};

class HexadecimalStringObject : public StringObjectBase {
public:
    HexadecimalStringObject();
    HexadecimalStringObject(const HexadecimalStringObject&) = delete;

    static HexadecimalStringObjectPtr CreateFromEncoded(BufferPtr value);
    static HexadecimalStringObjectPtr CreateFromEncoded(const char * value);
    static HexadecimalStringObjectPtr CreateFromEncoded(std::string_view value);

    static HexadecimalStringObjectPtr CreateFromDecoded(BufferPtr value);
    static HexadecimalStringObjectPtr CreateFromDecoded(const char * value);
    static HexadecimalStringObjectPtr CreateFromDecoded(std::string_view value);

    virtual bool IsDirty() const override {
        return (m_version > 0) || (_value->GetVersion() > 0);
    }

    virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Hexadecimal; }

    virtual BufferPtr GetValue() const override;
    virtual void SetValue(BufferPtr value) override;
    virtual void SetValue(std::string_view value) override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

    virtual HexadecimalStringObject* Clone(void) const override;

    virtual ~HexadecimalStringObject() = default;

private:
    BufferPtr GetRawValue() const;

    void SetRawValue(BufferPtr value);
    void SetRawValue(std::string_view value);

    BufferPtr _raw_value;
    mutable BufferPtr _value;
};

class LiteralStringObject : public StringObjectBase {
public:
    LiteralStringObject();
    LiteralStringObject(const LiteralStringObject&) = delete;

    static LiteralStringObjectPtr CreateFromEncoded(BufferPtr value);
    static LiteralStringObjectPtr CreateFromEncoded(const char * value);
    static LiteralStringObjectPtr CreateFromEncoded(std::string_view value);

    static LiteralStringObjectPtr CreateFromDecoded(BufferPtr value);
    static LiteralStringObjectPtr CreateFromDecoded(const char * value);
    static LiteralStringObjectPtr CreateFromDecoded(std::string_view value);

    virtual bool IsDirty() const override {
        return (m_version > 0) || (_value->GetVersion() > 0);
    }

    virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Literal; }
    virtual BufferPtr GetValue() const override;
    virtual void SetValue(BufferPtr value) override;
    virtual void SetValue(std::string_view value) override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

    virtual LiteralStringObject* Clone(void) const override;

    virtual ~LiteralStringObject() = default;

private:
    BufferPtr GetRawValue() const;
    BufferPtr GetRawValueDecoded() const;

    void SetRawValue(BufferPtr value);
    void SetRawValue(std::string_view value);

    BufferPtr _raw_value;
    mutable BufferPtr _value;
};

class StringObjectPtr : public Deferred<StringObjectBase> {
public:
    using Deferred<StringObjectBase>::Deferred;

public:
    StringObjectPtr();
};

} // syntax

inline bool operator==(const Deferred<syntax::StringObjectBase>& left, const Deferred<syntax::StringObjectBase>& right) {
    return *left == *right;
}

inline bool operator<(const Deferred<syntax::StringObjectBase>& left, const Deferred<syntax::StringObjectBase>& right) {
    return *left < *right;
}

} // vanillapdf

#endif /* _STRING_OBJECT_H */
