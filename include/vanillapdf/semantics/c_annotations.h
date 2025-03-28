#ifndef _C_ANNOTATIONS_H
#define _C_ANNOTATIONS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_annotations.h
    * \brief This file contains functions for working with document page annotations.
    */

    /**
    * \class PageAnnotationsHandle
    * \extends IUnknownHandle
    * \ingroup group_annotations
    * \brief
    * An array of annotation dictionaries that shall contain indirect
    * references to all \ref AnnotationHandle associated with the page.
    * \see \ref PageObjectHandle
    */

    /**
    * \class AnnotationHandle
    * \extends IUnknownHandle
    * \ingroup group_annotations
    * \brief
    * An annotation associates an object such as a note, sound, or movie
    * with a location on a page of a PDF document, or provides a way
    * to interact with the user by means of the mouse and keyboard.
    */

    /**
    * \class LinkAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * A link annotation represents either a hypertext link to a \ref DestinationHandle
    * elsewhere in the document or an [Action](PDF32000_2008.pdf#G11.2106801) to be performed.
    *
    * For more details please visit [section 12.5.6.5 - Link Annotations](PDF32000_2008.pdf#G11.1951136).
    */

    /**
    * \brief Derived types of \ref AnnotationHandle
    * \ingroup group_annotations
    */
    typedef enum {

        /**
        * \brief Undefined unitialized default value, triggers error when used
        */
        AnnotationType_Undefined = 0,
        /**
        * A text annotation represents a "sticky note" attached to a point in
        * the PDF document.
        */
        AnnotationType_Text,

        /**
        * A link annotation represents either a hypertext link to a
        * destination elsewhere in the document or an action to be performed
        */
        AnnotationType_Link,

        /**
        * A free text annotation (PDF 1.3) displays text directly on the page.
        */
        AnnotationType_FreeText,

        /**
        * The purpose of a line annotation (PDF 1.3) is to
        * display a single straight line on the page.
        */
        AnnotationType_Line,

        /**
        * Square and circle annotations (PDF 1.3) shall display,
        * respectively, a rectangle or an ellipse on the page.
        */
        AnnotationType_Square,

        /**
        * Square and circle annotations (PDF 1.3) shall display,
        * respectively, a rectangle or an ellipse on the page.
        */
        AnnotationType_Circle,

        /**
        * Polygon annotations (PDF 1.5) display closed polygons on the page.
        */
        AnnotationType_Polygon,

        /**
        * Polyline annotations (PDF 1.5) are similar to polygons,
        * except that the first and last vertex are not implicitly connected.
        */
        AnnotationType_PolyLine,

        /**
        * Text markup annotations shall appear as highlights, underlines,
        strikeouts (all PDF 1.3), or jagged ("squiggly") underlines (PDF 1.4)
        in the text of a document.
        */
        AnnotationType_Highlight,

        /**
        * Text markup annotations shall appear as highlights, underlines,
        strikeouts (all PDF 1.3), or jagged ("squiggly") underlines (PDF 1.4)
        in the text of a document.
        */
        AnnotationType_Underline,

        /**
        * Text markup annotations shall appear as highlights, underlines,
        strikeouts (all PDF 1.3), or jagged ("squiggly") underlines (PDF 1.4)
        in the text of a document.
        */
        AnnotationType_Squiggly,

        /**
        * Text markup annotations shall appear as highlights, underlines,
        strikeouts (all PDF 1.3), or jagged ("squiggly") underlines (PDF 1.4)
        in the text of a document.
        */
        AnnotationType_StrikeOut,

        /**
        * A rubber stamp annotation (PDF 1.3) displays text or
        * graphics intended to look as if they were stamped on
        * the page with a rubber stamp.
        */
        AnnotationType_RubberStamp,

        /**
        * A caret annotation (PDF 1.5) is a visual symbol
        * that indicates the presence of text edits.
        */
        AnnotationType_Caret,

        /** An ink annotation (PDF 1.3) represents a freehand
        * "scribble" composed of one or more disjoint paths.
        */
        AnnotationType_Ink,

        /**
        * A pop-up annotation (PDF 1.3) displays text in a
        * pop-up window for entry and editing.
        */
        AnnotationType_Popup,

        /**
        * A file attachment annotation (PDF 1.3) contains a
        * reference to a file, which typically shall be embedded
        * in the PDF file.
        */
        AnnotationType_FileAttachment,

        /**
        * A sound annotation (PDF 1.2) shall be analogous to a text annotation
        * except that instead of a text note, it contains sound recorded from
        * the computer's microphone or imported from a file.
        */
        AnnotationType_Sound,

        /**
        * A movie annotation (PDF 1.2) contains animated graphics and
        * sound to be presented on the computer screen and through the speakers.
        */
        AnnotationType_Movie,

        /**
        * Interactive forms use widget annotations (PDF 1.2) to
        * represent the appearance of fields and to manage user interactions.
        */
        AnnotationType_Widget,

        /**
        * A screen annotation (PDF 1.5) specifies a region of a
        * page upon which media clips may be played.
        */
        AnnotationType_Screen,

        /**
        * A printer's mark annotation (PDF 1.4) represents a graphic symbol,
        * such as a registration target,colour bar, or cut mark,
        * that may be added to a page to assist production personnel
        * in identifying components of a multiple-plate job and maintaining
        * consistent output during production.
        */
        AnnotationType_PrinterMark,

        /**
        * A trap network annotation (PDF 1.3) may be used to define the
        * trapping characteristics for a page of a PDF document.
        */
        AnnotationType_TrapNetwork,

        /**
        * A watermark annotation (PDF 1.6) shall be used to represent
        * graphics that shall be printed at a fixed size and position
        * on a page, regardless of the dimensions of the printed page.
        */
        AnnotationType_Watermark,

        /**
        * 3D annotations (PDF 1.6) are the means by which 3D artwork
        * shall be represented in a PDF document.
        */
        AnnotationType_TripleD,

        /**
        * A redaction annotation (PDF 1.7) identifies content
        * that is intended to be removed from the document.
        */
        AnnotationType_Redaction,
    } AnnotationType;

    /**
    * \memberof AnnotationHandle
    * @{
    */

    /**
    * \brief Get annotation type of object \p handle
    * \param handle a handle to the annotation class
    * \param result a pointer to variable will be filled with annotation type upon success, unchanged on failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetAnnotationType(AnnotationHandle* handle, AnnotationType* result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_ToUnknown(AnnotationHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref PageAnnotationsHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_FromUnknown(IUnknownHandle* handle, AnnotationHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_Release(AnnotationHandle* handle);

    /** @} */

    /**
    * \memberof LinkAnnotationHandle
    * @{
    */

    /**
    * \brief A destination that shall be displayed when the annotation is activated
    */
    VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_GetDestination(LinkAnnotationHandle* handle, DestinationHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_ToBaseAnnotation(LinkAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref LinkAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_FromBaseAnnotation(AnnotationHandle* handle, LinkAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_Release(LinkAnnotationHandle* handle);

    /** @} */

    /**
    * \memberof PageAnnotationsHandle
    * @{
    */

    /**
    * \brief Get size of annotation array
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_GetSize(PageAnnotationsHandle* handle, size_type* result);

    /**
    * \brief
    * Get single annotation from array at specific position
    * \param handle a handle to annotation collection
    * \param at position of the element in the \p handle collection
    * \param result a pointer to variable, that will contain annotation upon success, unchanged on failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_At(PageAnnotationsHandle* handle, size_type at, AnnotationHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_ToUnknown(PageAnnotationsHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref PageAnnotationsHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_FromUnknown(IUnknownHandle* handle, PageAnnotationsHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_Release(PageAnnotationsHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ANNOTATIONS_H */
