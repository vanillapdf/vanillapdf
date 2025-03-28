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

    virtual StringType GetStringType(void) const noexcept = 0;
    virtual Object::Type GetObjectType(void) const noexcept override { return Object::Type::String; }

    bool Equals(const StringObjectBase& other) const { return GetValue() == other.GetValue(); }
    bool operator==(const StringObjectBase& other) const { return Equals(other); }
    bool operator<(const StringObjectBase& other) const { return GetValue() < other.GetValue(); }

    virtual bool Equals(ObjectPtr other) const override;
    virtual size_t Hash() const override;

protected:
    // The library interface wants to be thread-safe as much as possible
    // Even though the are currently no cases for multi-thread access
    // to the dictonary, let's try to be visionary and prepare for this
    std::shared_ptr<std::recursive_mutex> _access_lock;
};

class HexadecimalStringObject : public StringObjectBase, public IModifyObserver {
public:
    HexadecimalStringObject();
    HexadecimalStringObject(const HexadecimalStringObject&) = delete;

    static HexadecimalStringObjectPtr CreateFromEncoded(BufferPtr value);
    static HexadecimalStringObjectPtr CreateFromEncoded(const char * value);
    static HexadecimalStringObjectPtr CreateFromEncoded(const std::string& value);

    static HexadecimalStringObjectPtr CreateFromDecoded(BufferPtr value);
    static HexadecimalStringObjectPtr CreateFromDecoded(const char * value);
    static HexadecimalStringObjectPtr CreateFromDecoded(const std::string& value);

    virtual void ObserveeChanged(const IModifyObservable*) override;

    virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Hexadecimal; }

    virtual BufferPtr GetValue() const override;
    virtual void SetValue(BufferPtr value) override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

    virtual HexadecimalStringObject* Clone(void) const override;

    virtual ~HexadecimalStringObject();

private:
    BufferPtr GetRawValue() const;
    void SetRawValue(BufferPtr value);

    BufferPtr _raw_value;
    mutable BufferPtr _value;
};

class LiteralStringObject : public StringObjectBase, public IModifyObserver {
public:
    LiteralStringObject();
    LiteralStringObject(const LiteralStringObject&) = delete;

    static LiteralStringObjectPtr CreateFromEncoded(BufferPtr value);
    static LiteralStringObjectPtr CreateFromEncoded(const char * value);
    static LiteralStringObjectPtr CreateFromEncoded(const std::string& value);

    static LiteralStringObjectPtr CreateFromDecoded(BufferPtr value);
    static LiteralStringObjectPtr CreateFromDecoded(const char * value);
    static LiteralStringObjectPtr CreateFromDecoded(const std::string& value);

    virtual void ObserveeChanged(const IModifyObservable*) override;

    virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Literal; }
    virtual BufferPtr GetValue() const override;
    virtual void SetValue(BufferPtr value) override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

    virtual LiteralStringObject* Clone(void) const override;

    virtual ~LiteralStringObject();

private:
    BufferPtr GetRawValue() const;
    BufferPtr GetRawValueDecoded() const;
    void SetRawValue(BufferPtr value);

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
