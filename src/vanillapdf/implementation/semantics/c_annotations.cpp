#include "precompiled.h"
#include "semantics/objects/annotations.h"
#include "semantics/objects/color.h"
#include "semantics/objects/date.h"
#include "semantics/objects/rectangle.h"

#include "vanillapdf/semantics/c_annotations.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetAnnotationType(AnnotationHandle* handle, AnnotationType* result)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (obj->GetAnnotationType()) {
    case AnnotationBase::Type::Text:
        *result = AnnotationType_Text; break;
    case AnnotationBase::Type::Link:
        *result = AnnotationType_Link; break;
    case AnnotationBase::Type::FreeText:
        *result = AnnotationType_FreeText; break;
    case AnnotationBase::Type::Line:
        *result = AnnotationType_Line; break;
    case AnnotationBase::Type::Square:
        *result = AnnotationType_Square; break;
    case AnnotationBase::Type::Circle:
        *result = AnnotationType_Circle; break;
    case AnnotationBase::Type::Polygon:
        *result = AnnotationType_Polygon; break;
    case AnnotationBase::Type::PolyLine:
        *result = AnnotationType_PolyLine; break;
    case AnnotationBase::Type::Highlight:
        *result = AnnotationType_Highlight; break;
    case AnnotationBase::Type::Underline:
        *result = AnnotationType_Underline; break;
    case AnnotationBase::Type::Squiggly:
        *result = AnnotationType_Squiggly; break;
    case AnnotationBase::Type::StrikeOut:
        *result = AnnotationType_StrikeOut; break;
    case AnnotationBase::Type::RubberStamp:
        *result = AnnotationType_RubberStamp; break;
    case AnnotationBase::Type::Caret:
        *result = AnnotationType_Caret; break;
    case AnnotationBase::Type::Ink:
        *result = AnnotationType_Ink; break;
    case AnnotationBase::Type::Popup:
        *result = AnnotationType_Popup; break;
    case AnnotationBase::Type::FileAttachment:
        *result = AnnotationType_FileAttachment; break;
    case AnnotationBase::Type::Sound:
        *result = AnnotationType_Sound; break;
    case AnnotationBase::Type::Movie:
        *result = AnnotationType_Movie; break;
    case AnnotationBase::Type::Widget:
        *result = AnnotationType_Widget; break;
    case AnnotationBase::Type::Screen:
        *result = AnnotationType_Screen; break;
    case AnnotationBase::Type::PrinterMark:
        *result = AnnotationType_PrinterMark; break;
    case AnnotationBase::Type::TrapNetwork:
        *result = AnnotationType_TrapNetwork; break;
    case AnnotationBase::Type::Watermark:
        *result = AnnotationType_Watermark; break;
    case AnnotationBase::Type::TripleD:
        *result = AnnotationType_TripleD; break;
    case AnnotationBase::Type::Redaction:
        *result = AnnotationType_Redaction; break;
    default:
        return VANILLAPDF_ERROR_GENERAL;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_ToUnknown(AnnotationHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<AnnotationBase, IUnknown, AnnotationHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_FromUnknown(IUnknownHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<IUnknown, AnnotationBase, IUnknownHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_Release(AnnotationHandle* handle) {
    return ObjectRelease<AnnotationBase, AnnotationHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_GetDestination(LinkAnnotationHandle* handle, DestinationHandle** result)
{
    LinkAnnotation* obj = reinterpret_cast<LinkAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDestinationPtr destination;
        bool contains = obj->Destination(destination);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = destination.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_ToBaseAnnotation(LinkAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<LinkAnnotation, AnnotationBase, LinkAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_FromBaseAnnotation(AnnotationHandle* handle, LinkAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, LinkAnnotation, AnnotationHandle, LinkAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_Release(LinkAnnotationHandle* handle) {
    return ObjectRelease<LinkAnnotation, LinkAnnotationHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_GetSize(PageAnnotationsHandle* handle, size_type* result)
{
    PageAnnotations* obj = reinterpret_cast<PageAnnotations*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetSize();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_At(PageAnnotationsHandle* handle, size_type at, AnnotationHandle** result)
{
    PageAnnotations* obj = reinterpret_cast<PageAnnotations*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto direct = obj->At(at);
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<AnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_ToUnknown(PageAnnotationsHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<PageAnnotations, IUnknown, PageAnnotationsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_FromUnknown(IUnknownHandle* handle, PageAnnotationsHandle** result) {
    return SafeObjectConvert<IUnknown, PageAnnotations, IUnknownHandle, PageAnnotationsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_Release(PageAnnotationsHandle* handle) {
    return ObjectRelease<PageAnnotations, PageAnnotationsHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_Append(PageAnnotationsHandle* handle, AnnotationHandle* annotation)
{
    PageAnnotations* obj = reinterpret_cast<PageAnnotations*>(handle);
    AnnotationBase* annot = reinterpret_cast<AnnotationBase*>(annotation);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(annot);

    try
    {
        obj->Append(annot);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

// Annotation property accessors

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetRect(AnnotationHandle* handle, RectangleHandle** result)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputRectanglePtr rect;
        bool contains = obj->GetRect(rect);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = rect.AddRefGet();
        *result = reinterpret_cast<RectangleHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetRect(AnnotationHandle* handle, RectangleHandle* value)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    Rectangle* rect = reinterpret_cast<Rectangle*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect);

    try
    {
        obj->SetRect(rect);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetContents(AnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr contents;
        bool contains = obj->GetContents(contents);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = contents.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetContents(AnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    LiteralStringObject* contents = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(contents);

    try
    {
        obj->SetContents(contents);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetColor(AnnotationHandle* handle, ColorHandle** result)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputColorPtr color;
        bool contains = obj->GetColor(color);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = color.AddRefGet();
        *result = reinterpret_cast<ColorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetColor(AnnotationHandle* handle, ColorHandle* value)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    Color* color = reinterpret_cast<Color*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(color);

    try
    {
        obj->SetColor(color);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}




VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetFlags(AnnotationHandle* handle, AnnotationFlags* result)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto flags = obj->GetFlags();
        *result = static_cast<AnnotationFlags>(flags);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetFlags(AnnotationHandle* handle, AnnotationFlags value)
{
    AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetFlags(static_cast<AnnotationBase::Flags>(value));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

// TextAnnotation

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_Create(RectangleHandle* rect, TextAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = TextAnnotation::Create(rect_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<TextAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_CreateWithContents(RectangleHandle* rect, LiteralStringObjectHandle* contents, TextAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    LiteralStringObject* contents_obj = reinterpret_cast<LiteralStringObject*>(contents);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(contents_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = TextAnnotation::Create(rect_obj, contents_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<TextAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_GetAuthor(TextAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    TextAnnotation* obj = reinterpret_cast<TextAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr author;
        bool contains = obj->GetAuthor(author);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_SetAuthor(TextAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    TextAnnotation* obj = reinterpret_cast<TextAnnotation*>(handle);
    LiteralStringObject* author = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_GetModificationDate(TextAnnotationHandle* handle, DateHandle** result)
{
    TextAnnotation* obj = reinterpret_cast<TextAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetModificationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_SetModificationDate(TextAnnotationHandle* handle, DateHandle* value)
{
    TextAnnotation* obj = reinterpret_cast<TextAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetModificationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_GetCreationDate(TextAnnotationHandle* handle, DateHandle** result)
{
    TextAnnotation* obj = reinterpret_cast<TextAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetCreationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_SetCreationDate(TextAnnotationHandle* handle, DateHandle* value)
{
    TextAnnotation* obj = reinterpret_cast<TextAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetCreationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_ToBaseAnnotation(TextAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<TextAnnotation, AnnotationBase, TextAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_FromBaseAnnotation(AnnotationHandle* handle, TextAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, TextAnnotation, AnnotationHandle, TextAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_Release(TextAnnotationHandle* handle) {
    return ObjectRelease<TextAnnotation, TextAnnotationHandle>(handle);
}

// HighlightAnnotation

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_Create(RectangleHandle* rect, ArrayObjectHandle* quadPoints, HighlightAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(quadPoints);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = HighlightAnnotation::Create(rect_obj, quad_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<HighlightAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetQuadPoints(HighlightAnnotationHandle* handle, ArrayObjectHandle** result)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        MixedArrayObjectPtr quadPoints;
        bool contains = obj->GetQuadPoints(quadPoints);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = quadPoints.AddRefGet();
        *result = reinterpret_cast<ArrayObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetQuadPoints(HighlightAnnotationHandle* handle, ArrayObjectHandle* value)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);

    try
    {
        obj->SetQuadPoints(quad_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetAuthor(HighlightAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr author;
        bool contains = obj->GetAuthor(author);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetAuthor(HighlightAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    LiteralStringObject* author = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetModificationDate(HighlightAnnotationHandle* handle, DateHandle** result)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetModificationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetModificationDate(HighlightAnnotationHandle* handle, DateHandle* value)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetModificationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetCreationDate(HighlightAnnotationHandle* handle, DateHandle** result)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetCreationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetCreationDate(HighlightAnnotationHandle* handle, DateHandle* value)
{
    HighlightAnnotation* obj = reinterpret_cast<HighlightAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetCreationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_ToBaseAnnotation(HighlightAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<HighlightAnnotation, AnnotationBase, HighlightAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_FromBaseAnnotation(AnnotationHandle* handle, HighlightAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, HighlightAnnotation, AnnotationHandle, HighlightAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_Release(HighlightAnnotationHandle* handle) {
    return ObjectRelease<HighlightAnnotation, HighlightAnnotationHandle>(handle);
}

// FreeTextAnnotation

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_Create(RectangleHandle* rect, LiteralStringObjectHandle* contents, LiteralStringObjectHandle* defaultAppearance, FreeTextAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    LiteralStringObject* contents_obj = reinterpret_cast<LiteralStringObject*>(contents);
    LiteralStringObject* da_obj = reinterpret_cast<LiteralStringObject*>(defaultAppearance);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(contents_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(da_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = FreeTextAnnotation::Create(rect_obj, contents_obj, da_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<FreeTextAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetDefaultAppearance(FreeTextAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        LiteralStringObjectPtr da;
        bool contains = obj->GetDefaultAppearance(da);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = da.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetDefaultAppearance(FreeTextAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    LiteralStringObject* da_obj = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(da_obj);

    try
    {
        obj->SetDefaultAppearance(da_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetAuthor(FreeTextAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr author;
        bool contains = obj->GetAuthor(author);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetAuthor(FreeTextAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    LiteralStringObject* author = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetModificationDate(FreeTextAnnotationHandle* handle, DateHandle** result)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetModificationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetModificationDate(FreeTextAnnotationHandle* handle, DateHandle* value)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetModificationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetCreationDate(FreeTextAnnotationHandle* handle, DateHandle** result)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetCreationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetCreationDate(FreeTextAnnotationHandle* handle, DateHandle* value)
{
    FreeTextAnnotation* obj = reinterpret_cast<FreeTextAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetCreationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_ToBaseAnnotation(FreeTextAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<FreeTextAnnotation, AnnotationBase, FreeTextAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_FromBaseAnnotation(AnnotationHandle* handle, FreeTextAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, FreeTextAnnotation, AnnotationHandle, FreeTextAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_Release(FreeTextAnnotationHandle* handle) {
    return ObjectRelease<FreeTextAnnotation, FreeTextAnnotationHandle>(handle);
}

// UnderlineAnnotation

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_Create(RectangleHandle* rect, ArrayObjectHandle* quadPoints, UnderlineAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(quadPoints);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = UnderlineAnnotation::Create(rect_obj, quad_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<UnderlineAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetQuadPoints(UnderlineAnnotationHandle* handle, ArrayObjectHandle** result)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        MixedArrayObjectPtr quadPoints;
        bool contains = obj->GetQuadPoints(quadPoints);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = quadPoints.AddRefGet();
        *result = reinterpret_cast<ArrayObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetQuadPoints(UnderlineAnnotationHandle* handle, ArrayObjectHandle* value)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);

    try
    {
        obj->SetQuadPoints(quad_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetAuthor(UnderlineAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr author;
        bool contains = obj->GetAuthor(author);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetAuthor(UnderlineAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    LiteralStringObject* author = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetModificationDate(UnderlineAnnotationHandle* handle, DateHandle** result)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetModificationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetModificationDate(UnderlineAnnotationHandle* handle, DateHandle* value)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetModificationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetCreationDate(UnderlineAnnotationHandle* handle, DateHandle** result)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetCreationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetCreationDate(UnderlineAnnotationHandle* handle, DateHandle* value)
{
    UnderlineAnnotation* obj = reinterpret_cast<UnderlineAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetCreationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_ToBaseAnnotation(UnderlineAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<UnderlineAnnotation, AnnotationBase, UnderlineAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_FromBaseAnnotation(AnnotationHandle* handle, UnderlineAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, UnderlineAnnotation, AnnotationHandle, UnderlineAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_Release(UnderlineAnnotationHandle* handle) {
    return ObjectRelease<UnderlineAnnotation, UnderlineAnnotationHandle>(handle);
}

// StrikeOutAnnotation

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_Create(RectangleHandle* rect, ArrayObjectHandle* quadPoints, StrikeOutAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(quadPoints);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = StrikeOutAnnotation::Create(rect_obj, quad_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<StrikeOutAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetQuadPoints(StrikeOutAnnotationHandle* handle, ArrayObjectHandle** result)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        MixedArrayObjectPtr quadPoints;
        bool contains = obj->GetQuadPoints(quadPoints);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = quadPoints.AddRefGet();
        *result = reinterpret_cast<ArrayObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetQuadPoints(StrikeOutAnnotationHandle* handle, ArrayObjectHandle* value)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);

    try
    {
        obj->SetQuadPoints(quad_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetAuthor(StrikeOutAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr author;
        bool contains = obj->GetAuthor(author);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetAuthor(StrikeOutAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    LiteralStringObject* author = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetModificationDate(StrikeOutAnnotationHandle* handle, DateHandle** result)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetModificationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetModificationDate(StrikeOutAnnotationHandle* handle, DateHandle* value)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetModificationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetCreationDate(StrikeOutAnnotationHandle* handle, DateHandle** result)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetCreationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetCreationDate(StrikeOutAnnotationHandle* handle, DateHandle* value)
{
    StrikeOutAnnotation* obj = reinterpret_cast<StrikeOutAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetCreationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_ToBaseAnnotation(StrikeOutAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<StrikeOutAnnotation, AnnotationBase, StrikeOutAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_FromBaseAnnotation(AnnotationHandle* handle, StrikeOutAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, StrikeOutAnnotation, AnnotationHandle, StrikeOutAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_Release(StrikeOutAnnotationHandle* handle) {
    return ObjectRelease<StrikeOutAnnotation, StrikeOutAnnotationHandle>(handle);
}

// SquigglyAnnotation

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_Create(RectangleHandle* rect, ArrayObjectHandle* quadPoints, SquigglyAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(quadPoints);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = SquigglyAnnotation::Create(rect_obj, quad_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<SquigglyAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetQuadPoints(SquigglyAnnotationHandle* handle, ArrayObjectHandle** result)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        MixedArrayObjectPtr quadPoints;
        bool contains = obj->GetQuadPoints(quadPoints);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = quadPoints.AddRefGet();
        *result = reinterpret_cast<ArrayObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetQuadPoints(SquigglyAnnotationHandle* handle, ArrayObjectHandle* value)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    MixedArrayObject* quad_obj = reinterpret_cast<MixedArrayObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(quad_obj);

    try
    {
        obj->SetQuadPoints(quad_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetAuthor(SquigglyAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr author;
        bool contains = obj->GetAuthor(author);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetAuthor(SquigglyAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    LiteralStringObject* author = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetModificationDate(SquigglyAnnotationHandle* handle, DateHandle** result)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetModificationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetModificationDate(SquigglyAnnotationHandle* handle, DateHandle* value)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetModificationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetCreationDate(SquigglyAnnotationHandle* handle, DateHandle** result)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetCreationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetCreationDate(SquigglyAnnotationHandle* handle, DateHandle* value)
{
    SquigglyAnnotation* obj = reinterpret_cast<SquigglyAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetCreationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_ToBaseAnnotation(SquigglyAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<SquigglyAnnotation, AnnotationBase, SquigglyAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_FromBaseAnnotation(AnnotationHandle* handle, SquigglyAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, SquigglyAnnotation, AnnotationHandle, SquigglyAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_Release(SquigglyAnnotationHandle* handle) {
    return ObjectRelease<SquigglyAnnotation, SquigglyAnnotationHandle>(handle);
}

// InkAnnotation

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_Create(RectangleHandle* rect, ArrayObjectHandle* inkList, InkAnnotationHandle** result)
{
    Rectangle* rect_obj = reinterpret_cast<Rectangle*>(rect);
    MixedArrayObject* ink_obj = reinterpret_cast<MixedArrayObject*>(inkList);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(rect_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(ink_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto annot = InkAnnotation::Create(rect_obj, ink_obj);
        auto ptr = annot.AddRefGet();
        *result = reinterpret_cast<InkAnnotationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetInkList(InkAnnotationHandle* handle, ArrayObjectHandle** result)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        MixedArrayObjectPtr inkList;
        bool contains = obj->GetInkList(inkList);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = inkList.AddRefGet();
        *result = reinterpret_cast<ArrayObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetInkList(InkAnnotationHandle* handle, ArrayObjectHandle* value)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    MixedArrayObject* ink_obj = reinterpret_cast<MixedArrayObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(ink_obj);

    try
    {
        obj->SetInkList(ink_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetAuthor(InkAnnotationHandle* handle, LiteralStringObjectHandle** result)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputLiteralStringObjectPtr author;
        bool contains = obj->GetAuthor(author);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = author.AddRefGet();
        *result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetAuthor(InkAnnotationHandle* handle, LiteralStringObjectHandle* value)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    LiteralStringObject* author = reinterpret_cast<LiteralStringObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(author);

    try
    {
        obj->SetAuthor(author);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetModificationDate(InkAnnotationHandle* handle, DateHandle** result)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetModificationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetModificationDate(InkAnnotationHandle* handle, DateHandle* value)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetModificationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetCreationDate(InkAnnotationHandle* handle, DateHandle** result)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputDatePtr date;
        bool contains = obj->GetCreationDate(date);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetCreationDate(InkAnnotationHandle* handle, DateHandle* value)
{
    InkAnnotation* obj = reinterpret_cast<InkAnnotation*>(handle);
    Date* date = reinterpret_cast<Date*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(date);

    try
    {
        obj->SetCreationDate(date);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_ToBaseAnnotation(InkAnnotationHandle* handle, AnnotationHandle** result) {
    return SafeObjectConvert<InkAnnotation, AnnotationBase, InkAnnotationHandle, AnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_FromBaseAnnotation(AnnotationHandle* handle, InkAnnotationHandle** result) {
    return SafeObjectConvert<AnnotationBase, InkAnnotation, AnnotationHandle, InkAnnotationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_Release(InkAnnotationHandle* handle) {
    return ObjectRelease<InkAnnotation, InkAnnotationHandle>(handle);
}

// PageAnnotations_Remove

VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_Remove(PageAnnotationsHandle* handle, size_type at)
{
    PageAnnotations* obj = reinterpret_cast<PageAnnotations*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        bool removed = obj->Remove(at);
        if (!removed) {
            return VANILLAPDF_ERROR_GENERAL;
        }
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
