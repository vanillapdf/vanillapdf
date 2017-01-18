#include "precompiled.h"

#include "semantics/objects/annotations.h"
#include "semantics/objects/destinations.h"
#include "semantics/objects/document.h"
#include "semantics/objects/name_dictionary.h"

#include "semantics/utils/semantic_utils.h"

namespace gotchangpdf {
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
PageAnnotations::PageAnnotations(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {}

AnnotationBase::Type TextAnnotation::GetType() const noexcept { return Type::Text; }
AnnotationBase::Type LinkAnnotation::GetType() const noexcept { return Type::Link; }
AnnotationBase::Type FreeTextAnnotation::GetType() const noexcept { return Type::FreeText; }
AnnotationBase::Type LineAnnotation::GetType() const noexcept { return Type::Line; }
AnnotationBase::Type SquareAnnotation::GetType() const noexcept { return Type::Square; }
AnnotationBase::Type CircleAnnotation::GetType() const noexcept { return Type::Circle; }
AnnotationBase::Type PolygonAnnotation::GetType() const noexcept { return Type::Polygon; }
AnnotationBase::Type PolyLineAnnotation::GetType() const noexcept { return Type::PolyLine; }
AnnotationBase::Type HighlightAnnotation::GetType() const noexcept { return Type::Highlight; }
AnnotationBase::Type UnderlineAnnotation::GetType() const noexcept { return Type::Underline; }
AnnotationBase::Type SquigglyAnnotation::GetType() const noexcept { return Type::Squiggly; }
AnnotationBase::Type StrikeOutAnnotation::GetType() const noexcept { return Type::StrikeOut; }
AnnotationBase::Type RubberStampAnnotation::GetType() const noexcept { return Type::RubberStamp; }
AnnotationBase::Type CaretAnnotation::GetType() const noexcept { return Type::Caret; }
AnnotationBase::Type InkAnnotation::GetType() const noexcept { return Type::Ink; }
AnnotationBase::Type PopupAnnotation::GetType() const noexcept { return Type::Popup; }
AnnotationBase::Type FileAttachmentAnnotation::GetType() const noexcept { return Type::FileAttachment; }
AnnotationBase::Type SoundAnnotation::GetType() const noexcept { return Type::Sound; }
AnnotationBase::Type MovieAnnotation::GetType() const noexcept { return Type::Movie; }
AnnotationBase::Type WidgetAnnotation::GetType() const noexcept { return Type::Widget; }
AnnotationBase::Type ScreenAnnotation::GetType() const noexcept { return Type::Screen; }
AnnotationBase::Type PrinterMarkAnnotation::GetType() const noexcept { return Type::PrinterMark; }
AnnotationBase::Type TrapNetworkAnnotation::GetType() const noexcept { return Type::TrapNetwork; }
AnnotationBase::Type WatermarkAnnotation::GetType() const noexcept { return Type::Watermark; }
AnnotationBase::Type TripleDAnnotation::GetType() const noexcept { return Type::TripleD; }
AnnotationBase::Type RedactionAnnotation::GetType() const noexcept { return Type::Redaction; }

types::uinteger PageAnnotations::Size() const { return _obj->Size(); }
AnnotationPtr PageAnnotations::At(types::uinteger index) const {
	auto obj = _obj->At(index);

	assert(syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(obj) && "Linked annotation is not dictionary");
	if (!syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(obj)) {
		throw GeneralException("Invalid annotation reference");
	}

	auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(obj);
	auto unique = AnnotationBase::Create(dict);
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
		auto catalog = document->GetDocumentCatalog();

		OutputNameDictionaryPtr name_dictionary_ptr;
		bool has_dictionary = catalog->Names(name_dictionary_ptr);
		if (!has_dictionary) {
			return false;
		}

		auto name_dictionary = name_dictionary_ptr.GetValue();

		OutputNameTreePtr<DestinationPtr> destinations_ptr;
		bool contains = name_dictionary->Dests(destinations_ptr);
		if (!contains) {
			return false;
		}

		auto destinations = destinations_ptr.GetValue();
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
		auto catalog = document->GetDocumentCatalog();

		OutputNamedDestinationsPtr destinations_ptr;
		bool has_destinations = catalog->Destinations(destinations_ptr);
		if (!has_destinations) {
			return false;
		}

		auto destinations = destinations_ptr.GetValue();

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
} // gotchangpdf
