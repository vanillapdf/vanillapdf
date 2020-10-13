#include "precompiled.h"

#include "semantics/objects/annotations.h"
#include "semantics/objects/destinations.h"
#include "semantics/objects/document.h"
#include "semantics/objects/name_dictionary.h"

#include "semantics/utils/semantic_utils.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

AnnotationBase::AnnotationBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
TextAnnotation::TextAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
LinkAnnotation::LinkAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
FreeTextAnnotation::FreeTextAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
LineAnnotation::LineAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
SquareAnnotation::SquareAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
CircleAnnotation::CircleAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
PolygonAnnotation::PolygonAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
PolyLineAnnotation::PolyLineAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
HighlightAnnotation::HighlightAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
UnderlineAnnotation::UnderlineAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
SquigglyAnnotation::SquigglyAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
StrikeOutAnnotation::StrikeOutAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
RubberStampAnnotation::RubberStampAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
CaretAnnotation::CaretAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
InkAnnotation::InkAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
PopupAnnotation::PopupAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
FileAttachmentAnnotation::FileAttachmentAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
SoundAnnotation::SoundAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
MovieAnnotation::MovieAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
WidgetAnnotation::WidgetAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
ScreenAnnotation::ScreenAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
PrinterMarkAnnotation::PrinterMarkAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
TrapNetworkAnnotation::TrapNetworkAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
WatermarkAnnotation::WatermarkAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
TripleDAnnotation::TripleDAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
RedactionAnnotation::RedactionAnnotation(syntax::DictionaryObjectPtr root) : AnnotationBase(root) {}
PageAnnotations::PageAnnotations(syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> root) : HighLevelObject(root) {}

types::size_type PageAnnotations::GetSize() const {
	return _obj->GetSize();
}

AnnotationPtr PageAnnotations::At(types::size_type index) const {
	auto obj = _obj->GetValue(index);
	auto unique = AnnotationBase::Create(obj);
	auto raw_ptr = unique.release();
	return AnnotationPtr(raw_ptr);
}

std::unique_ptr<AnnotationBase> AnnotationBase::Create(syntax::DictionaryObjectPtr root) {
	// Verify type, if it is included
	if (root->Contains(constant::Name::Type)) {
		syntax::ObjectPtr type_obj = root->Find(constant::Name::Type);
		if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
			throw GeneralException("Invalid annotation type");
		}

		syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

		if (type != constant::Name::Annot) {
			throw GeneralException("Invalid annotation type");
		}
	}

	if (!root->Contains(constant::Name::Subtype)) {
		throw GeneralException("Dictionary does not contain subtype");
	}

	syntax::ObjectPtr subtype_obj = root->Find(constant::Name::Subtype);

	if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(subtype_obj)) {
		throw GeneralException("Invalid annotation subtype");
	}

	syntax::NameObjectPtr subtype = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(subtype_obj);

	if (subtype == constant::Name::Text) {
		return make_unique<TextAnnotation>(root);
	}

	if (subtype == constant::Name::Link) {
		return make_unique<LinkAnnotation>(root);
	}

	if (subtype == constant::Name::FreeText) {
		return make_unique<FreeTextAnnotation>(root);
	}

	if (subtype == constant::Name::Line) {
		return make_unique<LineAnnotation>(root);
	}
	if (subtype == constant::Name::Square) {
		return make_unique<SquareAnnotation>(root);
	}

	if (subtype == constant::Name::Circle) {
		return make_unique<CircleAnnotation>(root);
	}

	if (subtype == constant::Name::Polygon) {
		return make_unique<PolygonAnnotation>(root);
	}

	if (subtype == constant::Name::PolyLine) {
		return make_unique<PolyLineAnnotation>(root);
	}

	if (subtype == constant::Name::Highlight) {
		return make_unique<HighlightAnnotation>(root);
	}

	if (subtype == constant::Name::Underline) {
		return make_unique<UnderlineAnnotation>(root);
	}

	if (subtype == constant::Name::Squiggly) {
		return make_unique<SquigglyAnnotation>(root);
	}

	if (subtype == constant::Name::StrikeOut) {
		return make_unique<StrikeOutAnnotation>(root);
	}

	if (subtype == constant::Name::RubberStamp) {
		return make_unique<RubberStampAnnotation>(root);
	}

	if (subtype == constant::Name::Caret) {
		return make_unique<CaretAnnotation>(root);
	}

	if (subtype == constant::Name::Ink) {
		return make_unique<InkAnnotation>(root);
	}

	if (subtype == constant::Name::Popup) {
		return make_unique<PopupAnnotation>(root);
	}

	if (subtype == constant::Name::FileAttachment) {
		return make_unique<FileAttachmentAnnotation>(root);
	}

	if (subtype == constant::Name::Sound) {
		return make_unique<SoundAnnotation>(root);
	}

	if (subtype == constant::Name::Movie) {
		return make_unique<MovieAnnotation>(root);
	}

	if (subtype == constant::Name::Widget) {
		return make_unique<WidgetAnnotation>(root);
	}

	if (subtype == constant::Name::Screen) {
		return make_unique<ScreenAnnotation>(root);
	}

	if (subtype == constant::Name::PrinterMark) {
		return make_unique<PrinterMarkAnnotation>(root);
	}

	if (subtype == constant::Name::TrapNetwork) {
		return make_unique<TrapNetworkAnnotation>(root);
	}

	if (subtype == constant::Name::Watermark) {
		return make_unique<WatermarkAnnotation>(root);
	}

	if (subtype == constant::Name::TripleD) {
		return make_unique<TripleDAnnotation>(root);
	}

	if (subtype == constant::Name::Redact) {
		return make_unique<RedactionAnnotation>(root);
	}

	throw GeneralException("Unknown annotation subtype");
}

bool LinkAnnotation::Destination(OutputDestinationPtr& result) const {
	if (!_obj->Contains(constant::Name::Dest)) {
		return false;
	}

	auto dest_obj = _obj->Find(constant::Name::Dest);
	if (syntax::ObjectUtils::IsType<syntax::MixedArrayObjectPtr>(dest_obj)) {
		auto array_obj = syntax::ObjectUtils::ConvertTo<syntax::MixedArrayObjectPtr>(dest_obj);
		auto destination = DestinationBase::Create(array_obj);
		result = destination.release();
		return true;
	}

	if (syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(dest_obj)) {
		auto dict_obj = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(dest_obj);
		auto destination = DestinationBase::Create(dict_obj);
		result = destination.release();
		return true;
	}

	if (syntax::ObjectUtils::IsType<syntax::StringObjectPtr>(dest_obj)) {
		auto document_ref = SemanticUtils::GetMappedDocument(_obj->GetFile());

		assert(!document_ref.IsEmpty() && "Document reference was not set");
		if (!document_ref.IsActive()) {
			return false;
		}

		DocumentPtr document = document_ref.GetReference();

		OutputCatalogPtr catalog_ptr;
		bool has_catalog = document->GetDocumentCatalog(catalog_ptr);
		if (!has_catalog) {
			return false;
		}

		OutputNameDictionaryPtr name_dictionary;
		bool has_dictionary = catalog_ptr->Names(name_dictionary);
		if (!has_dictionary) {
			return false;
		}

		OutputNameTreePtr<DestinationPtr> destinations;
		bool contains = name_dictionary->Dests(destinations);
		if (!contains) {
			return false;
		}

		auto destination_name = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(dest_obj);

		assert(destinations->Contains(destination_name) && "Referenced destination does not exist");
		if (!destinations->Contains(destination_name)) {
			return false;
		}

		result = destinations->Find(destination_name);
		return true;
	}

	if (syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(dest_obj)) {
		auto document_ref = SemanticUtils::GetMappedDocument(_obj->GetFile());

		assert(!document_ref.IsEmpty() && "Document reference was not set");
		if (!document_ref.IsActive()) {
			return false;
		}

		DocumentPtr document = document_ref.GetReference();

		OutputCatalogPtr catalog;
		bool has_catalog = document->GetDocumentCatalog(catalog);
		if (!has_catalog) {
			return false;
		}

		OutputNamedDestinationsPtr destinations;
		bool has_destinations = catalog->Destinations(destinations);
		if (!has_destinations) {
			return false;
		}

		auto destination_name = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(dest_obj);

		assert(destinations->Contains(destination_name) && "Referenced destination does not exist");
		if (!destinations->Contains(destination_name)) {
			return false;
		}

		result = destinations->Find(destination_name);
		return true;
	}

	throw GeneralException("Unknown link destination type");
}

} // semantics
} // vanillapdf
