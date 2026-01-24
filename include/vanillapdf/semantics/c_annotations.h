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
    * \class TextAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * A text annotation represents a "sticky note" attached to a point in the PDF document.
    * When closed, the annotation shall appear as an icon; when open, it shall display
    * a pop-up window containing the text of the note in a font and size chosen by the
    * conforming reader.
    */

    /**
    * \class HighlightAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * A highlight annotation appears as a highlight over a region of text in the document.
    * When opened, it shall display a pop-up window containing the text of the associated note.
    */

    /**
    * \class FreeTextAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * A free text annotation (PDF 1.3) displays text directly on the page.
    * Unlike an ordinary text annotation, a free text annotation has no
    * open or closed state; instead, the text shall always be visible.
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
    * \brief Annotation flags controlling visibility and behavior
    * \ingroup group_annotations
    *
    * These flags may be combined using bitwise OR operations.
    */
    typedef enum {
        /**
        * \brief No flags set
        */
        AnnotationFlags_None = 0,
        /**
        * \brief If set, do not display the annotation if it does not belong to one of the
        * standard annotation types and no annotation handler is available.
        */
        AnnotationFlags_Invisible = 1,
        /**
        * \brief If set, do not display or print the annotation or allow it to interact
        * with the user, regardless of its annotation type or annotation handler.
        */
        AnnotationFlags_Hidden = 2,
        /**
        * \brief If set, print the annotation when the page is printed.
        */
        AnnotationFlags_Print = 4,
        /**
        * \brief If set, do not scale the annotation's appearance to match the magnification
        * of the page.
        */
        AnnotationFlags_NoZoom = 8,
        /**
        * \brief If set, do not rotate the annotation's appearance to match the rotation
        * of the page.
        */
        AnnotationFlags_NoRotate = 16,
        /**
        * \brief If set, do not display the annotation on the screen or allow it to interact
        * with the user.
        */
        AnnotationFlags_NoView = 32,
        /**
        * \brief If set, do not allow the annotation to interact with the user.
        */
        AnnotationFlags_ReadOnly = 64,
        /**
        * \brief If set, do not allow the annotation to be deleted or its properties modified.
        */
        AnnotationFlags_Locked = 128,
        /**
        * \brief If set, invert the interpretation of the NoView flag for certain events.
        */
        AnnotationFlags_ToggleNoView = 256,
        /**
        * \brief If set, do not allow the contents of the annotation to be modified.
        */
        AnnotationFlags_LockedContents = 512
    } AnnotationFlags;

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
    * \brief Get the annotation rectangle
    * \param handle a handle to the annotation class
    * \param result a pointer to variable that will contain the rectangle upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetRect(AnnotationHandle* handle, RectangleHandle** result);

    /**
    * \brief Set the annotation rectangle
    * \param handle a handle to the annotation class
    * \param value the rectangle to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetRect(AnnotationHandle* handle, RectangleHandle* value);

    /**
    * \brief Get the annotation contents (text)
    * \param handle a handle to the annotation class
    * \param result a pointer to variable that will contain the contents string upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetContents(AnnotationHandle* handle, LiteralStringObjectHandle** result);

    /**
    * \brief Set the annotation contents (text)
    * \param handle a handle to the annotation class
    * \param value the contents string to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetContents(AnnotationHandle* handle, LiteralStringObjectHandle* value);

    /**
    * \brief Get the annotation color
    * \param handle a handle to the annotation class
    * \param result a pointer to variable that will contain the color upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetColor(AnnotationHandle* handle, ColorHandle** result);

    /**
    * \brief Set the annotation color
    * \param handle a handle to the annotation class
    * \param value the color to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetColor(AnnotationHandle* handle, ColorHandle* value);

    /**
    * \brief Get the annotation author (T entry)
    * \param handle a handle to the annotation class
    * \param result a pointer to variable that will contain the author string upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetAuthor(AnnotationHandle* handle, LiteralStringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry)
    * \param handle a handle to the annotation class
    * \param value the author string to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetAuthor(AnnotationHandle* handle, LiteralStringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry)
    * \param handle a handle to the annotation class
    * \param result a pointer to variable that will contain the date upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetModificationDate(AnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry)
    * \param handle a handle to the annotation class
    * \param value the date to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetModificationDate(AnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations only)
    * \param handle a handle to the annotation class
    * \param result a pointer to variable that will contain the date upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetCreationDate(AnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations only)
    * \param handle a handle to the annotation class
    * \param value the date to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetCreationDate(AnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation flags (F entry)
    * \param handle a handle to the annotation class
    * \param result a pointer to variable that will contain the flags upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_GetFlags(AnnotationHandle* handle, AnnotationFlags* result);

    /**
    * \brief Set the annotation flags (F entry)
    * \param handle a handle to the annotation class
    * \param value the flags to set (can be combined with bitwise OR)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_SetFlags(AnnotationHandle* handle, AnnotationFlags value);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Annotation_Release(AnnotationHandle* handle);

    /** @} */

    /**
    * \memberof TextAnnotationHandle
    * @{
    */

    /**
    * \brief Create a new text annotation with the specified rectangle
    * \param rect the bounding rectangle for the annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_Create(RectangleHandle* rect, TextAnnotationHandle** result);

    /**
    * \brief Create a new text annotation with the specified rectangle and contents
    * \param rect the bounding rectangle for the annotation
    * \param contents the text contents of the annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_CreateWithContents(RectangleHandle* rect, LiteralStringObjectHandle* contents, TextAnnotationHandle** result);

    /**
    * \brief Reinterpret current object as \ref AnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_ToBaseAnnotation(TextAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref AnnotationHandle to \ref TextAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_FromBaseAnnotation(AnnotationHandle* handle, TextAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_Release(TextAnnotationHandle* handle);

    /** @} */

    /**
    * \memberof HighlightAnnotationHandle
    * @{
    */

    /**
    * \brief Create a new highlight annotation with the specified rectangle and quad points
    * \param rect the bounding rectangle for the annotation
    * \param quadPoints an array of 8 numbers specifying the coordinates of the quadrilateral
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_Create(RectangleHandle* rect, ArrayObjectHandle* quadPoints, HighlightAnnotationHandle** result);

    /**
    * \brief Get the quad points array for text markup
    * \param handle a handle to the highlight annotation
    * \param result a pointer to variable that will contain the quad points array upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetQuadPoints(HighlightAnnotationHandle* handle, ArrayObjectHandle** result);

    /**
    * \brief Set the quad points array for text markup
    * \param handle a handle to the highlight annotation
    * \param value the quad points array to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetQuadPoints(HighlightAnnotationHandle* handle, ArrayObjectHandle* value);

    /**
    * \brief Reinterpret current object as \ref AnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_ToBaseAnnotation(HighlightAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref AnnotationHandle to \ref HighlightAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_FromBaseAnnotation(AnnotationHandle* handle, HighlightAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_Release(HighlightAnnotationHandle* handle);

    /** @} */

    /**
    * \memberof FreeTextAnnotationHandle
    * @{
    */

    /**
    * \brief Create a new free text annotation
    * \param rect the bounding rectangle for the annotation
    * \param contents the text contents of the annotation
    * \param defaultAppearance the default appearance string (font and size)
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_Create(RectangleHandle* rect, LiteralStringObjectHandle* contents, LiteralStringObjectHandle* defaultAppearance, FreeTextAnnotationHandle** result);

    /**
    * \brief Get the default appearance string
    * \param handle a handle to the free text annotation
    * \param result a pointer to variable that will contain the default appearance string upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetDefaultAppearance(FreeTextAnnotationHandle* handle, LiteralStringObjectHandle** result);

    /**
    * \brief Set the default appearance string
    * \param handle a handle to the free text annotation
    * \param value the default appearance string to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetDefaultAppearance(FreeTextAnnotationHandle* handle, LiteralStringObjectHandle* value);

    /**
    * \brief Reinterpret current object as \ref AnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_ToBaseAnnotation(FreeTextAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref AnnotationHandle to \ref FreeTextAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_FromBaseAnnotation(AnnotationHandle* handle, FreeTextAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_Release(FreeTextAnnotationHandle* handle);

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
    * \brief
    * Append an annotation to the collection
    * \param handle a handle to annotation collection
    * \param annotation the annotation to append
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_Append(PageAnnotationsHandle* handle, AnnotationHandle* annotation);

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
