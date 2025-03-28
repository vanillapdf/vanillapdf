#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

#include "utils/buffer.h"
#include "utils/deferred.h"

namespace vanillapdf {
namespace syntax {

class NameObject : public ContainableObject, public IModifyObserver {
public:
    NameObject();
    //NameObject(const NameObject&) = delete;

    static NameObjectPtr CreateFromEncoded(BufferPtr value);
    static NameObjectPtr CreateFromEncoded(const char* value);
    static NameObjectPtr CreateFromEncoded(const std::string& value);

    static NameObjectPtr CreateFromDecoded(BufferPtr value);
    static NameObjectPtr CreateFromDecoded(const char* value);
    static NameObjectPtr CreateFromDecoded(const std::string& value);

    virtual void ObserveeChanged(const IModifyObservable*) override;

    BufferPtr GetValue() const;
    void SetValue(BufferPtr value);

    bool Equals(const NameObject& other) const;
    virtual bool Equals(ObjectPtr other) const override;

    virtual Object::Type GetObjectType(void) const noexcept override;
    virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;
    virtual std::string ToString(void) const override;

    virtual size_t Hash() const override;
    virtual NameObject* Clone(void) const override;

    virtual ~NameObject();

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
