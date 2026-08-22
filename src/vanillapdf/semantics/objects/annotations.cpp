#include "precompiled.h"

#include "semantics/objects/annotations.h"
#include "semantics/objects/actions.h"
#include "semantics/objects/color.h"
#include "semantics/objects/date.h"
#include "semantics/objects/destinations.h"
#include "semantics/objects/document.h"
#include "semantics/objects/xobject.h"
#include "semantics/objects/name_dictionary.h"
#include "semantics/objects/rectangle.h"

#include "semantics/utils/semantic_utils.h"

#include "syntax/exceptions/syntax_exceptions.h"
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
            throw syntax::ObjectMissingException("Invalid annotation type");
        }

        syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

        if (type != constant::Name::Annot) {
            throw syntax::ObjectMissingException("Invalid annotation type");
        }
    }

    if (!root->Contains(constant::Name::Subtype)) {
        throw syntax::ObjectMissingException("Dictionary does not contain subtype");
    }

    syntax::ObjectPtr subtype_obj = root->Find(constant::Name::Subtype);

    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(subtype_obj)) {
        throw syntax::ObjectMissingException("Invalid annotation subtype");
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

    throw syntax::ObjectMissingException("Unknown annotation subtype");
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

    auto rect_obj = _obj->FindAs<syntax::ArrayObjectPtr<syntax::RealObjectPtr>>(constant::Name::Rect);
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

bool AnnotationBase::GetContents(syntax::OutputLiteralStringObjectPtr& result) const {
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

bool AnnotationBase::GetColor(OutputColorPtr& result) const {
    if (!_obj->Contains(constant::Name::C)) {
        return false;
    }

    auto color_obj = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::C);
    result = make_deferred<Color>(color_obj);
    return true;
}

void AnnotationBase::SetColor(ColorPtr color) {
    if (_obj->Contains(constant::Name::C)) {
        bool removed = _obj->Remove(constant::Name::C);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::C, color->GetObject());
}

// TextAnnotation - Markup annotation properties (Table 170)

bool TextAnnotation::GetAuthor(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

void TextAnnotation::SetAuthor(syntax::StringObjectPtr author) {
    if (_obj->Contains(constant::Name::T)) {
        bool removed = _obj->Remove(constant::Name::T);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::T, author);
}

bool TextAnnotation::GetModificationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::M)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
    result = make_deferred<Date>(date_obj);
    return true;
}

void TextAnnotation::SetModificationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::M)) {
        bool removed = _obj->Remove(constant::Name::M);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::M, date->GetObject());
}

bool TextAnnotation::GetCreationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::CreationDate)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
    result = make_deferred<Date>(date_obj);
    return true;
}

void TextAnnotation::SetCreationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::CreationDate)) {
        bool removed = _obj->Remove(constant::Name::CreationDate);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::CreationDate, date->GetObject());
}

// HighlightAnnotation - Markup annotation properties (Table 170)

bool HighlightAnnotation::GetAuthor(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

void HighlightAnnotation::SetAuthor(syntax::StringObjectPtr author) {
    if (_obj->Contains(constant::Name::T)) {
        bool removed = _obj->Remove(constant::Name::T);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::T, author);
}

bool HighlightAnnotation::GetModificationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::M)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
    result = make_deferred<Date>(date_obj);
    return true;
}

void HighlightAnnotation::SetModificationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::M)) {
        bool removed = _obj->Remove(constant::Name::M);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::M, date->GetObject());
}

bool HighlightAnnotation::GetCreationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::CreationDate)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
    result = make_deferred<Date>(date_obj);
    return true;
}

void HighlightAnnotation::SetCreationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::CreationDate)) {
        bool removed = _obj->Remove(constant::Name::CreationDate);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::CreationDate, date->GetObject());
}

// FreeTextAnnotation - Markup annotation properties (Table 170)

bool FreeTextAnnotation::GetAuthor(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

void FreeTextAnnotation::SetAuthor(syntax::StringObjectPtr author) {
    if (_obj->Contains(constant::Name::T)) {
        bool removed = _obj->Remove(constant::Name::T);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::T, author);
}

bool FreeTextAnnotation::GetModificationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::M)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
    result = make_deferred<Date>(date_obj);
    return true;
}

void FreeTextAnnotation::SetModificationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::M)) {
        bool removed = _obj->Remove(constant::Name::M);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::M, date->GetObject());
}

bool FreeTextAnnotation::GetCreationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::CreationDate)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
    result = make_deferred<Date>(date_obj);
    return true;
}

void FreeTextAnnotation::SetCreationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::CreationDate)) {
        bool removed = _obj->Remove(constant::Name::CreationDate);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::CreationDate, date->GetObject());
}

AnnotationBase::Flags AnnotationBase::GetFlags() const {
    if (!_obj->Contains(constant::Name::F)) {
        return Flags::None;
    }

    auto flags_obj = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::F);
    return static_cast<Flags>(flags_obj->GetIntegerValue());
}

void AnnotationBase::SetFlags(Flags flags) {
    if (_obj->Contains(constant::Name::F)) {
        bool removed = _obj->Remove(constant::Name::F);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    auto flags_obj = make_deferred<syntax::IntegerObject>(static_cast<int32_t>(flags));
    _obj->Insert(constant::Name::F, flags_obj);
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

HighlightAnnotationPtr HighlightAnnotation::Create() {
    syntax::DictionaryObjectPtr dict;
    dict->Insert(constant::Name::Type, constant::Name::Annot.Clone());
    dict->Insert(constant::Name::Subtype, constant::Name::Highlight.Clone());
    return make_deferred<HighlightAnnotation>(dict);
}

HighlightAnnotationPtr HighlightAnnotation::CreateFromRect(RectanglePtr rect) {
    auto dict = CreateBaseDictionary(constant::Name::Highlight.Clone(), rect);
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

// UnderlineAnnotation methods

UnderlineAnnotationPtr UnderlineAnnotation::Create() {
    syntax::DictionaryObjectPtr dict;
    dict->Insert(constant::Name::Type, constant::Name::Annot.Clone());
    dict->Insert(constant::Name::Subtype, constant::Name::Underline.Clone());
    return make_deferred<UnderlineAnnotation>(dict);
}

UnderlineAnnotationPtr UnderlineAnnotation::CreateFromRect(RectanglePtr rect) {
    auto dict = CreateBaseDictionary(constant::Name::Underline.Clone(), rect);
    return make_deferred<UnderlineAnnotation>(dict);
}

bool UnderlineAnnotation::GetQuadPoints(syntax::MixedArrayObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::QuadPoints)) {
        return false;
    }

    result = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::QuadPoints);
    return true;
}

void UnderlineAnnotation::SetQuadPoints(syntax::MixedArrayObjectPtr quadPoints) {
    if (_obj->Contains(constant::Name::QuadPoints)) {
        bool removed = _obj->Remove(constant::Name::QuadPoints);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::QuadPoints, quadPoints);
}

// UnderlineAnnotation - Markup annotation properties (Table 170)

bool UnderlineAnnotation::GetAuthor(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

void UnderlineAnnotation::SetAuthor(syntax::StringObjectPtr author) {
    if (_obj->Contains(constant::Name::T)) {
        bool removed = _obj->Remove(constant::Name::T);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::T, author);
}

bool UnderlineAnnotation::GetModificationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::M)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
    result = make_deferred<Date>(date_obj);
    return true;
}

void UnderlineAnnotation::SetModificationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::M)) {
        bool removed = _obj->Remove(constant::Name::M);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::M, date->GetObject());
}

bool UnderlineAnnotation::GetCreationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::CreationDate)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
    result = make_deferred<Date>(date_obj);
    return true;
}

void UnderlineAnnotation::SetCreationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::CreationDate)) {
        bool removed = _obj->Remove(constant::Name::CreationDate);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::CreationDate, date->GetObject());
}

// SquigglyAnnotation methods

SquigglyAnnotationPtr SquigglyAnnotation::Create() {
    syntax::DictionaryObjectPtr dict;
    dict->Insert(constant::Name::Type, constant::Name::Annot.Clone());
    dict->Insert(constant::Name::Subtype, constant::Name::Squiggly.Clone());
    return make_deferred<SquigglyAnnotation>(dict);
}

SquigglyAnnotationPtr SquigglyAnnotation::CreateFromRect(RectanglePtr rect) {
    auto dict = CreateBaseDictionary(constant::Name::Squiggly.Clone(), rect);
    return make_deferred<SquigglyAnnotation>(dict);
}

bool SquigglyAnnotation::GetQuadPoints(syntax::MixedArrayObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::QuadPoints)) {
        return false;
    }

    result = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::QuadPoints);
    return true;
}

void SquigglyAnnotation::SetQuadPoints(syntax::MixedArrayObjectPtr quadPoints) {
    if (_obj->Contains(constant::Name::QuadPoints)) {
        bool removed = _obj->Remove(constant::Name::QuadPoints);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::QuadPoints, quadPoints);
}

// SquigglyAnnotation - Markup annotation properties (Table 170)

bool SquigglyAnnotation::GetAuthor(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

void SquigglyAnnotation::SetAuthor(syntax::StringObjectPtr author) {
    if (_obj->Contains(constant::Name::T)) {
        bool removed = _obj->Remove(constant::Name::T);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::T, author);
}

bool SquigglyAnnotation::GetModificationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::M)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
    result = make_deferred<Date>(date_obj);
    return true;
}

void SquigglyAnnotation::SetModificationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::M)) {
        bool removed = _obj->Remove(constant::Name::M);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::M, date->GetObject());
}

bool SquigglyAnnotation::GetCreationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::CreationDate)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
    result = make_deferred<Date>(date_obj);
    return true;
}

void SquigglyAnnotation::SetCreationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::CreationDate)) {
        bool removed = _obj->Remove(constant::Name::CreationDate);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::CreationDate, date->GetObject());
}

// StrikeOutAnnotation methods

StrikeOutAnnotationPtr StrikeOutAnnotation::Create() {
    syntax::DictionaryObjectPtr dict;
    dict->Insert(constant::Name::Type, constant::Name::Annot.Clone());
    dict->Insert(constant::Name::Subtype, constant::Name::StrikeOut.Clone());
    return make_deferred<StrikeOutAnnotation>(dict);
}

StrikeOutAnnotationPtr StrikeOutAnnotation::CreateFromRect(RectanglePtr rect) {
    auto dict = CreateBaseDictionary(constant::Name::StrikeOut.Clone(), rect);
    return make_deferred<StrikeOutAnnotation>(dict);
}

bool StrikeOutAnnotation::GetQuadPoints(syntax::MixedArrayObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::QuadPoints)) {
        return false;
    }

    result = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::QuadPoints);
    return true;
}

void StrikeOutAnnotation::SetQuadPoints(syntax::MixedArrayObjectPtr quadPoints) {
    if (_obj->Contains(constant::Name::QuadPoints)) {
        bool removed = _obj->Remove(constant::Name::QuadPoints);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::QuadPoints, quadPoints);
}

// StrikeOutAnnotation - Markup annotation properties (Table 170)

bool StrikeOutAnnotation::GetAuthor(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

void StrikeOutAnnotation::SetAuthor(syntax::StringObjectPtr author) {
    if (_obj->Contains(constant::Name::T)) {
        bool removed = _obj->Remove(constant::Name::T);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::T, author);
}

bool StrikeOutAnnotation::GetModificationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::M)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
    result = make_deferred<Date>(date_obj);
    return true;
}

void StrikeOutAnnotation::SetModificationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::M)) {
        bool removed = _obj->Remove(constant::Name::M);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::M, date->GetObject());
}

bool StrikeOutAnnotation::GetCreationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::CreationDate)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
    result = make_deferred<Date>(date_obj);
    return true;
}

void StrikeOutAnnotation::SetCreationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::CreationDate)) {
        bool removed = _obj->Remove(constant::Name::CreationDate);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::CreationDate, date->GetObject());
}

// InkAnnotation methods

InkAnnotationPtr InkAnnotation::Create() {
    syntax::DictionaryObjectPtr dict;
    dict->Insert(constant::Name::Type, constant::Name::Annot.Clone());
    dict->Insert(constant::Name::Subtype, constant::Name::Ink.Clone());
    return make_deferred<InkAnnotation>(dict);
}

InkAnnotationPtr InkAnnotation::CreateFromRect(RectanglePtr rect) {
    auto dict = CreateBaseDictionary(constant::Name::Ink.Clone(), rect);
    return make_deferred<InkAnnotation>(dict);
}

bool InkAnnotation::GetInkList(syntax::MixedArrayObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::InkList)) {
        return false;
    }

    result = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::InkList);
    return true;
}

void InkAnnotation::SetInkList(syntax::MixedArrayObjectPtr inkList) {
    if (_obj->Contains(constant::Name::InkList)) {
        bool removed = _obj->Remove(constant::Name::InkList);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::InkList, inkList);
}

// InkAnnotation - Markup annotation properties (Table 170)

bool InkAnnotation::GetAuthor(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

void InkAnnotation::SetAuthor(syntax::StringObjectPtr author) {
    if (_obj->Contains(constant::Name::T)) {
        bool removed = _obj->Remove(constant::Name::T);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::T, author);
}

bool InkAnnotation::GetModificationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::M)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::M);
    result = make_deferred<Date>(date_obj);
    return true;
}

void InkAnnotation::SetModificationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::M)) {
        bool removed = _obj->Remove(constant::Name::M);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::M, date->GetObject());
}

bool InkAnnotation::GetCreationDate(OutputDatePtr& result) const {
    if (!_obj->Contains(constant::Name::CreationDate)) {
        return false;
    }

    auto date_obj = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
    result = make_deferred<Date>(date_obj);
    return true;
}

void InkAnnotation::SetCreationDate(DatePtr date) {
    if (_obj->Contains(constant::Name::CreationDate)) {
        bool removed = _obj->Remove(constant::Name::CreationDate);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::CreationDate, date->GetObject());
}

// WidgetAnnotation methods

WidgetAnnotationPtr WidgetAnnotation::CreateFromRect(RectanglePtr rect) {
    auto dict = CreateBaseDictionary(constant::Name::Widget.Clone(), rect);
    return make_deferred<WidgetAnnotation>(dict);
}

bool WidgetAnnotation::GetNormalAppearance(OutputFormXObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::AP)) {
        return false;
    }

    auto appearance_dictionary = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::AP);
    if (!appearance_dictionary->Contains(constant::Name::N)) {
        return false;
    }

    auto appearance_stream = appearance_dictionary->FindAs<syntax::StreamObjectPtr>(constant::Name::N);
    result = make_deferred<FormXObject>(appearance_stream);
    return true;
}

void WidgetAnnotation::SetNormalAppearance(FormXObjectPtr appearance) {
    if (!_obj->Contains(constant::Name::AP)) {
        syntax::DictionaryObjectPtr new_appearance_dictionary;
        _obj->Insert(constant::Name::AP, new_appearance_dictionary);
    }

    auto appearance_dictionary = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::AP);

    // The appearance stream is an indirect object,
    // so the appearance dictionary stores a reference to it
    syntax::IndirectReferenceObjectPtr reference = make_deferred<syntax::IndirectReferenceObject>(appearance->GetObject());
    appearance_dictionary->Insert(constant::Name::N, reference, true);
}

bool WidgetAnnotation::GetAppearanceCharacteristics(syntax::OutputDictionaryObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::MK)) {
        return false;
    }

    result = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::MK);
    return true;
}

void WidgetAnnotation::SetAppearanceCharacteristics(syntax::DictionaryObjectPtr value) {
    _obj->Insert(constant::Name::MK, value, true);
}

// PageAnnotations methods

PageAnnotations::PageAnnotations() {
    // Default constructor creates an empty array
}

void PageAnnotations::Append(AnnotationPtr annotation) {
    _obj->Append(annotation->GetObject());
}

bool PageAnnotations::Remove(types::size_type index) {
    return _obj->Remove(index);
}

bool LinkAnnotation::Action(OutputActionPtr& result) const {
    if (!_obj->Contains(constant::Name::A)) {
        return false;
    }

    try {
        auto action_dict = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::A);
        result = ActionBase::Create(action_dict);
        return true;
    }
    catch (ExceptionBase& ex) {
        spdlog::warn("Could not resolve link annotation action: {}", ex.what());
        return false;
    }
}

} // semantics
} // vanillapdf
