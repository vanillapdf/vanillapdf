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
		class PageNodeBase; using PageNodeBasePtr = Deferred<PageNodeBase>;
		class PageTree; using PageTreePtr = Deferred<PageTree>;
		class PageTreeNode; using PageTreeNodePtr = Deferred<PageTreeNode>;
		class PageObject; using PageObjectPtr = Deferred<PageObject>;

		class ResourceDictionary; using ResourceDictionaryPtr = Deferred<ResourceDictionary>;

		class Rectangle; using RectanglePtr = Deferred<Rectangle>;
		class Date;
		class Rectangle;
		class Tree;

		class ContentStream; using ContentStreamPtr = Deferred<ContentStream>;
		class Contents; using ContentsPtr = Deferred<Contents>;

		class OutlineItemColor; using OutlineItemColorPtr = Deferred<OutlineItemColor>;
		class OutlineItemFlags; using OutlineItemFlagsPtr = Deferred<OutlineItemFlags>;
		class OutlineBase; using OutlineBasePtr = Deferred<OutlineBase>;
		class Outline; using OutlinePtr = Deferred<Outline>;
		class OutlineItem; using OutlineItemPtr = Deferred<OutlineItem>;
		class PageRange; using PageRangePtr = Deferred<PageRange>;
		class ViewerPreferences; using ViewerPreferencesPtr = Deferred<ViewerPreferences>;
		class DeveloperExtensions; using DeveloperExtensionsPtr = Deferred<DeveloperExtensions>;
		class PageLabel; using PageLabelPtr = Deferred<PageLabel>;
		class PageLabels; using PageLabelsPtr = Deferred<PageLabels>;

		namespace contents
		{
			class ContentObjectBase; using ContentObjectBasePtr = Deferred<ContentObjectBase>;
			class OperationBeginText; using OperationBeginTextPtr = Deferred<OperationBeginText>;
			class OperationEndText; using OperationEndTextPtr = Deferred<OperationEndText>;
			class OperationTextShow; using OperationTextShowPtr = Deferred<OperationTextShow>;
			class TextObject; using TextObjectPtr = Deferred<TextObject>;
		}
	}
}

#endif /* _SEMANTICS_FWD_H */
