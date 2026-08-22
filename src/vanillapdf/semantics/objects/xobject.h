#ifndef _XOBJECT_H
#define _XOBJECT_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/stream_object.h"

namespace vanillapdf {
namespace semantics {

class XObjectBase : public HighLevelObject<syntax::StreamObjectPtr> {
public:
    enum class Type {
        Undefined = 0,
        Form,
        Image,
        PostScript
    };

public:
    explicit XObjectBase(syntax::StreamObjectPtr obj);
    static XObjectPtr Create(syntax::StreamObjectPtr obj);

    virtual Type GetXObjectType() const noexcept = 0;
};

class FormXObject : public XObjectBase {
public:
    explicit FormXObject(syntax::StreamObjectPtr obj);
    virtual Type GetXObjectType() const noexcept override;

    // Creates an empty form XObject backed by a new indirect stream object.
    // Form XObjects are referenced indirectly (e.g. from an annotation's /AP
    // entry), so the stream has to own an xref entry - this registers one
    // within the document's file.
    static FormXObjectPtr Create(DocumentPtr document);

    // Required
    bool GetBoundingBox(OutputRectanglePtr& result) const;
    void SetBoundingBox(RectanglePtr value);

    // Optional
    bool GetMatrix(syntax::MixedArrayObjectPtr& result) const;
    void SetMatrix(syntax::MixedArrayObjectPtr value);
    bool GetResources(OutputResourceDictionaryPtr& result) const;
    void SetResources(syntax::DictionaryObjectPtr value);
};

class ImageXObject : public XObjectBase {
public:
    explicit ImageXObject(syntax::StreamObjectPtr obj);
    virtual Type GetXObjectType() const noexcept override;
};

class PostScriptXObject : public XObjectBase {
public:
    explicit PostScriptXObject(syntax::StreamObjectPtr obj);
    virtual Type GetXObjectType() const noexcept override;
};

inline XObjectBase::Type FormXObject::GetXObjectType() const noexcept { return Type::Form; }
inline XObjectBase::Type ImageXObject::GetXObjectType() const noexcept { return Type::Image; }
inline XObjectBase::Type PostScriptXObject::GetXObjectType() const noexcept { return Type::PostScript; }

} // semantics
} // vanillapdf

#endif /* _XOBJECT_H */
