#ifndef _SEMANTICS_FWD_H
#define _SEMANTICS_FWD_H

#include "syntax_fwd.h"

namespace gotchangpdf
{
	namespace semantics
	{
		template <typename KeyT, typename ValueT>
		class TreeBase;
		template <typename KeyT, typename ValueT>
		using TreeBasePtr = Deferred<TreeBase<KeyT, ValueT>>;

		template <typename ValueT>
		class NameTree;
		template <typename ValueT>
		using NameTreePtr = Deferred<NameTree<ValueT>>;

		template <typename ValueT>
		class NumberTree;
		template <typename ValueT>
		using NumberTreePtr = Deferred<NumberTree<ValueT>>;

		class Document; using DocumentPtr = Deferred<Document>;
		class Catalog; using CatalogPtr = Deferred<Catalog>;
		class DocumentInfo; using DocumentInfoPtr = Deferred<DocumentInfo>; using OutputDocumentInfoPtr = OutputPointer<DocumentInfoPtr>;
		class PageNodeBase; using PageNodeBasePtr = Deferred<PageNodeBase>;
		class PageTree; using PageTreePtr = Deferred<PageTree>;
		class PageTreeNode; using PageTreeNodePtr = Deferred<PageTreeNode>;
		class PageObject; using PageObjectPtr = Deferred<PageObject>;

		class ResourceDictionary; using ResourceDictionaryPtr = Deferred<ResourceDictionary>;

		class Rectangle; using RectanglePtr = Deferred<Rectangle>;
		class Date; using DatePtr = Deferred<Date>; using OutputDatePtr = OutputPointer<DatePtr>;
		class Rectangle;
		class Tree;

		class ContentStream; using ContentStreamPtr = Deferred<ContentStream>;
		class Contents; using ContentsPtr = Deferred<Contents>; using OutputContentsPtr = OutputPointer<ContentsPtr>;

		class OutlineItemColor; using OutlineItemColorPtr = Deferred<OutlineItemColor>; using OutputOutlineItemColorPtr = OutputPointer<OutlineItemColorPtr>;
		class OutlineItemFlags; using OutlineItemFlagsPtr = Deferred<OutlineItemFlags>; using OutputOutlineItemFlagsPtr = OutputPointer<OutlineItemFlagsPtr>;
		class OutlineBase; using OutlineBasePtr = Deferred<OutlineBase>;
		class Outline; using OutlinePtr = Deferred<Outline>; using OutputOutlinePtr = OutputPointer<OutlinePtr>;
		class OutlineItem; using OutlineItemPtr = Deferred<OutlineItem>; using OutputOutlineItemPtr = OutputPointer<OutlineItemPtr>;
		class PageRange; using PageRangePtr = Deferred<PageRange>; using OutputPageRangePtr = OutputPointer<PageRangePtr>;
		class ViewerPreferences; using ViewerPreferencesPtr = Deferred<ViewerPreferences>; using OutputViewerPreferencesPtr = OutputPointer<ViewerPreferencesPtr>;
		class DeveloperExtension; using DeveloperExtensionPtr = Deferred<DeveloperExtension>; using OutputDeveloperExtensionPtr = OutputPointer<DeveloperExtensionPtr>;
		class DeveloperExtensions; using DeveloperExtensionsPtr = Deferred<DeveloperExtensions>; using OutputDeveloperExtensionsPtr = OutputPointer<DeveloperExtensionsPtr>;
		class PageLabel; using PageLabelPtr = Deferred<PageLabel>; using OutputPageLabelPtr = OutputPointer<PageLabelPtr>;
		class PageLabels; using PageLabelsPtr = Deferred<PageLabels>; using OutputPageLabelsPtr = OutputPointer<PageLabelsPtr>;
	}
}

#endif /* _SEMANTICS_FWD_H */
