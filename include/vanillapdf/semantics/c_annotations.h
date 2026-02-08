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
    * \brief Get the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_GetAuthor(TextAnnotationHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_SetAuthor(TextAnnotationHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_GetModificationDate(TextAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_SetModificationDate(TextAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_GetCreationDate(TextAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextAnnotation_SetCreationDate(TextAnnotationHandle* handle, DateHandle* value);

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
    * \brief Create a new empty highlight annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_Create(HighlightAnnotationHandle** result);

    /**
    * \brief Create a new highlight annotation with the specified rectangle
    * \param rect the bounding rectangle for the annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_CreateFromRect(RectangleHandle* rect, HighlightAnnotationHandle** result);

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
    * \brief Get the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetAuthor(HighlightAnnotationHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetAuthor(HighlightAnnotationHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetModificationDate(HighlightAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetModificationDate(HighlightAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_GetCreationDate(HighlightAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION HighlightAnnotation_SetCreationDate(HighlightAnnotationHandle* handle, DateHandle* value);

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
    * \brief Get the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetAuthor(FreeTextAnnotationHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetAuthor(FreeTextAnnotationHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetModificationDate(FreeTextAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetModificationDate(FreeTextAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_GetCreationDate(FreeTextAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FreeTextAnnotation_SetCreationDate(FreeTextAnnotationHandle* handle, DateHandle* value);

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
    * \class UnderlineAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * An underline annotation appears as an underline beneath text in the document.
    * When opened, it shall display a pop-up window containing the text of the associated note.
    */

    /**
    * \memberof UnderlineAnnotationHandle
    * @{
    */

    /**
    * \brief Create a new empty underline annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_Create(UnderlineAnnotationHandle** result);

    /**
    * \brief Create a new underline annotation with the specified rectangle
    * \param rect the bounding rectangle for the annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_CreateFromRect(RectangleHandle* rect, UnderlineAnnotationHandle** result);

    /**
    * \brief Get the quad points array for text markup
    * \param handle a handle to the underline annotation
    * \param result a pointer to variable that will contain the quad points array upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetQuadPoints(UnderlineAnnotationHandle* handle, ArrayObjectHandle** result);

    /**
    * \brief Set the quad points array for text markup
    * \param handle a handle to the underline annotation
    * \param value the quad points array to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetQuadPoints(UnderlineAnnotationHandle* handle, ArrayObjectHandle* value);

    /**
    * \brief Get the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetAuthor(UnderlineAnnotationHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetAuthor(UnderlineAnnotationHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetModificationDate(UnderlineAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetModificationDate(UnderlineAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_GetCreationDate(UnderlineAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_SetCreationDate(UnderlineAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Reinterpret current object as \ref AnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_ToBaseAnnotation(UnderlineAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref AnnotationHandle to \ref UnderlineAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_FromBaseAnnotation(AnnotationHandle* handle, UnderlineAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION UnderlineAnnotation_Release(UnderlineAnnotationHandle* handle);

    /** @} */

    /**
    * \class StrikeOutAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * A strikeout annotation appears as a strikethrough line across text in the document.
    * When opened, it shall display a pop-up window containing the text of the associated note.
    */

    /**
    * \memberof StrikeOutAnnotationHandle
    * @{
    */

    /**
    * \brief Create a new empty strikeout annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_Create(StrikeOutAnnotationHandle** result);

    /**
    * \brief Create a new strikeout annotation with the specified rectangle
    * \param rect the bounding rectangle for the annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_CreateFromRect(RectangleHandle* rect, StrikeOutAnnotationHandle** result);

    /**
    * \brief Get the quad points array for text markup
    * \param handle a handle to the strikeout annotation
    * \param result a pointer to variable that will contain the quad points array upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetQuadPoints(StrikeOutAnnotationHandle* handle, ArrayObjectHandle** result);

    /**
    * \brief Set the quad points array for text markup
    * \param handle a handle to the strikeout annotation
    * \param value the quad points array to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetQuadPoints(StrikeOutAnnotationHandle* handle, ArrayObjectHandle* value);

    /**
    * \brief Get the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetAuthor(StrikeOutAnnotationHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetAuthor(StrikeOutAnnotationHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetModificationDate(StrikeOutAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetModificationDate(StrikeOutAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_GetCreationDate(StrikeOutAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_SetCreationDate(StrikeOutAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Reinterpret current object as \ref AnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_ToBaseAnnotation(StrikeOutAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref AnnotationHandle to \ref StrikeOutAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_FromBaseAnnotation(AnnotationHandle* handle, StrikeOutAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StrikeOutAnnotation_Release(StrikeOutAnnotationHandle* handle);

    /** @} */

    /**
    * \class SquigglyAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * A squiggly annotation appears as a jagged underline beneath text in the document.
    * When opened, it shall display a pop-up window containing the text of the associated note.
    */

    /**
    * \memberof SquigglyAnnotationHandle
    * @{
    */

    /**
    * \brief Create a new empty squiggly annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_Create(SquigglyAnnotationHandle** result);

    /**
    * \brief Create a new squiggly annotation with the specified rectangle
    * \param rect the bounding rectangle for the annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_CreateFromRect(RectangleHandle* rect, SquigglyAnnotationHandle** result);

    /**
    * \brief Get the quad points array for text markup
    * \param handle a handle to the squiggly annotation
    * \param result a pointer to variable that will contain the quad points array upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetQuadPoints(SquigglyAnnotationHandle* handle, ArrayObjectHandle** result);

    /**
    * \brief Set the quad points array for text markup
    * \param handle a handle to the squiggly annotation
    * \param value the quad points array to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetQuadPoints(SquigglyAnnotationHandle* handle, ArrayObjectHandle* value);

    /**
    * \brief Get the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetAuthor(SquigglyAnnotationHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetAuthor(SquigglyAnnotationHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetModificationDate(SquigglyAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetModificationDate(SquigglyAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_GetCreationDate(SquigglyAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_SetCreationDate(SquigglyAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Reinterpret current object as \ref AnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_ToBaseAnnotation(SquigglyAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref AnnotationHandle to \ref SquigglyAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_FromBaseAnnotation(AnnotationHandle* handle, SquigglyAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SquigglyAnnotation_Release(SquigglyAnnotationHandle* handle);

    /** @} */

    /**
    * \class InkAnnotationHandle
    * \extends AnnotationHandle
    * \ingroup group_annotations
    * \brief
    * An ink annotation (PDF 1.3) represents a freehand "scribble" composed of
    * one or more disjoint paths. When opened, it shall display a pop-up window
    * containing the text of the associated note.
    */

    /**
    * \memberof InkAnnotationHandle
    * @{
    */

    /**
    * \brief Create a new empty ink annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_Create(InkAnnotationHandle** result);

    /**
    * \brief Create a new ink annotation with the specified rectangle
    * \param rect the bounding rectangle for the annotation
    * \param result a pointer to variable that will contain the new annotation upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_CreateFromRect(RectangleHandle* rect, InkAnnotationHandle** result);

    /**
    * \brief Get the ink list (array of stroked paths)
    * \param handle a handle to the ink annotation
    * \param result a pointer to variable that will contain the ink list array upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetInkList(InkAnnotationHandle* handle, ArrayObjectHandle** result);

    /**
    * \brief Set the ink list (array of stroked paths)
    * \param handle a handle to the ink annotation
    * \param value the ink list array to set
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetInkList(InkAnnotationHandle* handle, ArrayObjectHandle* value);

    /**
    * \brief Get the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetAuthor(InkAnnotationHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the annotation author (T entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetAuthor(InkAnnotationHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetModificationDate(InkAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation modification date (M entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetModificationDate(InkAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Get the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_GetCreationDate(InkAnnotationHandle* handle, DateHandle** result);

    /**
    * \brief Set the annotation creation date (CreationDate entry, markup annotations)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_SetCreationDate(InkAnnotationHandle* handle, DateHandle* value);

    /**
    * \brief Reinterpret current object as \ref AnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_ToBaseAnnotation(InkAnnotationHandle* handle, AnnotationHandle** result);

    /**
    * \brief Convert \ref AnnotationHandle to \ref InkAnnotationHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_FromBaseAnnotation(AnnotationHandle* handle, InkAnnotationHandle** result);

    /**
    * \copydoc Annotation_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InkAnnotation_Release(InkAnnotationHandle* handle);

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
    * \brief An action to be performed when the link annotation is activated
    */
    VANILLAPDF_API error_type CALLING_CONVENTION LinkAnnotation_GetAction(LinkAnnotationHandle* handle, ActionHandle** result);

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
    * \brief
    * Remove an annotation from the collection at the specified position
    * \param handle a handle to annotation collection
    * \param at position of the element to remove from the \p handle collection
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageAnnotations_Remove(PageAnnotationsHandle* handle, size_type at);

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
