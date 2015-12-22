#ifndef _C_VIEWER_PREFERENCES_H
#define _C_VIEWER_PREFERENCES_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		NonFullScreenPageMode_UseNone = 0,
		NonFullScreenPageMode_UseOutlines,
		NonFullScreenPageMode_UseThumbs,
		NonFullScreenPageMode_UseOC
	} NonFullScreenPageMode, *PNonFullScreenPageMode;

	typedef enum
	{
		ReadingOrder_LeftToRight = 0,
		ReadingOrder_RightToLeft
	} ReadingOrder, *PReadingOrder;

	typedef enum PrintScaling
	{
		PrintScaling_AppDefault = 0,
		PrintScaling_None
	} PrintScaling, *PPrintScaling;

	typedef enum
	{
		Duplex_Simplex = 0,
		Duplex_DuplexFlipShortEdge,
		Duplex_DuplexFlipLongEdge
	} Duplex, *PDuplex;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideToolbar(ViewerPreferencesHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideMenubar(ViewerPreferencesHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideWindowUI(ViewerPreferencesHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetFitWindow(ViewerPreferencesHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDisplayDocTitle(ViewerPreferencesHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNonFullScreenPageMode(ViewerPreferencesHandle handle, PNonFullScreenPageMode result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDirection(ViewerPreferencesHandle handle, PReadingOrder result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewArea(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewClip(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintArea(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintClip(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintScaling(ViewerPreferencesHandle handle, PPrintScaling result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDuplex(ViewerPreferencesHandle handle, PDuplex result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPickTrayByPDFSize(ViewerPreferencesHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintPageRange(ViewerPreferencesHandle handle, PPageRangeHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNumCopies(ViewerPreferencesHandle handle, PIntegerHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_Release(ViewerPreferencesHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSize(PageRangeHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSubrange(PageRangeHandle handle, integer_type at, PPageSubRangeHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetFirstPage(PageSubRangeHandle handle, PIntegerHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetLastPage(PageSubRangeHandle handle, PIntegerHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_Release(PageSubRangeHandle handle);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_Release(PageRangeHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_VIEWER_PREFERENCES_H */
