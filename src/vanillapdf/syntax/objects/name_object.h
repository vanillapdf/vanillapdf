#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

#include "utils/buffer.h"
#include "utils/deferred.h"

namespace vanillapdf {
namespace syntax {

class NameObject : public ContainableObject {
public:
    NameObject() = default;
    //NameObject(const NameObject&) = delete;

    static NameObjectPtr CreateFromEncoded(BufferPtr value);
    static NameObjectPtr CreateFromEncoded(std::string_view value);

    static NameObjectPtr CreateFromDecoded(BufferPtr value);
    static NameObjectPtr CreateFromDecoded(std::string_view value);

    virtual bool IsDirty() const override {
        return (m_version > 0) || (_value->GetVersion() > 0);
    }

    BufferPtr GetValue() const;
    void SetValue(BufferPtr value);
    void SetValue(std::string_view value);

    bool Equals(const NameObject& other) const;
    virtual bool Equals(ObjectPtr other) const override;

    virtual Object::Type GetObjectType(void) const noexcept override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;
    virtual std::string ToString(void) const override;

    virtual size_t Hash() const override;
    virtual NameObject* Clone(void) const override;

    virtual ~NameObject() = default;

private:
    BufferPtr _value;

    std::string GetHexadecimalNotation(char ch) const;
};

} // syntax

bool operator==(const syntax::NameObject& left, const syntax::NameObject& right);
bool operator!=(const syntax::NameObject& left, const syntax::NameObject& right);
bool operator<(const syntax::NameObject& left, const syntax::NameObject& right);

bool operator==(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right);
bool operator!=(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right);
bool operator<(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right);

} // vanillapdf

namespace std {

template <> struct hash<vanillapdf::syntax::NameObject> {
    size_t operator()(const vanillapdf::syntax::NameObject& name) const;
};

} // std

#endif /* _NAME_OBJECT_H */
