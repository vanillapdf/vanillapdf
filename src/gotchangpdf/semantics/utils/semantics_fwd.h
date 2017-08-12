#ifndef _SEMANTICS_FWD_H
#define _SEMANTICS_FWD_H

#include "utils/deferred.h"

#include "syntax/utils/syntax_fwd.h"
#include "syntax/utils/output_pointer.h"

namespace gotchangpdf {
namespace semantics {

using syntax::OutputPointer;

template <typename KeyT, typename ValueT>
class TreeBase;

template <typename KeyT, typename ValueT>
using TreeBasePtr = DeferredContainer<TreeBase<KeyT, ValueT>>;

class TreeNodeBase; using TreeNodeBasePtr = Deferred<TreeNodeBase>;
class TreeNodeRoot; using TreeNodeRootPtr = Deferred<TreeNodeRoot>;
class TreeNodeIntermediate; using TreeNodeIntermediatePtr = Deferred<TreeNodeIntermediate>;
class TreeNodeLeaf; using TreeNodeLeafPtr = Deferred<TreeNodeLeaf>;

template <typename ValueT>
class NameTree;

template <typename ValueT>
using NameTreePtr = DeferredContainer<NameTree<ValueT>>;

template <typename ValueT>
using OutputNameTreePtr = OutputPointer<NameTreePtr<ValueT>>;

template <typename ValueT>
class NumberTree;
template <typename ValueT>
using NumberTreePtr = DeferredContainer<NumberTree<ValueT>>;

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
class NameDictionary; using NameDictionaryPtr = Deferred<NameDictionary>; using OutputNameDictionaryPtr = OutputPointer<NameDictionaryPtr>;

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
class NamedDestinations; using NamedDestinationsPtr = DeferredContainer<NamedDestinations>; using OutputNamedDestinationsPtr = OutputPointer<NamedDestinationsPtr>;

// Annotations
class AnnotationBase; using AnnotationPtr = Deferred<AnnotationBase>;
class LinkAnnotation; using LinkAnnotationPtr = Deferred<LinkAnnotation>;
class PageAnnotations; using PageAnnotationsPtr = Deferred<PageAnnotations>; using OutputPageAnnotationsPtr = OutputPointer<PageAnnotationsPtr>;

// Fonts
class FontMap; using FontMapPtr = Deferred<FontMap>; using OutputFontMapPtr = OutputPointer<FontMapPtr>;
class FontBase; using FontPtr = Deferred<FontBase>; using OutputFontPtr = OutputPointer<FontPtr>;
class CompositeFont; using CompositeFontPtr = Deferred<CompositeFont>;

// Character maps
class CharacterMapBase; using CharacterMapBasePtr = Deferred<CharacterMapBase>;
class EmbeddedCharacterMap; using EmbeddedCharacterMapPtr = Deferred<EmbeddedCharacterMap>;
class UnicodeCharacterMap; using UnicodeCharacterMapPtr = Deferred<UnicodeCharacterMap>; using OuputUnicodeCharacterMapPtr = OutputPointer<UnicodeCharacterMapPtr>;

// Fields
class Field; using FieldPtr = Deferred<Field>;
class FieldCollection; using FieldCollectionPtr = Deferred<FieldCollection>;
class ButtonField; using ButtonFieldPtr = Deferred<ButtonField>;
class TextField; using TextFieldPtr = Deferred<TextField>;
class ChoiceField; using ChoiceFieldPtr = Deferred<ChoiceField>;
class SignatureField; using SignatureFieldPtr = Deferred<SignatureField>;

// Digital signatures
class DigitalSignature; using DigitalSignaturePtr = Deferred<DigitalSignature>; using OuputDigitalSignaturePtr = OutputPointer<DigitalSignaturePtr>;
class ByteRange; using ByteRangePtr = Deferred<ByteRange>;
class ByteRangeCollection; using ByteRangeCollectionPtr = Deferred<ByteRangeCollection>; using OuputByteRangeCollectionPtr = OutputPointer<ByteRangeCollectionPtr>;

// Interactive forms
class InteractiveForm; using InteractiveFormPtr = Deferred<InteractiveForm>; using OuputInteractiveFormPtr = OutputPointer<InteractiveFormPtr>;

// Utils
class DocumentSigner; using DocumentSignerPtr = Deferred<DocumentSigner>;
class SigningOptions; using SigningOptionsPtr = Deferred<SigningOptions>;

} // semantics
} // gotchangpdf

#endif /* _SEMANTICS_FWD_H */
