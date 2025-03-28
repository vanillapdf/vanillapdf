#ifndef _ANNOTATIONS_H
#define _ANNOTATIONS_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/destinations.h"

namespace vanillapdf {
namespace semantics {

class AnnotationBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    enum Type {
        Undefined = 0,
        Text,
        Link,
        FreeText,
        Line,
        Square,
        Circle,
        Polygon,
        PolyLine,
        Highlight,
        Underline,
        Squiggly,
        StrikeOut,
        RubberStamp,
        Caret,
        Ink,
        Popup,
        FileAttachment,
        Sound,
        Movie,
        Widget,
        Screen,
        PrinterMark,
        TrapNetwork,
        Watermark,
        TripleD,
        Redaction
    };

public:
    explicit AnnotationBase(syntax::DictionaryObjectPtr root);
    static std::unique_ptr<AnnotationBase> Create(syntax::DictionaryObjectPtr root);

    virtual AnnotationBase::Type GetAnnotationType() const noexcept = 0;
};

class TextAnnotation : public AnnotationBase {
public:
    explicit TextAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class LinkAnnotation : public AnnotationBase {
public:
    explicit LinkAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;

    bool Destination(OutputDestinationPtr& result) const;
};

class FreeTextAnnotation : public AnnotationBase {
public:
    explicit FreeTextAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class LineAnnotation : public AnnotationBase {
public:
    explicit LineAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class SquareAnnotation : public AnnotationBase {
public:
    explicit SquareAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class CircleAnnotation : public AnnotationBase {
public:
    explicit CircleAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class PolygonAnnotation : public AnnotationBase {
public:
    explicit PolygonAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class PolyLineAnnotation : public AnnotationBase {
public:
    explicit PolyLineAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class HighlightAnnotation : public AnnotationBase {
public:
    explicit HighlightAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class UnderlineAnnotation : public AnnotationBase {
public:
    explicit UnderlineAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class SquigglyAnnotation : public AnnotationBase {
public:
    explicit SquigglyAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class StrikeOutAnnotation : public AnnotationBase {
public:
    explicit StrikeOutAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class RubberStampAnnotation : public AnnotationBase {
public:
    explicit RubberStampAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class CaretAnnotation : public AnnotationBase {
public:
    explicit CaretAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class InkAnnotation : public AnnotationBase {
public:
    explicit InkAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class PopupAnnotation : public AnnotationBase {
public:
    explicit PopupAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class FileAttachmentAnnotation : public AnnotationBase {
public:
    explicit FileAttachmentAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class SoundAnnotation : public AnnotationBase {
public:
    explicit SoundAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class MovieAnnotation : public AnnotationBase {
public:
    explicit MovieAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class WidgetAnnotation : public AnnotationBase {
public:
    explicit WidgetAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class ScreenAnnotation : public AnnotationBase {
public:
    explicit ScreenAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class PrinterMarkAnnotation : public AnnotationBase {
public:
    explicit PrinterMarkAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class TrapNetworkAnnotation : public AnnotationBase {
public:
    explicit TrapNetworkAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class WatermarkAnnotation : public AnnotationBase {
public:
    explicit WatermarkAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class TripleDAnnotation : public AnnotationBase {
public:
    explicit TripleDAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};

class RedactionAnnotation : public AnnotationBase {
public:
    explicit RedactionAnnotation(syntax::DictionaryObjectPtr root);
    virtual AnnotationBase::Type GetAnnotationType() const noexcept override;
};


class PageAnnotations : public HighLevelObject<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>> {
public:
    explicit PageAnnotations(syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> root);
    types::size_type GetSize() const;
    AnnotationPtr At(types::size_type index) const;
};

inline AnnotationBase::Type TextAnnotation::GetAnnotationType() const noexcept { return Type::Text; }
inline AnnotationBase::Type LinkAnnotation::GetAnnotationType() const noexcept { return Type::Link; }
inline AnnotationBase::Type FreeTextAnnotation::GetAnnotationType() const noexcept { return Type::FreeText; }
inline AnnotationBase::Type LineAnnotation::GetAnnotationType() const noexcept { return Type::Line; }
inline AnnotationBase::Type SquareAnnotation::GetAnnotationType() const noexcept { return Type::Square; }
inline AnnotationBase::Type CircleAnnotation::GetAnnotationType() const noexcept { return Type::Circle; }
inline AnnotationBase::Type PolygonAnnotation::GetAnnotationType() const noexcept { return Type::Polygon; }
inline AnnotationBase::Type PolyLineAnnotation::GetAnnotationType() const noexcept { return Type::PolyLine; }
inline AnnotationBase::Type HighlightAnnotation::GetAnnotationType() const noexcept { return Type::Highlight; }
inline AnnotationBase::Type UnderlineAnnotation::GetAnnotationType() const noexcept { return Type::Underline; }
inline AnnotationBase::Type SquigglyAnnotation::GetAnnotationType() const noexcept { return Type::Squiggly; }
inline AnnotationBase::Type StrikeOutAnnotation::GetAnnotationType() const noexcept { return Type::StrikeOut; }
inline AnnotationBase::Type RubberStampAnnotation::GetAnnotationType() const noexcept { return Type::RubberStamp; }
inline AnnotationBase::Type CaretAnnotation::GetAnnotationType() const noexcept { return Type::Caret; }
inline AnnotationBase::Type InkAnnotation::GetAnnotationType() const noexcept { return Type::Ink; }
inline AnnotationBase::Type PopupAnnotation::GetAnnotationType() const noexcept { return Type::Popup; }
inline AnnotationBase::Type FileAttachmentAnnotation::GetAnnotationType() const noexcept { return Type::FileAttachment; }
inline AnnotationBase::Type SoundAnnotation::GetAnnotationType() const noexcept { return Type::Sound; }
inline AnnotationBase::Type MovieAnnotation::GetAnnotationType() const noexcept { return Type::Movie; }
inline AnnotationBase::Type WidgetAnnotation::GetAnnotationType() const noexcept { return Type::Widget; }
inline AnnotationBase::Type ScreenAnnotation::GetAnnotationType() const noexcept { return Type::Screen; }
inline AnnotationBase::Type PrinterMarkAnnotation::GetAnnotationType() const noexcept { return Type::PrinterMark; }
inline AnnotationBase::Type TrapNetworkAnnotation::GetAnnotationType() const noexcept { return Type::TrapNetwork; }
inline AnnotationBase::Type WatermarkAnnotation::GetAnnotationType() const noexcept { return Type::Watermark; }
inline AnnotationBase::Type TripleDAnnotation::GetAnnotationType() const noexcept { return Type::TripleD; }
inline AnnotationBase::Type RedactionAnnotation::GetAnnotationType() const noexcept { return Type::Redaction; }

} // semantics
} // vanillapdf

#endif /* _ANNOTATIONS_H */
