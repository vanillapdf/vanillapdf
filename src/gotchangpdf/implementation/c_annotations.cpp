#include "precompiled.h"
#include "semantics/objects/annotations.h"

#include "gotchangpdf/c_annotations.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Annotation_GetType(AnnotationHandle handle, AnnotationType* result)
{
	AnnotationBase* obj = reinterpret_cast<AnnotationBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetType()) {
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
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Annotation_ToLink(AnnotationHandle handle, LinkAnnotationHandle* result)
{
	return SafeObjectConvert<AnnotationBase, LinkAnnotation, AnnotationHandle, LinkAnnotationHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Annotation_Release(AnnotationHandle handle)
{
	return ObjectRelease<AnnotationBase, AnnotationHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LinkAnnotation_GetDestination(LinkAnnotationHandle handle, DestinationHandle* result)
{
	LinkAnnotation* obj = reinterpret_cast<LinkAnnotation*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDestinationPtr destination;
		bool contains = obj->Destination(destination);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = destination.AddRefGet();
		*result = reinterpret_cast<DestinationHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LinkAnnotation_Release(LinkAnnotationHandle handle)
{
	return ObjectRelease<LinkAnnotation, LinkAnnotationHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageAnnotations_Size(PageAnnotationsHandle handle, size_type* result)
{
	PageAnnotations* obj = reinterpret_cast<PageAnnotations*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->Size();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageAnnotations_At(PageAnnotationsHandle handle, size_type at, AnnotationHandle* result)
{
	PageAnnotations* obj = reinterpret_cast<PageAnnotations*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->At(at);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<AnnotationHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageAnnotations_Release(PageAnnotationsHandle handle)
{
	return ObjectRelease<PageAnnotations, PageAnnotationsHandle>(handle);
}