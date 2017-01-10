#ifndef _C_VIEWER_PREFERENCES_H
#define _C_VIEWER_PREFERENCES_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_viewer_preferences.h
	* This file contains class definitions for ViewerPreferencesHandle
	*/

	/**
	* \brief
	* The document's page mode, specifying how to display
	* the document on exiting full-screen mode.
	*/
	typedef enum
	{
		/**
		* \brief Neither document outline nor thumbnail images visible
		*/
		NonFullScreenPageMode_UseNone = 0,

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
	* \brief Pointer to NonFullScreenPageMode
	*/
	typedef NonFullScreenPageMode *PNonFullScreenPageMode;

	/**
	* \brief The predominant reading order for text.
	*/
	typedef enum
	{
		/**
		* \brief Left to right
		*/
		ReadingOrder_LeftToRight = 0,

		/**
		* \brief
		* Right to left (including vertical writing systems,
		* such as Chinese, Japanese, and Korean).
		*/
		ReadingOrder_RightToLeft
	} ReadingOrder;

	/**
	* \brief Pointer to ReadingOrder
	*/
	typedef ReadingOrder *PReadingOrder;

	/**
	* \brief
	* The page scaling option that shall be selected when
	* a print dialog is displayed for this document.
	*/
	typedef enum PrintScaling
	{
		/**
		* \brief
		* Indicates the conforming reader's default print scaling
		*/
		PrintScaling_AppDefault = 0,

		/**
		* \brief No page scaling
		*/
		PrintScaling_None
	} PrintScaling;

	/**
	* \brief Pointer to PrintScaling
	*/
	typedef PrintScaling *PPrintScaling;

	/**
	* \brief
	* The paper handling option that shall be used
	* when printing the file from the print dialog.
	*/
	typedef enum
	{
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
	* \brief Pointer to Duplex
	*/
	typedef Duplex *PDuplex;

	/**
	* \memberof ViewerPreferencesHandle
	* @{
	*/

	/**
	* \brief
	* A flag specifying whether to hide the conforming reader's
	* tool bars when the document is active.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideToolbar(ViewerPreferencesHandle handle, PBooleanObjectHandle result);

	/**
	* \brief
	* A flag specifying whether to hide the conforming reader's
	* menu bar when the document is active.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideMenubar(ViewerPreferencesHandle handle, PBooleanObjectHandle result);

	/**
	* \brief
	* A flag specifying whether to hide user interface elements
	* in the document's window (such as scroll bars and navigation controls),
	* leaving only the document's contents displayed.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideWindowUI(ViewerPreferencesHandle handle, PBooleanObjectHandle result);

	/**
	* \brief
	* A flag specifying whether to resize the document's
	* window to fit the size of the first displayed page.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetFitWindow(ViewerPreferencesHandle handle, PBooleanObjectHandle result);

	/**
	* \brief
	* A flag specifying whether to position the document's
	* window in the center of the screen.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetCenterWindow(ViewerPreferencesHandle handle, PBooleanObjectHandle result);

	/**
	* \brief
	* A flag specifying whether the window's title bar should display
	* the document title taken from the Title entry of the document
	* information dictionary (see 14.3.3, "Document Information Dictionary").
	*
	* If false, the title bar should instead display
	* the name of the PDF file containing the document.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDisplayDocTitle(ViewerPreferencesHandle handle, PBooleanObjectHandle result);

	/**
	* \copydoc ::NonFullScreenPageMode
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNonFullScreenPageMode(ViewerPreferencesHandle handle, PNonFullScreenPageMode result);

	/**
	* \copydoc ::ReadingOrder
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDirection(ViewerPreferencesHandle handle, PReadingOrder result);

	/**
	* \brief
	* The name of the page boundary representing the area of a page that
	* shall be displayed when viewing the document on the screen.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewArea(ViewerPreferencesHandle handle, PNameObjectHandle result);

	/**
	* \brief
	* The name of the page boundary to which the contents of a page shall
	* be clipped when viewing the document on the screen.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewClip(ViewerPreferencesHandle handle, PNameObjectHandle result);

	/**
	* \brief
	* The name of the page boundary representing the area of a page that shall
	* be rendered when printing the document.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintArea(ViewerPreferencesHandle handle, PNameObjectHandle result);

	/**
	* \brief
	* The name of the page boundary to which the contents of a page shall be
	* clipped when printing the document.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintClip(ViewerPreferencesHandle handle, PNameObjectHandle result);

	/**
	* \copydoc ::PrintScaling
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintScaling(ViewerPreferencesHandle handle, PPrintScaling result);

	/**
	* \copydoc ::Duplex
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDuplex(ViewerPreferencesHandle handle, PDuplex result);

	/**
	* \brief
	* A flag specifying whether the PDF page size
	* shall be used to select the input paper tray.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPickTrayByPDFSize(ViewerPreferencesHandle handle, PBooleanObjectHandle result);

	/**
	* \brief
	* The page numbers used to initialize the print
	* dialog box when the file is printed.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintPageRange(ViewerPreferencesHandle handle, PPageRangeHandle result);

	/**
	* \brief
	* The number of copies that shall be printed when
	* the print dialog is opened for this file.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNumCopies(ViewerPreferencesHandle handle, PIntegerObjectHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_Release(ViewerPreferencesHandle handle);

	/** @} */

	/**
	* \memberof PageRangeHandle
	* @{
	*/

	/**
	* \brief Get size of the collections
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSize(PageRangeHandle handle, out_integer_type result);

	/**
	* \brief Get sub-range at index \p at
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSubrange(PageRangeHandle handle, integer_type at, PPageSubRangeHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_Release(PageRangeHandle handle);

	/** @} */

	/**
	* \memberof PageSubRangeHandle
	* @{
	*/

	/**
	* \brief Get the index of first last page in the sub-range
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetFirstPage(PageSubRangeHandle handle, PIntegerObjectHandle result);

	/**
	* \brief Get the index of last last page in the sub-range
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetLastPage(PageSubRangeHandle handle, PIntegerObjectHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_Release(PageSubRangeHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_VIEWER_PREFERENCES_H */
