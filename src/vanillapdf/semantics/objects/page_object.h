#ifndef _PAGE_H
#define _PAGE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/page_node_base.h"
#include "semantics/objects/rectangle.h"
#include "semantics/objects/resource_dictionary.h"
#include "semantics/objects/page_contents.h"
#include "semantics/objects/annotations.h"

namespace vanillapdf {
namespace semantics {

class PageObject : public PageNodeBase {
public:
    explicit PageObject(syntax::DictionaryObjectPtr obj);

    static PageObjectPtr Create(DocumentPtr document);
    static PageObjectPtr Create(syntax::DictionaryObjectPtr obj);

    bool GetResources(OutputResourceDictionaryPtr& result) const;
    void SetResources(ResourceDictionaryPtr resources);

    // Page boundaries (14.11.2). The getters read only the page's own entry:
    // /MediaBox, /CropBox and /Rotate are inheritable (Table 30), but a value
    // inherited from the page tree is not resolved (#511).
    bool GetMediaBox(OutputRectanglePtr& result) const;
    void SetMediaBox(RectanglePtr);

    bool GetCropBox(OutputRectanglePtr& result) const;
    void SetCropBox(RectanglePtr value);

    bool GetBleedBox(OutputRectanglePtr& result) const;
    void SetBleedBox(RectanglePtr value);

    bool GetTrimBox(OutputRectanglePtr& result) const;
    void SetTrimBox(RectanglePtr value);

    bool GetArtBox(OutputRectanglePtr& result) const;
    void SetArtBox(RectanglePtr value);

    // Clockwise rotation in degrees applied when the page is displayed
    // or printed. The getter returns the stored value unmodified, which in
    // a file may be negative, 360 or more, or malformed. The setter stores
    // the value as given and rejects one that is not a multiple of 90.
    bool GetRotate(types::big_int& result) const;
    void SetRotate(types::big_int value);

    // Size of a default user space unit in multiples of 1/72 inch;
    // the value has to be positive
    bool GetUserUnit(types::real& result) const;
    void SetUserUnit(types::real value);

    // Transparency group attributes of the page group (11.6.6)
    bool GetGroup(OutputTransparencyGroupPtr& result) const;
    void SetGroup(TransparencyGroupPtr value);

    bool GetAnnotations(OutputPageAnnotationsPtr& result) const;
    void SetAnnotations(PageAnnotationsPtr annots);

    bool GetContents(OutputPageContentsPtr& result) const;
    void SetContents(PageContentsPtr contents);

    virtual NodeType GetNodeType(void) const noexcept override { return NodeType::Object; }

private:
    bool GetRectangle(const syntax::NameObject& name, OutputRectanglePtr& result) const;
    void SetRectangle(const syntax::NameObject& name, RectanglePtr value);

    mutable OutputPageContentsPtr m_contents;
};

class PageObjectExtensions {
public:
    static bool GetInheritableResources(const PageObject& obj, OutputResourceDictionaryPtr& result);
};

} // semantics
} // vanillapdf

#endif /* _PAGE_H */
