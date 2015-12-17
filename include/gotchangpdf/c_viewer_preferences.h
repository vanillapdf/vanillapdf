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
		PageMode_UseNone = 0,
		PageMode_UseOutlines,
		PageMode_UseThumbs,
		PageMode_UseOC
	} PageMode, *PPageMode;

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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNonFullScreenPageMode(ViewerPreferencesHandle handle, PPageMode result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDirection(ViewerPreferencesHandle handle, PReadingOrder result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewArea(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewClip(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintArea(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintClip(ViewerPreferencesHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintScaling(ViewerPreferencesHandle handle, PPrintScaling result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDuplex(ViewerPreferencesHandle handle, PDuplex result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPickTrayByPDFSize(ViewerPreferencesHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNumCopies(ViewerPreferencesHandle handle, PIntegerHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_Release(ViewerPreferencesHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_VIEWER_PREFERENCES_H */
