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
		class Catalog; using CatalogPtr = Deferred<Catalog>; using OutputCatalogPtr = OutputPointer<CatalogPtr>;
		class DocumentInfo; using DocumentInfoPtr = Deferred<DocumentInfo>; using OutputDocumentInfoPtr = OutputPointer<DocumentInfoPtr>;
		class PageNodeBase; using PageNodeBasePtr = Deferred<PageNodeBase>;
		class PageTree; using PageTreePtr = Deferred<PageTree>; using OutputPageTreePtr = OutputPointer<PageTreePtr>;
		class PageTreeNode; using PageTreeNodePtr = Deferred<PageTreeNode>;
		class PageObject; using PageObjectPtr = Deferred<PageObject>; using OutputPageObjectPtr = OutputPointer<PageObjectPtr>;

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

		// Destinations
		class DestinationBase; using DestinationPtr = Deferred<DestinationBase>; using OutputDestinationPtr = OutputPointer<DestinationPtr>;
		class XYZDestination; using XYZDestinationPtr = Deferred<XYZDestination>;
		class FitDestination; using FitDestinationPtr = Deferred<FitDestination>;
		class FitHorizontalDestination; using FitHorizontalDestinationPtr = Deferred<FitHorizontalDestination>;
		class FitVerticalDestination; using FitVerticalDestinationPtr = Deferred<FitVerticalDestination>;
		class FitRectangleDestination; using FitRectangleDestinationPtr = Deferred<FitRectangleDestination>;
		class FitBoundingBoxDestination; using FitBoundingBoxDestinationPtr = Deferred<FitBoundingBoxDestination>;
		class FitBoundingBoxHorizontalDestination; using FitBoundingBoxHorizontalDestinationPtr = Deferred<FitBoundingBoxHorizontalDestination>;
		class FitBoundingBoxVerticalDestination; using FitBoundingBoxVerticalDestinationPtr = Deferred<FitBoundingBoxVerticalDestination>;
		class NamedDestinations; using NamedDestinationsPtr = Deferred<NamedDestinations>; using OutputNamedDestinationsPtr = OutputPointer<NamedDestinationsPtr>;

		// Annotations
		class AnnotationBase; using AnnotationPtr = Deferred<AnnotationBase>;
		class LinkAnnotation; using LinkAnnotationPtr = Deferred<LinkAnnotation>;
		class PageAnnotations; using PageAnnotationsPtr = Deferred<PageAnnotations>; using OutputPageAnnotationsPtr = OutputPointer<PageAnnotationsPtr>;
	}
}

#endif /* _SEMANTICS_FWD_H */
