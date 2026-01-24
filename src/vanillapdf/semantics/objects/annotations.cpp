#include "precompiled.h"

#include "semantics/objects/annotations.h"
#include "semantics/objects/destinations.h"
#include "semantics/objects/document.h"
#include "semantics/objects/name_dictionary.h"
#include "semantics/objects/rectangle.h"

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

    // Update 27.9.2025:
    // In the file issue3188.pdf there is a link annotation with destination LI0.
    // The document catalog does not contain the entry Dests or Names, so we are not able to resolve this destination.
    // Foxit reader is able to read the document and in the white area it shows clickable cursor,
    // meaning the annotation is there. When this is clicked nothing happens.

    try {
        auto dest_obj = _obj->Find(constant::Name::Dest);
        auto destination = DestinationBase::ResolveDestination(dest_obj);
        result = destination;
        return true;
    }
    catch (ExceptionBase& ex) {
        spdlog::warn("Could not resolve link annotation destination: {}", ex.what());
        return false;
    }
}

// AnnotationBase property accessors

syntax::DictionaryObjectPtr AnnotationBase::CreateBaseDictionary(
    const syntax::NameObjectPtr& subtype, RectanglePtr rect) {
    syntax::DictionaryObjectPtr dict;
    dict->Insert(constant::Name::Type, constant::Name::Annot.Clone());
    dict->Insert(constant::Name::Subtype, subtype->Clone());
    dict->Insert(constant::Name::Rect, rect->GetObject());
    return dict;
}

bool AnnotationBase::GetRect(OutputRectanglePtr& result) const {
    if (!_obj->Contains(constant::Name::Rect)) {
        return false;
    }

    auto rect_obj = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IntegerObjectPtr>>(constant::Name::Rect);
    auto rect = make_deferred<Rectangle>(rect_obj);
    result = rect;
    return true;
}

void AnnotationBase::SetRect(RectanglePtr rect) {
    if (_obj->Contains(constant::Name::Rect)) {
        bool removed = _obj->Remove(constant::Name::Rect);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::Rect, rect->GetObject());
}

bool AnnotationBase::GetContents(syntax::LiteralStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::Contents)) {
        return false;
    }

    result = _obj->FindAs<syntax::LiteralStringObjectPtr>(constant::Name::Contents);
    return true;
}

void AnnotationBase::SetContents(syntax::LiteralStringObjectPtr contents) {
    if (_obj->Contains(constant::Name::Contents)) {
        bool removed = _obj->Remove(constant::Name::Contents);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::Contents, contents);
}

bool AnnotationBase::GetColor(syntax::ArrayObjectPtr<syntax::RealObjectPtr>& result) const {
    if (!_obj->Contains(constant::Name::C)) {
        return false;
    }

    result = _obj->FindAs<syntax::ArrayObjectPtr<syntax::RealObjectPtr>>(constant::Name::C);
    return true;
}

void AnnotationBase::SetColor(syntax::ArrayObjectPtr<syntax::RealObjectPtr> color) {
    if (_obj->Contains(constant::Name::C)) {
        bool removed = _obj->Remove(constant::Name::C);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::C, color);
}

// TextAnnotation Create methods

TextAnnotationPtr TextAnnotation::Create(RectanglePtr rect) {
    auto dict = CreateBaseDictionary(constant::Name::Text.Clone(), rect);
    return make_deferred<TextAnnotation>(dict);
}

TextAnnotationPtr TextAnnotation::Create(RectanglePtr rect, syntax::LiteralStringObjectPtr contents) {
    auto dict = CreateBaseDictionary(constant::Name::Text.Clone(), rect);
    dict->Insert(constant::Name::Contents, contents);
    return make_deferred<TextAnnotation>(dict);
}

// HighlightAnnotation methods

HighlightAnnotationPtr HighlightAnnotation::Create(RectanglePtr rect,
    syntax::MixedArrayObjectPtr quadPoints) {
    auto dict = CreateBaseDictionary(constant::Name::Highlight.Clone(), rect);
    dict->Insert(constant::Name::QuadPoints, quadPoints);
    return make_deferred<HighlightAnnotation>(dict);
}

bool HighlightAnnotation::GetQuadPoints(syntax::MixedArrayObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::QuadPoints)) {
        return false;
    }

    result = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::QuadPoints);
    return true;
}

void HighlightAnnotation::SetQuadPoints(syntax::MixedArrayObjectPtr quadPoints) {
    if (_obj->Contains(constant::Name::QuadPoints)) {
        bool removed = _obj->Remove(constant::Name::QuadPoints);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::QuadPoints, quadPoints);
}

// FreeTextAnnotation methods

FreeTextAnnotationPtr FreeTextAnnotation::Create(RectanglePtr rect,
    syntax::LiteralStringObjectPtr contents,
    syntax::LiteralStringObjectPtr defaultAppearance) {
    auto dict = CreateBaseDictionary(constant::Name::FreeText.Clone(), rect);
    dict->Insert(constant::Name::Contents, contents);
    dict->Insert(constant::Name::DA, defaultAppearance);
    return make_deferred<FreeTextAnnotation>(dict);
}

bool FreeTextAnnotation::GetDefaultAppearance(syntax::LiteralStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::DA)) {
        return false;
    }

    result = _obj->FindAs<syntax::LiteralStringObjectPtr>(constant::Name::DA);
    return true;
}

void FreeTextAnnotation::SetDefaultAppearance(syntax::LiteralStringObjectPtr da) {
    if (_obj->Contains(constant::Name::DA)) {
        bool removed = _obj->Remove(constant::Name::DA);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::DA, da);
}

// PageAnnotations methods

PageAnnotations::PageAnnotations() {
    // Default constructor creates an empty array
}

void PageAnnotations::Append(AnnotationPtr annotation) {
    _obj->Append(annotation->GetObject());
}

} // semantics
} // vanillapdf
