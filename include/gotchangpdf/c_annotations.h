#ifndef _C_ANNOTATIONS_H
#define _C_ANNOTATIONS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		AnnotationType_Text = 0,
		AnnotationType_Link,
		AnnotationType_FreeText,
		AnnotationType_Line,
		AnnotationType_Square,
		AnnotationType_Circle,
		AnnotationType_Polygon,
		AnnotationType_PolyLine,
		AnnotationType_Highlight,
		AnnotationType_Underline,
		AnnotationType_Squiggly,
		AnnotationType_StrikeOut,
		AnnotationType_RubberStamp,
		AnnotationType_Caret,
		AnnotationType_Ink,
		AnnotationType_Popup,
		AnnotationType_FileAttachment,
		AnnotationType_Sound,
		AnnotationType_Movie,
		AnnotationType_Widget,
		AnnotationType_Screen,
		AnnotationType_PrinterMark,
		AnnotationType_TrapNetwork,
		AnnotationType_Watermark,
		AnnotationType_TripleD,
		AnnotationType_Redact,
	} AnnotationType, *PAnnotationType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Annotation_GetType(AnnotationHandle handle, PAnnotationType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Annotation_ToLink(AnnotationHandle handle, PLinkAnnotationHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Annotation_Release(AnnotationHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION LinkAnnotation_GetDestination(LinkAnnotationHandle handle, PDestinationHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LinkAnnotation_Release(LinkAnnotationHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageAnnotations_Size(PageAnnotationsHandle handle, out_size_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageAnnotations_At(PageAnnotationsHandle handle, size_type at, PAnnotationHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageAnnotations_Release(PageAnnotationsHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_ANNOTATIONS_H */
