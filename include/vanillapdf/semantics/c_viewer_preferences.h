#ifndef _C_VIEWER_PREFERENCES_H
#define _C_VIEWER_PREFERENCES_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_viewer_preferences.h
    * \brief This file contains class definitions for \ref ViewerPreferencesHandle
    */

    /**
    * \class ViewerPreferencesHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * Controlling the way the document shall
    * be presented on the screen or in print.
    * \see \ref CatalogHandle
    */

    /**
    * \class PageRangeHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * Collection of \ref PageSubRangeHandle
    * \see \ref ViewerPreferencesHandle
    */

    /**
    * \class PageSubRangeHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * Specifies first and last page number of defined range
    * \see \ref PageRangeHandle
    */

    /**
    * \brief
    * The document's page mode, specifying how to display
    * the document on exiting full-screen mode.
    */
    typedef enum {
        NonFullScreenPageMode_Undefined = 0,

        /**
        * \brief Neither document outline nor thumbnail images visible
        */
        NonFullScreenPageMode_UseNone,

        /**
        * \brief Document outline visible
        */
        NonFullScreenPageMode_UseOutlines,

        /**
        * \brief Thumbnail images visible
        */
        NonFullScreenPageMode_UseThumbs,

        /**
        * \brief Optional content group panel visible
        */
        NonFullScreenPageMode_UseOC
    } NonFullScreenPageMode;

    /**
    * \brief The predominant reading order for text.
    */
    typedef enum {
        ReadingOrder_Undefined = 0,

        /**
        * \brief Left to right
        */
        ReadingOrder_LeftToRight,

        /**
        * \brief
        * Right to left (including vertical writing systems,
        * such as Chinese, Japanese, and Korean).
        */
        ReadingOrder_RightToLeft
    } ReadingOrder;

    /**
    * \brief
    * The page scaling option that shall be selected when
    * a print dialog is displayed for this document.
    */
    typedef enum {
        PrintScaling_Undefined = 0,

        /**
        * \brief
        * Indicates the conforming reader's default print scaling
        */
        PrintScaling_AppDefault,

        /**
        * \brief No page scaling
        */
        PrintScaling_None
    } PrintScaling;

    /**
    * \brief
    * The paper handling option that shall be used
    * when printing the file from the print dialog.
    */
    typedef enum {
        Duplex_Undefined = 0,

        /**
        * \brief Print single-sided
        */
        Duplex_Simplex = 0,

        /**
        * \brief Duplex and flip on the short edge of the sheet
        */
        Duplex_DuplexFlipShortEdge,

        /**
        * \brief Duplex and flip on the long edge of the sheet
        */
        Duplex_DuplexFlipLongEdge
    } Duplex;

    /**
    * \memberof ViewerPreferencesHandle
    * @{
    */

    /**
    * \brief
    * A flag specifying whether to hide the conforming reader's
    * tool bars when the document is active.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideToolbar(ViewerPreferencesHandle* handle, BooleanObjectHandle** result);

    /**
    * \brief
    * A flag specifying whether to hide the conforming reader's
    * menu bar when the document is active.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideMenubar(ViewerPreferencesHandle* handle, BooleanObjectHandle** result);

    /**
    * \brief
    * A flag specifying whether to hide user interface elements
    * in the document's window (such as scroll bars and navigation controls),
    * leaving only the document's contents displayed.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideWindowUI(ViewerPreferencesHandle* handle, BooleanObjectHandle** result);

    /**
    * \brief
    * A flag specifying whether to resize the document's
    * window to fit the size of the first displayed page.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetFitWindow(ViewerPreferencesHandle* handle, BooleanObjectHandle** result);

    /**
    * \brief
    * A flag specifying whether to position the document's
    * window in the center of the screen.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetCenterWindow(ViewerPreferencesHandle* handle, BooleanObjectHandle** result);

    /**
    * \brief
    * A flag specifying whether the window's title bar should display
    * the document title taken from the Title entry of the document
    * information dictionary (see 14.3.3, "Document Information Dictionary").
    *
    * If false, the title bar should instead display
    * the name of the PDF file containing the document.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDisplayDocTitle(ViewerPreferencesHandle* handle, BooleanObjectHandle** result);

    /**
    * \copydoc NonFullScreenPageMode
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNonFullScreenPageMode(ViewerPreferencesHandle* handle, NonFullScreenPageMode* result);

    /**
    * \copydoc ReadingOrder
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDirection(ViewerPreferencesHandle* handle, ReadingOrder* result);

    /**
    * \brief
    * The name of the page boundary representing the area of a page that
    * shall be displayed when viewing the document on the screen.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewArea(ViewerPreferencesHandle* handle, NameObjectHandle** result);

    /**
    * \brief
    * The name of the page boundary to which the contents of a page shall
    * be clipped when viewing the document on the screen.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewClip(ViewerPreferencesHandle* handle, NameObjectHandle** result);

    /**
    * \brief
    * The name of the page boundary representing the area of a page that shall
    * be rendered when printing the document.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintArea(ViewerPreferencesHandle* handle, NameObjectHandle** result);

    /**
    * \brief
    * The name of the page boundary to which the contents of a page shall be
    * clipped when printing the document.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintClip(ViewerPreferencesHandle* handle, NameObjectHandle** result);

    /**
    * \copydoc PrintScaling
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintScaling(ViewerPreferencesHandle* handle, PrintScaling* result);

    /**
    * \copydoc Duplex
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDuplex(ViewerPreferencesHandle* handle, Duplex* result);

    /**
    * \brief
    * A flag specifying whether the PDF page size
    * shall be used to select the input paper tray.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPickTrayByPDFSize(ViewerPreferencesHandle* handle, BooleanObjectHandle** result);

    /**
    * \brief
    * The page numbers used to initialize the print
    * dialog box when the file is printed.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintPageRange(ViewerPreferencesHandle* handle, PageRangeHandle** result);

    /**
    * \brief
    * The number of copies that shall be printed when
    * the print dialog is opened for this file.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNumCopies(ViewerPreferencesHandle* handle, IntegerObjectHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ViewerPreferences_Release(ViewerPreferencesHandle* handle);

    /** @} */

    /**
    * \memberof PageRangeHandle
    * @{
    */

    /**
    * \brief Get size of the collections
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageRange_GetSize(PageRangeHandle* handle, size_type* result);

    /**
    * \brief Get sub-range at index \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageRange_GetSubrange(PageRangeHandle* handle, size_type at, PageSubRangeHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageRange_Release(PageRangeHandle* handle);

    /** @} */

    /**
    * \memberof PageSubRangeHandle
    * @{
    */

    /**
    * \brief Get the index of first last page in the sub-range
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageSubRange_GetFirstPage(PageSubRangeHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief Get the index of last last page in the sub-range
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageSubRange_GetLastPage(PageSubRangeHandle* handle, IntegerObjectHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageSubRange_Release(PageSubRangeHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_VIEWER_PREFERENCES_H */
