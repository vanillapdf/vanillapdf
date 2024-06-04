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

	bool GetMediaBox(OutputRectanglePtr& result) const;
	void SetMediaBox(RectanglePtr);

	bool GetAnnotations(OutputPageAnnotationsPtr& result) const;
	void SetAnnotations(PageAnnotationsPtr annots);

	bool GetContents(OutputPageContentsPtr& result) const;
	void SetContents(PageContentsPtr contents);

	virtual NodeType GetNodeType(void) const noexcept override { return NodeType::Object; }

private:
	mutable OutputPageContentsPtr m_contents;
};

class PageObjectExtensions {
public:
	static bool GetInheritableResources(const PageObject& obj, OutputResourceDictionaryPtr& result);
};

} // semantics
} // vanillapdf

#endif /* _PAGE_H */
